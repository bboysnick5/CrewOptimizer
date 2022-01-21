//
//  DutyRuleTransit.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/7/22.
//

#ifndef DutyRuleTransit_hpp
#define DutyRuleTransit_hpp

#include <cstdint>

#include <array>
#include <variant>
#include <type_traits>

#include <magic_enum.hpp>

#include "Definition.hpp"
#include "Duty.hpp"
#include "DutyRuleBase.hpp"

namespace rule {

static constexpr std::uint8_t VecMapThreshold = 16;

enum class MinMaxRule : std::uint8_t {
    kMinRule = 0,
    kMaxRule = 1
};

template <MinMaxRule MinMaxEnum, std::uint8_t NumEntries>
class DutyRuleTransit final : public RuleBase<duty::Duty> {
public:
    
    virtual bool Check(const duty::Duty&, const sec::Sector&) const override;
    
private:
    

    
    using ConnectTypeDurationMapArr = std::array<chr::DurationMins4B, magic_enum::enum_count<sec::OprType>()>;
    /*
    struct CharArrConnectTypeDurationMapPair {
        std::array<>
        ConnectTypeDurationMapArr map_arr;
    }; */
    
    std::conditional_t<NumEntries == 1, ConnectTypeDurationMapArr,
                       std::conditional_t<NumEntries <= VecMapThreshold,
                                          ds::SmallStackVec<std::pair<std::string_view,
                                                            ConnectTypeDurationMapArr>, NumEntries>,
                                          ds::HashMap<std::string_view, ConnectTypeDurationMapArr>>>
                      rule_tb_;

};

}

#endif /* DutyRuleTransit_hpp */
