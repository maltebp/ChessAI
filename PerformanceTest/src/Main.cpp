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


#ifdef _DEBUG
    #error "Do not run performance test in Debug mode!"
#endif


struct TestCase {
    unsigned int id;
    std::string name;
    State state;
};

const std::vector<TestCase> TEST_CASES = 
    {
        // WARNING: changing order or IDs of test cases will break our analysis setup

        { 0, "Start state", State::createDefault() },

        { 1, "Early game 1",    {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"} }, // Source: https://www.chessprogramming.org/Perft_Results
        { 2, "Early game 2",    {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"} }, // Source: https://www.chessprogramming.org/Perft_Results
        
        { 3, "Mid game 1",      {"5rk1/1R3p2/p5pp/2PbP3/5P2/8/PP3BPP/R5K1 w - - 1 27"} }, // Source: engine arena
        { 4, "Mid game 2",      {"N4b1Q/pp2kpp1/8/2p2b2/8/3P4/PP1K1PPP/n1B4R w - - 3 19"} }, // Source: engine arena
        
        { 5, "Late game 1",     {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1"} }, // Source: https://www.chessprogramming.org/Perft_Results
        { 6, "Late game 2",     {"8/4Rp2/4P1pk/3R3p/5P2/1P6/P4BPP/7K w - - 1 44"} }, // Source: engine arena
    };

const unsigned int SAMPLES = 2;

const unsigned int DEPTH = 6;

std::ostream& out = std::cout;

std::fstream csvDepths;


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
            out << " Sample " << (i+1) << ".. " << std::flush;
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
            << testCase.name << ","
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
        outputDir / (timeString + ".csv"),
        std::fstream::out | std::fstream::app
    );
    csvDepths << std::fixed<< std::setprecision(2);
    csvDepths
        << "case,"
        << "case_name,"
        << "depth,"
        << "time,"
        << "nodes,"
        << "branching,"
        << "cutoffs,"
        << "checkmates,"
        << "draws"
        << std::endl;

    auto startTime = std::chrono::system_clock::now();

    runTests(TEST_CASES);

    auto endTime = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    out << "\nTotal test time: " << (elapsed.count() / 1000.0) << " sec" << std::endl;

    csvDepths.close();
}