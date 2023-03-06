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

std::vector<sec::Sector> IoController::ParseSchFileStoreSecStrs(std::string_view sch_file_name) {
    std::ifstream sch_fs(sch_file_name);
    if (!sch_fs.is_open()) {
        std::cerr << "Could not open sch file: " << sch_file_name << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::vector<sec::Sector> secs;
    secs.reserve(kDefaultReserveSecs);
    sec_strs_.reserve(kDefaultReserveSecs);
    std::string line_str;
    file::SecSvIdxType sec_sv_idx = 0;
    while (std::getline(sch_fs, line_str)) {
        sec_parser_.SetCurLineToParseSecSvIdx(line_str, sec_sv_idx++);
        sec_strs_.push_back(std::move(line_str));
        secs.emplace_back(sec_parser_);
    }
    sec_strs_.shrink_to_fit();
    secs.shrink_to_fit();
    return secs;
}

void IoController::WriteSecToFile(std::string_view sol_file_name, std::span<const sec::Sector> secs_span) {
    std::ofstream sol_fs(sol_file_name);
    if (!sol_fs.is_open()) {
        std::cerr << "Could not open output solution file: " << sol_file_name << std::endl;
        std::exit(EXIT_FAILURE);
    }
    std::transform(secs_span.begin(), secs_span.end(), std::ostream_iterator<std::string>(sol_fs, "\n"),
                   [this](const sec::Sector& sec){
        return sec.SerializeGivenSecStr(sec_strs_[sec.GetSecSvIdx()]);
    });
    /*
    for (auto iters : boost::combine(sec_strs_, secs)) {
        boost::get<1>(iters).SerializeAlterOrigStr(boost::get<0>(iters));
        sol_fs << boost::get<0>(iters) + '\n';
    }

    std::for_each(boost::make_zip_iterator(boost::make_tuple(std::begin(sec_strs_), std::begin(secs.cbegin()))), boost::make_zip_iterator(boost::make_tuple(std::end(sec_strs_), std::end(secs.cend()))),
                  std::begin(sec_strs_), std::end(sec_strs_), [](const std::string& sec_str){
        sol_fs << secs.
    }); */
}
