//
//  DutyRuleTransit.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/7/22.
//

#include "DutyRuleTransit.hpp"

namespace rule {

template <MinMaxRule MinMaxEnum, std::uint8_t NumEntries>
bool DutyRuleTransit<MinMaxEnum, NumEntries>::Check(const duty::Duty&, const sec::Sector&) const {
    return true;
}

}
