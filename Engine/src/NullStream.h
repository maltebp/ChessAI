#pragma once

#include <iostream>


class NullStream : public std::ostream {
private:

    class Buffer : public std::streambuf {
    public:

        int overflow(int c) { 
            return c;
        }

    };

public:

    NullStream() 
        : std::ostream(&buffer)
    { }


private:

    Buffer buffer;

};

