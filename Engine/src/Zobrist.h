#pragma once
#include "Piece.h"
#include <random>

namespace Zobrist {
	static unsigned long long zobristTable[8][8][12];
	static std::mt19937 mt(01234567);

	//This code was inspired by the code found here: https://www.geeksforgeeks.org/minimax-algorithm-in-game-theory-set-5-zobrist-hashing/
	static unsigned long long randomInt() {
		std::uniform_int_distribution<unsigned long long> dist(0, UINT64_MAX);
		return dist(mt);
	}

	static void initZobristTable() {
		for (int x = 0; x < 12; x++) {
			for (int y = 0; y < 8; y++) {
				for (int z = 0; z < 8; z++) {
					zobristTable[z][y][x] = randomInt();
				}
			}
		}
	}
	static int getIndex(Piece piece) {
		switch (piece.getType()) {
		case PieceType::KING: 
			if (piece.getColor() == PieceColor::WHITE) return 0;
			else return 1;
			
		case PieceType::QUEEN:	
			if (piece.getColor() == PieceColor::WHITE) return 2;
			else return 3;
			
		case PieceType::ROOK: 	
			if (piece.getColor() == PieceColor::WHITE) return 4;
			else return 5;
			
		case PieceType::KNIGHT: 
			if (piece.getColor() == PieceColor::WHITE) return 6;
			else return 7;
			
		case PieceType::PAWN: 	
			if (piece.getColor() == PieceColor::WHITE) return 8;
			else return 9;
			
		case PieceType::BISHOP: 
			if (piece.getColor() == PieceColor::WHITE) return 10;
			else return 11;
		default:
			return -1;
		}
	}

	static unsigned long long calcHashValue(const Piece board[8][8]) {
		unsigned long long tempHash = 0;
		for (int x = 0; x < 8; x++) {
			for (int y = 0; y < 8; y++) {
				if (board[x][y] != Piece()) {
					tempHash ^= zobristTable[x][y][getIndex(board[x][y])];
				}
			}
		}
		return tempHash;
	}
}