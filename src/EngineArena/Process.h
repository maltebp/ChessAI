#pragma once

#include <vector>
#include <string>
#include <thread>
#include <functional>

#include "external/subprocess.h"


class Process {
public:

    using OutputListener = std::function<void(const std::string&)>;

    Process(const std::vector<std::string>& commandLine, OutputListener stdOutListener, OutputListener stdErrorListener);

    Process(const Process&) = delete;

    Process(Process&&) = delete;

    ~Process();

    void writeLine(const std::string& input);

    int terminate();

    Process& operator=(const Process&) = delete;

private:

    void readOutput();

    void readError();

private:

    bool started = false;

    bool terminated = false;

    subprocess_s process;

    FILE* processStdIn;

    std::thread* readOutputThread;

    std::thread* readErrorThread;

    OutputListener stdOutListener;

    OutputListener stdErrorListener;

};