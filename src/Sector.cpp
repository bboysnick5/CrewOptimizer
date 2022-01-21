//
//  Sector.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#include "Sector.hpp"

#include <ctime>

namespace sec {

Sector::Sector(const Parser<ser::SecField>& parser) : comp_idx_(0), sec_svs_idx_(parser.GetSecSvIdx()) {
    
    // flight number and leg number
    fn_ = {parser.ParseField<decltype(sec::FltNum::fn_numeric), ser::SecField::kFltNo>(),
           parser.ParseField<decltype(sec::FltNum::fn_suffix), ser::SecField::kFltNoSuffix>()};
    leg_no_ = parser.ParseField<sec::LegNoType, ser::SecField::kLegNo>();
    
    // next flight number
    next_fn_ = {parser.ParseField<decltype(sec::FltNum::fn_numeric), ser::SecField::kNextFltNo>(),
                parser.ParseField<decltype(sec::FltNum::fn_suffix), ser::SecField::kNextFltNoSuffix>()};
    
    // dep arr airport
    std::int8_t dep_arp_utc_offset_sign = parser.ParseField<char, ser::SecField::kDepArpUtcOffsetHhMmSign>() == '+' ? 1 : -1;
    dep_arp_ = {parser.ParseField<decltype(Airport::arp_cd), ser::SecField::kDepArpCd>(),
                dep_arp_utc_offset_sign * parser.ParseField<chr::DurationMins4B, ser::SecField::kDepArpUtcOffsetHhMm>()};
    std::int8_t arr_arp_utc_offset_sign = parser.ParseField<char, ser::SecField::kArrArpUtcOffsetHhMmSign>() == '+' ? 1 : -1;
    arr_arp_ = {parser.ParseField<decltype(Airport::arp_cd), ser::SecField::kArrArpCd>(),
                arr_arp_utc_offset_sign * parser.ParseField<chr::DurationMins4B, ser::SecField::kArrArpUtcOffsetHhMm>()};
    
    // block time
    const auto blk_tm = parser.ParseField<chr::DurationMins4B, ser::SecField::kBlkTm>();
    
    // dep/arr local or utc time multiplier
    std::uint8_t file_local_utc_multiplier = parser.ParseField<decltype(ser::kLocalInd), ser::SecField::kLocalUtcInd>() == ser::kLocalInd ? 1 : 0;
    
    // departure date time
    const auto dep_ymd_local = parser.ParseField<chr::TpDaysUnixEpoch4B, ser::SecField::kDepYmd>();
    const auto dep_hm_local = parser.ParseField<chr::DurationMins4B, ser::SecField::kDepHhMm>();
    dep_dt_utc_ = chr::ToUtc(dep_ymd_local + dep_hm_local, file_local_utc_multiplier * dep_arp_.utc_offset_duration);
    
    // arrival date time
    arr_dt_utc_ = dep_dt_utc_ + blk_tm;
    const std::int8_t arr_day_offset_sign = parser.ParseField<char, ser::SecField::kArrDayOffsetSign>() == '+' ? 1 : -1;
    const auto arr_ymd_local = dep_ymd_local + arr_day_offset_sign * parser.ParseField<chr::DurationDays4B, ser::SecField::kArrDayOffset>();
    const auto arr_ymdhm_local = arr_ymd_local + parser.ParseField<chr::DurationMins4B, ser::SecField::kArrHhMm>();
    if (arr_dt_utc_ != chr::ToUtc(arr_ymdhm_local, file_local_utc_multiplier * arr_arp_.utc_offset_duration)) {
        std::cerr << "Error reading schedule file. Different arrival time\n";
        std::exit(EXIT_FAILURE);
    }
    
    // equipment
    eqp_cd_ = parser.ParseField<sec::EqpCd, ser::SecField::kEqpCd>();
    
    // Region type
    reg_type_ = parser.ParseField<sec::RegionType, ser::SecField::kRegionType>();
    
}

std::string Sector::SerializeGivenSecStr(std::string sec_str) const {
    constexpr auto& arr_ymd_str_format = ser::SecFieldEnumFieldFormatMap[util::to_underlying(ser::SecField::kArrYmd)];
    sec_str.replace(arr_ymd_str_format.start_idx, arr_ymd_str_format.length,
                    date::format(arr_ymd_str_format.format_sv.data(),
                                 chr::ToLocal(arr_dt_utc_,  arr_arp_.utc_offset_duration)));
    sec_str[ser::SecFieldEnumFieldFormatMap[util::to_underlying(ser::SecField::kSolComp)].start_idx] = comp_idx_ + '0';
    constexpr auto& mysterious_padding_str_format = ser::SecFieldEnumFieldFormatMap[util::to_underlying(ser::SecField::kMysteriousPadding)];
    sec_str.replace(mysterious_padding_str_format.start_idx, mysterious_padding_str_format.length,
                    mysterious_padding_str_format.format_sv);
    return sec_str;
}

ser::SecSvIdxType Sector::GetSecSvIdx() const {
    return sec_svs_idx_;
}


bool Sector::operator == (const Sector& other) const {
    return sec_svs_idx_ == other.sec_svs_idx_;
}

bool SecComparator::operator () (const Sector &a, const Sector &b) const {
    const auto a_dep_ymd_local = chr::floor<chr::DurationDays4B>(chr::ToLocal(a.dep_dt_utc_, a.dep_arp_.utc_offset_duration));
    const auto b_dep_ymd_local = chr::floor<chr::DurationDays4B>(chr::ToLocal(b.dep_dt_utc_, b.dep_arp_.utc_offset_duration));
    if (a_dep_ymd_local < b_dep_ymd_local)
        return true;
    if (a_dep_ymd_local > b_dep_ymd_local)
        return false;
    return arp_comp_(a.dep_arp_, b.dep_arp_) || (a.dep_arp_ == b.dep_arp_ && a.dep_dt_utc_ < b.dep_dt_utc_);
}

sec::EqpCd Sector::GetEqpCd() const {
    return eqp_cd_;
}


}
