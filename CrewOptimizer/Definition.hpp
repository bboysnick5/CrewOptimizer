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

#include <array>
#include <chrono>
#include <string>
#include <string_view>

#include <magic_enum.hpp>

#include "Utility.hpp"

//#include <boost/chrono.hpp>


namespace gen {


using AirlineCode = std::array<char, 3>;
using TpSysClkMins = std::chrono::time_point<std::chrono::system_clock, std::chrono::minutes>;
using TpSysClkDays = std::chrono::time_point<std::chrono::system_clock, std::chrono::days>;
using DurationMins = std::chrono::minutes;
using DurationDays = std::chrono::days;
using RuleSetCompIdx = std::uint8_t;


struct Airport {
    std::array<char, 3> arp_cd;
    DurationMins utc_offset_duration;
};

using FltGrpCd = std::array<char, 3>;

}

namespace sec {

using LegNo = std::uint8_t;
using Id = std::uint16_t;

struct FltNum {
    std::uint16_t fn_numeric;
    char fn_suffix;
};

struct TailNum {
    char tn_prefix;
    std::uint16_t tn_numeric;
};

using EqpCd = std::array<char, 3>;

enum class RegionType : char {
    kDom = 'D',
    kInt = 'I',
    kReg = 'R'
};

enum class OprType {
    kOpr,
    kDhd,
    kRst
};
}

namespace dut {


}

namespace prg {


}


namespace ser {

namespace input {
static constexpr std::string_view kYmdFormat{"%m%d%Y"};
static constexpr std::string_view kHmFormat{"%H%M"};
static constexpr std::string_view kDayFormat{"%d"};
}

namespace output {

static constexpr std::string_view kOutputFormatPrefix{"{:"};
static constexpr std::string_view kOutputFormatSuffix{"}"};

template <const std::string_view &input_format>
static constexpr auto InputToOutputFormat() {
    return util::sv_concat_v<kOutputFormatPrefix, input_format, kOutputFormatSuffix>;
}

static constexpr std::string_view kYmdFormat = InputToOutputFormat<input::kYmdFormat>();
static constexpr std::string_view kHmFormat = InputToOutputFormat<input::kHmFormat>();
static constexpr std::string_view kDayFormat = InputToOutputFormat<input::kDayFormat>();
}

static constexpr char kLocalInd = 'L';

enum class SecField : std::uint8_t {
    kAirlineCode = 0,
    kFltNo,
    kFltNoSuffix,
    kLegNo,
    kDepYmd,
    kArrYmd,
    kDepHhMm,
    kDepArpUtcOffsetHhMmSign,
    kDepArpUtcOffsetHhMm,
    kBlkTm,
    kEqpCd,
    kDepArpCd,
    kArrArpCd,
    kArrHhMm,
    kArrArpUtcOffsetHhMmSign,
    kArrArpUtcOffsetHhMm,
    kArrDayOffsetSign,
    kArrDayOffset,
    kSolComp,
    kNextFltNo,
    kNextFltNoSuffix,
    kRegionType,
    kLocalUtcInd,
    kId,
    kMysteriousPadding
};

struct FileFieldFormat {
    const std::uint8_t start_idx;
    const std::uint8_t length;
    const std::string_view str_chrono_format;
    const std::string_view str_output_format;
};

static constexpr std::array<FileFieldFormat, magic_enum::enum_count<ser::SecField>()> SecFieldStrFormatMap {{
    {3, 3, {}, {":>3"}},                                    // kAirlineCode
    {7, 5, {}, {":0>5"}},                                   // kFltNo
    {11, 1, {}, {}},                                        // kFltNoSuffix
    {12, 2, {}, ":0>2"},                                    // kLegNo
    {15, 8, input::kYmdFormat, output::kYmdFormat},         // kDepYmd,
    {24, 8, input::kYmdFormat, output::kYmdFormat},         // kArrYmd,
    {48, 4, input::kHmFormat, output::kHmFormat},           // kDepHhMm
    {71, 1, {}, {}},                                        // kDepArpUtcOffsetHhMmSign
    {72, 4, input::kHmFormat, output::kHmFormat},           // kDepArpUtcOffsetHhMm
    {61, 4, input::kHmFormat, output::kHmFormat},           // kBlkTm
    {41, 3, {}, {}},                                        // kEqpCd
    {45, 3, {}, {}},                                        // kDepArpCd
    {53, 3, {}, {}},                                        // kArrArpCd
    {76, 1, {}, {}},                                        // kArrArpUtcOffsetHhMmSign
    {77, 4, input::kHmFormat, output::kHmFormat},           // kArrArpUtcOffsetHhMm
    {82, 1, {}, {}},                                        // kArrDayOffsetSign
    {83, 1, input::kDayFormat, output::kDayFormat},         // kArrDayOffset
    {84, 1, {}, {}},                                        // kSolComp
    {88, 5, {}, {}},                                        // kNextFltNo
    {92, 1, {}, {}},                                        // kNextFltNoSuffix
    {96, 1, {}, {}},                                        // kRegionType
    {100, 1, {}, {}},                                       // kLocalUtcInd
    {180, 6, {}, {}},                                       // kId
    {192, 3, {}, {}},                                       // kMysteriousPadding
}};

}
#endif /* Definition_hpp */
