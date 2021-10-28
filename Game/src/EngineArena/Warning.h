#pragma once

#define WARN(x, ...) printf("\033[33mWarning: " x "\033[0m\n", ##__VA_ARGS__)