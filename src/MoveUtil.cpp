#include "MoveUtil.h"

namespace MoveUtil {

	

	

	void getAllKingMoves(const State& state,Position oldPos, Piece piece, std::vector<Move>& moves) {
		Position positions[] = {
			oldPos.getNeighbourN(),
			oldPos.getNeighbourS(),
			oldPos.getNeighbourE(),
			oldPos.getNeighbourW(),
			oldPos.getNeighbourNE(),
			oldPos.getNeighbourNW(),
			oldPos.getNeighbourSW(),
			oldPos.getNeighbourSE()
		};
		

		for (Position newPos : positions)
		{
			//Check that position exists
			if (!newPos.isFieldInBoard()) {
				continue;
			}

			//Check for friendly piece
			bool friendlyPieceAtPos = state.board[newPos.x][newPos.y].getColor() == piece.getColor();
			if (friendlyPieceAtPos) {
				continue;
			}

			//Check that move does not "check" king

			//Add to list of moves
			Move move = { oldPos,newPos };
			moves.push_back(move);
		}

	


	}

	void getAllKnightMoves(const State& state, Position oldPos, Piece piece, std::vector<Move>& moves) {
		Position positions[] = {
			oldPos.getNeighbourN().getNeighbourNW(),
			oldPos.getNeighbourN().getNeighbourNE(),
			oldPos.getNeighbourW().getNeighbourNW(),
			oldPos.getNeighbourW().getNeighbourSW(),
			oldPos.getNeighbourS().getNeighbourSW(),
			oldPos.getNeighbourS().getNeighbourSE(),
			oldPos.getNeighbourE().getNeighbourNE(),
			oldPos.getNeighbourE().getNeighbourSE()
		};

		for (Position newPos : positions)
		{
			//Check that position exists
			if (!newPos.isFieldInBoard()) {
				continue;
			}

			//Check for friendly piece
			bool friendlyPieceAtPos = state.board[newPos.x][newPos.y].getColor() == piece.getColor();
			if (friendlyPieceAtPos) {
				continue;
			}

			//Check that move does not "check" king

			//Add to list of moves
			Move move = { oldPos,newPos };
			moves.push_back(move);
		}
	}


	void getMovesForPiece(const State& state, const Piece piece, Position position, std::vector<Move>& moves) {


		switch (piece.getType())
		{
		case PieceType::KING:
			getAllKingMoves(state, position, piece, moves);
			break;
		case PieceType::BISHOP:
			break;
		case PieceType::PAWN:
			break;
		case PieceType::QUEEN:
			break;
		case PieceType::KNIGHT:
			getAllKnightMoves(state, position, piece, moves);
			break;
		case PieceType::ROOK:
			break;

		default:
			break;
		}

	}

	std::vector<Move> getAllMoves(const State& state) {
		auto colorToMove = state.getTurnColor();
		std::vector<Move> moves;
		for (unsigned int y = 0; y < 8; y++) {
			for (unsigned int x = 0; x < 8; x++) {
				Piece piece = state.board[x][y];
				if (piece.getType() == PieceType::NONE) {
					continue;
				}
				if (piece.getColor() == colorToMove) {
					getMovesForPiece(state, piece, { x,y }, moves);
				}
			}
		}

		return moves;
	}



	







}