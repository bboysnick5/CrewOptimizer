//
//  Parser.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/27/21.
//

#ifndef Parser_hpp
#define Parser_hpp


#include <cstdint>
#include <cstring>

#include <array>
#include <charconv>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <utility>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <vector>

#include <date/date.h>
#include <magic_enum.hpp>

#include "Definition.hpp"
#include "Utility.hpp"


class Parser {
public:
    void SetCurLineToParse(std::string_view line_sv);
    
    template <typename ReturnType, std::enable_if_t<!std::is_enum_v<ReturnType>, bool> = true>
    ReturnType ParseField(ser::SecField field_enum) const;
    
    template <typename ReturnType, std::enable_if_t<std::is_enum_v<ReturnType>, bool> = true>
    ReturnType ParseField(ser::SecField field_enum) const;
    
private:
    std::string_view SubSvFromField(ser::SecField field_enum) const;
    
    static void CheckAndAbortForBadInput(ser::SecField field_enum, bool is_consistent);
    
    template<typename ReturnType, std::enable_if_t<std::is_same_v<std::remove_const_t<ReturnType>, char>, bool> = true>
    static ReturnType ParseFieldSv(ser::SecField field_enum, std::string_view field_sv);
    
    template<typename ReturnType, std::enable_if_t<util::is_array_v<typename ReturnType::value_type, std::tuple_size_v<ReturnType>> && std::is_same_v<typename ReturnType::value_type, char>, bool> = true>
    static ReturnType ParseFieldSv(ser::SecField field_enum, std::string_view field_sv);

    template<typename ReturnType, std::enable_if_t<std::is_same_v<ReturnType, std::uint8_t> ||
                                                   std::is_same_v<ReturnType, std::uint16_t>, bool> = true>
    static ReturnType ParseFieldSv(ser::SecField field_enum, std::string_view field_sv);
    
    template<typename ReturnType, std::enable_if_t<util::is_chrono_time_point_v<std::chrono::system_clock> ||
                                                   (util::is_chrono_duration_v<typename ReturnType::rep,
                                                                               typename ReturnType::period> &&
                                                    !std::is_same_v<ReturnType, gen::DurationDays>),
                                                   bool> = true>
    static ReturnType ParseFieldSv(ser::SecField field_enum, std::string_view field_sv);
    
    // patch for pre C++ 20 chrono lib compiler. Can be substitude with chrono::parse and format %Q
    // once C++ 20 chrono lands.
    template<typename ReturnType, std::enable_if_t<std::is_same_v<ReturnType, gen::DurationDays>, bool> = true>
    static ReturnType ParseFieldSv(ser::SecField field_enum, std::string_view field_sv);
    
    // Data field
    std::string_view line_sv_;
};


inline void Parser::SetCurLineToParse(std::string_view line_sv) {
    line_sv_ = line_sv;
}

template <typename ReturnType, std::enable_if_t<!std::is_enum_v<ReturnType>, bool>>
inline ReturnType Parser::ParseField(ser::SecField field_enum) const {
    return ParseFieldSv<ReturnType>(field_enum, SubSvFromField(field_enum));
}

template <typename ReturnType, std::enable_if_t<std::is_enum_v<ReturnType>, bool>>
inline ReturnType Parser::ParseField(ser::SecField field_enum) const {
    return magic_enum::enum_cast<ReturnType>(ParseFieldSv<std::underlying_type_t<ReturnType>>(field_enum, SubSvFromField(field_enum))).value();
}

template<typename ReturnType, std::enable_if_t<std::is_same_v<std::remove_const_t<ReturnType>, char>, bool>>
inline ReturnType Parser::ParseFieldSv(ser::SecField field_enum, std::string_view field_sv) {
    CheckAndAbortForBadInput(field_enum, field_sv.size() == 1);
    return field_sv[0];
}

template<typename ReturnType, std::enable_if_t<util::is_array_v<typename ReturnType::value_type, std::tuple_size_v<ReturnType>> && std::is_same_v<typename ReturnType::value_type, char>, bool>>
inline ReturnType Parser::ParseFieldSv(ser::SecField field_enum, std::string_view field_sv) {
    auto impl_array_size = std::tuple_size_v<ReturnType>;
    CheckAndAbortForBadInput(field_enum, field_sv.size() == impl_array_size);
    ReturnType result_arr;
    std::copy(field_sv.data(), field_sv.data() + impl_array_size, result_arr.begin());
    return result_arr;
}

template<typename ReturnType, std::enable_if_t<std::is_same_v<ReturnType, std::uint8_t> ||
                                               std::is_same_v<ReturnType, std::uint16_t>, bool>>
inline ReturnType Parser::ParseFieldSv(ser::SecField field_enum, std::string_view field_sv) {
    ReturnType impl_integer;
    auto conv_result = std::from_chars(field_sv.data(), field_sv.data()+field_sv.size(), impl_integer);
    CheckAndAbortForBadInput(field_enum, conv_result.ec != std::errc::invalid_argument);
    return impl_integer;
}

template<typename ReturnType, std::enable_if_t<util::is_chrono_time_point_v<std::chrono::system_clock> ||
                                               (util::is_chrono_duration_v<typename ReturnType::rep,
                                                                           typename ReturnType::period> &&
                                                !std::is_same_v<ReturnType, gen::DurationDays>),
                                               bool>>
inline ReturnType Parser::ParseFieldSv(ser::SecField field_enum, std::string_view field_sv) {
    ReturnType tp;
    std::stringstream field_ss(std::string(field_sv), std::ios_base::in);
    bool is_consistent = bool(field_ss >> date::parse(std::string(
                         ser::SecFieldStrFormatMap[util::to_underlying(field_enum)].str_chrono_format), tp));
    CheckAndAbortForBadInput(field_enum, is_consistent);
    return tp;
}

template<typename ReturnType, std::enable_if_t<std::is_same_v<ReturnType, gen::DurationDays>, bool>>
inline ReturnType Parser::ParseFieldSv(ser::SecField field_enum, std::string_view field_sv) {
    return gen::DurationDays(std::stoul(std::string(field_sv)));
}


inline void Parser::CheckAndAbortForBadInput(ser::SecField field_enum, bool is_consistent) {
    if (!is_consistent) {
        std::cerr << "Field " << magic_enum::enum_name(field_enum)
                  << " is not consistent between file format and implementation.\n";
        std::exit(EXIT_FAILURE);
    }
}

inline std::string_view Parser::SubSvFromField(ser::SecField field_enum) const {
    const auto[start_idx, count, ignore0] = ser::SecFieldStrFormatMap[util::to_underlying(field_enum)];
    return line_sv_.substr(start_idx, count);
}



#endif /* Parser_hpp */
