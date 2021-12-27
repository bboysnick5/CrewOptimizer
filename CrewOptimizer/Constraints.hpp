//
//  Constraints.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/28/21.
//

#ifndef Constraints_hpp
#define Constraints_hpp

#include "ConstraintBase.hpp"

class PrgBaseCst : ConstraintBase {
    virtual bool Check() override;
};

bool PrgBaseCst::Check() {
    return true;
}

#endif /* Constraints_hpp */
