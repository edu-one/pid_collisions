/* Copyright (C) Denys Valchuk - All Rights Reserved
 * ZHZhbGNodWtAZ21haWwuY29tCg==
 */

#include <gtest/gtest.h>
#include <pidcoll.h>

#include <vector>
#include <cstring>

namespace mock {
    struct PidInfo {
        uint16_t pid;
        uint8_t cc;
    };
    using PidInfos = std::vector<PidInfo>;

    std::vector<char> prepareMessages(const PidInfos &pidInfos, size_t remainingBytes = 0) {
        const size_t size = dv::pidcoll::MESSAGE_SIZE * pidInfos.size() + remainingBytes;
        std::vector<char> data(size, '=');
        size_t offset = 0;
        for (const auto &info : pidInfos) {
            dv::pidcoll::Header header = {0, 0, 0, 0, info.pid, 0, 0, info.cc};
            std::memcpy(data.data() + offset, &header, dv::pidcoll::HEADER_SIZE);
            offset += dv::pidcoll::MESSAGE_SIZE;
        }
        return data;
    }
} // namespace mock

TEST(CheckPIDTest, HeaderFieldsOverflow) {
    dv::pidcoll::Header header = {0, 0, 0, 0, 8192, 0, 0, 17};
    EXPECT_EQ(0, header.pid);
    EXPECT_EQ(1, header.cc);
}

TEST(CheckPIDTest, ZeroBuf) { EXPECT_THROW(dv::pidcoll::checkPID(nullptr, 5000), std::invalid_argument); }

TEST(CheckPIDTest, LessThanMessageSize) { EXPECT_THROW(dv::pidcoll::checkPID("1234567", 7), std::invalid_argument); }

TEST(CheckPIDTest, OneMessage) {
    mock::PidInfo pidInfo = {0, 11};
    std::vector<char> data = mock::prepareMessages({pidInfo});
    EXPECT_EQ(11, dv::pidcoll::checkPID(data.data(), data.size()));
}

TEST(CheckPIDTest, ThreeMessagesNoCollisions) {
    const mock::PidInfos pidInfos = {
        {0, 11},
        {1, 11},
        {2, 11}
    };
    auto data = mock::prepareMessages(pidInfos);
    EXPECT_EQ(11, dv::pidcoll::checkPID(data.data(), data.size()));
}

TEST(CheckPIDTest, ThreeMessages2Collisions) {
    const mock::PidInfos pidInfos = {
        {0, 11},
        {2, 12},
        {5, 14}
    };
    auto data = mock::prepareMessages(pidInfos);
    EXPECT_EQ(14, dv::pidcoll::checkPID(data.data(), data.size()));
}

TEST(PidDiffTest, PidOverflow) { EXPECT_THROW(dv::pidcoll::pidDiff(8192, 0), std::runtime_error); }

TEST(PidDiffTest, PrevPidOverflow) { EXPECT_THROW(dv::pidcoll::pidDiff(0, 8192), std::runtime_error); }

TEST(PidDiffTest, PidIsEqualToPrevPid) { EXPECT_THROW(dv::pidcoll::pidDiff(11, 11), std::runtime_error); }

TEST(PidDiffTest, PidLessThanPrevPid) {
    EXPECT_EQ(1, dv::pidcoll::pidDiff(0, 8191));
    EXPECT_EQ(4, dv::pidcoll::pidDiff(2, 8190));
}

TEST(PidDiffTest, PidGreaterThanPrevPid) {
    EXPECT_EQ(8191, dv::pidcoll::pidDiff(8191, 0));
    EXPECT_EQ(1, dv::pidcoll::pidDiff(3, 2));
}

TEST(CalcCCTest, PidDiffGreaterThanMaxPid) { EXPECT_THROW(dv::pidcoll::calcCC(8192, 0), std::invalid_argument); }

TEST(CalcCCTest, PidDiffIsZero) { EXPECT_THROW(dv::pidcoll::calcCC(0, 0), std::invalid_argument); }

TEST(CalcCCTest, PidDiffIsEqualToOne) { EXPECT_EQ(7, dv::pidcoll::calcCC(1, 7)); }

TEST(CalcCCTest, CcGreaterThanMaxCc) { EXPECT_THROW(dv::pidcoll::calcCC(0, 16), std::invalid_argument); }

TEST(CalcCCTest, CcCalculation) {
    EXPECT_EQ(0, dv::pidcoll::calcCC(1, 0));
    EXPECT_EQ(1, dv::pidcoll::calcCC(2, 0));
    EXPECT_EQ(5, dv::pidcoll::calcCC(5, 1));
    EXPECT_EQ(0, dv::pidcoll::calcCC(16, 1));
    EXPECT_EQ(2, dv::pidcoll::calcCC(16, 3));
    EXPECT_EQ(2, dv::pidcoll::calcCC(8191, 4));
}
