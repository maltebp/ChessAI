#pragma once

#include <mutex>
#include <condition_variable>

namespace Util {

        // Simplifying wrapper around the condition_variable for  boolean variables
    template<typename T>
    class VariableSignal {
    public:


        T wait() {
            std::unique_lock lock(mutex);

            if( valueWritten ) {
                return t;
            }

            T localT;
            conditionVar.wait(lock, [&](){
                if( valueWritten ) {
                    localT = t;
                    return true;
                }
                return false;
            });
            return localT;
        }


        void set(const T& valueToWrite) {
            {
                std::lock_guard lock(mutex);
                t = valueToWrite;
                valueWritten = true;
            }
            conditionVar.notify_all();
        }


        void reset() {
            {
                std::lock_guard lock(mutex);
                valueWritten = false;
            }
        }


        void operator=(const T& value) {
            set(value);
        }


    private:

        T t;

        bool valueWritten = false;

        std::mutex mutex;

        std::condition_variable conditionVar;

    };

}
