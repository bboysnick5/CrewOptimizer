//
//  Airport.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/11/22.
//

#ifndef Airport_hpp
#define Airport_hpp

#include "Definition.hpp"
#include "file/FieldFormat.hpp"
#include "file/Parser.hpp"

namespace arp {

using ArpCdType = std::uint16_t;
using UtcOffsetDurType = chr::DurationMins;

struct Airport {
    // constexpr Airport(auto&& arp_cd_arg, auto&& utc_offset_arg)
    //   : arp_cd(arp_cd_arg), utc_offset_duration(utc_offset_arg) {}

    constexpr bool operator==(const Airport& other) const {
        return !(*this != other);
    }

    constexpr bool operator!=(const Airport& other) const {
        return arp_cd != other.arp_cd;
    }

    ArpCdType arp_cd;
    UtcOffsetDurType utc_offset_duration;
};

struct ArpComparator {
    ArpComparator(Airport main_base = kDefaultBaseArp)
        : main_base_(main_base) {}

    bool operator()(const Airport& a, const Airport& b) const {
        const auto a_timezone_diff_with_base =
            chr::abs(main_base_.utc_offset_duration - a.utc_offset_duration);
        const auto b_timezone_diff_with_base =
            chr::abs(main_base_.utc_offset_duration - b.utc_offset_duration);
        if (a_timezone_diff_with_base < b_timezone_diff_with_base) return true;
        if (a_timezone_diff_with_base > b_timezone_diff_with_base) return false;
        if (a != b) {
            if (a == main_base_) return true;
            if (b == main_base_) return false;
            return a.arp_cd < b.arp_cd;
        }
        return false;
    }

    constexpr static chr::DurationMins kDefaultBaseUtcOffset{480};
    constexpr static Airport kDefaultBaseArp = {
        *file::parser::ParseTokenDirect<
            ArpCdType, file::FieldFormat{0, 3, file::kEmptyFormat},
            file::parser::Pattern::kConvertAlnumToUint>(
            std::string_view("PEK")),
        kDefaultBaseUtcOffset};

    Airport main_base_;
};

}  // namespace arp

#endif /* Airport_hpp */
