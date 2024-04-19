/* Copyright (C) Denys Valchuk - All Rights Reserved
 * ZHZhbGNodWtAZ21haWwuY29tCg==
 */

#include "pidcoll.h"

#include <stdexcept>
#include <iostream>
#include <optional>

namespace
{
    struct PidInfo {
        uint16_t pid;
        uint8_t cc;
    };

} // unnamed namespace


uint8_t dv::pidcoll::calcCC(size_t pidDiff, uint8_t cc) {
    if (pidDiff > MAX_PID)
        throw std::invalid_argument("pidDiff is greater than MAX_PID");
    if (pidDiff == 0)
        throw std::invalid_argument("pidDiff is zero");
    if (cc > MAX_CC)
        throw std::invalid_argument("cc is greater than MAX_CC");
    if (pidDiff == 1) // no collisions
        return cc;
    const size_t missedPids = pidDiff - 1;
    return (missedPids + cc) % (MAX_CC + 1);
}

size_t dv::pidcoll::pidDiff(uint16_t pid, uint16_t prevPid) {
    if (pid > MAX_PID || prevPid > MAX_PID)
        throw std::runtime_error("Malformed message, pid or prevPid is greater than MAX_PID");
    if (pid < prevPid) {
        return MAX_PID - prevPid + pid + 1;
    }
    if (pid == prevPid)
        throw std::runtime_error("Malformed message, pid is equal to prevPid");
    return pid - prevPid;
}

uint8_t dv::pidcoll::checkPID(char * buf, size_t size) {
    if (!buf)
        throw std::invalid_argument("buf is nullptr");
    if (size < MESSAGE_SIZE)
        throw std::invalid_argument("size is less than MESSAGE_SIZE");
    const size_t messagesCount = size / MESSAGE_SIZE;
    const size_t remainingBytes = size % MESSAGE_SIZE;
    if (remainingBytes != 0)
        std::cout << "Warning: buf size " << size << " contain remaining bytes " << remainingBytes << "..." << std::endl;
    std::optional<PidInfo> pidInfo;
    for (size_t i = 0; i < messagesCount; ++i) {
        Header& header = *reinterpret_cast<Header *>(buf + i * MESSAGE_SIZE);
        if (!pidInfo) {
            pidInfo = PidInfo{header.pid, header.cc};
            std::cout << "First message: pid=" << +pidInfo->pid << ", cc=" << +pidInfo->cc << std::endl;
            continue;
        }
        const auto prevInfo = *pidInfo;
        if (const auto diff = pidDiff(header.pid, prevInfo.pid); diff != 1) {
            header.cc = calcCC(diff, prevInfo.cc);
            std::cout << "Found collision: pid=" << +header.pid << ", previous pid=" << +prevInfo.pid << ", cc=" << +header.cc << std::endl;
        }
        pidInfo = PidInfo{header.pid, header.cc};
    }
    if (!pidInfo)
        throw std::runtime_error("Unexpected error: pidInfo was not initialized");
    return pidInfo->cc;
}
