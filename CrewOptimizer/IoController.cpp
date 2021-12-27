//
//  IoController.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/26/21.
//

#include "IoController.hpp"

std::vector<Sector> IoController::ParseSchFile(std::ifstream &sch_fs) {
    std::vector<Sector> secs;
    secs.reserve(2000);
    for (std::string line_str; std::getline(sch_fs, line_str);) {
        parser_.SetCurLineToParse(line_str);
        secs.emplace_back(parser_);
    }
    secs.shrink_to_fit();
    return secs;
}
