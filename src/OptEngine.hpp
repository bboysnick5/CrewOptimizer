//
//  OptEngine.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/29/21.
//

#ifndef OptEngine_hpp
#define OptEngine_hpp

#include <span>
#include <vector>

#include "ortools/sat/cp_model.h"

#include "Duty.hpp"
#include "Sector.hpp"
#include "Solution.hpp"

class OptEngine {
    
    
    
    OptEngine(std::span<const sec::Sector> input_sectors);
    
    void optimize();
    
private:
    
    void EnumDuties();
    
    const std::span<const sec::Sector> input_sectors_;
    std::vector<duty::Duty> duty_pool_;
    std::span<const sec::Sector> leftover_sectors_;
    Solution sol_;
};



#endif /* OptEngine_hpp */
