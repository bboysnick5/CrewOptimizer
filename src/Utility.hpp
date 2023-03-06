//
//  Utility.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/5/21.
//

#ifndef Utility_hpp
#define Utility_hpp

#include <array>
#include <boost/hana.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <tl/expected.hpp>
#include <magic_enum.hpp>
#include <sstream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include "Chrono.hpp"

namespace util {

template <class Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum e) noexcept {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

template <class T>
struct constexpr_false_helper : std::false_type {};

template <class T>
struct is_array : std::is_array<T> {};
template <class T, std::size_t N>
struct is_array<std::array<T, N>> : std::true_type {};
template <class T, std::size_t N>
inline constexpr bool is_array_v = is_array<std::array<T, N>>::value;

/*
template <class Clock>
struct is_chrono_time_point : std::false_type {};
template <class Clock>
struct is_chrono_time_point<std::chrono::time_point<Clock>>:std::true_type{};
template <class Clock>
inline constexpr bool is_chrono_time_point_v =
is_chrono_time_point<Clock>::value;


template <class T>
struct is_chrono_duration : std::false_type {};
template <class R, class P>
struct is_chrono_duration<std::chrono::duration<R, P>> : std::true_type {};
template <class R, class P>
inline constexpr bool is_chrono_duration_v =
is_chrono_duration<std::chrono::duration<R, P>>::value;

template <class Rep1, class Period1, class Rep2, class Period2, class Rep3,
class Period3> bool WithinChronoDurationRange(std::chrono::duration<Rep1,
Period1> test_duraiton, std::chrono::duration<Rep2, Period2> min_duration,
                               std::chrono::duration<Rep3, Period3>
max_duration) { return test_duraiton < max_duration && test_duraiton >
min_duration;
}
*/

// for debug -----------------------
template <auto val>
constexpr void static_print() {
#if !defined(__GNUC__) || defined(__clang__)
    int static_print_is_implemented_only_for_gcc = 0;
#else
    int unused = 0;
#endif
};
//  --------------------------

template <typename ForwardToType = void>
struct PassThruFunctor {
    constexpr decltype(auto) operator()(auto&&... ts) {
        if constexpr (sizeof...(ts) == 1) {
            return std::get<0>(std::forward<decltype(ts)>(ts)...);
        } else if constexpr (!std::same_as<ForwardToType, void>) {
            return std::forward<ForwardToType>(
                {std::forward<decltype(ts)>(ts)...});
        } else {
            static_assert(constexpr_false_helper<ForwardToType>{},
                          "didn't pass in forward to type");
        }
    }
};

// constexpr auto PassThru = []<typename T>(T&& t) { return std::forward<Ts>(t);
// };

constexpr bool isdigit(char c) noexcept { return c >= '0' && c <= '9'; }

constexpr bool isxdigit(char c) noexcept {
    return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

constexpr bool islower(char8_t c) noexcept { return c >= 'a' && c <= 'z'; }

constexpr bool isupper(char8_t c) noexcept { return c >= 'A' && c <= 'Z'; }

constexpr bool isalpha(char8_t c) noexcept { return islower(c) || isupper(c); }

constexpr bool isalnum(char8_t c) noexcept { return isalpha(c) || isdigit(c); }

constexpr int tolower(char8_t c) noexcept {
    return isupper(c) ? c + ('z' - 'Z') : c;
}

constexpr int toupper(char8_t c) noexcept {
    return islower(c) ? c - ('z' - 'Z') : c;
}

template <template <class... Ts> class ApplyToType, class... TempArgs>
struct type_apply;

template <template <class... Ts> class ApplyToType, class... TempArgs>
struct type_apply<ApplyToType, std::tuple<TempArgs...>> {
    using type = ApplyToType<TempArgs...>;
};

template <template <class... Ts> class ApplyToType, class... TempArgs>
using type_apply_t = typename type_apply<ApplyToType, TempArgs...>::type;

template <std::size_t N, class... Ts>
using NthType = std::tuple_element_t<N, std::tuple<Ts...>>;

/*
template <auto V>
requires std::is_enum_v<decltype(V)>
struct EnumConstant
    : std::integral_constant<std::underlying_type_t<decltype(V)>,
                             magic_enum::enum_integer(V)> {
    constexpr decltype(auto) operator()() const noexcept {
        return *magic_enum::enum_cast<decltype(V)>(this->value);
    }
}; */

template <auto EnumValue>
constexpr auto hana_enum_c =
    boost::hana::int_<magic_enum::enum_integer(EnumValue)>{};

template <auto Value, template <typename A, A> typename C>
using automized = C<decltype(Value), Value>;

template <auto Value>
using int_c = automized<Value, std::integral_constant>;

template <class ExpectedRedOp = std::plus<>,
          class UnexpectedRedOp = std::plus<>>
constexpr decltype(auto) AggregateExpected(auto&& first_e, auto&&... rest_es, bool all_expected = true) {
    auto&& result = first_e;
    bool all_expected = first_e.has_value();
    ExpectedRedOp e_red_op;

    auto&& rest_aggregates = AggregateExpected<ExpectedRedOp, UnexpectedRedOp>(
        std::forward<decltype(rest_es)>(rest_es)...);
    if (first_e && rest_aggregates) {
        return ExpectedRedOp{}(first_e, rest_aggregates);
    }

    if (first_e && all_expected) {
        AggregateExpected<ExpectedRedOp, UnexpectedRedOp>(ExpectedRedOp{}(*first_e, ))
    }

    if (... && expecteds) {
        return (... e_red_op expecteds.value());
    }
    for (auto&& e : {rest_es...}) {
        if (all_expected && e.has_value()) {
            result = e_red_op(*result, *e);
        } else {
            all_expected = false;
        }
    if (!expected) {

    }
}

template <class ExpectedRedOp = std::plus<>,
          class UnexpectedRedOp = std::plus<>>
constexpr decltype(auto) AggregateExpected(auto&& e) {
    auto&& result = first_e;
    bool all_expected = first_e.has_value();
    ExpectedRedOp e_red_op;

    if (... && expecteds) {
        return (... e_red_op expecteds.value());
    }
    for (auto&& e : {rest_es...}) {
        if (all_expected && e.has_value()) {
            result = e_red_op(*result, *e);
        } else {
            all_expected = false;
        }
        if (!expected) {
        }
    }
}

    template <typename>
    struct is_tuple : std::false_type {};

    template <typename... T>
    struct is_tuple<std::tuple<T...>> : std::true_type {};

    template <typename... T>
    inline constexpr bool is_tuple_v = is_tuple<T...>::value;

    /*
    template <class Tuple>
    constexpr decltype(auto) unpack_tuple(Tuple&& t) {
        using Indices = std::make_index_sequence<
            std::tuple_size_v<std::remove_reference_t<Tuple>>>;
        return std::get<Indices{}>(std::forward<Tuple>(t))...);
    }

    template <class Tuple, std::size_t... I>
    constexpr decltype(auto) unpack_tuple_impl(Tuple&& t,
                                        std::index_sequence<I...>) {
        return std::get<I>(std::forward<Tuple>(t))...);
    }
    }  // namespace detail

    template <class F, class Tuple>
    constexpr decltype(auto) apply(F&& f, Tuple&& t) {
        return detail::apply_impl(
            std::forward<F>(f), std::forward<Tuple>(t),
            std::make_index_sequence<
                std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    } */

}  // namespace util

#endif /* Utility_hpp */
