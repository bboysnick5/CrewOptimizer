//
//  Sector.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#include "Sector.hpp"

#include <ctime>

namespace sec {

std::optional<Sector> Sector::Create(std::string_view line,
                                     file::LineSvIdxType idx) {
    return detail::WrapUnique(new Sector());
}

Sector::Sector(std::string_view line, file::LineSvIdxType idx)
    : comp_idx_(0), sec_svs_idx_(idx) {
    fn_ = file::parser::ParseLine<FltNumType, file::SecLineField::kFltNo,
                                  file::parser::Pattern::kTranslateAlnumToUint>(
        line);
    leg_no_ =
        file::parser::ParseLine<LegNoType, file::SecLineField::kLegNo,
                                file::parser::Pattern::kTranslateNumToUint>(
            line);
    next_fn_ =
        file::parser::ParseLine<FltNumType, file::SecLineField::kNextFltNo,
                                file::parser::Pattern::kTranslateAlnumToUint>(
            line);
    dep_arp_ = {
        arp_cd(file::parser::ParseLine<
               arp::ArpCdType, file::SecLineField::kDepArpCd,
               file::parser::Pattern::kTranslateAlnumToUint>(line)),
        utc_offset_duration(
            file::parser::ParseLine<arp::UtcOffsetDurType,
                                    file::SecLineField::kDepArpUtcOffsetHhMm,
                                    file::parser::Pattern::kChronoDirect>(
                line))};

    arr_arp_ = {
        arp_cd(file::parser::ParseLine<
               arp::ArpCdType, file::SecLineField::kArrArpCd,
               file::parser::Pattern::kTranslateAlnumToUint>(line)),
        utc_offset_duration(
            file::parser::ParseLine<arp::UtcOffsetDurType,
                                    file::SecLineField::kArrArpUtcOffsetHhMm,
                                    file::parser::Pattern::kChronoDirect>(
                line))};

    // block time
    const auto blk_tm =
        file::parser::ParseLine<chr::DurationMins, file::SecLineField::kBlkMins,
                                file::parser::Pattern::kChronoDirect>(line);

    // dep/arr local or utc time multiplier
    std::uint8_t file_local_utc_multiplier =
        file::parser::ParseLine<file::SecLocalUtcInd, file::SecLineField,
                                file::SecLineField::kLocalUtcInd,
                                file::parser::Pattern::kEnumStrMatching>(
            line) == file::SecLocalUtcInd::kUtc
            ? 0
            : 1;

    // departure date time
    const auto dep_ymd_local =
        parser.ParseField<chr::TpDaysUnixEpoch, file::SecLineField::kDepYmd>();
    const auto dep_hm_local =
        parser.ParseField<chr::DurationMins, file::SecLineField::kDepHhMm>();
    dep_dt_utc_ =
        chr::ToUtc(dep_ymd_local + dep_hm_local,
                   file_local_utc_multiplier * dep_arp_.utc_offset_duration);

    // arrival date time
    arr_dt_utc_ = dep_dt_utc_ + blk_tm;
    const std::int8_t arr_day_offset_sign =
        parser.ParseField<char, file::SecLineField::kArrDayOffsetSign>() == '+'
            ? 1
            : -1;
    const auto arr_ymd_local =
        dep_ymd_local +
        arr_day_offset_sign *
            parser.ParseField<chr::DurationDays,
                              file::SecLineField::kArrDayOffset>();
    const auto arr_ymdhm_local =
        arr_ymd_local +
        parser.ParseField<chr::DurationMins, file::SecLineField::kArrHhMm>();
    if (arr_dt_utc_ !=
        chr::ToUtc(arr_ymdhm_local,
                   file_local_utc_multiplier * arr_arp_.utc_offset_duration)) {
        std::cerr << "Error reading schedule file. Different arrival time\n";
        std::exit(EXIT_FAILURE);
    }

    // equipment
    eqp_cd_ = parser.ParseField<sec::EqpCd, file::SecLineField::kEqpCd>();

    // Region type
    reg_type_ =
        parser.ParseField<sec::RegionType, file::SecLineField::kRegionType>();
}

std::string Sector::SerializeGivenSecStr(std::string sec_str) const {
    constexpr auto& arr_ymd_str_format =
        file::SecLineFieldEnumFieldFormatMap[util::to_underlying(
            file::SecLineField::kArrYmd)];
    sec_str.replace(
        arr_ymd_str_format.start_idx, arr_ymd_str_format.length,
        date::format(arr_ymd_str_format.format_str.data(),
                     chr::ToLocal(arr_dt_utc_, arr_arp_.utc_offset_duration)));
    sec_str[file::SecLineFieldEnumFieldFormatMap
                [util::to_underlying(file::SecLineField::kSolComp)]
                    .start_idx] = comp_idx_ + '0';
    constexpr auto& mysterious_padding_str_format =
        file::SecLineFieldEnumFieldFormatMap[util::to_underlying(
            file::SecLineField::kMysteriousPadding)];
    sec_str.replace(mysterious_padding_str_format.start_idx,
                    mysterious_padding_str_format.length,
                    mysterious_padding_str_format.format_str);
    return sec_str;
}

file::SecSvIdxType Sector::GetSecSvIdx() const { return sec_svs_idx_; }

bool Sector::operator==(const Sector& other) const {
    return sec_svs_idx_ == other.sec_svs_idx_;
}

bool SecComparator::operator()(const Sector& a, const Sector& b) const {
    const auto a_dep_ymd_local = chr::floor<chr::DurationDays>(
        chr::ToLocal(a.dep_dt_utc_, a.dep_arp_.utc_offset_duration));
    const auto b_dep_ymd_local = chr::floor<chr::DurationDays>(
        chr::ToLocal(b.dep_dt_utc_, b.dep_arp_.utc_offset_duration));
    if (a_dep_ymd_local < b_dep_ymd_local) return true;
    if (a_dep_ymd_local > b_dep_ymd_local) return false;
    return arp_comp_(a.dep_arp_, b.dep_arp_) ||
           (a.dep_arp_ == b.dep_arp_ && a.dep_dt_utc_ < b.dep_dt_utc_);
}

sec::EqpCd Sector::GetEqpCd() const { return eqp_cd_; }

}  // namespace sec
