#pragma once
#include <stdint.h>

/// \class Var
/// \brief A simple version of the classic Variant, as seen in engines such as Godot
struct Var {
    enum class Type {
        Int, Float, Byte, Char = Byte, UInt, UByte, Data
    }  type;
    union {

        // all are just their Type prefixed with d for data
        int dInt;
        unsigned dUInt;
        float dFloat;
        char dChar, dByte;
        uint8_t dUByte;
        void* dData;
    }data;

};
