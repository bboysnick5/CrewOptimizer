//
//  Duty.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/27/21.
//

#ifndef Duty_hpp
#define Duty_hpp

#include <vector>

#include "Definition.hpp"
#include "Sector.hpp"

class Duty {
    
    chr::DurationMins duty_tm_, fdp_tm_, brief_tm_, debrief_tm_;
    std::vector<Sector> sectors_;
};

#endif /* Duty_hpp */
