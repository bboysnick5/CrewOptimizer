//
//  FieldFormat.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/28/22.
//

#ifndef FieldFormat_hpp
#define FieldFormat_hpp

#include <cstdint>
#include <string>

namespace file {

using LineSvIdxType = std::uint16_t;

struct FieldFormat {
    const std::uint8_t char_or_token_start_idx;
    const std::uint8_t length;
    const std::string_view& format_str;
};

static constexpr std::string_view kEmptyFormat = "";
static constexpr std::string_view kYmdFormat = "%m%d%Y";
static constexpr std::string_view kHmFormat = "%H%M";
static constexpr std::string_view kSignedHmFormat = "%z";
static constexpr std::string_view kPaddingReplacementCharFormat = " ";

enum class SecLineField : std::uint8_t {
    kAirlineCode = 0,
    kFltNo,
    kLegNo,
    kDepYmd,
    kArrYmd,
    kDepHhMm,
    kDepArpUtcOffsetHhMm,
    kBlkMins,
    kEqpCd,
    kDepArpCd,
    kArrArpCd,
    kArrHhMm,
    kArrArpUtcOffsetHhMm,
    kArrDayOffset,
    kSolComp,
    kNextFltNo,
    kRegionType,
    kLocalUtcInd,
    kId,
    kMysteriousPadding,
};

enum class SecLocalUtcInd : char { kLocal = 'L', kUtc = 'G', kDefault = ' ' };

}  // namespace file

#endif /* FieldFormat_hpp */
