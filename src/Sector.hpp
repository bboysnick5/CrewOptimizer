//
//  Sector.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#ifndef Sector_hpp
#define Sector_hpp



#include <string>
#include <string_view>

#include "Airport.hpp"
#include "Complement.hpp"
#include "Definition.hpp"
#include "Parser.hpp"


class Sector {
    
public:
    
    using LegNoType = std::uint8_t;
    using AirlineCode = std::array<char, 3>;
    using RuleSetCompIdx = std::uint8_t;


    
    
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
    
    
    Sector(const Parser& parser);
    
    /*
    Sector(const Sector& other) noexcept = default;
    Sector& operator=(const Sector& other) noexcept = default;
    
    Sector(Sector&& other) noexcept;
    Sector& operator = (Sector &&other) noexcept;
    */
    
    std::string SerializeGivenSecStr(std::string sec_str) const;
    
    ser::SecSvIdxType GetSecSvIdx() const; 
    
    bool operator == (const Sector& other) const;

    
    //bool operator < (const Sector& other) const;
    
private:
    

    
    ser::SecSvIdxType sec_svs_idx_;
    AirlineCode al_cd_;
    chr::TpSysClkMins dep_dt_utc_, arr_dt_utc_;
    FltNum fn_, next_fn_;
    LegNoType leg_no_;
    EqpCd eqp_cd_;
    Airport dep_arp_, arr_arp_;
    chr::DurationMins blk_tm_;
    Complement comp_;
    RuleSetCompIdx comp_idx_;
    OprType opr_type_;
    RegionType reg_type_;
    
    friend class SecComparator;
    //friend std::ostream& operator << (std::ostream& stream, const Sector& sec);
    //friend std::istream& operator >> (std::istream &stream, Sector& sec);
};

struct SecComparator {
  
    SecComparator(ArpComparator arp_comp = ArpComparator{}) : arp_comp_(arp_comp) {}
    
    bool operator() (const Sector &a, const Sector &b) const;
    
    const ArpComparator arp_comp_;
};


#endif /* Sector_hpp */
