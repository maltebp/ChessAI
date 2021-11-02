#pragma once

#include <mutex>
#include <condition_variable>


namespace Util {


    class Signal {
    public:

        
        void wait() {
            std::unique_lock lock(mutex); 
            if( signal ) {
                return;
            }

            conditionVar.wait(lock, [this](){
                return this->signal;
            });
        }

        
        void set() {
            {
                std::lock_guard lock(mutex);
                signal = true;
            }
            conditionVar.notify_all();
        }

        
        void reset() {
            {
                std::lock_guard lock(mutex);
                signal = false;
            }
        }


        bool isSet() {
            {
                std::lock_guard lock(mutex);
                return signal;
            }
        }


    private:

        bool signal = false;

        std::mutex mutex;

        std::condition_variable conditionVar;

    };

}