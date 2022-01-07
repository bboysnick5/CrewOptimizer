//
//  TestUtility.hpp
//  CrewOptimizer
//
//  Created by Yunlong Liu on 1/6/22.
//

#ifndef TestUtility_hpp
#define TestUtility_hpp


#include <string_view>


namespace tst_util {

bool CompareFiles(std::string_view filename_1, std::string_view filename_2);

}


#endif /* TestUtility_hpp */
