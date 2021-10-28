#pragma once

#include <vector>
#include <string>
#include <thread>
#include <functional>

#include "external/subprocess.h"


namespace Util {

    class Process {
    public:

        using OutputListener = std::function<void(const std::string&)>;

    public:

        Process(const std::vector<std::string>& commandLine, OutputListener stdOutListener, OutputListener stdErrorListener);

        Process(const Process&) = delete;

        Process(Process&&) = delete;

        ~Process();

        void writeLine(const std::string& input);

        int terminate();

        Process& operator=(const Process&) = delete;

    private:

        // Signature for subprocess_read_stderr and subprocess_read_stdout
        using OutputReader = std::function<unsigned int(subprocess_s*, char*, unsigned int)>;

    private:

        void readOutput(OutputReader, OutputListener);

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

}