#pragma once
#include <cstdint>
#include <vector>

enum class PieceColor {
    BLACK = 0b00000000,
    WHITE = 0b10000000
};


enum class PieceType {
    NONE    = 0b00000000,
    KING    = 0b00000001,
    QUEEN   = 0b00000010,
    KNIGHT  = 0b00000100,
    BISHOP  = 0b00001000,
    ROOK    = 0b00010000,
    PAWN    = 0b00100000
};


class Piece {
public:


    // Create empty piece
    Piece() { }


    Piece(PieceColor color, PieceType type)
        :   data( 
                static_cast<std::underlying_type_t<PieceColor>>(color) |
                static_cast<std::underlying_type_t<PieceType>>(type) 
            )
    { }


    PieceType getType() const {
        uint8_t typeBits = data & 0b01111111;
        return static_cast<PieceType>(typeBits);
    }


    PieceColor getColor() const {
        uint8_t colorBit = data & 0b10000000;
        return static_cast<PieceColor>(colorBit);
    }


private:

    uint8_t data = 0;

};
