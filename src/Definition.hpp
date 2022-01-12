//
//  Definition.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/27/21.
//

#ifndef Definition_hpp
#define Definition_hpp

#include <ctime>
#include <cstdint>
#include <cstdlib>

#include <array>
#include <chrono>
#include <string>
#include <string_view>

#include <magic_enum.hpp>

#include "Chrono.hpp"
#include "Utility.hpp"

//#include <boost/chrono.hpp>




namespace dut {

namespace dft {
static constexpr std::uint8_t kDefaultMaxSecsOneDuty = 16;
static constexpr chr::DurationDays kDefaultMaxDutyInDays(4);
static constexpr chr::DurationHours kDefaultMaxTransitInHours(24);
}

static std::uint8_t kMaxSecsOneDuty = dft::kDefaultMaxSecsOneDuty;
static chr::DurationDays kMaxDutyInDays = dft::kDefaultMaxDutyInDays;
static chr::DurationHours kMaxTransitInHours = dft::kDefaultMaxTransitInHours;

}

namespace prg {


}


namespace ser {

using SecSvIdxType = std::uint16_t;

static constexpr std::string_view kYmdFormat{"%m%d%Y"};
static constexpr std::string_view kHmFormat{"%H%M"};
static constexpr std::string_view kDayFormat{"%d"};


static constexpr char kLocalInd = 'L';

enum class SecField : std::uint8_t {
    kAirlineCode = 0,
    kFltNo = 1,
    kFltNoSuffix = 2,
    kLegNo = 3,
    kDepYmd = 4,
    kArrYmd = 5,
    kDepHhMm = 6,
    kDepArpUtcOffsetHhMmSign = 7,
    kDepArpUtcOffsetHhMm = 8,
    kBlkTm = 9,
    kEqpCd = 10,
    kDepArpCd = 11,
    kArrArpCd = 12,
    kArrHhMm = 13,
    kArrArpUtcOffsetHhMmSign = 14,
    kArrArpUtcOffsetHhMm = 15,
    kArrDayOffsetSign = 16,
    kArrDayOffset = 17,
    kSolComp = 18,
    kNextFltNo = 19,
    kNextFltNoSuffix = 20,
    kRegionType = 21,
    kLocalUtcInd = 22,
    kId = 23,
    kMysteriousPadding = 24,
};

struct FileFieldFormat {
    const std::uint8_t start_idx;
    const std::uint8_t length;
    const std::string_view str_chrono_format;
};

static constexpr std::array<FileFieldFormat, magic_enum::enum_count<ser::SecField>()> SecFieldStrFormatMap {{
    {3, 3, {}},                                    // kAirlineCode = 0
    {7, 5, {}},                                   // kFltNo = 1
    {11, 1, {}},                                        // kFltNoSuffix = 2
    {12, 2, {}},                                    // kLegNo = 3
    {15, 8, kYmdFormat},         // kDepYmd = 4
    {24, 8, kYmdFormat},         // kArrYmd = 5
    {48, 4, kHmFormat},           // kDepHhMm = 6
    {71, 1, {}},                                        // kDepArpUtcOffsetHhMmSign = 7
    {72, 4, kHmFormat},           // kDepArpUtcOffsetHhMm = 8
    {61, 4, kHmFormat},           // kBlkTm = 9
    {41, 3, {}},                                        // kEqpCd = 10
    {45, 3, {}},                                        // kDepArpCd = 11
    {53, 3, {}},                                        // kArrArpCd = 12
    {56, 4, kHmFormat},           // kArrHhMm = 13
    {76, 1, {}},                                        // kArrArpUtcOffsetHhMmSign = 14
    {77, 4, kHmFormat},           // kArrArpUtcOffsetHhMm = 15
    {82, 1, {}},                                        // kArrDayOffsetSign = 16
    {83, 1, kDayFormat},         // kArrDayOffset = 17
    {84, 1, {}},                                        // kSolComp = 18
    {88, 5, {}},                                        // kNextFltNo = 19
    {92, 1, {}},                                        // kNextFltNoSuffix = 20
    {96, 1, {}},                                        // kRegionType = 21
    {100, 1, {}},                                       // kLocalUtcInd = 22
    {180, 6, {}},                                       // kId = 23
    {192, 3, {}},                                       // kMysteriousPadding = 24
}};

}
#endif /* Definition_hpp */
