//
//  IoController.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/26/21.
//

#ifndef IoController_hpp
#define IoController_hpp

#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "Parser.hpp"
#include "Sector.hpp"


class IoController {
public:
    std::vector<Sector> ParseSchFileStoreSecStrs(std::string_view sch_file_name);
    void WriteSecToFile(std::string_view sol_file_name, std::span<const Sector> secs);

private:
    static constexpr std::uint16_t kDefaultReserveSecs = 2048;
    
    Parser parser_{};
    std::vector<std::string> sec_strs_{};
};



#endif /* IoController_hpp */
