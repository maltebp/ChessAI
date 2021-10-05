#pragma once
#include <iostream>
#include "Position.h"

struct Move {
	
	Position fromField, toField;

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

	bool operator==(const Move& other) const {
		return this->fromField == other.fromField && this->toField == other.toField;
	}

	friend std::ostream& operator << (std::ostream& stream, const Move& move) {
		stream << move.fromField.toAlgebraicNotation() << " -> " << move.toField.toAlgebraicNotation();
		return stream;
	}
};

