/*
 * @Author: Nick Liu
 * @Date: 2021-11-27 08:03:03
 * @LastEditTime: 2022-09-14 13:27:13
 * @LastEditors: Nick Liu
 * @Description:
 * @FilePath: /CrewOptimizer/src/Definition.hpp
 */
//
//  Definition.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/27/21.
//

#ifndef Definition_hpp
#define Definition_hpp

#include <cstdint>
#include <cstdlib>

#include <array>
#include <string>
#include <string_view>

#include <absl/container/flat_hash_map.h>
#include <boost/container/small_vector.hpp>
#include <folly/FBVector.h>
#include <folly/small_vector.h>

#include "Chrono.hpp"
#include "Utility.hpp"

// #include <boost/chrono.hpp>

namespace ds {
template <class T, std::size_t N>
using SmallStackVec =
    folly::small_vector<T, N, folly::small_vector_policy::NoHeap, std::uint8_t>;

template <class T> using Vector = folly::fbvector<T>;

template <class K, class V> using HashMap = absl::flat_hash_map<K, V>;

// using String = folly::fbstring;
} // namespace ds

namespace dut {

namespace dft {
static constexpr std::uint8_t kDefaultMaxSecsOneDuty = 16;
static constexpr chr::DurationDays kDefaultMaxDutyInDays(4);
static constexpr chr::DurationHours kDefaultMaxTransitInHours(24);
} // namespace dft

static std::uint8_t kMaxSecsOneDuty = dft::kDefaultMaxSecsOneDuty;
static chr::DurationDays kMaxDutyInDays = dft::kDefaultMaxDutyInDays;
static chr::DurationHours kMaxTransitInHours = dft::kDefaultMaxTransitInHours;

} // namespace dut

namespace prg {}

#endif /* Definition_hpp */
