#include "MoveUtil.h"

namespace MoveUtil {
	
	void getAllPawnMoves(const State& state, Position oldPos, Piece piece, std::vector<Move>& moves) {
		std::vector<Position> positions;
		
		int y_initial = 6;
		int y_direction = -1;
		if (piece.getColor() == PieceColor::WHITE) {
			y_direction = 1;
			y_initial = 1;
		}
		
		//Forward
		Position candidatePos = { oldPos.x,oldPos.y + y_direction };
		if (candidatePos.isFieldInBoard()) {
			bool fieldEmpty = state.board[candidatePos.x][candidatePos.y].getType() == PieceType::NONE;
			if (fieldEmpty) {
				positions.push_back(candidatePos);
			}
		}
		

		//West capture
		candidatePos = { oldPos.x -1,oldPos.y + y_direction };
		if (candidatePos.isFieldInBoard()) {
			bool fieldEmpty = state.board[candidatePos.x][candidatePos.y].getType() == PieceType::NONE;
			bool enemyOnField = !fieldEmpty && state.board[candidatePos.x][candidatePos.y].getColor() == PieceColor::WHITE;
			if (enemyOnField) {
				positions.push_back(candidatePos);
			}
		}
		

		//East capture
		candidatePos = { oldPos.x +1,oldPos.y + y_direction };
		if (candidatePos.isFieldInBoard()) {
			bool fieldEmpty = state.board[candidatePos.x][candidatePos.y].getType() == PieceType::NONE;
			bool enemyOnField = !fieldEmpty && state.board[candidatePos.x][candidatePos.y].getColor() == PieceColor::WHITE;
			if (enemyOnField) {
				positions.push_back(candidatePos);
			}
		}

		//Double forward move
		candidatePos = { oldPos.x,oldPos.y + 2*y_direction };
		bool isInInitialPosition = oldPos.y == y_initial;
		if (candidatePos.isFieldInBoard()) {
			bool wayIsClear = state.board[oldPos.x][oldPos.y+y_direction].getType() == PieceType::NONE;
			bool goalIsClear = state.board[oldPos.x][oldPos.y + 2*y_direction].getType() == PieceType::NONE;
			if (isInInitialPosition && wayIsClear && goalIsClear) {
				positions.push_back(candidatePos);
			}
		}
		

		for (Position newPos : positions)
		{
			//Check for check

			//Add to list of moves
			Move move = { oldPos,newPos };
			moves.push_back(move);
		}

	}

	void addSlidingPositions(int dx, int dy, const State& state, Position oldPos, Piece piece, std::vector<Position>& positions) {
		Position currentPos = oldPos;
		while (true) {
			currentPos.x += dx;
			currentPos.y += dy;

			if (!currentPos.isFieldInBoard()) {
				break;
			}

			Piece currentPiece = state.board[currentPos.x][currentPos.y];


			if (currentPiece.getType() == PieceType::NONE) {
				positions.push_back(currentPos);
				continue;
			}

			bool isFriendlyPiece = currentPiece.getColor() == piece.getColor();
			if (isFriendlyPiece) {
				break;
			}

			positions.push_back(currentPos);

			bool isEnemyPiece = currentPiece.getColor() != piece.getColor();
			if (isEnemyPiece) {
				break;
			}
		}
	}

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

	void getAllSliderMoves(const State& state, Position oldPos, Piece piece, std::vector<Move>& moves) {
		std::vector<Position> positions;
		//NSEW
		if (piece.getType() != PieceType::BISHOP) {
			addSlidingPositions(1, 0, state, oldPos, piece, positions);
			addSlidingPositions(-1, 0, state, oldPos, piece, positions);
			addSlidingPositions(0, 1, state, oldPos, piece, positions);
			addSlidingPositions(0, -1, state, oldPos, piece, positions);
		}

		//Diagonals
		if (piece.getType() != PieceType::ROOK) {
			addSlidingPositions(1, 1, state, oldPos, piece, positions);
			addSlidingPositions(-1, -1, state, oldPos, piece, positions);
			addSlidingPositions(-1, 1, state, oldPos, piece, positions);
			addSlidingPositions(1, -1, state, oldPos, piece, positions);
		}

		for (Position newPos : positions)
		{
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
		case PieceType::ROOK:
		case PieceType::BISHOP:
		case PieceType::QUEEN:
			getAllSliderMoves(state, position, piece, moves);
			break;
		case PieceType::PAWN:
			getAllPawnMoves(state, position, piece, moves);
			break;
		case PieceType::KNIGHT:
			getAllKnightMoves(state, position, piece, moves);
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


	State executeMove(State& oldState, Move move) {
		State newState = oldState; //Copy constructor
		//NOTE: No sanity checks here for performance
		//TODO take care of special moves (castling, en pessant and so on)

		//"Pick up" old piece
		Piece piece = newState.board[move.fromField.x][move.fromField.y];
		newState.board[move.fromField.x][move.fromField.y] = { PieceColor::NONE, PieceType::NONE };

		//"Put on" new field
		newState.board[move.toField.x][move.toField.y] = piece;

		//Increment turn counter
		newState.turn++;
		return newState;
	}


	







}