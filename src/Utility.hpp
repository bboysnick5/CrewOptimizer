//
//  Utility.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/5/21.
//

#ifndef Utility_hpp
#define Utility_hpp

#include <ctime>

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace util {

template <typename Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class T>
struct is_array:std::is_array<T>{};
template<class T, std::size_t N>
struct is_array<std::array<T,N>>:std::true_type{};
template <class T, std::size_t N>
inline constexpr bool is_array_v = is_array<std::array<T, N>>::value;


template <typename Clock>
struct is_chrono_time_point : std::false_type {};
template <typename Clock>
struct is_chrono_time_point<std::chrono::time_point<Clock>>:std::true_type{};
template <typename Clock>
inline constexpr bool is_chrono_time_point_v = is_chrono_time_point<Clock>::value;


template <typename T>
struct is_chrono_duration : std::false_type {};
template <typename R, typename P>
struct is_chrono_duration<std::chrono::duration<R, P>> : std::true_type {};
template <typename R, typename P>
inline constexpr bool is_chrono_duration_v = is_chrono_duration<std::chrono::duration<R, P>>::value;

template <class Rep1, class Period1, class Rep2, class Period2, class Rep3, class Period3>
bool WithinChronoDurationRange(std::chrono::duration<Rep1, Period1> test_duraiton,
                               std::chrono::duration<Rep2, Period2> min_duration,
                               std::chrono::duration<Rep3, Period3> max_duration) {
    return test_duraiton < max_duration && test_duraiton > min_duration;
}



/*
template <std::string_view const&... Svs>
struct sv_concat
{
    // Join all strings into a single std::array of chars
    static constexpr auto impl() noexcept
    {
        constexpr std::size_t len = (Svs.size() + ... + 0);
        std::array<char, len + 1> arr{};
        auto append = [i = 0, &arr](auto const& s) mutable {
            for (auto c : s) arr[i++] = c;
        };
        (append(Svs), ...);
        arr[len] = 0;
        return arr;
    }
    // Give the joined string static storage
    static constexpr auto arr = impl();
    // View as a std::string_view
    static constexpr std::string_view value {arr.data(), arr.size() - 1};
};
// Helper to get the value out
template <std::string_view const&... Svs>
static constexpr auto sv_concat_v = sv_concat<Svs...>::value;
*/

}

#endif /* Utility_hpp */
