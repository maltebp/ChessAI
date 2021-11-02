#include "MoveUtil.h"
#include <cstdlib>

namespace MoveUtil {


	int manhattanDistFromMiddle(const Position pos) {
		int xDist = pos.x < 4 ? 3 - pos.x : 4 - pos.x;
		xDist = xDist < 0 ? -xDist : xDist;
		int yDist = pos.y < 4 ? 3 - pos.y : 4 - pos.y;
		yDist = yDist < 0 ? -yDist : yDist;

		return xDist + yDist;
	}


	Piece getFirstPieceInSlidingPosition(const State& state, Position origin, int dx, int dy) {
		Position currentPos = origin;

		while (true) {
			currentPos.x += dx;
			currentPos.y += dy;

			if (!currentPos.isFieldInBoard()) {
				break;
			}

			Piece currentPiece = state[currentPos];

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
		bool threathenedByPawn = isPawnThreathening(state, position, threatenedByBlack);
		if (threathenedByPawn) {
			return true;
		}

		// Check Knights
		bool threathenedByKnight = isKnightThreathening(state, position, threatenedByBlack);
		if (threathenedByKnight) {
			return true;
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
			if (kingPosition.isFieldInBoard() && state[kingPosition] == attackingKing) {
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

	void getPawnPromotionMoves(const State& state, Position oldPos, Piece piece, GenerationList& moves) {
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
	
	void getAllPawnMoves(const State& state, Position oldPos, Piece piece, GenerationList& moves) {
		static std::vector<Position> positions;
		positions.clear();
		
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

		 // En Passant
		if( state.enPassantTarget.isFieldInBoard() ){
			int yDir = piece.getColor() == PieceColor::WHITE ? 1 : -1;
			
			Position leftPosition = { oldPos.x-1, oldPos.y+yDir };
		 	Position rightPosition = { oldPos.x+1, oldPos.y + yDir };

		 	if( leftPosition == state.enPassantTarget || rightPosition == state.enPassantTarget) {
		 		positions.push_back(state.enPassantTarget);
		 	}
		}

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

	

	void getAllKingMoves(const State& state,Position oldPos, Piece piece, GenerationList& moves) {
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

	void getAllKnightMoves(const State& state, Position oldPos, Piece piece, GenerationList& moves) {
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

	/*
	Gets the reachable sliding positions for a sliding piece
	This does not check if the king is checked, if the piece moves there (which is more computationally expensive)
	*/
	void getAllSliderPositionsForPiece(const State& state, Position oldPos, Piece piece, std::vector<Position>& positions) {
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
	}

	/*
	Gets the possible moves for a sliding piece
	DOES check if the king is checked, if the piece moves there
	*/
	void getAllSliderMoves(const State& state, Position oldPos, Piece piece, GenerationList& moves) {
		static std::vector<Position> positions;
		positions.clear();

		getAllSliderPositionsForPiece(state, oldPos, piece, positions);

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


	void getMovesForPiece(const State& state, const Piece piece, Position position, GenerationList& moves) {

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

	void getCastlingMoves(State state, GenerationList& moves) {
		bool isWhite = state.getTurnColor() == PieceColor::WHITE;
		bool whiteKingInPlace = state.board[4][0].getType() == PieceType::KING 
			&& state.board[4][0].getColor()==PieceColor::WHITE;
		bool blackKingInPlace = state.board[4][7].getType() == PieceType::KING
			&& state.board[4][7].getColor() == PieceColor::BLACK;
		if (isWhite && whiteKingInPlace) {
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
		else if (blackKingInPlace) {//Is black
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

	
	void getAllMoves(const State& state, GenerationList& moves) {

		moves.clear();
		auto colorToMove = state.getTurnColor();

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
	}


	void updateCastlingBools(const State& oldState, Move move, State& newState) {
		bool isWhitesMove = oldState.turn % 2 == 0;

		//Update castling booleans
		Piece piece = oldState[move.fromField];
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

		// If anything moves from or to Rook fields, we can safely
		// disable castling in the given side
		if( move.toField == Position{0,0} || move.fromField == Position{0,0}) {
			newState.whiteCanCastleQueenSide = false;
		}
		if( move.toField == Position{7,0} || move.fromField == Position{7,0}) {
			newState.whiteCanCastleKingSide = false;
		}
		if( move.toField == Position{0,7} || move.fromField == Position{0,7}) {
			newState.blackCanCastleQueenSide = false;
		}
		if( move.toField == Position{7,7} || move.fromField == Position{7,7}) {
			newState.blackCanCastleKingSide = false;
		}
	}
	
	State executeMove(const State& oldState, Move move) {
		State newState = oldState; //Copy constructor
		//NOTE: No sanity checks here for performance
		//TODO take care of special moves (castling, en pessant and so on)

		newState.drawCounter++;

		//"Pick up" old piece
		Piece piece = newState.board[move.fromField.x][move.fromField.y];
		newState[move.fromField] = { PieceColor::NONE, PieceType::NONE };

		PieceColor targetColor = newState[move.toField].getColor();
		bool isEnemyOnPosition = targetColor != PieceColor::NONE && targetColor != piece.getColor();
		if( piece.getType() == PieceType::PAWN || isEnemyOnPosition ) {
			newState.drawCounter = 0;
		}
		
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

		// Check if En Passant capture
		if( oldState.enPassantTarget.isFieldInBoard() &&
			move.toField == oldState.enPassantTarget
		) {
			int enemyDirectionSign = oldState.turn % 2 == 0 ? -1 : 1;
			Position enemyPawnPosition = oldState.enPassantTarget;
			enemyPawnPosition.y += enemyDirectionSign;
			newState[enemyPawnPosition] = Piece();
			newState.drawCounter = 0;
		}	

		// Check if castling move
		unsigned int dx = move.getXDistance();
		bool isCastlingMove = piece.getType() == PieceType::KING && dx == 2;
		if (isCastlingMove) {
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

			//Update bool indicating that castling has happened
			if (piece.getColor() == PieceColor::WHITE) {
				newState.whiteHasCastled = true;
			}
			else {
				newState.blackHasCastled = true;
			}
		}

		// Check if promotion move
		if (move.promotesTo != PieceType::NONE) {
			Piece oldPiece = newState[move.toField];
			newState[move.toField] = Piece{ oldPiece.getColor(), move.promotesTo };
		}

		updateCastlingBools(oldState, move, newState);

		// Increment turn counter
		newState.turn++;
		return newState;
	}

	/**
	Is a rook threathening this position?
	*/
	bool isRooksThreathening(const State& state, Position pos, bool whitesPerspective) {
		Piece nsew[4] = {
			getFirstPieceInSlidingPosition(state, pos, 1, 0),
			getFirstPieceInSlidingPosition(state, pos, 0, 1),
			getFirstPieceInSlidingPosition(state, pos, -1, 0),
			getFirstPieceInSlidingPosition(state, pos, 0, -1)
		};

		PieceColor enemyColor = whitesPerspective ? PieceColor::BLACK : PieceColor::WHITE;
		for (auto nsewPiece : nsew) {
			if (nsewPiece == Piece(enemyColor, PieceType::ROOK)) return true;
		}
		return false;
	}

	/**
	Is a bishop threathening this position?
	*/
	bool isBishopThreathening(const State& state, Position pos, bool whitesPerspective) {
		Piece diagonals[4] = {
			getFirstPieceInSlidingPosition(state, pos, 1, 1),
			getFirstPieceInSlidingPosition(state, pos, 1, -1),
			getFirstPieceInSlidingPosition(state, pos, -1, -1),
			getFirstPieceInSlidingPosition(state, pos, -1, 1)
		};
		
		PieceColor enemyColor = whitesPerspective ? PieceColor::BLACK : PieceColor::WHITE;
		for (auto piece : diagonals) {
			if (piece == Piece(enemyColor, PieceType::BISHOP)) return true;
		}
		return false;
	}

	/**
	Is a knight threathening this position?
	*/
	bool isKnightThreathening(const State& state, Position position, bool whitesPerspective) {
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

		PieceColor enemyColor = whitesPerspective ? PieceColor::BLACK : PieceColor::WHITE;
		Piece enemyKnight = Piece(enemyColor, PieceType::KNIGHT);

		for (auto knightPosition : knightPositions) {
			if (knightPosition.isFieldInBoard() && state[knightPosition] == enemyKnight) {
				return true;
			}
		}
		return false;
	}

	/**
	Is a pawn threathening this position
	*/
	bool isPawnThreathening(const State& state, Position pos, bool whitesPerspective) {
		
		int enemyYDirection = whitesPerspective ? -1 : 1;
		Position westEnemyPos = { pos.x - 1, pos.y - enemyYDirection };
		Position eastEnemyPos = { pos.x + 1, pos.y - enemyYDirection };

		PieceColor enemyPieceColor = whitesPerspective ? PieceColor::BLACK : PieceColor::WHITE;
		bool pawnInWest = westEnemyPos.isFieldInBoard()
			&& state[westEnemyPos].getColor() == enemyPieceColor
			&& state[westEnemyPos].getType() == PieceType::PAWN;

		bool pawnInEast = eastEnemyPos.isFieldInBoard()
			&& state[eastEnemyPos].getColor() == enemyPieceColor
			&& state[eastEnemyPos].getType() == PieceType::PAWN;

		//Threathened by en passant (rare but possible case)
		bool isInEnPassantSquare = state.enPassantTarget.x == pos.x 
			&& state.enPassantTarget.y == pos.y + enemyYDirection;
		bool isPawn = state[pos].getType() == PieceType::PAWN;
		bool threathenedByEnPassant = isInEnPassantSquare && isPawn;

		return pawnInEast || pawnInWest || threathenedByEnPassant;
	}


}