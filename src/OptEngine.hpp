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

#include "Duty.hpp"
#include "Sector/Sector.hpp"
#include "Solution.hpp"
//#include "ortools/sat/cp_model.h"

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
