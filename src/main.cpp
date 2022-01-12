//
//  main.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#include <iostream>

#include "Complement.hpp"
#include "Duty.hpp"
#include "IoController.hpp"
#include "OptEngine.hpp"
#include "Sector.hpp"





int main(int argc, const char * argv[]) {
    // insert code here...
    if (argc != 3) {
        std::cerr << "error in command line args\n";
        std::exit(EXIT_FAILURE);
    }
    
    IoController io_ctl{};
    std::vector<Sector> secs = io_ctl.ParseSchFileStoreSecStrs(argv[1]);
    std::sort(secs.begin(), secs.end(), SecComparator{});
    io_ctl.WriteSecToFile(argv[2], secs);

    
    return 0;
}
