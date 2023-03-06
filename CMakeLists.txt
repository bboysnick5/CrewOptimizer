set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -march=native -DNDEBUG")
cmake_minimum_required(VERSION 3.12)
project(CrewOptimizer VERSION 0.1.0)

include(CTest)
enable_testing()

#set(OpenMP_CXX_FLAGS "-fopenmp")
#set(OpenMP_CXX_LIB_NAMES "libomp")


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_TOOLCHAIN_FILE=/Users/yunlongliu/vcpkg/scripts/buildsystems/vcpkg.cmake)

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        SYSTEM /usr/local/include
)


link_directories( /usr/local/lib/)

find_package(absl)
find_package(Boost)
find_package(fmt)
find_package(folly)
find_package(glog)
find_package(magic_enum)
find_package(PkgConfig REQUIRED)
pkg_check_modules (JEMALLOC jemalloc)
pkg_search_module(JEMALLOC REQUIRED jemalloc)
#pkg_check_modules (CLP REQUIRED clp)
#pkg_search_module(CLP REQUIRED clp)
include_directories(${JEMALLOC_INCLUDE_DIRS})
#include_directories(${CLP_INCLUDE_DIRS})

#pkg_check_modules(CBC REQUIRED cbc IMPORTED_TARGET)
#find_package(CLP CONFIG REQUIRED)
find_package(Protobuf REQUIRED)
#find_package(ortools CONFIG)
#add_subdirectory(or-tools)
find_package(tl-expected CONFIG REQUIRED)
find_path(REFL_CPP_INCLUDE_DIRS "refl.hpp")


#include(FetchContent)
#FetchContent_Declare(
 # or-tools
 # GIT_REPOSITORY https://github.com/google/or-tools.git
 # GIT_TAG        master
#)

# After the following call, the CMake targets defined by or-tools
# will be defined and available to the rest of the build
#FetchContent_MakeAvailable(or-tools)



#find_package(OpenMP)
#find_package(Vc)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.h"
        "${PROJECT_SOURCE_DIR}/src/*.hpp"
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        )

add_executable(CrewOptimizer ${all_SRCS})
target_link_libraries(CrewOptimizer PRIVATE folly)
#target_link_libraries(CrewOptimizer PRIVATE ortools::ortools)
target_link_libraries(CrewOptimizer PRIVATE ${JEMALLOC_LIBRARIES})
#target_link_libraries(CrewOptimizer PRIVATE ${CBC_LIBRARIES})

#target_link_libraries(CrewOptimizer PRIVATE ${CLP_LIBRARIES})
target_include_directories(CrewOptimizer PRIVATE ${REFL_CPP_INCLUDE_DIRS})
target_link_libraries(CrewOptimizer PRIVATE tl::expected)

#target_link_libraries(274NN666 PUBLIC Vc::Vc)
#target_link_libraries(274NN666 PUBLIC absl::random_random)
#if(OpenMP_CXX_FOUND)
#    target_link_libraries(274NN666 PUBLIC OpenMP::OpenMP_CXX)
#endif()

target_compile_options(CrewOptimizer PRIVATE -Wunused)

project(CrewOptimizer)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

message(${CMAKE_CXX_FLAGS_RELEASE})