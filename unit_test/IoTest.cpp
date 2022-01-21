//
//  IoTest.cpp
//  CrewOptimizerUnitTest
//
//  Created by Yunlong Liu on 1/6/22.
//

#include "IoTest.hpp"

#include <algorithm>
#include <iterator>

#include <gtest/gtest.h>

#include "TestDefinition.hpp"
#include "TestUtility.hpp"
#include "../src/IoController.hpp"
#include "../src/Parser.hpp"
#include "../src/Sector.hpp"


class IoControllerTest : public ::testing::Test {
protected:
    void SetUp() override {
        secs_ = io_ctl_.ParseSchFileStoreSecStrs(tst::kTestInputSchFile);
        
        std::ifstream correct_schedule_fs(tst::kTestCorrectOutputSchFile);
        if (!correct_schedule_fs.is_open()) {
            std::cerr << "Could not open correct sch file: " << tst::kTestCorrectOutputSchFile << std::endl;
            std::exit(EXIT_FAILURE);
        }
        correct_sec_lines_.reserve(secs_.size());
        std::string correct_sec_line;
        while (std::getline(correct_schedule_fs, correct_sec_line)) {
            correct_sec_lines_.push_back(correct_sec_line);
        }
    }
        
    std::vector<std::string> correct_sec_lines_;
    std::vector<sec::Sector> secs_;
    IoController io_ctl_{};
};


TEST_F(IoControllerTest, SecOutFileEq) {
    io_ctl_.WriteSecToFile(tst::kTestOutputSchFile, secs_);
    EXPECT_TRUE(tst_util::CompareFiles(tst::kTestOutputSchFile, tst::kTestCorrectOutputSchFile));
}

TEST_F(IoControllerTest, SecSortTwoLineTest) {
    std::vector<sec::Sector> two_line_secs{secs_[1291], secs_[997]};
    std::vector<sec::Sector> two_line_secs_correct(two_line_secs.crbegin(), two_line_secs.crend());
    std::sort(two_line_secs.begin(), two_line_secs.end(), sec::SecComparator{});
    EXPECT_EQ(two_line_secs, two_line_secs_correct);
    std::sort(two_line_secs.begin(), two_line_secs.end(), sec::SecComparator{});
    EXPECT_EQ(two_line_secs, two_line_secs_correct);
}


TEST_F(IoControllerTest, SecSortWholeFileTest) {
    std::sort(secs_.begin(), secs_.end(), sec::SecComparator{});
    io_ctl_.WriteSecToFile(tst::kTestOutputSchFile, secs_);
    EXPECT_TRUE(tst_util::CompareFiles(tst::kTestOutputSchFile, tst::kTestCorrectSortedOutputSchFile));
}

TEST_F(IoControllerTest, AlphaNumericTranslateTest) {
    EXPECT_EQ(9327, secs_[0].GetEqpCd());
    EXPECT_EQ(9336, secs_[4].GetEqpCd());
}
