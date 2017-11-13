//
// Created by arrouan on 28/09/16.
//

#ifndef PDC_EVOL_MODEL_CHRONO_H
#define PDC_EVOL_MODEL_CHRONO_H

#include <chrono>

class Chrono {
public:
    Chrono() {}
    Chrono(char * name) {
        this->name = name;
    }
    Chrono(char * name, long duration): Chrono(name) {
        this->duration = duration;
    }

    void start() {
        startTime = std::chrono::high_resolution_clock::now();
    }

    long stop() {
        endTime = std::chrono::high_resolution_clock::now();
        long duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime-startTime).count();
        return duration;
    }

    long duration;

private:
    char * name;
    std::chrono::_V2::system_clock::time_point startTime, endTime;
};


#endif //PDC_EVOL_MODEL_CHRONO_H
