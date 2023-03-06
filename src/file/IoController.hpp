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

#include "Definition.hpp"
#include "Parser.hpp"
#include "Sector/Sector.hpp"

class IoController {
   public:
    std::vector<sec::Sector> ParseSchFileStoreSecStrs(
        std::string_view sch_file_name);
    void WriteSecToFile(std::string_view sol_file_name,
                        std::span<const sec::Sector> secs);

   private:
    static constexpr std::uint16_t kDefaultReserveSecs = 2048;

    std::vector<std::string_view> sec_strs_{};
};

#endif /* IoController_hpp */
