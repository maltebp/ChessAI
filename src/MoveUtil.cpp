#include "MoveUtil.h"
#include <cstdlib>

namespace MoveUtil {

	Piece getFirstPieceInSlidingPosition(const State& state, Position origin, int dx, int dy) {
		Position currentPos = origin;

		while (true) {
			currentPos.x += dx;
			currentPos.y += dy;

			if (!currentPos.isFieldInBoard()) {
				break;
			}

			Piece currentPiece = state.getPiece(currentPos);

			if (currentPiece != Piece()) {
				return currentPiece;
			}
		}

		return Piece();
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

	/**
	Does not look at who's turn it is. Simply looks at positions of piece.
	*/
	bool isFieldThreatened(const State& state, const Position position, bool threatenedByBlack) {
		//From white's perspective, it is black that is the color to move. Vice versa.
		PieceColor colorToMove = threatenedByBlack ? PieceColor::BLACK : PieceColor::WHITE;

		//Check pawns
		//From whites perspective, pawns can attack from north and "down". Vice versea.
		int direction = threatenedByBlack ? -1 : 1;

		Position eastPawnPos = { position.x + 1, position.y + direction };
		Position westPawnPos = { position.x - 1, position.y + direction };

		Piece attackingPawn = Piece(colorToMove, PieceType::PAWN);

		if (eastPawnPos.isFieldInBoard() && state.getPiece(eastPawnPos) == attackingPawn) {
			return true;
		}
		if (westPawnPos.isFieldInBoard() && state.getPiece(westPawnPos) == attackingPawn) {
			return true;
		}

		// Check Knights
		Position knightPositions[] = {
			position.getNeighbourN().getNeighbourNW(),
			position.getNeighbourN().getNeighbourNE(),
			position.getNeighbourW().getNeighbourNW(),
			position.getNeighbourW().getNeighbourSW(),
			position.getNeighbourS().getNeighbourSW(),
			position.getNeighbourS().getNeighbourSE(),
			position.getNeighbourE().getNeighbourNE(),
			position.getNeighbourE().getNeighbourSE()
		};

		Piece currentKnight = Piece(colorToMove, PieceType::KNIGHT);

		for (auto knightPosition : knightPositions) {
			if (knightPosition.isFieldInBoard() && state.getPiece(knightPosition) == currentKnight) {
				return true;
			}
		}

		// Look at "sliding" positions
		Piece nsew[4] = {
			getFirstPieceInSlidingPosition(state, position, 1, 0),
			getFirstPieceInSlidingPosition(state, position, 0, 1),
			getFirstPieceInSlidingPosition(state, position, -1, 0),
			getFirstPieceInSlidingPosition(state, position, 0, -1)
		};

		for (auto nsewPiece : nsew) {
			if (nsewPiece == Piece(colorToMove, PieceType::ROOK)) return true;
			if (nsewPiece == Piece(colorToMove, PieceType::QUEEN)) return true;
		}

		Piece diagonals[4] = {
			getFirstPieceInSlidingPosition(state, position, 1, 1),
			getFirstPieceInSlidingPosition(state, position, 1, -1),
			getFirstPieceInSlidingPosition(state, position, -1, -1),
			getFirstPieceInSlidingPosition(state, position, -1, 1)
		};

		for (auto diagonalsPiece : diagonals) {
			if (diagonalsPiece == Piece(colorToMove, PieceType::BISHOP)) return true;
			if (diagonalsPiece == Piece(colorToMove, PieceType::QUEEN)) return true;
		}

		// Check kings
		Position kingPositions[] = {
			position.getNeighbourN(),
			position.getNeighbourS(),
			position.getNeighbourE(),
			position.getNeighbourW(),
			position.getNeighbourNE(),
			position.getNeighbourNW(),
			position.getNeighbourSW(),
			position.getNeighbourSE()
		};

		Piece attackingKing = Piece(colorToMove, PieceType::KING);

		for (auto kingPosition : kingPositions) {
			if (kingPosition.isFieldInBoard() && state.getPiece(kingPosition) == attackingKing) {
				return true;
			}
		}

		return false;
	}

	/**
	Return true if opponent player is in check
	False otherwise
	*/
	bool isKingThreatened(const State& state) {

		bool whitesTurn = state.turn % 2 == 0;

		PieceColor opponentColor = whitesTurn ? PieceColor::BLACK : PieceColor::WHITE;

		// Find king's position
		Position kingPosition = state.getPiecePosition({ opponentColor, PieceType::KING });

		//Check if the field is threatened from opponents pov.
		return isFieldThreatened(state, kingPosition, !whitesTurn);
	}

	void getPawnPromotionMoves(const State& state, Position oldPos, Piece piece, std::vector<Move>& moves) {
		PieceType possiblePieces[] = { PieceType::BISHOP, PieceType::KNIGHT, PieceType::QUEEN, PieceType::ROOK };
		unsigned int yval = piece.getColor() == PieceColor::WHITE ? 7 : 0;
		Position newPos = Position{ oldPos.x, yval };

		for (PieceType type : possiblePieces) {

			Move move = { oldPos,newPos, type };
			State newState = MoveUtil::executeMove(state, move);

			//Check for check
			if (isKingThreatened(newState)) {
				continue;
			}

			//Add to list of moves
			moves.push_back(move);
		}
	}
	
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
				bool whitePromotion = oldPos.y == 6 && piece.getColor() == PieceColor::WHITE;
				bool blackPromotion = oldPos.y == 1 && piece.getColor() == PieceColor::BLACK;

				if (whitePromotion || blackPromotion) { //If it is a promotion move
					getPawnPromotionMoves(state, oldPos, piece, moves);
				}
				else { //If it is an ordinary move
					positions.push_back(candidatePos);
				}
			}
		}
		

		//West capture
		candidatePos = { oldPos.x -1,oldPos.y + y_direction };
		if (candidatePos.isFieldInBoard()) {
			bool fieldEmpty = state.board[candidatePos.x][candidatePos.y].getType() == PieceType::NONE;
			bool enemyOnField = !fieldEmpty && state.board[candidatePos.x][candidatePos.y].getColor() != piece.getColor();
			bool isEnPassant = state.enPassantTarget == candidatePos;
			if (enemyOnField || isEnPassant) {
				positions.push_back(candidatePos);
			}
		}
		

		//East capture
		candidatePos = { oldPos.x +1,oldPos.y + y_direction };
		if (candidatePos.isFieldInBoard()) {
			bool fieldEmpty = state.board[candidatePos.x][candidatePos.y].getType() == PieceType::NONE;
			bool enemyOnField = !fieldEmpty && state.board[candidatePos.x][candidatePos.y].getColor() != piece.getColor();
			bool isEnPassant = state.enPassantTarget == candidatePos;
			if (enemyOnField || isEnPassant) {
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

		// // En Passant
		// if( state.enPassantTarget.isFieldInBoard() ){
		// 	int directionSign = 

		// 	Position leftPosition = oldPos;
		// 	leftPosition.x -= 1;
			
		// 	Position rightPosition = oldPos;
		// 	rightPosition.x += 1;

		// 	if( leftPosition == state.enPassantTarget || rightPosition == state.enPassantTarget) {
		// 		positions.push_back(state.enPassantTarget);
		// 	}
		// }

		for (Position newPos : positions)
		{
			Move move = { oldPos,newPos };
			State newState = MoveUtil::executeMove(state, move);

			//Check for check
			if (isKingThreatened(newState)) {
				continue;
			}

			//Add to list of moves
			moves.push_back(move);
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
			Move move = { oldPos,newPos };
			State newState = MoveUtil::executeMove(state, move);
			if (isKingThreatened(newState)) {
				continue;
			}

			//Add to list of moves
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
			Move move = { oldPos,newPos };
			State newState = MoveUtil::executeMove(state, move);
			if (isKingThreatened(newState)) {
				continue;
			}

			//Add to list of moves
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
			//Check that move does not "check" king
			Move move = { oldPos,newPos };
			State newState = MoveUtil::executeMove(state, move);
			if (isKingThreatened(newState)) {
				continue;
			}

			//Add to list of moves
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

	void getCastlingMoves(State state, std::vector<Move>& moves) {
		bool isWhite = state.getTurnColor() == PieceColor::WHITE;
		if (isWhite) {
			if (state.whiteCanCastleQueenSide) {
				//Check that fields in the middle are empty
				bool field1Empty = state.board[1][0].getType() == PieceType::NONE;
				bool field2Empty = state.board[2][0].getType() == PieceType::NONE;
				bool field3Empty = state.board[3][0].getType() == PieceType::NONE;
				if (field1Empty && field2Empty && field3Empty) {
					//Check that fields are not threatened
					if (!isFieldThreatened(state, { 2,0 }, true) &&
						!isFieldThreatened(state, { 3,0 }, true) &&
						!isFieldThreatened(state, { 4,0 }, true)
						) {
						moves.push_back({ { 4,0 } , {2,0} });
					}
				}
			}
			if (state.whiteCanCastleKingSide) {
				//Check that fields in the middle are empty
				bool field1Empty = state.board[5][0].getType() == PieceType::NONE;
				bool field2Empty = state.board[6][0].getType() == PieceType::NONE;
				if (field1Empty && field2Empty) {
					//Check that fields are not threatened
					if (!isFieldThreatened(state, { 4,0 }, true) &&
						!isFieldThreatened(state, { 5,0 }, true) &&
						!isFieldThreatened(state, { 6,0 }, true)
						) {
						moves.push_back({ { 4,0 } , {6,0} });
					}
				}
			}
		}
		else {//Is black
			if (state.blackCanCastleQueenSide) {
				//Check that fields in the middle are empty
				bool field1Empty = state.board[1][7].getType() == PieceType::NONE;
				bool field2Empty = state.board[2][7].getType() == PieceType::NONE;
				bool field3Empty = state.board[3][7].getType() == PieceType::NONE;
				if (field1Empty && field2Empty && field3Empty) {
					//Check that fields are not threatened
					if (!isFieldThreatened(state, { 2,7 }, false) &&
						!isFieldThreatened(state, { 3,7 }, false) &&
						!isFieldThreatened(state, { 4,7 }, false)
						) {
						moves.push_back({ { 4,7 } , {2,7} });
					}
				}
			}
			if (state.blackCanCastleKingSide) {
				//Check that fields in the middle are empty
				bool field1Empty = state.board[5][7].getType() == PieceType::NONE;
				bool field2Empty = state.board[6][7].getType() == PieceType::NONE;
				if (field1Empty && field2Empty) {
					//Check that fields are not threatened
					if (!isFieldThreatened(state, { 4,7 }, false) &&
						!isFieldThreatened(state, { 5,7 }, false) &&
						!isFieldThreatened(state, { 6,7 }, false)
						) {
						moves.push_back({ { 4,7 } , {6,7} });
					}
				}
			}
		}
	}

	std::vector<Move> getAllMoves(const State& state) {
		auto colorToMove = state.getTurnColor();

		//Generate ordinary moves
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

		//Castling - bools only indicate if piece have moved before, which rules out castling completely
		bool whiteCanCastle = state.whiteCanCastleKingSide || state.whiteCanCastleQueenSide;
		bool blackCanCastle = state.blackCanCastleKingSide || state.blackCanCastleQueenSide;
		bool playerCanCastle = (colorToMove == PieceColor::WHITE && whiteCanCastle) 
			|| (colorToMove == PieceColor::BLACK && blackCanCastle);
		if (playerCanCastle) {
			getCastlingMoves(state, moves);
		}

		//Promotion?


		return moves;
	}

	void updateCastlingBools(const State& oldState, Move move, State& newState) {
		bool isWhitesMove = oldState.turn % 2 == 0;

		//Update castling booleans
		Piece piece = oldState.getPiece(move.fromField);
		if (piece.getType() == PieceType::KING) {
			//Definitely disables castling for current player
			if (isWhitesMove) {
				newState.whiteCanCastleKingSide = false;
				newState.whiteCanCastleQueenSide = false;
			}
			else {
				newState.blackCanCastleKingSide = false;
				newState.blackCanCastleQueenSide = false;
			}
		}
		else if (piece.getType() == PieceType::ROOK) {
			//If we're moving from origianl rook position ==> disable that sides castling
			if (move.fromField == Position{ 0, 0 }) {
				newState.whiteCanCastleQueenSide = false;
			}
			else if (move.fromField == Position{ 7, 0 }) {
				newState.whiteCanCastleKingSide = false;
			}
			else if (move.fromField == Position{ 0, 7 }) {
				newState.blackCanCastleQueenSide = false;
			}
			else if (move.fromField == Position{ 7, 7 }) {
				newState.blackCanCastleKingSide = false;
			}
		}
	}
	
	State executeMove(const State& oldState, Move move) {
		State newState = oldState; //Copy constructor
		//NOTE: No sanity checks here for performance
		//TODO take care of special moves (castling, en pessant and so on)

		//"Pick up" old piece
		Piece piece = newState.board[move.fromField.x][move.fromField.y];
		newState[move.fromField] = { PieceColor::NONE, PieceType::NONE };

		//"Put on" new field
		newState[move.toField] = piece;

		// Check if double pawn move
 		if(piece.getType() == PieceType::PAWN && move.getYDistance() == 2) {
			int directionSign = oldState.turn % 2 == 0 ? 1 : -1;
			 newState.enPassantTarget = move.toField;
			 newState.enPassantTarget.y -= directionSign;
		}
		else {
			newState.enPassantTarget = Position();
		}

		// Check if En Passant
		if( oldState.enPassantTarget.isFieldInBoard() &&
			move.toField == oldState.enPassantTarget
		) {
			int enemyDirectionSign = oldState.turn % 2 == 0 ? -1 : 1;
			Position enemyPawnPosition = oldState.enPassantTarget;
			enemyPawnPosition.y += enemyDirectionSign;
			newState[enemyPawnPosition] = Piece();
		}	

		// Check if castling move
		unsigned int dx = move.getXDistance();
		if (piece.getType() == PieceType::KING && dx==2) {
			int yval = move.fromField.y;
			if (move.toField.x == 6) {
				//"Pick up" rook
				newState.board[7][yval] = { PieceColor::NONE, PieceType::NONE };

				//Put rook on new field
				newState.board[5][yval] = {piece.getColor(),PieceType::ROOK};
			}
			else {
				//"Pick up" rook
				newState.board[0][yval] = { PieceColor::NONE, PieceType::NONE };

				//Put rook on new field
				newState.board[3][yval] = { piece.getColor(),PieceType::ROOK };
			}
		}

		//Check if promotion move
		if (move.promotesTo != PieceType::NONE) {
			Piece oldPiece = newState[move.toField];
			newState[move.toField] = Piece{ oldPiece.getColor(), move.promotesTo };
		}

		updateCastlingBools(oldState, move, newState);

		//Increment turn counter
		newState.turn++;
		return newState;
	}
}