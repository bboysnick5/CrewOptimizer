//
//  Airport.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/11/22.
//

#ifndef Airport_hpp
#define Airport_hpp

#include "Definition.hpp"



struct Airport {
    
    bool operator == (const Airport& other) const {
        return !(*this != other);
    }
    
    bool operator != (const Airport& other) const {
        return arp_cd != other.arp_cd;
    }
    
    std::array<char, 3> arp_cd;
    chr::DurationMins utc_offset_duration;

};



struct ArpComparator {
    
    ArpComparator(Airport main_base = kDefaultBaseArp) : main_base_(main_base) {}
    
    bool operator() (const Airport &a, const Airport &b) const {
        const auto a_timezone_diff_with_base = chr::abs(main_base_.utc_offset_duration - a.utc_offset_duration);
        const auto b_timezone_diff_with_base = chr::abs(main_base_.utc_offset_duration - b.utc_offset_duration);
        if (a_timezone_diff_with_base < b_timezone_diff_with_base)
            return true;
        if (a_timezone_diff_with_base > b_timezone_diff_with_base)
            return false;
        if (a != b) {
            if (a == main_base_)
                return true;
            if (b == main_base_)
                return false;
            return a.arp_cd < b.arp_cd;
        }
        return false;
    }
    
    constexpr static chr::DurationMins kDefaultBaseUtcOffset{480};
    constexpr static Airport kDefaultBaseArp{{'P', 'E', 'K'}, kDefaultBaseUtcOffset};
    
    Airport main_base_;
};






#endif /* Airport_hpp */
