#pragma once

#include <string>

#include "Piece.h"
#include "Position.h"


class State {
public:

	// Creates a State with an empty board
	State() = default;

	// Create state from FEN-string
	State(const char* fen);

	// Create state from FEN-string
	State(const std::string& fen);

	// Get the Piece at the given Position in the board
	Piece getPiece(Position position) const;

	// Get the Position of the first Piece matching the given Piece, when looking
	// from the botom right corner, to the top left (in row-major order)
	Position getPiecePosition(const Piece& piece) const;

	// Get the color of the Player whos turn it is
	PieceColor getTurnColor() const;

	bool operator==(const State& other) const;

	bool operator!=(const State& other) const;

	const Piece& operator[](Position position) const;

	Piece& operator[](const Position& position);

	std::string toFEN() const;

	// Creates the starting state of a normal game
    static State createDefault();

public:

	// "Half turn" 
	int turn = 0;

    Piece board[8][8];

	// The position that a pawn can move, to capture an opponent pawn
	// using en passant
	Position enPassantTarget;
	
	bool whiteCanCastleKingSide = true;
	
	bool whiteCanCastleQueenSide = true;
	
	bool blackCanCastleKingSide = true;
	
	bool blackCanCastleQueenSide = true;

	// Number of turns since last capture or pawn move
	unsigned int drawCounter = 0;

};