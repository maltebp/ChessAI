#include "State.h"

#include <cassert>
#include <sstream>
#include <exception>

#include "FENUtil.h"
#include "TUIUtil.h"


State::State(const char* fenString) {
    FENUtil::parseFEN(fenString, *this);    
}


State::State(const std::string& fenString) {
    FENUtil::parseFEN(fenString, *this);    
}


Position State::getPiecePosition(const Piece& piece) const {
    for(unsigned int x=0; x<8; x++) {
        for(unsigned int y=0; y<8; y++) {
            if (board[x][y] == piece) {
                return {x,y};
            }
        }
    }
    return Position();
}


PieceColor State::getTurnColor() const {
    return turn % 2 ? PieceColor::BLACK: PieceColor::WHITE;
}


bool State::isWhitesTurn() const {
    return turn % 2 == 0;
}


bool State::isBlacksTurn() const {
    return turn % 2 == 1;
}


std::string State::toFEN() const {
    return FENUtil::generateFEN(*this);
}


std::string State::toPrettyString(const std::string& prefix, Move lastWhiteMove, Move lastBlackMove) const {
    return TUIUtil::getPrettyBoard(*this, prefix, lastWhiteMove, lastBlackMove);
}


bool State::operator==(const State& other) const {		
    if( turn != other.turn ) return false;
    if( enPassantTarget != other.enPassantTarget ) return false;
    if( whiteCanCastleKingSide != other.whiteCanCastleKingSide ) return false;
    if( whiteCanCastleQueenSide != other.whiteCanCastleQueenSide ) return false;
    if( blackCanCastleKingSide != other.blackCanCastleKingSide ) return false;
    if( blackCanCastleQueenSide != other.blackCanCastleQueenSide ) return false;
    if( drawCounter != other.drawCounter ) return false;

    for(unsigned int x=0; x<8; x++) {
        for(unsigned int y=0; y<8; y++) {
            if( board[x][y] != other.board[x][y] ) {
                return false;
                }
        }
    }

    return true;
}


bool State::operator!=(const State& other) const {
    return !(*this == other);
}


const Piece& State::operator[](Position position) const {
    assert(position.x < 8);
    assert(position.y < 8);
    return board[position.x][position.y];
}


Piece& State::operator[](const Position& position) {
    assert(position.x < 8);
    assert(position.y < 8);
    return board[position.x][position.y];
}


State State::createDefault() {
    
    State state;

    state.board[0][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
    state.board[1][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
    state.board[2][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
    state.board[3][0] = Piece(PieceColor::WHITE, PieceType::QUEEN);
    state.board[4][0] = Piece(PieceColor::WHITE, PieceType::KING);
    state.board[5][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
    state.board[6][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
    state.board[7][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
    state.board[0][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[1][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[2][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[3][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[4][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[5][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[6][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[7][1] = Piece(PieceColor::WHITE, PieceType::PAWN);


    state.board[0][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
    state.board[1][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
    state.board[2][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
    state.board[3][7] = Piece(PieceColor::BLACK, PieceType::QUEEN);
    state.board[4][7] = Piece(PieceColor::BLACK, PieceType::KING);
    state.board[5][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
    state.board[6][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
    state.board[7][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
    state.board[0][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[1][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[2][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[3][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[4][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[5][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[6][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[7][6] = Piece(PieceColor::BLACK, PieceType::PAWN);

    return state;
}