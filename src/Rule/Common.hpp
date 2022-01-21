//
//  Common.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/12/22.
//

#ifndef Common_hpp
#define Common_hpp

#include "../Definition.hpp"


namespace rule {

enum class RuleDataType {
    
};

template <class K, class V>
class RuleEssense {
    
};

template <class Rep, class Period>
bool WithinDuration(chr::Duration<Rep, Period> dur, chr::Duration<Rep, Period> min, chr::Duration<Rep, Period> max, bool &early_stop) {
    if (dur > max) {
        early_stop = true;
        return false;
    }
    return min < dur;
}

template <class Rep, class Period>
bool LessThanDuration(chr::Duration<Rep, Period> dur, chr::Duration<Rep, Period> max, bool &early_stop) {
    return dur < max;
}

}


#endif /* Common_hpp */
