//
//  OptEngine.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/29/21.
//

#include "OptEngine.hpp"

OptEngine::OptEngine(std::span<const Sector> input_sectors) :
input_sectors_(input_sectors), leftover_sectors_(input_sectors), sol_{} {}

void OptEngine::optimize() {
    
}
