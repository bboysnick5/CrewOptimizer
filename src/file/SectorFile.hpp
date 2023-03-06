//
//  SectorFile.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/28/22.
//

#ifndef SectorFile_hpp
#define SectorFile_hpp

#include <boost/hana.hpp>
#include <cstdint>

#include "../Utility.hpp"
#include "FieldFormat.hpp"

namespace file {

namespace detail {

namespace hana = boost::hana;

static constexpr auto kSecFieldFormatMap = hana::make_map(
    hana::make_pair(util::hana_enum_c<SecLineField::kAirlineCode>,
                    FieldFormat{3, 3, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kFltNo>,
                    FieldFormat{7, 5, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kLegNo>,
                    FieldFormat{12, 2, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kDepYmd>,
                    FieldFormat{15, 8, kYmdFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kArrYmd>,
                    FieldFormat{24, 8, kYmdFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kEqpCd>,
                    FieldFormat{41, 3, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kDepArpCd>,
                    FieldFormat{45, 3, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kDepHhMm>,
                    FieldFormat{48, 4, kHmFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kArrArpCd>,
                    FieldFormat{53, 3, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kArrHhMm>,
                    FieldFormat{56, 4, kHmFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kBlkMins>,
                    FieldFormat{61, 4, kHmFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kDepArpUtcOffsetHhMm>,
                    FieldFormat{71, 5, kSignedHmFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kArrArpUtcOffsetHhMm>,
                    FieldFormat{76, 5, kSignedHmFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kArrDayOffset>,
                    FieldFormat{82, 2, kHmFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kNextFltNo>,
                    FieldFormat{88, 5, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kRegionType>,
                    FieldFormat{96, 1, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kLocalUtcInd>,
                    FieldFormat{100, 1, kEmptyFormat}),
    hana::make_pair(util::hana_enum_c<SecLineField::kMysteriousPadding>,
                    FieldFormat{192, 3, kPaddingReplacementCharFormat}));
}  // namespace detail
}  // namespace file

#endif /* SectorFile_hpp */
