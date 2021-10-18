#pragma once

#include <sstream>

#include "Util.h"
#include "State.h"


namespace FENUtil {

    
    void parseFENBoard(const std::string& boardToken, State& state) {
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

        
    void parseFENTurn(const std::string& colorToken, const std::string& turnToken, State& state) {

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


    void parseFENCastling(const std::string& castlingToken, State& state) {
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


    void parseFENEnPassant(const std::string& enPassantToken, State& state) {

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


    void parseFENDrawCounter(const std::string& drawCounterToken, State& state) {
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

    void parseFEN(const std::string& fenString, State& state) {
        std::vector<std::string> tokens = Util::splitString(fenString, " \t\n\0");

        assert(tokens.size() == 6);

        parseFENBoard(tokens[0], state);
        parseFENCastling(tokens[2], state);
        parseFENEnPassant(tokens[3], state);
        parseFENDrawCounter(tokens[4], state);
        parseFENTurn(tokens[1], tokens[5], state);
    }


    std::string toFEN(const State& state) {
        return std::string();
    }

}