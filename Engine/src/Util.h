#pragma once

#include <functional>
#include <vector>
#include <string>
#include <sstream>


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

}

