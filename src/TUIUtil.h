#pragma once

#include <sstream>

#include "State.h"


namespace TUIUtil {

    static std::string getPrettyBoard(const State& state, const std::string& prefix, Move lastWhiteMove = Move(), Move lastBlackMove = Move()) {
    
        const int BOARD_WIDTH = 8*4+1; // In characters (without column labels)

        std::stringstream ss;        

        // Figures out whos color should be displayed on the position
        auto getPositionHighlightColor = [&](Position position) {
            bool whitesTurn = state.turn % 2 == 0;
            bool whitePrecedence = !whitesTurn;

            bool isWhiteField = position == lastWhiteMove.fromField || position == lastWhiteMove.toField;
            bool isBlackField = position == lastBlackMove.fromField || position == lastBlackMove.toField;

            if( lastWhiteMove != Move() && isWhiteField && (whitePrecedence || !isBlackField) ) {
                return PieceColor::WHITE;
            }

            if( lastBlackMove != Move() && isBlackField && (!whitePrecedence || !isWhiteField) ) {
                return PieceColor::BLACK;
            }

            return PieceColor::NONE;
        };

        // Print column labels
        auto printColumnLabels = [&]() {
            ss << prefix << "  ";
            for( int x=0; x < BOARD_WIDTH; x++ ) {
                char columnLabel = ('a' + x / 4);
                bool placeColumnLabel = (x % 2 == 0 && x % 4 != 0);
                ss << (placeColumnLabel ? columnLabel : ' ');
            }
        };

        // Print top labels
        printColumnLabels();

        ss << "\n";
        
        for( int y=16; y >= 0 ; y-- ) {

            ss << prefix;

            // Horizontal divider 
            if( y % 2 == 0 ) {
                ss << "  ";
                for( int x = 0; x < BOARD_WIDTH; x++ ) {
                    ss << (x % 4 == 0 ? '+' : '-');
                }
            }
            // Line with Piece characters
            else {
                int rowNumber = (y / 2 + 1);


                // Print right row label
                ss << rowNumber << ' ';

                for( int x=0; x < BOARD_WIDTH; x++ ) {
                    if( x % 4 == 0 ) {
                        ss << '|';
                    }
                    else if(x % 2 == 0) {   
                        Position position = { (unsigned int) (x/4),(unsigned int)(y/2) };
                        PieceColor positionHighlight = getPositionHighlightColor(position);

                        char pieceChar = state[position].getAlgebraicChar();

                        if( positionHighlight != PieceColor::NONE ) {
                            std::string terminalHightColor = positionHighlight == PieceColor::WHITE ? "42": "44";
                            ss << "\033[" << terminalHightColor << "m"; // Set background
                            ss << pieceChar;
                            ss << "\033[0m"; // Reset terminal colors
                        }
                        else {
                            ss << pieceChar;
                        }
                    }
                    else {
                        ss << ' ';
                    }
                }

                // Print left row label
                ss << ' ' << rowNumber;
            }
            ss << "\n";	
        }

        // Print bottom column labels
        printColumnLabels();

        return ss.str();        
    }

}
