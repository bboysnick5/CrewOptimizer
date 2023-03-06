//
//  main.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 11/26/21.
//

#include <absl/container/fixed_array.h>
#include <glog/logging.h>

#include <iostream>

#include "Complement.hpp"
#include "Duty.hpp"
#include "OptEngine.hpp"
#include "Sector/Sector.hpp"
#include "file/IoController.hpp"

struct test {
    std::array<char, 6> key;
    std::array<std::chrono::minutes, 4> mins;
};

int main(int argc, const char* argv[]) {
    // insert code here...

    google::InitGoogleLogging(argv[0]);

    if (argc != 3) {
        std::cerr << "error in command line args\n";
        std::exit(EXIT_FAILURE);
    }

    IoController io_ctl{};
    std::vector<sec::Sector> secs = io_ctl.ParseSchFileStoreSecStrs(argv[1]);
    std::sort(secs.begin(), secs.end(), sec::SecComparator{});
    io_ctl.WriteSecToFile(argv[2], secs);

    std::cout << sizeof(sec::Sector) << '\n'
              << sizeof(ds::SmallStackVec<test, 1>) << '\n'
              << sizeof(boost::container::small_vector<test, 1>) << '\n'
              << sizeof(absl::FixedArray<test, 1>) << '\n'
              << sizeof(test) << '\n'
              << sizeof(sec::Sector);

    return 0;
}
