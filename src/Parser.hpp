//
//  Parser.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/27/21.
//

#ifndef Parser_hpp
#define Parser_hpp

#include <cctype>
#include <cstdint>
#include <cstring>

#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <ios>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include <date/date.h>
#include <magic_enum.hpp>

#include "Definition.hpp"
#include "Utility.hpp"

template <class FieldEnum>
class Parser {
public:
    void SetCurLineToParseSecSvIdx(std::string_view line_sv, ser::SecSvIdxType idx);
    
    ser::SecSvIdxType GetSecSvIdx() const;
    
    template <class ReturnType, FieldEnum FE, std::enable_if_t<!std::is_enum_v<ReturnType>, bool> = true>
    ReturnType ParseField() const;
    
    template <class ReturnEnumType, FieldEnum FE, std::enable_if_t<std::is_enum_v<ReturnEnumType>, bool> = true>
    ReturnEnumType ParseField() const;
    
private:
    template <FieldEnum FE>
    std::string_view SubSvFromField() const;
    
    template <FieldEnum FE>
    static void CheckAndAbortForBadInput(bool is_consistent);
    
    template <class ReturnCharType, FieldEnum FE,
              std::enable_if_t<std::is_same_v<std::remove_const_t<ReturnCharType>, char>, bool> = true>
    static ReturnCharType ParseFieldSv(std::string_view field_sv);
    
    template <class ReturnArrType, FieldEnum FE,
              std::enable_if_t<util::is_array_v<typename ReturnArrType::value_type, std::tuple_size_v<ReturnArrType>> &&
                               std::is_same_v<typename ReturnArrType::value_type, char>, bool> = true>
    static ReturnArrType ParseFieldSv(std::string_view field_sv);

    // convert str to uint
    template <class ReturnUintType, FieldEnum FE,
              std::enable_if_t<(std::is_same_v<ReturnUintType, std::uint8_t> ||
                                std::is_same_v<ReturnUintType, std::uint16_t>) &&
                               ser::SecFieldEnumFieldFormatMap[util::to_underlying(FE)].format_sv ==
                                   ser::kConvertStrToInt,
                               bool> = true>
    static ReturnUintType ParseFieldSv(std::string_view field_sv);
    
    // translate alphanumeric to uint on base of 36
    template <class ReturnUintType, FieldEnum FE,
              std::enable_if_t<(std::is_same_v<ReturnUintType, std::uint8_t> ||
                                std::is_same_v<ReturnUintType, std::uint16_t>) &&
                               ser::SecFieldEnumFieldFormatMap[util::to_underlying(FE)].format_sv ==
                                   ser::kTranslateAlnumToUint,
                               bool> = true>
    static ReturnUintType ParseFieldSv(std::string_view field_sv);
    
    template <class ReturnChronoType, FieldEnum FE,
              std::enable_if_t<util::is_chrono_time_point_v<std::chrono::system_clock> ||
                               (util::is_chrono_duration_v<typename ReturnChronoType::rep,
                                                           typename ReturnChronoType::period> &&
                                !std::is_same_v<ReturnChronoType, chr::DurationDays4B>),
                               bool> = true>
    static ReturnChronoType ParseFieldSv(std::string_view field_sv);
    
    // patch for pre C++ 20 chrono lib compiler. Can be substitude with chrono::parse and format %Q
    // once C++ 20 chrono lands.
    template <class ReturnChronoDaysType, FieldEnum FE,
              std::enable_if_t<std::is_same_v<ReturnChronoDaysType, chr::DurationDays4B>, bool> = true>
    static ReturnChronoDaysType ParseFieldSv(std::string_view field_sv);

    // Data field
    std::string_view line_sv_;
    ser::SecSvIdxType cur_sec_sv_idx_ = 0;
};

template <class FieldEnum>
inline void Parser<FieldEnum>::SetCurLineToParseSecSvIdx(std::string_view line_sv, ser::SecSvIdxType idx) {
    line_sv_ = line_sv;
    cur_sec_sv_idx_ = idx;
}

template <class FieldEnum>
inline ser::SecSvIdxType Parser<FieldEnum>::GetSecSvIdx() const {
    return cur_sec_sv_idx_;
}

template <class FieldEnum>
template <class ReturnType, FieldEnum FE, std::enable_if_t<!std::is_enum_v<ReturnType>, bool>>
inline ReturnType Parser<FieldEnum>::ParseField() const {
    return Parser<FieldEnum>::template ParseFieldSv<ReturnType, FE>(SubSvFromField<FE>());
}

template <class FieldEnum>
template <class ReturnEnumType, FieldEnum FE, std::enable_if_t<std::is_enum_v<ReturnEnumType>, bool>>
inline ReturnEnumType Parser<FieldEnum>::ParseField() const {
    return magic_enum::enum_cast<ReturnEnumType>(
               ParseFieldSv<std::underlying_type_t<ReturnEnumType>, FE>(SubSvFromField<FE>())).value();
}

template <class FieldEnum>
template <class ReturnCharType, FieldEnum FE,
          std::enable_if_t<std::is_same_v<std::remove_const_t<ReturnCharType>, char>, bool>>
inline ReturnCharType Parser<FieldEnum>::ParseFieldSv(std::string_view field_sv) {
    CheckAndAbortForBadInput<FE>(field_sv.size() == 1);
    return field_sv[0];
}

template <class FieldEnum>
template <class ReturnArrType, FieldEnum FE,
          std::enable_if_t<util::is_array_v<typename ReturnArrType::value_type, std::tuple_size_v<ReturnArrType>> &&
                           std::is_same_v<typename ReturnArrType::value_type, char>, bool>>
inline ReturnArrType Parser<FieldEnum>::ParseFieldSv(std::string_view field_sv) {
    auto impl_array_size = std::tuple_size_v<ReturnArrType>;
    CheckAndAbortForBadInput<FE>(field_sv.size() == impl_array_size);
    ReturnArrType result_arr;
    std::copy(field_sv.data(), field_sv.data() + impl_array_size, result_arr.begin());
    return result_arr;
}

template <class FieldEnum>
template <class ReturnUintType, FieldEnum FE,
          std::enable_if_t<(std::is_same_v<ReturnUintType, std::uint8_t> ||
                            std::is_same_v<ReturnUintType, std::uint16_t>) &&
                           ser::SecFieldEnumFieldFormatMap[util::to_underlying(FE)].format_sv ==
                               ser::kConvertStrToInt,
                           bool>>
inline ReturnUintType Parser<FieldEnum>::ParseFieldSv(std::string_view field_sv) {
    ReturnUintType result_uint;
    const auto conv_result = std::from_chars(field_sv.data(), field_sv.data() + field_sv.size(), result_uint);
    CheckAndAbortForBadInput<FE>(conv_result.ec != std::errc::invalid_argument);
    return result_uint;
}

template <class FieldEnum>
template <class ReturnUintType, FieldEnum FE,
          std::enable_if_t<(std::is_same_v<ReturnUintType, std::uint8_t> ||
                            std::is_same_v<ReturnUintType, std::uint16_t>) &&
                           ser::SecFieldEnumFieldFormatMap[util::to_underlying(FE)].format_sv == ser::kTranslateAlnumToUint, bool>>
ReturnUintType Parser<FieldEnum>::ParseFieldSv(std::string_view field_sv) {
    CheckAndAbortForBadInput<FE>(((std::is_same_v<ReturnUintType, std::uint8_t> && field_sv.size() == 1) ||
                                  (std::is_same_v<ReturnUintType, std::uint16_t> && field_sv.size() <= 3)) &&
                                 std::all_of(field_sv.cbegin(), field_sv.cend(),
                                             [](const char c) {return std::isalnum(c);}));
    return std::accumulate(field_sv.cbegin(), field_sv.cend(), 0, [](ReturnUintType result_uint, char ch) {
        return result_uint * 36 + (std::isdigit(ch) ? (ch - '0') : (std::toupper(ch) - 'A' + 10));
    });
}

template <class FieldEnum>
template <class ReturnChronoType, FieldEnum FE,
          std::enable_if_t<util::is_chrono_time_point_v<std::chrono::system_clock> ||
                           (util::is_chrono_duration_v<typename ReturnChronoType::rep,
                                                       typename ReturnChronoType::period> &&
                            !std::is_same_v<ReturnChronoType, chr::DurationDays4B>), bool>>
inline ReturnChronoType Parser<FieldEnum>::ParseFieldSv(std::string_view field_sv) {
    ReturnChronoType tp;
    std::stringstream field_ss(std::string(field_sv), std::ios_base::in);
    bool is_consistent = bool(field_ss >> date::parse(std::string(
                         ser::SecFieldEnumFieldFormatMap[util::to_underlying(FE)].format_sv), tp));
    CheckAndAbortForBadInput<FE>(is_consistent);
    return tp;
}

template <class FieldEnum>
template <class ReturnChronoDaysType, FieldEnum FE,
          std::enable_if_t<std::is_same_v<ReturnChronoDaysType, chr::DurationDays4B>, bool>>
inline ReturnChronoDaysType Parser<FieldEnum>::ParseFieldSv(std::string_view field_sv) {
    // cannot use date::parse because days can be 0 which is not allowed in date lib "%d" format.
    // Can be replaced with std::chrono::parse once compiler implementations are out.
    std::uint8_t num_days;
    auto conv_result = std::from_chars(field_sv.data(), field_sv.data()+field_sv.size(), num_days);
    CheckAndAbortForBadInput<FE>(conv_result.ec != std::errc::invalid_argument);
    return chr::DurationDays4B(num_days);
}

template <class FieldEnum>
template <FieldEnum FE>
inline void Parser<FieldEnum>::CheckAndAbortForBadInput(bool is_consistent) {
    if (!is_consistent) {
        std::cerr << "Field " << magic_enum::enum_name(FE)
                  << " is not consistent between file format and implementation.\n";
        std::exit(EXIT_FAILURE);
    }
}

template <class FieldEnum>
template <FieldEnum FE>
inline std::string_view Parser<FieldEnum>::SubSvFromField() const {
    const auto[start_idx, count, ignore0] = ser::SecFieldEnumFieldFormatMap[util::to_underlying(FE)];
    return line_sv_.substr(start_idx, count);
}


#endif /* Parser_hpp */
