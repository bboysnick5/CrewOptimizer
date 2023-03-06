//
//  Sector.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#ifndef Sector_hpp
#define Sector_hpp

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

#include "Sector/DataModel.hpp"

namespace sec {

// namespace hana = boost::hana;

class Sector {
   public:
    static std::optional<Sector> Create(std::string_view line,
                                        file::LineSvIdxType idx) noexcept;

    Sector(const Sector &) = delete;
    Sector &operator=(const Sector &) = delete;

    constexpr std::string SerializeGivenSecStr(std::string sec_str) const;

    constexpr file::LineSvIdxType GetSecSvIdx() const;

    constexpr bool operator==(const Sector &other) const;

    constexpr EqpCd GetEqpCd() const;

   private:
    SecData data_;

    constexpr Sector() noexcept;
    friend struct SecComparator;
    // friend std::ostream& operator << (std::ostream& stream, const Sector&
    // sec); friend std::istream& operator >> (std::istream &stream, Sector&
    // sec);
};

struct SecComparator {
    SecComparator(arp::ArpComparator arp_comp = arp::ArpComparator{})
        : arp_comp_(arp_comp) {}

    bool operator()(const Sector &a, const Sector &b) const;

    const arp::ArpComparator arp_comp_;
};

}  // namespace sec

#endif /* Sector_hpp */
