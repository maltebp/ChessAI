#pragma once

#include <string>

#include "Piece.h"
#include "Position.h"
#include "Move.h"


class State {
public:

	// Creates a State with an empty board
	State() = default;

	// Create state from FEN-string
	// Note: const char* variant is needed for string literals:
	//   State state = "my fen string";
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

	bool isWhitesTurn() const;

	bool isBlacksTurn() const;

	std::string toFEN() const;

	/**
	 * @brief	Creates a multi-string visualizing the board using ASCII characters,
	 *  		and terminal colors.
	 * 			The "last moves" will be highlighted using terminal colors, if they
	 * 			are not the default Move()
	 * 
	 * @param prefix	Prefix string to add before each line
	 * @param lastWhiteMove 	Last white move performed
	 * @param lastBlackMove 	Last black move performed
	 */
	std::string toPrettyString(const std::string& prefix = "", Move lastWhiteMove = Move(), Move lastBlackMove = Move()) const;

	bool operator==(const State& other) const;

	bool operator!=(const State& other) const;

	const Piece& operator[](Position position) const;

	Piece& operator[](const Position& position);


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

	bool blackHasCastled = false;

	bool whiteHasCastled = false;

	Position lastMovedPiecePosition = Position();

	// Number of turns since last capture or pawn move
	unsigned int drawCounter = 0;

};