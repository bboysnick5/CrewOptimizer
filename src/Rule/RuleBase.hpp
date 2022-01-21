//
//  RuleBase.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/7/22.
//


/*
 All kinds of rules:
 1. within a time period. e.g. min and max transit time
 2. within a discrete math range. e.g. max num of sectors in a duty
 3. complement
 4. aircraft change
 
 */

#ifndef RuleBase_hpp
#define RuleBase_hpp

#include <cstdint>

#include <string_view>

#include "Pairing.hpp"

template <class DutyOrPrg>
class RuleBase {
public:
    virtual bool Check(const DutyOrPrg&, const typename DutyOrPrg::ContainedType&) const = 0;
    
private:
    std::string_view rule_name_, rule_desc_;
    std::uint32_t rule_id_;
};

enum class RuleDataType {
    kSingleLine,
    kLinearSearch,
    kHashSearch
};


template <class K, class V>
class RuleEssense {
    
};



#endif /* RuleBase_hpp */
