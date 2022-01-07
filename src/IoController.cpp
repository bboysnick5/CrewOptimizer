//
//  IoController.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 12/26/21.
//

#include <fstream>

#include <boost/range/combine.hpp>
#include "boost/tuple/tuple.hpp"

#include "IoController.hpp"

std::vector<Sector> IoController::ParseSchFileStoreSecStrs(std::string_view sch_file_name) {
    std::ifstream sch_fs(sch_file_name);
    if (!sch_fs.is_open()) {
        std::cerr << "Could not open sch file: " << sch_file_name << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::vector<Sector> secs;
    secs.reserve(kDefaultReserveSecs);
    sec_strs_.reserve(kDefaultReserveSecs);
    std::string line_str; 
    while (std::getline(sch_fs, line_str)) {
        parser_.SetCurLineToParse(line_str);
        sec_strs_.push_back(line_str);
        secs.emplace_back(parser_);
    }
    sec_strs_.shrink_to_fit();
    secs.shrink_to_fit();
    return secs;
}

void IoController::WriteSecToFile(std::string_view sol_file_name, std::span<const Sector> secs) {
    std::ofstream sol_fs(sol_file_name);
    if (!sol_fs.is_open()) {
        std::cerr << "Could not open output solution file: " << sol_file_name << std::endl;
        std::exit(EXIT_FAILURE);
    }
    for (auto iters : boost::combine(sec_strs_, secs)) {
        boost::get<1>(iters).SerializeAlterOrigStr(boost::get<0>(iters));
        sol_fs << boost::get<0>(iters) + '\n';
    }
    /*
    std::for_each(boost::make_zip_iterator(boost::make_tuple(std::begin(sec_strs_), std::begin(secs.begin()))), boost::make_zip_iterator(boost::make_tuple(std::end(sec_strs_), std::end(secs.end()))),
                  std::begin(sec_strs_), std::end(sec_strs_), [](const std::string& sec_str){
        sol_fs << secs.
    }); */
}
