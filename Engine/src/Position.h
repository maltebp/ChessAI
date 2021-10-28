#pragma once
#include <iostream>

struct Position {

	unsigned int x = UINT32_MAX;
	unsigned int y = UINT32_MAX;

	static bool fromAlgebraicNotation(const std::string& str, Position& outputPosition) {
		if( str.length() != 2 ) return false;

		Position position = {
			7 - (unsigned int)('h' - str.at(0)),
			7 - (unsigned int)('8' - str.at(1))
		};

		if( !position.isFieldInBoard() ) return false;

		outputPosition = position;	

		return true;
	}

	std::string toAlgebraicNotation() const {
		std::string str = "";
		str += (char)x + 'a';
		str += (char)y+1 + '0';
		return str;
	}

	Position getNeighbourN() const {
		return{ x,y + 1 };
	}

	Position getNeighbourS() const {
		return{ x,y - 1 };
	}

	Position getNeighbourW() const {
		return{ x - 1,y };
	}

	Position getNeighbourE() const {
		return{ x + 1,y };
	}

	Position getNeighbourNE() const {
		return{ x + 1 ,y + 1 };
	}

	Position getNeighbourNW() const {
		return{ x - 1,y + 1 };
	}

	Position getNeighbourSE() const {
		return{ x + 1,y - 1 };
	}

	Position getNeighbourSW() const {
		return{ x - 1,y - 1 };
	}

	bool isFieldInBoard() const {
		return x <= 7 && y <= 7;
	}
	
	bool operator==(const Position& other) const {
		return this->x == other.x && this->y == other.y;
	}

	bool operator!=(const Position& other) const {
		return !(*this == other);
	}

	friend std::ostream& operator<<(std::ostream& stream, const Position pos) {
		stream << "(" << pos.x << "," << pos.y << ")";
		return stream;
	}

};