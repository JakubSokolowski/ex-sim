//
// Created by jakub on 16.06.18.
//

#ifndef HESOYAM_RANDUTIL_H
#define HESOYAM_RANDUTIL_H

#include <random>
#include <thread>


void WaitRandom(int low, int up) {
    std::mt19937 rng_m{ std::random_device{}()};
    static thread_local std::uniform_int_distribution<> dist(low, up);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng_m)));
}
int RandomInRange(int low, int up) {
    std::mt19937 rng_m{ std::random_device{}()};
    std::uniform_int_distribution<> dist(low, up);
    return dist(rng_m);
}
#endif //HESOYAM_RANDUTIL_H
