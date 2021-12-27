//
//  ConstraintBase.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/28/21.
//

#ifndef ConstraintBase_hpp
#define ConstraintBase_hpp

#include <string>

#include "Pairing.hpp"

class ConstraintBase {
public:
    virtual bool Check() = 0;
    
protected:
    std::string rule_name_, rule_desc_;
    Pairing prg;
};

#endif /* ConstraintBase_hpp */
