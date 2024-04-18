/* Copyright (C) Denys Valchuk - All Rights Reserved
 * ZHZhbGNodWtAZ21haWwuY29tCg==
 */

#include "pidcoll.h"

#include <stdexcept>

int dv::pidcoll::factorial(int n) {
    if (n < 0)
        throw std::invalid_argument("Factorial of negative number is undefined");
    if (n == 0)
        return 1;
    return n * factorial(n - 1);
}
