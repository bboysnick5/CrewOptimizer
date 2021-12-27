//
//  IoController.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/26/21.
//

#ifndef IoController_hpp
#define IoController_hpp

#include <fstream>
#include <string>
#include <vector>

#include "Parser.hpp"
#include "Sector.hpp"


class IoController {
public:
    std::vector<Sector> ParseSchFile(std::ifstream &sch_fs);

private:
    Parser parser_;
    std::vector<std::string> sec_strs_;
};



#endif /* IoController_hpp */
