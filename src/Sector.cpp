//
//  Sector.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#include "Sector.hpp"

#include <ctime>

Sector::Sector(const Parser& parser) : comp_idx(0) {
    // airline code
    al_cd_ = parser.ParseField<gen::AirlineCode>(ser::SecField::kAirlineCode);
    
    // flight number and leg number
    fn_ = {parser.ParseField<decltype(sec::FltNum::fn_numeric)>(ser::SecField::kFltNo),
           parser.ParseField<decltype(sec::FltNum::fn_suffix)>(ser::SecField::kFltNoSuffix)};
    leg_no_ = parser.ParseField<sec::LegNo>(ser::SecField::kLegNo);
    
    // next flight number
    next_fn_ = {parser.ParseField<decltype(sec::FltNum::fn_numeric)>(ser::SecField::kNextFltNo),
                    parser.ParseField<decltype(sec::FltNum::fn_suffix)>(ser::SecField::kNextFltNoSuffix)};
    
    // dep arr airport
    std::int8_t dep_arp_utc_offset_sign = parser.ParseField<char>(ser::SecField::kDepArpUtcOffsetHhMmSign) == '+' ? 1 : -1;
    dep_arp_ = {parser.ParseField<decltype(gen::Airport::arp_cd)>(ser::SecField::kDepArpCd),
                dep_arp_utc_offset_sign * parser.ParseField<gen::DurationMins>(ser::SecField::kDepArpUtcOffsetHhMm)};
    std::int8_t arr_arp_utc_offset_sign = parser.ParseField<char>(ser::SecField::kArrArpUtcOffsetHhMmSign) == '+' ? 1 : -1;
    arr_arp_ = {parser.ParseField<decltype(gen::Airport::arp_cd)>(ser::SecField::kArrArpCd),
                arr_arp_utc_offset_sign * parser.ParseField<gen::DurationMins>(ser::SecField::kArrArpUtcOffsetHhMm)};
    
    // block time
    blk_tm_ = parser.ParseField<gen::DurationMins>(ser::SecField::kBlkTm);
    
    // dep/arr local or utc time multiplier
    std::uint8_t file_local_utc_multiplier = parser.ParseField<decltype(ser::kLocalInd)>(ser::SecField::kLocalUtcInd) == ser::kLocalInd ? 1 : 0;
    
    // departure date time
    const auto dep_ymd_local = parser.ParseField<gen::TpSysClkDays>(ser::SecField::kDepYmd);
    const auto dep_hm_local = parser.ParseField<gen::DurationMins>(ser::SecField::kDepHhMm);
    dep_dt_utc_ = dep_ymd_local + dep_hm_local - file_local_utc_multiplier * dep_arp_.utc_offset_duration;
    
    // arrival date time
    arr_dt_utc_ = dep_dt_utc_ + blk_tm_;
    const std::int8_t arr_day_offset_sign = parser.ParseField<char>(ser::SecField::kArrDayOffsetSign) == '+' ? 1 : -1;
    const auto arr_ymd_local = dep_ymd_local + arr_day_offset_sign * parser.ParseField<gen::DurationDays>(ser::SecField::kArrDayOffset);
    const auto arr_ymdhm_local = arr_ymd_local + parser.ParseField<gen::DurationMins>(ser::SecField::kArrHhMm);
    if (arr_dt_utc_ != arr_ymdhm_local - file_local_utc_multiplier * arr_arp_.utc_offset_duration) {
        std::cerr << "Error reading schedule file. Different arrival time\n";
        std::exit(EXIT_FAILURE);
    }
    
    // equipment
    eqp_cd_ = parser.ParseField<sec::EqpCd>(ser::SecField::kEqpCd);
    
    // Region type
    reg_type_ = parser.ParseField<sec::RegionType>(ser::SecField::kRegionType);
    
    // ID
    id_ = parser.ParseField<sec::Id>(ser::SecField::kId);
}

void Sector::SerializeAlterOrigStr(std::string &sec_str) const {
    constexpr auto& arr_ymd_str_format = ser::SecFieldStrFormatMap[util::to_underlying(ser::SecField::kArrYmd)];
    sec_str.replace(arr_ymd_str_format.start_idx, arr_ymd_str_format.length,
                    date::format(arr_ymd_str_format.str_chrono_format.data(), arr_dt_utc_ + arr_arp_.utc_offset_duration));
    sec_str[ser::SecFieldStrFormatMap[util::to_underlying(ser::SecField::kSolComp)].start_idx] = comp_idx + '0';
    constexpr auto& mysterious_padding_str_format = ser::SecFieldStrFormatMap[util::to_underlying(ser::SecField::kMysteriousPadding)];
    sec_str.replace(mysterious_padding_str_format.start_idx, mysterious_padding_str_format.length,
                    std::string(mysterious_padding_str_format.length, ' '));
}
