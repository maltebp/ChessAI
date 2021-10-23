#pragma once
#include <iostream>
#include "Position.h"
#include "Piece.h"

struct Move {
	
	Position fromField, toField;

	PieceType promotesTo = PieceType::NONE;

	static bool fromAlgebraicNotation(const std::string& inputStr, Move &move) {
		if( inputStr.length() != 4 ) return false;

		Position fromPosition;
		if( !Position::fromAlgebraicNotation(inputStr.substr(0, 2), fromPosition) ) {
			return false;
		}

		Position toPosition;
		if( !Position::fromAlgebraicNotation(inputStr.substr(2, 2), toPosition) ) {
			return false;
		} 

		move = { fromPosition, toPosition };
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
		return this->fromField == other.fromField && this->toField == other.toField;
	}

	bool operator!=(const Move& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator << (std::ostream& stream, const Move& move) {
		stream << move.fromField.toAlgebraicNotation() << " -> " << move.toField.toAlgebraicNotation();
		return stream;
	}
};

