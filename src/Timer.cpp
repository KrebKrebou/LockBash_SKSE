#include "Timer.h"

namespace LB {

    int Wait::Cooldown(int val) {
        Active = true;
        while (Active == true) {

            std::this_thread::sleep_for(std::chrono::milliseconds(val));

            Active = false;
        }
        return 0;
    }

    int Wait::Timer(int val) {

            std::this_thread::sleep_for(std::chrono::milliseconds(val));
            return 0;
    }
}
