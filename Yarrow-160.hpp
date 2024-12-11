#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <random>
#include <cstdint>
#include <thread>

#include "SHA-1.hpp"
#include "DES.hpp"

using namespace std;

class YarrowGenerator {
private:
    uint32_t n;         // размер шифруемого сообщения 
    uint32_t k;         // размер ключа
    uint32_t P1;        // количество бит, после генерации которых необходимо запустить обновление ключа
    uint32_t P2;        // количество бит, после которых нужно запустить обновление ключа и счётчика
    uint32_t sigma1;
    uint32_t sigma2;
    uint32_t C;         // количество запусков механизмов обновления
    uint32_t counter;   // начальное значение n-битного счётчика
    string KEY;         // значение ключа
    string entropy;     // накопленная энтропия
    uint64_t seed;      // зерно генератора
public:
    YarrowGenerator(uint64_t initial_seed = 0);
    ~YarrowGenerator();

    uint32_t generate();
private:
    void collect_entroty();
    void updateKey();
    void updateKeyCounter();
    string encrypt(string str);
};