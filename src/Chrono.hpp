//
//  Chrono.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/12/22.
//

#ifndef Chrono_hpp
#define Chrono_hpp

#include <cstdint>

#include <chrono>
#include <ratio>

namespace chr {

using DurationMins4B = std::chrono::duration<std::int32_t, std::ratio<60>>;
using DurationHours4B = std::chrono::duration<std::int32_t,
                                              std::ratio_multiply<std::ratio<60>, DurationMins4B::period>>;
using DurationDays4B = std::chrono::duration<std::int32_t,
                                             std::ratio_multiply<std::ratio<24>, DurationHours4B::period>>;

using TpMinsUnixEpoch4B = std::chrono::time_point<std::chrono::system_clock, DurationMins4B>;
using TpDaysUnixEpoch4B = std::chrono::time_point<std::chrono::system_clock, DurationDays4B>;



template <class Rep, class Period>
using Duration = std::chrono::duration<Rep, Period>;

using std::chrono::floor;
using std::chrono::abs;

template <class Clock, class DurationType>
using TimePoint = std::chrono::time_point<Clock, DurationType>;

template <class Clock, class DurationType, class Rep, class Period>
auto ToLocal(TimePoint<Clock, DurationType> utc_tp, Duration<Rep, Period> utc_offset) {
    return utc_tp + utc_offset;
}

template <class Clock, class DurationType, class Rep, class Period>
auto ToUtc(TimePoint<Clock, DurationType> local_tp, Duration<Rep, Period> utc_offset) {
    return local_tp - utc_offset;
}

/*
template <class ToDuration, class Rep, class Period>
constexpr auto Floor = std::chrono::floor<ToDuration, Rep, Period>;

template <class Rep, class Period>
constexpr auto Abs = std::chrono::abs<Rep, Period>;
*/

}



#endif /* Chrono_hpp */
