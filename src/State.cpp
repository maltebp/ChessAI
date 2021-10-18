#include "State.h"

#include <cassert>
#include <sstream>
#include <exception>


State::State(const char* fenString) {
    const char* next = fenString;

    int row = 7; 
    int column = 0;

    // 0 -> Board
    // 1 -> Turn
    // 2 -> Castling
    // 3 -> En Passant
    // 4 -> Half move since last capture or pawn advance(ignored)?
    // 5 -> Number of full moves
    int partIndex = 0;

    bool whitesTurn = false;
    whiteCanCastleKingSide = false;
    whiteCanCastleQueenSide = false;
    blackCanCastleKingSide = false;
    blackCanCastleQueenSide = false;

    std::string fullPart = "";

    char c;
    while(true) {
        c = *next;
        next++;

        if( c == ' ' || c == '\0' ) {

            // Parse En Passant position
            if( partIndex == 3 ) {
                if( fullPart != "-" ) {
                    bool validPosition = Position::fromAlgebraicNotation(fullPart, enPassantTarget);
                    assert(validPosition);
                }
            }

            if( partIndex == 4 ) {
                drawCounter = std::stoi(fullPart);
                assert(drawCounter >= 0);
            }
            
            // Parse turn number
            if( partIndex == 5 ) {
                int turnNumber = std::stoi(fullPart);
                assert(turnNumber > 0);
                // FEN turn starts from 1, we start from 0
                turn = turnNumber*2 - (whitesTurn ? 2 : 1);
            }

            if( c == '\0' ) {
                break;
            }
            
            fullPart = "";
            partIndex++;
            continue;
        }

        fullPart += c;

        assert(partIndex <= 5);

        // Board
        if( partIndex == 0 ) {
            if( c == '/' ) {
                column = 0;
                row--;
            }
            else if ( c >= '1' && c <= '9' ) {
                column += (int)c - (int)'1' + 1;
            }
            else {
                bool validPiece = Piece::fromAlgebraicChar(c, board[column][row]);
                assert(validPiece);
                column++;
            }
        }

        // Whose turn
        if( partIndex == 1 ) {
            assert(c == 'w' || c == 'b');
            whitesTurn = c == 'w';
        }

        // Castling
        if( partIndex == 2 ) {
            if( c == 'K' ) {
                whiteCanCastleKingSide = true;
            }
            else if( c == 'Q' ) {
                whiteCanCastleQueenSide = true;
            }
            else if( c == 'k' ) {
                blackCanCastleKingSide = true;
            }
            else if( c == 'q' ) {
                blackCanCastleQueenSide = true;
            }
            else {
                assert(c == '-');
            }
        }

        // En passant
        if( partIndex == 3) {
            // Handled by "fullPart string"
        }

        // We don't store this in our state yet
        if( partIndex == 4) {
            // Handled by "fullPart string"
        }

        if( partIndex == 5) {
            assert(c >= '0' && c <= '9');
        }
    }
}


// Create state from FEN-string
State::State(const std::string& fenString)
    :   State(fenString.data())
{ }


Position State::getPiecePosition(const Piece& piece) const {
    for(unsigned int x=0; x<8; x++) {
        for(unsigned int y=0; y<8; y++) {
            if (board[x][y] == piece) {
                return {x,y};
            }
        }
    }
    return Position();
}


PieceColor State::getTurnColor() const {
    return turn % 2 ? PieceColor::BLACK: PieceColor::WHITE;
}


bool State::operator==(const State& other) const {		
    if( turn != other.turn ) return false;
    if( enPassantTarget != other.enPassantTarget ) return false;
    if( whiteCanCastleKingSide != other.whiteCanCastleKingSide ) return false;
    if( whiteCanCastleQueenSide != other.whiteCanCastleQueenSide ) return false;
    if( blackCanCastleKingSide != other.blackCanCastleKingSide ) return false;
    if( blackCanCastleQueenSide != other.blackCanCastleQueenSide ) return false;
    if( drawCounter != other.drawCounter ) return false;

    for(unsigned int x=0; x<8; x++) {
        for(unsigned int y=0; y<8; y++) {
            if( board[x][y] != other.board[x][y] ) {
                return false;
            }
        }
    }

    return true;
}


bool State::operator!=(const State& other) const {
    return !(*this == other);
}


const Piece& State::operator[](Position position) const {
    assert(position.x < 8);
    assert(position.y < 8);
    return board[position.x][position.y];
}


Piece& State::operator[](const Position& position) {
    assert(position.x < 8);
    assert(position.y < 8);
    return board[position.x][position.y];
}





std::string State::toFEN() const {
    std::stringstream fen;

    // The board
    int emptyCount = 0;
    for( int y = 7; y >= 0; y-- ) {
        for( int x = 0; x < 8; x++ ) {
            Piece piece = board[x][y];

            if( piece == Piece() ) {
                emptyCount++;
            }
            else {
                if( emptyCount != 0 ) {
                    fen << emptyCount;
                    emptyCount = 0;
                }
                fen << piece.getAlgebraicChar();
            }
        }

        if( emptyCount != 0 ) {
            fen << emptyCount;
            emptyCount = 0;
        }
        if( y != 0 ) {
            fen << '/';
        }
    }

    // Which player to make move
    fen << " " << (turn % 2 == 0 ? 'w' : 'b');
    
    // Castling
    fen << " ";
    std::string castlingFlags;
    if( whiteCanCastleKingSide ) {
        castlingFlags += "K";
    }
    if( whiteCanCastleQueenSide ) {
        castlingFlags += "Q";
    }
    if( blackCanCastleKingSide ) {
        castlingFlags += "k";
    }
    if( blackCanCastleQueenSide ) {
        castlingFlags += "q";
    }
    fen << (castlingFlags.size() == 0 ? "-" : castlingFlags);

    // En Passant
    fen << ' ' << (enPassantTarget == Position() ? "-" : enPassantTarget.toAlgebraicNotation());

    // Moves since last pawn or captue (not recorded in our game yet)
    fen << ' ' << drawCounter;

    // Turn
    fen << ' '  << (turn / 2 + 1);

    return fen.str();
}


State State::createDefault() {
    
    State state;

    state.board[0][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
    state.board[1][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
    state.board[2][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
    state.board[3][0] = Piece(PieceColor::WHITE, PieceType::QUEEN);
    state.board[4][0] = Piece(PieceColor::WHITE, PieceType::KING);
    state.board[5][0] = Piece(PieceColor::WHITE, PieceType::BISHOP);
    state.board[6][0] = Piece(PieceColor::WHITE, PieceType::KNIGHT);
    state.board[7][0] = Piece(PieceColor::WHITE, PieceType::ROOK);
    state.board[0][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[1][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[2][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[3][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[4][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[5][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[6][1] = Piece(PieceColor::WHITE, PieceType::PAWN);
    state.board[7][1] = Piece(PieceColor::WHITE, PieceType::PAWN);


    state.board[0][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
    state.board[1][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
    state.board[2][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
    state.board[3][7] = Piece(PieceColor::BLACK, PieceType::QUEEN);
    state.board[4][7] = Piece(PieceColor::BLACK, PieceType::KING);
    state.board[5][7] = Piece(PieceColor::BLACK, PieceType::BISHOP);
    state.board[6][7] = Piece(PieceColor::BLACK, PieceType::KNIGHT);
    state.board[7][7] = Piece(PieceColor::BLACK, PieceType::ROOK);
    state.board[0][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[1][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[2][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[3][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[4][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[5][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[6][6] = Piece(PieceColor::BLACK, PieceType::PAWN);
    state.board[7][6] = Piece(PieceColor::BLACK, PieceType::PAWN);

    return state;
}