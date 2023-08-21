#pragma once

#include <Arduino.h>

template <typename M, typename N, typename K, typename L>
struct Fouth
{
    M val_1;
    N val_2;
    K val_3;
    L val_4;

    Fouth(M v1, N v2, K v3, L v4)
        : val_1(v1), val_2(v2), val_3(v3), val_4(v4) {}

    // use default values for copy construction and assignment

    M first() { return val_1; }
    const M first() const { return val_1; }

    N second() { return val_2; }
    const N second() const { return val_2; }

    K third() { return val_3; }
    const K third() const { return val_3; }

    L fouth() { return val_4; }
    const L fouth() const { return val_4; }
};