//
//  Pairing.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/28/21.
//

#include "Pairing.hpp"

std::uint8_t Pairing::size() const {
    return duties_.size();
}

Duty& Pairing::operator[](std::uint8_t idx) {
    return duties_[idx];
}

Duty Pairing::operator[](std::uint8_t idx) const {
    return duties_[idx];
}
