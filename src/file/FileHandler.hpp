//
//  FileHandler.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/28/22.
//

#ifndef FileHandler_hpp
#define FileHandler_hpp

#include <fmt/format.h>
#include <glog/logging.h>

#include <concepts>
#include <cstdint>
#include <magic_enum.hpp>
#include <type_traits>

#include "../Definition.hpp"
#include "../Utility.hpp"
#include "SectorFile.hpp"

namespace file {

template <auto FieldEnum>
consteval FieldFormat GetFieldFormat() {
    if constexpr (std::same_as<decltype(FieldEnum), SecLineField>) {
        return detail::kSecFieldFormatMap[util::hana_enum_c<FieldEnum>];
    } else {
        static_assert(util::constexpr_false_helper<decltype(FieldEnum)>{},
                      "Passed in wrong field enum. Program terminate\n");
        // swapped to unreachable c++23
        return FieldFormat{0, 0, kEmptyFormat};
    }
}

}  // namespace file
#endif /* FileHandler_hpp */
