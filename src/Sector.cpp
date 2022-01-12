//
//  Sector.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#include "Sector.hpp"

#include <ctime>

/*
Sector::Sector(Sector&& other) noexcept : id_(std::move(other.id_)), al_cd_(std::move(other.al_cd_)),
    dep_dt_utc_(std::move(other.dep_dt_utc_)), arr_dt_utc_(std::move(other.arr_dt_utc_)), fn_(std::move(other.fn_)),
    next_fn_(std::move(other.next_fn_)), leg_no_(std::move(other.leg_no_)), eqp_cd_(std::move(other.eqp_cd_)),
    dep_arp_(std::move(other.dep_arp_)), arr_arp_(std::move(other.arr_arp_)), blk_tm_(std::move(other.blk_tm_)),
    comp_(std::move(other.comp_)), comp_idx_(std::move(other.comp_idx_)), opr_type_(std::move(other.opr_type_)),
    reg_type_(std::move(other.reg_type_)) {}
 
Sector& Sector::operator = (Sector &&other) noexcept {
    if (this != &other) {
        id_ = std::move(other.id_);
        al_cd_ = std::move(other.al_cd_);
        dep_dt_utc_= std::move(other.dep_dt_utc_);
        arr_dt_utc_= std::move(other.arr_dt_utc_);
        fn_= std::move(other.fn_);
        next_fn_= std::move(other.next_fn_);
        leg_no_= std::move(other.leg_no_);
        eqp_cd_= std::move(other.eqp_cd_);
        dep_arp_= std::move(other.dep_arp_);
        arr_arp_= std::move(other.arr_arp_);
        blk_tm_ = std::move(other.blk_tm_);
        comp_= std::move(other.comp_);
        comp_idx_= std::move(other.comp_idx_);
        opr_type_= std::move(other.opr_type_);
        reg_type_= std::move(other.reg_type_);
    }
    return *this;
}
*/

Sector::Sector(const Parser& parser) : comp_idx_(0), sec_svs_idx_(parser.GetSecSvIdx()) {
    // airline code
    al_cd_ = parser.ParseField<AirlineCode>(ser::SecField::kAirlineCode);
    
    // flight number and leg number
    fn_ = {parser.ParseField<decltype(Sector::FltNum::fn_numeric)>(ser::SecField::kFltNo),
           parser.ParseField<decltype(Sector::FltNum::fn_suffix)>(ser::SecField::kFltNoSuffix)};
    leg_no_ = parser.ParseField<Sector::LegNoType>(ser::SecField::kLegNo);
    
    // next flight number
    next_fn_ = {parser.ParseField<decltype(Sector::FltNum::fn_numeric)>(ser::SecField::kNextFltNo),
                    parser.ParseField<decltype(Sector::FltNum::fn_suffix)>(ser::SecField::kNextFltNoSuffix)};
    
    // dep arr airport
    std::int8_t dep_arp_utc_offset_sign = parser.ParseField<char>(ser::SecField::kDepArpUtcOffsetHhMmSign) == '+' ? 1 : -1;
    dep_arp_ = {parser.ParseField<decltype(Airport::arp_cd)>(ser::SecField::kDepArpCd),
                dep_arp_utc_offset_sign * parser.ParseField<chr::DurationMins>(ser::SecField::kDepArpUtcOffsetHhMm)};
    std::int8_t arr_arp_utc_offset_sign = parser.ParseField<char>(ser::SecField::kArrArpUtcOffsetHhMmSign) == '+' ? 1 : -1;
    arr_arp_ = {parser.ParseField<decltype(Airport::arp_cd)>(ser::SecField::kArrArpCd),
                arr_arp_utc_offset_sign * parser.ParseField<chr::DurationMins>(ser::SecField::kArrArpUtcOffsetHhMm)};
    
    // block time
    blk_tm_ = parser.ParseField<chr::DurationMins>(ser::SecField::kBlkTm);
    
    // dep/arr local or utc time multiplier
    std::uint8_t file_local_utc_multiplier = parser.ParseField<decltype(ser::kLocalInd)>(ser::SecField::kLocalUtcInd) == ser::kLocalInd ? 1 : 0;
    
    // departure date time
    const auto dep_ymd_local = parser.ParseField<chr::TpSysClkDays>(ser::SecField::kDepYmd);
    const auto dep_hm_local = parser.ParseField<chr::DurationMins>(ser::SecField::kDepHhMm);
    dep_dt_utc_ = chr::ToUtc(dep_ymd_local + dep_hm_local, file_local_utc_multiplier * dep_arp_.utc_offset_duration);
    
    // arrival date time
    arr_dt_utc_ = dep_dt_utc_ + blk_tm_;
    const std::int8_t arr_day_offset_sign = parser.ParseField<char>(ser::SecField::kArrDayOffsetSign) == '+' ? 1 : -1;
    const auto arr_ymd_local = dep_ymd_local + arr_day_offset_sign * parser.ParseField<chr::DurationDays>(ser::SecField::kArrDayOffset);
    const auto arr_ymdhm_local = arr_ymd_local + parser.ParseField<chr::DurationMins>(ser::SecField::kArrHhMm);
    if (arr_dt_utc_ != chr::ToUtc(arr_ymdhm_local, file_local_utc_multiplier * arr_arp_.utc_offset_duration)) {
        std::cerr << "Error reading schedule file. Different arrival time\n";
        std::exit(EXIT_FAILURE);
    }
    
    // equipment
    eqp_cd_ = parser.ParseField<Sector::EqpCd>(ser::SecField::kEqpCd);
    
    // Region type
    reg_type_ = parser.ParseField<Sector::RegionType>(ser::SecField::kRegionType);
    
}

std::string Sector::SerializeGivenSecStr(std::string sec_str) const {
    constexpr auto& arr_ymd_str_format = ser::SecFieldStrFormatMap[util::to_underlying(ser::SecField::kArrYmd)];
    sec_str.replace(arr_ymd_str_format.start_idx, arr_ymd_str_format.length,
                    date::format(arr_ymd_str_format.str_chrono_format.data(), chr::ToLocal(arr_dt_utc_,  arr_arp_.utc_offset_duration)));
    sec_str[ser::SecFieldStrFormatMap[util::to_underlying(ser::SecField::kSolComp)].start_idx] = comp_idx_ + '0';
    constexpr auto& mysterious_padding_str_format = ser::SecFieldStrFormatMap[util::to_underlying(ser::SecField::kMysteriousPadding)];
    sec_str.replace(mysterious_padding_str_format.start_idx, mysterious_padding_str_format.length,
                    std::string(mysterious_padding_str_format.length, ' '));
    return sec_str;
}

ser::SecSvIdxType Sector::GetSecSvIdx() const {
    return sec_svs_idx_;
}


bool Sector::operator == (const Sector& other) const {
    return sec_svs_idx_ == other.sec_svs_idx_;
}


bool SecComparator::operator () (const Sector &a, const Sector &b) const {
    const auto a_dep_ymd_local = chr::floor<chr::DurationDays>(chr::ToLocal(a.dep_dt_utc_, a.dep_arp_.utc_offset_duration));
    const auto b_dep_ymd_local = chr::floor<chr::DurationDays>(chr::ToLocal(b.dep_dt_utc_, b.dep_arp_.utc_offset_duration));
    if (a_dep_ymd_local < b_dep_ymd_local)
        return true;
    if (a_dep_ymd_local > b_dep_ymd_local)
        return false;
    return arp_comp_(a.dep_arp_, b.dep_arp_) || (a.dep_arp_ == b.dep_arp_ && a.dep_dt_utc_ < b.dep_dt_utc_);
}
