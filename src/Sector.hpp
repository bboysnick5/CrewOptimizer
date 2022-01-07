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

#include "Complement.hpp"
#include "Definition.hpp"
#include "Parser.hpp"


class Sector {
public:
    Sector(const Parser& parser);
    
    void SerializeAlterOrigStr(std::string &sec_str) const;
    
private:
    
    sec::Id id_;
    gen::AirlineCode al_cd_;
    gen::TpSysClkMins dep_dt_utc_, arr_dt_utc_;
    sec::FltNum fn_, next_fn_;
    sec::LegNo leg_no_;
    sec::EqpCd eqp_cd_;
    sec::TailNum tn_, nxt_tn_;
    gen::Airport dep_arp_, arr_arp_;
    gen::DurationMins blk_tm_;
    Complement comp_;
    gen::RuleSetCompIdx comp_idx;
    sec::OprType opr_type_;
    sec::RegionType reg_type_;
    
    //friend std::ostream& operator << (std::ostream& stream, const Sector& sec);
    //friend std::istream& operator >> (std::istream &stream, Sector& sec);
};


#endif /* Sector_hpp */
