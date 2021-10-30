#include <iostream>
#include <iomanip>
#include <chrono>
#include <cassert>

#include "State.h"
#include "../src/MinMax.h" // Have to do this to avoid ambiguity with "minmax.h" (windows header file)


const unsigned int SAMPLES = 4;

const unsigned int DEPTH = 7;

std::ostream& out = std::cout;


struct TestCase {
    std::string name;
    State state;
};


void runTestCase(TestCase testCase) {

    out << "\n\n";
    out << "---------------------------------------------------" << std::endl;

    out << '\n';
    out << testCase.name << std::endl;
    out << testCase.state.toFEN() << std::endl;

    // Count pieces
    int whitePieces = 0;
    int blackPieces = 0;
    for( int x = 0; x < 8; x++ ) {
        for( int y = 0; y < 8; y++ ) {
            Piece piece = testCase.state.board[x][y];
            if( piece.getType() == PieceType::NONE ) {
                continue;
            }

            if( piece.getColor() == PieceColor::WHITE ) {
                whitePieces++;
            }
            else {
                blackPieces++;
            }
        }
    }  

    out << "\n  Pieces:" << std::endl;
    out << "    White: " << whitePieces << std::endl;
    out << "    Black: " << blackPieces << std::endl;

    out << "\n  Running tests..." << std::endl;
    MinMaxSearcher::Result samples[SAMPLES];
    for( int i=0; i < SAMPLES; i++ ) {
        out << "    Sample 1.. " << std::flush;
        samples[i] = MinMaxSearcher::search(testCase.state, DEPTH);
        out << "Done" << std::endl;
    }

    out << "\n  Results:" << std::endl;

    MinMaxSearcher::Result averageResult;

    // Compute sum
    for( int i=0; i < DEPTH + 1; i++ ) {
        averageResult.depthResults.push_back(MinMaxSearcher::DepthResult());
    }
    for( int i=0; i < SAMPLES; i++ ) {
        averageResult.searchTime += samples[i].searchTime;
        averageResult.staticEvaluations += samples[i].staticEvaluations;

        for( int j=0; j < DEPTH + 1; j++ ) {
            averageResult.depthResults[j].branchingFactor += samples[i].depthResults[j].branchingFactor;
            averageResult.depthResults[j].cutOffs += samples[i].depthResults[j].cutOffs;
            averageResult.depthResults[j].nodesVisited += samples[i].depthResults[j].nodesVisited;
        }
    }
    
    // Average sum
    averageResult.searchTime /= SAMPLES;
    averageResult.staticEvaluations /= SAMPLES;

    for( int i=0; i < DEPTH + 1; i++ ) {
        averageResult.depthResults[i].branchingFactor /= SAMPLES;
        averageResult.depthResults[i].cutOffs /= SAMPLES;
        averageResult.depthResults[i].nodesVisited /= SAMPLES;
    }

    for( int i=0; i < DEPTH + 1; i++ ) {
        MinMaxSearcher::DepthResult depthResult = averageResult.depthResults[DEPTH-i];
        out << '\n';
        out << "    Depth " << i << ":" << std::endl;
        out << "      Nodes visited:  " << depthResult.nodesVisited << std::endl;
        out << "      Branch factor:  " << depthResult.branchingFactor << std::endl;
        out << "      Cut offs:       " << depthResult.cutOffs << std::endl;
    }

    double averageBranchFactor = 0;
    unsigned long long totalNodesVisited = 0;
    unsigned long long totalCutOffs = 0;

    for( int i=0; i < DEPTH + 1; i++ ) {
        averageBranchFactor += averageResult.depthResults[i].branchingFactor;
        totalNodesVisited += averageResult.depthResults[i].nodesVisited;
        totalCutOffs += averageResult.depthResults[i].cutOffs;
    }

    averageBranchFactor /= (DEPTH-1);

    int cutOffFactor = (int) ((double)totalCutOffs / totalNodesVisited * 100);

    out << '\n';
    out << "    Totals: " << std::endl;
    out << "      Search time:     " << averageResult.searchTime << " sec" << std::endl;
    out << "      Evaluations:     " << averageResult.staticEvaluations << std::endl;
    out << "      Nodes visisted:  " << totalNodesVisited << std::endl;
    out << "      Branch factor:   " << averageBranchFactor << std::endl;
    out << "      Cut offs:        " << totalCutOffs << " (" << cutOffFactor << "%)" << std::endl;
}


void runTests(std::vector<TestCase> testCases) {
    assert(testCases.size() > 0 && "There must be at least 1 test case");

    out << "\nRunning Performance Tests" << std::endl;

    out << "\nStates:" << std::endl;

    for( auto& testCase : testCases ) {
        out << "  " << testCase.name << "    (" << testCase.state.toFEN() << ")" << std::endl;  
    }

    out << '\n';
    out << "Samples: " << SAMPLES << std::endl;;
    out << "Depth:   " << DEPTH << std::endl;;

    for( auto& testCase : testCases ) {
        runTestCase(testCase);
    }
}


int main(int argc, char* argv[]) {

    out << std::setprecision(2);

    runTests({

        { "Start state", State::createDefault() },

        

    });

}