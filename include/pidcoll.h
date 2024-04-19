/* Copyright (C) Denys Valchuk - All Rights Reserved
 * ZHZhbGNodWtAZ21haWwuY29tCg== 
 */

#ifndef __DV_INCLUDE_PIDCOLL_H__
#define __DV_INCLUDE_PIDCOLL_H__

#include <cstdint>
#include <cstddef>

namespace dv::pidcoll
{
    struct Header {
        int8_t h1 :8;
        int8_t h2 :1;
        int8_t h3 :1;
        int8_t h4 :1;
        uint16_t pid :13;
        int8_t h5 :2;
        int8_t h6 :2;
        uint8_t cc :4;
    };

    struct Message {
        Header header;
        char data[184];
    };

    constexpr size_t HEADER_SIZE = 8;  // we ca not use sizeof(Header) because of bit fields
    constexpr size_t MESSAGE_SIZE = sizeof(Message::data) + HEADER_SIZE;

    constexpr uint16_t MAX_PID = 8191; // 2^13 - 1
    constexpr uint8_t  MAX_CC  = 15;   // 2^4 - 1

    /*
     * @brief Calculate CC value based on PIDs difference and previous CC value
     * @param pidDiff PIDs difference ()
     */
    uint8_t calcCC(size_t pidDiff, uint8_t cc);

    size_t pidDiff(uint16_t pid, uint16_t prevPid);

    uint8_t checkPID(char * buf, size_t size);
}

#endif // __DV_INCLUDE_PIDCOLL_H__
