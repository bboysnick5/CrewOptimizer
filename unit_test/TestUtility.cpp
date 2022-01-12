//
//  TestUtility.cpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/6/22.
//

#include "TestUtility.hpp"

#include <algorithm>
#include <fstream>
#include <iterator>
#include <string_view>

namespace tst_util {

bool CompareFiles(std::string_view filename1, std::string_view filename2) {
  std::ifstream f1(filename1, std::ifstream::binary|std::ifstream::ate);
  std::ifstream f2(filename2, std::ifstream::binary|std::ifstream::ate);

  if (f1.fail() || f2.fail()) {
    return false; //file problem
  }

  if (f1.tellg() != f2.tellg()) {
    return false; //size mismatch
  }

  //seek back to beginning and use std::equal to compare contents
  f1.seekg(0, std::ifstream::beg);
  f2.seekg(0, std::ifstream::beg);
  return std::equal(std::istreambuf_iterator<char>(f1.rdbuf()),
                    std::istreambuf_iterator<char>(),
                    std::istreambuf_iterator<char>(f2.rdbuf()));
}

}
