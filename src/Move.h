#pragma once
#include <iostream>
#include "Position.h"

struct Move {
	
	Position fromField, toField;

	bool operator==(const Move& other) const {
		return this->fromField == other.fromField && this->toField == other.toField;
	}

	friend std::ostream& operator << (std::ostream& stream, const Move& move) {
		stream << move.fromField.toAlgebraicNotation() << " -> " << move.toField.toAlgebraicNotation();
		return stream;
	}
};

