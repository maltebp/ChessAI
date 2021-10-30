#include <iostream>
#include <fstream>
#include <iomanip>
#include <chrono>
#include <cassert>
#include <filesystem>

#include "Util.h"
#include "State.h"
#include "../src/MinMax.h" // Have to do this to avoid ambiguity with "minmax.h" (windows header file)


namespace fs = std::filesystem;


struct TestCase {
    unsigned int id;
    std::string name;
    State state;
};

const std::vector<TestCase> TEST_CASES = 
    {
        { 0, "Start state", State::createDefault() },
        { 1, "Misc state 1", {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"} }
    };

const unsigned int SAMPLES = 3;

const unsigned int DEPTH = 5;

std::ostream& out = std::cout;

std::fstream csvDepths;

std::fstream csvTotals;


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

    std::vector<MinMaxSearcher::Result> results;
    std::vector<MinMaxSearcher::Result> averageResults;

    for( int depth=1; depth <= DEPTH; depth++ ) {

        out << "    Depth " << depth  << ".. " << std::flush;

        MinMaxSearcher::Result samples[SAMPLES];
        MinMaxSearcher::Result averageSample;

        for( int i=0; i < SAMPLES; i++ ) {
            out << " Sample " << i << ".. " << std::flush;
            samples[i] = MinMaxSearcher::search(testCase.state, depth);

            averageSample.searchTime += samples[i].searchTime;
            averageSample.staticEvaluations += samples[i].staticEvaluations;
            averageSample.branchingFactor += samples[i].branchingFactor;
            averageSample.cutOffs += samples[i].cutOffs;
            averageSample.nodesVisited += samples[i].nodesVisited;
            averageSample.checkmates += samples[i].checkmates;
            averageSample.draws += samples[i].draws;
        }

        averageSample.searchTime /= SAMPLES;
        averageSample.staticEvaluations /= SAMPLES;
        averageSample.branchingFactor/= SAMPLES;
        averageSample.cutOffs /= SAMPLES;
        averageSample.nodesVisited /= SAMPLES;
        averageSample.checkmates /= SAMPLES;
        averageSample.draws /= SAMPLES;

        results.push_back(averageSample);

        out << "Done" << std::endl;
    }
    

    out << "\n  Results:" << std::endl;

    double averageBranchFactor = 0;
    unsigned long long totalNodesVisited = 0;
    unsigned long long totalCutOffs = 0;
    unsigned long long totalCheckMates = 0;
    unsigned long long totalDraws = 0;

    for( int i=0; i < DEPTH; i++ ) {

        MinMaxSearcher::Result depthResult = results[i];
        int cutOffFactor = (double)totalCutOffs / totalNodesVisited;


        out << '\n';
        out << "    Depth " << (i+1) << ":" << std::endl;
        out << "      Search time:    " << depthResult.searchTime << " sec" << std::endl;
        out << "      Nodes visited:  " << depthResult.nodesVisited << std::endl;
        out << "      Branch factor:  " << depthResult.branchingFactor << std::endl;
        out << "      Cut offs:       " << depthResult.cutOffs << " (" << cutOffFactor << ")" << std::endl;
        out << "      Evaluations:    " << depthResult.staticEvaluations << std::endl;
        out << "      Checkmates:     " << depthResult.checkmates << std::endl;
        out << "      Draws:          " << depthResult.draws << std::endl;

        csvDepths 
            << testCase.id << ','
            << i << ','
            << depthResult.searchTime << ','
            << depthResult.nodesVisited << ','
            << depthResult.branchingFactor << ','
            << depthResult.cutOffs << ','
            << depthResult.checkmates << ','
            << depthResult.draws 
            << std::endl;

        averageBranchFactor += depthResult.branchingFactor;
        totalNodesVisited += depthResult.nodesVisited;
        totalCutOffs += depthResult.cutOffs;
        totalCheckMates += depthResult.checkmates;
        totalDraws += depthResult.draws;
    }
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

    out << std::fixed<< std::setprecision(2);

    fs::path outputDir = fs::current_path() / "performance-tests/";
    fs::create_directories(outputDir);

    std::string timeString = Util::getDateString("%d-%m-%H-%M-%S");

    csvDepths.open(
        outputDir / (timeString + "-depths.csv"),
        std::fstream::out | std::fstream::app
    );
    csvDepths << std::fixed<< std::setprecision(2);
    csvDepths
        << "case,"
        << "depth,"
        << "time,"
        << "nodes,"
        << "branching,"
        << "cutoffs,"
        << "checkmates,"
        << "draws"
        << std::endl;

    runTests(TEST_CASES);

    csvTotals.close();
    csvDepths.close();
}