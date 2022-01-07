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
    Duty& operator[](std::uint8_t idx);
    Duty operator[](std::uint8_t idx) const;
    std::uint8_t size() const;
    
private:
    gen::DurationMins prg_time_;
    std::uint32_t prg_no_;
    std::vector<Duty> duties_;
    gen::Airport base_;
};



#endif /* Pairing_hpp */
