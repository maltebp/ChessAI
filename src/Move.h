#pragma once
#include <iostream>
#include "Position.h"
#include "Piece.h"

struct Move {
	
	Position fromField, toField;

	PieceType promotesTo = PieceType::NONE;

	
	static bool fromAlgebraicNotation(const std::string& inputStr, Move &move) {
		if( inputStr.length() > 5 || inputStr.length() < 4 ) return false;

		Position fromPosition;
		if( !Position::fromAlgebraicNotation(inputStr.substr(0, 2), fromPosition) ) {
			return false;
		}

		Position toPosition;
		if( !Position::fromAlgebraicNotation(inputStr.substr(2, 2), toPosition) ) {
			return false;
		} 

		Piece promotesTo;
		if( inputStr.length() == 5 ) {
			bool valid = Piece::fromAlgebraicChar(inputStr[4], promotesTo);
		}

		move = { fromPosition, toPosition, promotesTo.getType() };
		return true;
	}


	unsigned int getYDistance() const {
		int dist = fromField.y - toField.y;
		// Non-branching abs hack
		return (unsigned int)(dist * ((dist>0) - (dist<0)));
	}


	unsigned int getXDistance() const {
		int dist = fromField.x - toField.x;
		// Non-branching abs hack
		return (unsigned int)(dist * ((dist>0) - (dist<0)));
	}


	bool operator==(const Move& other) const {
		return 	this->fromField == other.fromField &&
				this->toField == other.toField &&
				this->promotesTo == other.promotesTo;
	}

	
	bool operator!=(const Move& other) const {
		return !(*this == other);
	}
	

	friend std::ostream& operator << (std::ostream& stream, const Move& move) {
		stream << move.fromField.toAlgebraicNotation() << " -> " << move.toField.toAlgebraicNotation();
		if( move.promotesTo != PieceType::NONE ) {
			// Promotions are always indicated with small letter
			stream << " (Promotion: " << Piece{PieceColor::BLACK, move.promotesTo}.getAlgebraicChar() << ")";
		}
		return stream;
	}

};

