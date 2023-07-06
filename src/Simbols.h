#pragma once
#include <Arduino.h>

byte UpArrow[8] = {
    0b00100,
    0b01110,
    0b11111,
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b00000,
};
byte DownArrow[8] = {
    0b00100,
    0b00100,
    0b00100,
    0b00100,
    0b11111,
    0b01110,
    0b00100,
    0b00000,
};
byte RightArrow[8] = {
    0b00100,
    0b00110,
    0b00110,
    0b11111,
    0b11111,
    0b00110,
    0b00110,
    0b00100,
};
