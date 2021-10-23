#pragma once

#include <sstream>

#include "State.h"


namespace TUIUtil {

    static std::string getPrettyBoard(const State& state, Move lastWhiteMove = Move(), Move lastBlackMove = Move()) {
        
        std::stringstream ss;

        auto getPositionColor = [&](Position position) {

            bool whitesTurn = state.turn % 2 == 0;
            bool whitePrecedence = !whitesTurn;

            bool isWhiteField = position == lastWhiteMove.fromField || position == lastWhiteMove.toField;
            bool isBlackField = position == lastBlackMove.fromField || position == lastBlackMove.toField;
            
            if( lastWhiteMove != Move() && isWhiteField && (whitePrecedence || !isBlackField) ) {
                return "42";
            }

            if( lastBlackMove != Move() && isBlackField && (!whitePrecedence || !isWhiteField) ) {
                return "44";
            }

            return "0";
        };

        for( int y=16; y >= 0 ; y-- ) {

            if( y % 2 == 1 ) {
                ss << '\t' << (y / 2 + 1) << ' ';
            }
            else {
                ss << "\t  ";
            }

            for( int x=0; x < 8*4+1; x++ ) {

                // Row line 
                if( y % 2 == 0 ) {
                    ss << (x % 4 == 0 ? '+' : '-');
                }

                // Piece line
                else {
                    if( x % 4 == 0 ) {
                        ss << '|';
                    }
                    else if(x % 2 == 0) {      
                        Position position = { (unsigned int) (x/4),(unsigned int)(y/2) };
                        Piece piece = state[position];
                        ss << "\033[0m" << "\033[" << getPositionColor(position) << "m"; // 47 = white background, 30 = black text
                        ss << piece.getAlgebraicChar();
                        ss << "\033[0m"; // Reset terminal colors
                    }
                    else {
                        ss << ' ';
                    }
                }
            }
            ss << "\n";	
        }

        ss << "\t  ";
        for( int x=0; x < 8*4; x++ ) {
            bool columnLabel = (x % 2 == 0 && x % 4 != 0);
            ss << (columnLabel ? (char)('a' + x / 4) : ' ');
        }

        return ss.str();        
    }

}
