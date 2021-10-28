#pragma once

#include <functional>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <ctime>
#include <iomanip>



namespace Util {

    // Tests if the given Container (map, list etc.) contains the given element
    template<typename Container, typename T>
    static bool contains(const Container& container, T element) {
        auto it = std::find(container.begin(), container.end(), element);
        return it != container.end();
    }

     
    // Split the string into smaller strings delimited by the give delimiters.
    static std::vector<std::string> splitString(const std::string& input, const std::string& delimiters) {

        std::vector<std::string> tokens;
        std::stringstream ss;

        // Anonymous function to add current token to list
        auto addToken = [&](){
            std::string token = ss.str();
            if( token.length() > 0 ) {
                tokens.push_back(token);
            }
            ss.str("");
        };
            
        for( auto c : input ) {
            bool isDelimiter = Util::contains(delimiters, c);
            if(isDelimiter) {
                addToken();
            }
            else {
                ss << c;
            }
        }
        addToken();
        
        return tokens;
    }


    /**
     * @brief	Combines the list of strings into a single string, and insert the give 'combiner'
     *          between each (not before first and after last)
     * 
     * @param tokens    Vector of strings to combine
     * @param combiner  String to insert between two tokens (can be empty to simply combine them)
     * @param fromIndex Index of the first element from 'tokens' to combine
     * @return Combined string
     */
    static std::string combineStrings(const std::vector<std::string>& tokens, const std::string& combiner, size_t fromIndex = 0) {
        if( fromIndex >= tokens.size()) {
            return "";
        }

        std::stringstream ss;
        ss << tokens[fromIndex];
        for(size_t i = fromIndex+1; i < tokens.size(); i++ ) {
            ss << combiner << tokens[i];
        }

        return ss.str();
    }


    // See formatting @ https://en.cppreference.com/w/cpp/io/manip/put_time
    static std::string getDateString(const std::string& formatting) {
        auto now = std::chrono::system_clock::now();
        auto in_time_t = std::chrono::system_clock::to_time_t(now);

        tm newTime;
        localtime_s(&newTime, &in_time_t);

        std::stringstream ss;
        ss << std::put_time(&newTime, formatting.c_str());
        return ss.str();
    }

}

