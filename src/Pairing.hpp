//
//  Pairing.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/28/21.
//

#ifndef Pairing_hpp
#define Pairing_hpp

#include <vector>

#include "Duty.hpp"

class Pairing {
   public:
    using ContainedType = duty::Duty;

    duty::Duty& operator[](std::uint8_t idx);
    duty::Duty operator[](std::uint8_t idx) const;
    std::uint8_t size() const;

   private:
    chr::DurationMins prg_time_;
    std::uint32_t prg_no_;
    std::vector<duty::Duty> duties_;
    arp::Airport base_;
};

#endif /* Pairing_hpp */
