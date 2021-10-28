#include "Process.h"

#include <cassert>
#include <cassert>
#include <exception>
#include <sstream>


namespace Util {

    constexpr size_t OUTPUT_BUFFER_SIZE = 1;


    Process::Process(const std::vector<std::string>& commandLine, OutputListener stdOutListener, OutputListener stdErrorListener)
        :   stdOutListener(stdOutListener),
            stdErrorListener(stdErrorListener)
    {
        assert(stdOutListener != nullptr && "stdOutListener must not be null");
        assert(stdErrorListener != nullptr && "stdErrorListener must not be null");

        std::vector<const char*> args;
        for( auto& element : commandLine ) {
            args.push_back(element.c_str());        
        }
        // subprocess.h requires a NULL element in the end 
        args.push_back(NULL);
        
        int creationResult = subprocess_create(
            args.data(),
            subprocess_option_enable_async,
            &process
        );

        if( creationResult != 0 ) {
            throw std::exception("Engine process failed to start");
        }

        started = true;

        processStdIn = subprocess_stdin(&process);

        readOutputThread = new std::thread([=](){
            readOutput(subprocess_read_stdout, stdOutListener);
        });

        readErrorThread = new std::thread([=](){
            readOutput(subprocess_read_stderr, stdErrorListener);
        });
    }


    Process::~Process() {
        terminate();
    }

    void Process::writeLine(const std::string& input) {
        int result = fputs((input+'\n').c_str(), processStdIn);    
        if( result < 0 ) {
            throw std::exception("Process 'fputs' error");
        }
        fflush(processStdIn);
    }


    int Process::terminate() {

        if( !started || terminated ) return 0;

        // Note: we are ignoring the success code on both of these subprocess calls
        subprocess_terminate(&process);
        int returnCode;
        subprocess_join(&process, &returnCode);

        readOutputThread->join();
        readErrorThread->join();

        delete readOutputThread;
        delete readErrorThread;

        subprocess_destroy(&process);

        terminated = true;
        
        return returnCode;
    }


    void Process::readOutput( OutputReader outputReader, OutputListener listener) {
        char buffer[OUTPUT_BUFFER_SIZE];    
        unsigned int bytesRead;
        std::stringstream builder;
        while( (bytesRead = outputReader(&process, buffer, OUTPUT_BUFFER_SIZE)) != 0) {
            for(unsigned int i=0; i<bytesRead; i++) {
                char c = buffer[i];
                if( c == '\n' || c == '\r') {
                    std::string line = builder.str();
                    if( line.length() > 0 ) {
                        listener(line);
                    }
                    builder.str("");
                }
                else {
                    builder << c;
                }            
            }
        }

        // Notify of any remaining output
        std::string line = builder.str();
        if( line.length() > 0 ) {
            listener(line);
        }
    }



}