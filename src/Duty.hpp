//
//  Duty.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/27/21.
//

#ifndef Duty_hpp
#define Duty_hpp

#include <limits>
#include <vector>

#include "Definition.hpp"
#include "Sector/Sector.hpp"

namespace duty {

static constexpr std::uint8_t kMaxSecsOneDuty =
    std::numeric_limits<std::uint8_t>::max();

class Duty {
   public:
    using ContainedType = sec::Sector;

    const sec::Sector& operator[](std::uint8_t idx) const;

   private:
    chr::DurationMins duty_tm_, fdp_tm_, brief_tm_, debrief_tm_;
    ds::SmallStackVec<sec::Sector, kMaxSecsOneDuty> sectors_;
};

}  // namespace duty

#endif /* Duty_hpp */
