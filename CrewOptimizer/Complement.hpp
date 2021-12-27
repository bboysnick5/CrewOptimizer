//
//  Complement.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/28/21.
//

#ifndef Complement_hpp
#define Complement_hpp

#include <cstdint>

#include <array>

enum class FDRank : std::uint8_t {
    kCap = 0,
    kRCap,
    kFO,
    kRFO,
    kSO,
    Count
};

enum class CCRank : std::uint8_t {
    kCF = 0,
    kPS,
    kFA,
    kFB,
    kSS,
    Count
};

enum class AMRank : std::uint8_t {
    kAMCF,
    kAMSF,
    kAMST,
    Count
};

class Complement {
    std::array<std::uint8_t, static_cast<std::uint8_t>(FDRank::Count)> fd_comp_{0};
    std::array<std::uint8_t, static_cast<std::uint8_t>(CCRank::Count)> cc_comp_{0};
    std::array<std::uint8_t, static_cast<std::uint8_t>(AMRank::Count)> am_comp_{0};
};

#endif /* Complement_hpp */
