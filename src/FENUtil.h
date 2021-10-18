#pragma once

#include <sstream>

#include "Util.h"
#include "State.h"


namespace FENUtil {

    
    static void parseFENBoard(const std::string& boardToken, State& state) {
        assert(!boardToken.empty());

        int x = 0;
        int y = 7; // FEN is parsed from top-left to bottom right

        for( auto c : boardToken ) {
            if( c == '/' ) {
                x = 0;
                y--;
                continue;
            }

            assert(x >= 0 && x <= 7);
            assert(y <= 7 && y >= 0);

            if ( c >= '1' && c <= '9' ) {
                x += c - '0';
            }
            else {
                Piece piece;
                bool validPiece = Piece::fromAlgebraicChar(c, piece);
                state.board[x][y] = piece;
                assert(validPiece);
                x++;
            }
        }         
    }

        
    static void parseFENTurn(const std::string& colorToken, const std::string& turnToken, State& state) {

        assert(colorToken == "w" || colorToken == "b");
        bool whitesTurn = colorToken == "w";

        assert(turnToken.size() > 0);

        int turnNumber;
        try {
            turnNumber = std::stoi(turnToken);
        }
        catch(const std::exception&) {
            assert(false);
        }

        assert(turnNumber > 0);

        state.turn = turnNumber*2 - (whitesTurn ? 2 : 1);
    }


    static void parseFENCastling(const std::string& castlingToken, State& state) {
        assert(!castlingToken.empty());
        assert(castlingToken.size() <= 4);

        state.whiteCanCastleKingSide = false;
        state.whiteCanCastleQueenSide = false;
        state.blackCanCastleKingSide = false;
        state.blackCanCastleQueenSide = false;

        if( castlingToken.size() == 1 ) {
            assert(castlingToken[0] == '-');
            return;
        }

        assert( castlingToken.size() <= 4 );

        for( auto c : castlingToken ) {
            if( c == 'K' ) state.whiteCanCastleKingSide = true;
            if( c == 'Q' ) state.whiteCanCastleQueenSide = true;
            if( c == 'k' ) state.blackCanCastleKingSide = true;
            if( c == 'q' ) state.blackCanCastleQueenSide = true;
        }
    }


    static void parseFENEnPassant(const std::string& enPassantToken, State& state) {

        if( enPassantToken.size() == 1 ) {
            assert(enPassantToken[0] == '-');
            state.enPassantTarget = Position();
            return;
        }

        assert(enPassantToken.size() == 2);

        Position enPassantTarget;
        bool validPosition = Position::fromAlgebraicNotation(enPassantToken, enPassantTarget);
        assert(validPosition);

        state.enPassantTarget = enPassantTarget;
    }


    static void parseFENDrawCounter(const std::string& drawCounterToken, State& state) {
        assert(!drawCounterToken.empty());

        int drawCounter;;
        try {
            drawCounter = std::stoi(drawCounterToken);
        }
        catch(const std::exception&) {
            assert(false);
        }

        assert(drawCounter >= 0);
        state.drawCounter = drawCounter;
    }


    static void parseFEN(const std::string& fenString, State& state) {
        std::vector<std::string> tokens = Util::splitString(fenString, " \t\n\0");

        assert(tokens.size() == 6);

        parseFENBoard(tokens[0], state);
        parseFENCastling(tokens[2], state);
        parseFENEnPassant(tokens[3], state);
        parseFENDrawCounter(tokens[4], state);
        parseFENTurn(tokens[1], tokens[5], state);
    }


    static std::string generateFEN(const State& state) {
        std::stringstream fen;

        // The board
        int emptyCount = 0;
        for( int y = 7; y >= 0; y-- ) {
            for( int x = 0; x < 8; x++ ) {
                Piece piece = state.board[x][y];

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
        fen << " " << (state.turn % 2 == 0 ? 'w' : 'b');
        
        // Castling
        fen << " ";
        std::string castlingFlags;
        if( state.whiteCanCastleKingSide ) {
            castlingFlags += "K";
        }
        if( state.whiteCanCastleQueenSide ) {
            castlingFlags += "Q";
        }
        if( state.blackCanCastleKingSide ) {
            castlingFlags += "k";
        }
        if( state.blackCanCastleQueenSide ) {
            castlingFlags += "q";
        }
        fen << (castlingFlags.size() == 0 ? "-" : castlingFlags);

        // En Passant
        fen << ' ' << (state.enPassantTarget == Position() ? "-" : state.enPassantTarget.toAlgebraicNotation());

        // Moves since last pawn or captue (not recorded in our game yet)
        fen << ' ' << state.drawCounter;

        // Turn
        fen << ' '  << (state.turn / 2 + 1);

        return fen.str();
    }

}