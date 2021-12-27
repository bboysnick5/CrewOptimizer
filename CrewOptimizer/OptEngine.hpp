//
//  OptEngine.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/29/21.
//

#ifndef OptEngine_hpp
#define OptEngine_hpp

#include <span>

#include "ortools/sat/cp_model.h"

#include "Solution.hpp"

class OptEngine {
    
    OptEngine(std::span<const Sector> input_sectors);
    
    void optimize();
    
private:
    const std::span<const Sector> input_sectors_;
    std::span<const Sector> leftover_sectors_;
    Solution sol_;
};



#endif /* OptEngine_hpp */
