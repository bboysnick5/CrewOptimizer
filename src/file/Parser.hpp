//
//  Parser.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/27/21.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <date/date.h>
#include <fmt/Format.h>
#include <glog/logging.h>

#include <algorithm>
#include <array>
#include <boost/math/special_functions/pow.hpp>
#include <cctype>
#include <charconv>
#include <cmath>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <magic_enum.hpp>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <tl/expected.hpp>
#include <tuple>
#include <type_traits>
#include <utility>

#include "../Chrono.hpp"
#include "../Definition.hpp"
#include "../Utility.hpp"
#include "FileHandler.hpp"

namespace file {

namespace parser {

// enum class CharOrTokenParsing : std::uint8_t { kChar, kToken };

enum class Pattern : std::uint8_t {
    kSingleChar,
    kConvertAlnumToUint,
    kParseNum,
    kChronoDirect,
    kChronoSignedDuration,  // can be fixed with c++ 20 chrono lib
    kEnumStrMatching,
    KNotApplicable,
};

struct ParserError {
    std::string_view msg;

    constexpr decltype(auto) operator+(const ParserError& other) {
        return std::forward<ParserError>({absl::StrCat(msg, other.msg)});
    }
};
/*
template <FieldFormat Format>
constexpr std::string_view SubSvForField(std::string_view line) {
    return line.substr(Format.char_or_token_start_idx, Format.length);
}
*/

template <class ReturnType, FieldFormat Format, Pattern Pattern>
requires(
    Pattern == Pattern::kSingleChar &&
    std::same_as<std::remove_const_t<ReturnType>, char> &&
    Format.length ==
        1) constexpr tl::expected<ReturnType,
                                  ParserError> ParseToken(auto&& token_sv) {
    return token_sv[0];
}

template <class ReturnType, FieldFormat Format, Pattern Pattern>
requires(Pattern == Pattern::kConvertAlnumToUint &&
         boost::math::pow<Format.length>(36) <
             std::numeric_limits<ReturnType>::max()) constexpr tl::
    expected<ReturnType, ParserError> ParseToken(auto&& token_sv) {
    ReturnType res = 0;
    char ch;
    for (std::uint8_t ui = 0; ui < token_sv.size(); ++ui) {
        ch = token_sv[ui];
        if (!util::isalnum(ch))
            return tl::make_unexpected(ParserError{
                fmt::format("field string: {} is not alphanumeric, invalid "
                            "char: {}, at {}\n",
                            token_sv, ch, ui)});
        res = res * 36 +
              (util::isdigit(ch) ? (ch - '0') : (util::toupper(ch) - 'A' + 10));
    }
    return res;
}

template <class ReturnType, FieldFormat Format, Pattern Pattern>
requires(Pattern == Pattern::kParseNum) constexpr tl::expected<
    ReturnType, ParserError> ParseToken(auto&& token_sv) {
    ReturnType result_integral;
    const auto conv_result = std::from_chars(
        token_sv.data(), token_sv.data() + token_sv.size(), result_integral);
    switch (conv_result.ec) {
        case std::errc():
            return result_integral;
        case std::errc::invalid_argument:
            return tl::make_unexpected(std::forward<ParserError>(
                {fmt::format("unable to parse number string: {}, invalid "
                             "char: {}, at {}\n",
                             token_sv, conv_result.ptr[0],
                             conv_result.ptr - token_sv.cbegin())}));
        case std::errc::result_out_of_range:
            return tl::make_unexpected(std::forward<ParserError>(
                {fmt::format("unable to parse number string: {}, return type: "
                             "{} out of range\n",
                             token_sv, typeid(ReturnType).name())}));
    }
}

template <class ReturnType, FieldFormat Format, Pattern Pattern>
requires(Pattern == Pattern::kChronoDirect)
    tl::expected<ReturnType, ParserError> ParseToken(auto&& token_sv) {
    ReturnType tp_or_duration;
    std::istringstream field_ss(std::string(token_sv), std::ios_base::in);
    if (field_ss >> date::parse(Format.format_str.data(), tp_or_duration))
        return tp_or_duration;
    return tl::make_unexpected(ParserError{fmt::format(
        "unable to parse date time string: {} with required Format: {}",
        token_sv, Format.format_str)});
}

template <class ReturnType, FieldFormat Format, Pattern Pattern>
requires(Pattern == Pattern::kChronoSignedDuration) constexpr tl::expected<
    ReturnType, ParserError> ParseToken(auto&& token_sv) {
    return ParseToken<chr::ChronoRep, Format, Pattern::kParseNum>(token_sv)
        .and_then([](const auto int_rep) { return ReturnType{int_rep}; });
}

template <class ReturnType, FieldFormat Format, Pattern Pattern>
requires(Pattern == Pattern::kEnumStrMatching &&
         std::is_enum_v<ReturnType>) constexpr tl::
    expected<ReturnType, ParserError> ParseToken(auto&& token_sv) {
    if (const auto cast_result = magic_enum::enum_cast<ReturnType>(token_sv);
        cast_result)
        return cast_result;
    return tl::make_unexpected(
        fmt::format("unable to match enum string: {} to enum type: {}",
                    token_sv, magic_enum::enum_type_name<ReturnType>()));
}

template <class ParsingSigTuple>
struct ParseFieldFunctor {
    constexpr decltype(auto) operator()(auto&& line) {
        return ParseField<std::tuple_element_t<0, ParsingSigTuple>,
                          file::GetFieldFormat<
                              std::tuple_element_t<1, ParsingSigTuple>{}()>(),
                          std::tuple_element_t<2, ParsingSigTuple>{}()>(
            std::forward<decltype(line)>(line));
    }
};

template <class ReturnType, FieldFormat Format, Pattern Pattern>
constexpr tl::expected<ReturnType, ParserError> ParseField(auto&& line) {
    return ParseToken<ReturnType, Format, Pattern>(
        line.substr(Format.char_or_token_start_idx, Format.length));
}

template <class ReturnType, FieldFormat Format, Pattern Pattern>
constexpr tl::expected<ReturnType, ParserError> ParseTokenDirect(
    auto&& token_sv) {
    return ParseToken<ReturnType, Format, Pattern>(token_sv);
}

/*
template <util::tuple<ReturnType, auto FieldEnum, Pattern>
              ParsingSignatures...>
constexpr tl::expected<ReturnType, ParserError> ParseLine(
    std::string_view line) {
    constexpr file::FieldFormat Format_handle = GetFieldFormat<FieldEnum>();

    // LOG(INFO) << "Found " << num_cookies << " cookies";

    return {(ParseToken<std::get<0>(ParsingSignatures),
                        std::get<1>(ParsingSignatures),
                        set::get<2>(ParsingSignatures)>(
        SubSvForField<Format_handle>(line))...)};
}*/

}  // namespace parser
}  // namespace file

#endif /* Parser_hpp */
