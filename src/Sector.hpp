//
//  Sector.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#ifndef Sector_hpp
#define Sector_hpp


#include <cstdint>
#include <string>
#include <string_view>

#include "Airport.hpp"
#include "Complement.hpp"
#include "Definition.hpp"
#include "Parser.hpp"

namespace sec {


enum class RegionType : char {
    kDom = 'D',
    kInt = 'I',
    kReg = 'R'
};

enum class OprType : std::uint8_t {
    kOpr,
    kDhd,
};

using EqpCd = std::uint16_t;
using LegNoType = std::uint8_t;
using RuleSetCompIdx = std::uint8_t;

struct FltNum {
    std::uint16_t fn_numeric;
    char fn_suffix;
};

class Sector {
    
public:
    
    Sector(const Parser<ser::SecField>& parser);
    
    std::string SerializeGivenSecStr(std::string sec_str) const;
    
    ser::SecSvIdxType GetSecSvIdx() const; 
    
    bool operator == (const Sector& other) const;
    
    EqpCd GetEqpCd() const;
    
private:
    
    ser::SecSvIdxType sec_svs_idx_;
    chr::TpMinsUnixEpoch4B dep_dt_utc_, arr_dt_utc_;
    FltNum fn_, next_fn_;
    LegNoType leg_no_;
    EqpCd eqp_cd_;
    Airport dep_arp_, arr_arp_;
    Complement comp_;
    RuleSetCompIdx comp_idx_; // to be fixed
    OprType opr_type_;
    RegionType reg_type_;
    
    friend struct SecComparator;
    //friend std::ostream& operator << (std::ostream& stream, const Sector& sec);
    //friend std::istream& operator >> (std::istream &stream, Sector& sec);
};

struct SecComparator {
  
    SecComparator(ArpComparator arp_comp = ArpComparator{}) : arp_comp_(arp_comp) {}
    
    bool operator() (const Sector &a, const Sector &b) const;
    
    const ArpComparator arp_comp_;
};

}

#endif /* Sector_hpp */
