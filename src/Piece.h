#pragma once
#include <cstdint>
#include <vector>

enum class PieceColor : uint8_t {
    NONE  = 0b00000000,
    BLACK = 0b10000000,
    WHITE = 0b01000000
};


enum class PieceType : uint8_t {
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
        uint8_t typeBits = data & 0b00111111;
        return static_cast<PieceType>(typeBits);
    }


    PieceColor getColor() const {
        uint8_t colorBit = data & 0b11000000;
        return static_cast<PieceColor>(colorBit);
    }


    char getAlgebraicChar() const {
        PieceColor color = getColor();
        PieceType type = getType();

        if( color == PieceColor::NONE ) return ' ';
        if( type == PieceType::NONE ) return ' ';

        char colorOffset = color == PieceColor::BLACK ? 32 : 0;

        switch( getType() ) {
            case PieceType::KING: 	return 'K' + colorOffset;
            case PieceType::QUEEN:	return 'Q' + colorOffset;
            case PieceType::ROOK: 	return 'R' + colorOffset;
            case PieceType::KNIGHT: return 'N' + colorOffset;
            case PieceType::PAWN: 	return 'P' + colorOffset;
            case PieceType::BISHOP: return 'B' + colorOffset;
        }

        throw std::exception("Unknown piece type");
    }


private:

    uint8_t data = 0;

};
