//
//  DutyRuleTransit.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/7/22.
//

#ifndef DutyRuleTransit_hpp
#define DutyRuleTransit_hpp

#include "Definition.hpp"
#include "DutyRuleBase.hpp"

class DutyRuleTransit final : public DutyRuleBase {
public:
    virtual bool Check() const override;
    
private:
    chr::DurationMins min_transit_duration_;
    chr::DurationMins max_transit_duration_;
};

#endif /* DutyRuleTransit_hpp */
