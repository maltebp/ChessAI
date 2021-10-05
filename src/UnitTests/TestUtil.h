#pragma once

#include "external/catch.hpp"


// Tests if the given Container (map, list etc.) contains the given element
template<typename Container, typename T>
bool contains(const Container& container, T element) {
    auto it = std::find(container.begin(), container.end(), element);
    return it != container.end();
}
