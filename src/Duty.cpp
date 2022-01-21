//
//  Duty.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/27/21.
//

#include "Duty.hpp"

namespace duty {

const sec::Sector& Duty::operator[](std::uint8_t idx) const {
    return sectors_[idx];
}

}
