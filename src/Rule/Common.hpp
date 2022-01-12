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

template <typename Rep, class Period>
bool WithinDuration(chr::Duration<Rep, Period> dur, chr::Duration<Rep, Period> min, chr::Duration<Rep, Period> max, bool &early_stop) {
    if (dur > max) {
        early_stop = true;
        return false;
    }
    return min < dur;
}

template <typename Rep, class Period>
bool LessThanDuration(chr::Duration<Rep, Period> dur, chr::Duration<Rep, Period> max, bool &early_stop) {
    return dur < max;
}

}


#endif /* Common_hpp */
