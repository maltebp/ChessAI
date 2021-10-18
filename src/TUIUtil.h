#pragma once

#include <iostream>

#include "State.h"


namespace TUIUtil {

    static void printBoard(const State& state, Move lastWhiteMove, Move lastBlackMove) {
        std::cout << "\n";

        auto getPositionColor = [&](Position position) {

            bool isColoredField = (position.x + 7-position.y) % 2;
            
            bool whitesTurn = state.turn % 2 == 0;
            bool whitePrecedence = !whitesTurn;

            bool isWhiteField = position == lastWhiteMove.fromField || position == lastWhiteMove.toField;
            bool isBlackField = position == lastBlackMove.fromField || position == lastBlackMove.toField;

            
            if( isWhiteField && (whitePrecedence || !isBlackField) ) {
                return "42";
            }

            if( isBlackField && (!whitePrecedence || !isWhiteField) ) {
                return "44";
            }

            return isColoredField ? "47;30" : "0";
        };

        for( int y=7; y >= 0 ; y-- ) {
            std::cout << '\t' << (y+1) << ' ';		
            for( int x=0; x < 8; x++ ) {
                Position position = { (unsigned int)x,(unsigned int)y };
                Piece piece = state.board[x][y];
                std::cout << "\033[0m" << "\033[" << getPositionColor(position) << "m"; // 47 = white background, 30 = black text
                std::cout << piece.getAlgebraicChar() << ' ';
                std::cout << "\033[0m"; // Reset terminal colors
            }
            std::cout << "\n";	
        }

        std::cout << "\t  ";
        for( int x=0; x < 8; x++ ) {
            std::cout << (char)('a' + x) << ' ';
        }
        std::cout << std::endl;
    }

}
