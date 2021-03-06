//
//  DutyRuleBase.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/7/22.
//

#ifndef DutyRuleBase_hpp
#define DutyRuleBase_hpp

#include "Duty.hpp"
#include "RuleBase.hpp"

class DutyRuleBase : public RuleBase<duty::Duty> {
    
protected:
    const duty::Duty& duty;
};

#endif /* DutyRuleBase_hpp */
