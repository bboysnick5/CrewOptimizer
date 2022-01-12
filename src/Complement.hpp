//
//  Complement.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/28/21.
//

#ifndef Complement_hpp
#define Complement_hpp

#include <cstdint>

#include <magic_enum.hpp>

#include <array>

enum class FDRank : std::uint8_t {
    kCap = 0,
    kRCap,
    kFO,
    kRFO,
    kSO,
};

enum class CCRank : std::uint8_t {
    kCF = 0,
    kPS,
    kFA,
    kFB,
    kSS,
};

enum class AMRank : std::uint8_t {
    kAMCF,
    kAMSF,
    kAMST，
};

class Complement {
    std::array<std::uint8_t, static_cast<std::uint8_t>(magic_enum::enum_count<FDRank>())> fd_comp_{0};
    std::array<std::uint8_t, static_cast<std::uint8_t>(magic_enum::enum_count<CCRank>())> cc_comp_{0};
    std::array<std::uint8_t, static_cast<std::uint8_t>(magic_enum::enum_count<AMRank>())> am_comp_{0};
};

#endif /* Complement_hpp */
