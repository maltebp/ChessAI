#pragma once
#include <stdlib.h>
#include "MoveUtil.h"

class Heuristic {
public:
	
	static int danielsenHeuristic(const State& state) {
		static std::vector<Position> slidingPositions;


		int sum = 0;
		int piecesLeft = 0;
		Position blackKingPos, whiteKingPos;
		int whiteSum = 0;
		int blackSum = 0;
		int whitesMinorPieceThreaths = 0;
		int blacksMinorPieceThreaths = 0;

		//Pieces and their positional values
		int current;
		for (unsigned int i = 0; i < 8; i++) {
			for (unsigned int j = 0; j < 8; j++) {
				slidingPositions.clear();
				current = 0;
				Piece piece = state.board[i][j];
				int sign = piece.getColor() == PieceColor::WHITE ? 1 : -1;
				bool whitePiece = sign == 1;
				Position pos = { i,j };
				bool minorPieceThreathening = false;
				switch (piece.getType())
				{
				case PieceType::KING:
				{
					//This will be usefull if we are in endgame 
					if (piece.getColor() == PieceColor::WHITE) {
						whiteKingPos = { i,j };
					}
					else {
						blackKingPos = { i,j };
					}
					piecesLeft++;
					break;
				}
				case PieceType::QUEEN:
				{
					MoveUtil::getAllSliderPositionsForPiece(state, pos, piece, slidingPositions);
					int controlledSquares = (int)slidingPositions.size();
					current = 900 + 1 * controlledSquares;
					minorPieceThreathening = MoveUtil::isRooksThreathening(state, pos, whitePiece)
						|| MoveUtil::isBishopThreathening(state, pos, whitePiece)
						|| MoveUtil::isKnightThreathening(state, pos, whitePiece)
						|| MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;

					break;
				}
				case PieceType::ROOK:
				{
					MoveUtil::getAllSliderPositionsForPiece(state, pos, piece, slidingPositions);
					int controlledSquares = (int)slidingPositions.size();
					current = (int)(500 + 1.5 * controlledSquares);
					minorPieceThreathening = MoveUtil::isBishopThreathening(state, pos, whitePiece)
						|| MoveUtil::isKnightThreathening(state, pos, whitePiece)
						|| MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;
					break;
				}
				case PieceType::BISHOP:
				{
					MoveUtil::getAllSliderPositionsForPiece(state, pos, piece, slidingPositions);
					int controlledSquares = (int)slidingPositions.size();
					current = 300 + 2 * controlledSquares;
					minorPieceThreathening = MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;
					break;
				}
				case PieceType::KNIGHT:
				{
					int distFromCenter = MoveUtil::manhattanDistFromMiddle(pos);
					current = 300 + 3 * (4 - distFromCenter);

					minorPieceThreathening = MoveUtil::isPawnThreathening(state, pos, whitePiece);
					piecesLeft++;
					break;
				}
				case PieceType::PAWN:
				{
					//This conversion mirrors the field-positions-board, if piece is black
					int yvalue = piece.getColor() == PieceColor::WHITE ? pos.y : 7 - pos.y;
					current = 100 + (int)pawnFieldValuesForWhite[pos.x][yvalue];

					//Double pawns - check if there is a pawn of my own color in front of me
					//(It doesn't matter that we look in the same way for black and white, and we cannot go out of the board this way)
					Piece pieceInFront = state.board[pos.x][pos.y + 1];
					bool isPawnOfSameColor = pieceInFront.getColor() == piece.getColor() && pieceInFront.getType() == piece.getType();
					if (isPawnOfSameColor) {
						//We have a double pawn
						current += 8;
					}
					piecesLeft++;
					break;
				}
				case PieceType::NONE:
					break;
				}

				//Update minor-piece-threath counters
				if (whitePiece && minorPieceThreathening) {
					//If it is a white piece and it is threathened by a minor piece
					whitesMinorPieceThreaths++;
				}
				else if (minorPieceThreathening) {
					//If it is a black piece and it is threathened by a minor piece
					blacksMinorPieceThreaths++;
				}

				sum += sign * current;
				if (whitePiece) {
					whiteSum += current;
				}
				else {
					blackSum += current;
				}
			}
		}

		//Castling
		sum -= state.blackHasCastled * 16;
		sum += state.whiteHasCastled * 16;

		//Minor pieces threaths
		sum += minorPiecesThreatheningPoints(whitesMinorPieceThreaths);
		sum -= minorPiecesThreatheningPoints(blacksMinorPieceThreaths);

		//This part is from original Danielsen Heuristic, added to handle endgame better
		//---------------------------------------------------
		bool isEndgame = (whiteSum <= ENDGAME_SCORE_THRESHOLD || blackSum <= ENDGAME_SCORE_THRESHOLD);
		if (isEndgame) {

			int whiteDistFromCenter = MoveUtil::manhattanDistFromMiddle(whiteKingPos);
			int blackDistFromCenter = MoveUtil::manhattanDistFromMiddle(blackKingPos);
			whiteSum -= 1 * whiteDistFromCenter;
			blackSum -= 1 * blackDistFromCenter;
			sum += -1 * whiteDistFromCenter;
			sum -= -1 * blackDistFromCenter;

			bool isMating = (whiteSum <= MATING_SCORE_THRESHOLD || blackSum <= MATING_SCORE_THRESHOLD);
			if (isMating) {
				int distBetweenKings = std::abs((int)whiteKingPos.x - (int)blackKingPos.x) + std::abs((int)whiteKingPos.y - (int)blackKingPos.y);

				if (whiteSum >= blackSum) {//white winning
					sum -= -8 * blackDistFromCenter;
					sum += -2 * distBetweenKings;
				}
				else { //black winning
					sum += -8 * whiteDistFromCenter;
					sum -= -2 * distBetweenKings;

				}

			}
		}

		//---------------------------------------------------

		return sum;
	}

	static int minorPiecesThreatheningPoints(int num) {
		return num == 0 ? 2 : num == 1 ? -10 : -50;
	}

	constexpr static int ENDGAME_WINNER_SCORE_THRESHOLD = 500;

	constexpr static int MATING_SCORE_THRESHOLD = 600;

	constexpr static int ENDGAME_SCORE_THRESHOLD = 1500;


};


const static double pawnFieldValuesForWhite[8][8] = {
		{0,-2,-4,-4,-3,8,23,0},
		{0,0,-1,0,2,14,30,0},
		{0,3,3.5,6,9.5,23,41.5,0},
		{0,4,5,8,12,26,44,0},
		{0,5,6.5,10,14.5,29,47.5,0},
		{0,1,0.5,2,4.5,17,33.5,0},
		{0,-2,-4,-4,-3,8,23,0},
		{0,-2,-4,-4,-3,8,23,0},
};