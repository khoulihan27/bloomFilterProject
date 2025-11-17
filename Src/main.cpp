#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>
#include <ctime>
#include <fstream>
#include <unordered_set>
#include <algorithm>

#include "bloom.h"
#include "hashFunc.h"

const int n = 100000; // size of S

// indices of each array map to one another for ease
std::vector<int> c = {5, 10, 20, 50, 100, 200}; // const multiplied to n, fix this first
std::vector<int> k = {4, 8, 15, 36, 70, 140}; // num hashes, tune to find optimal for c (ceiling of c ln 2 is MOST optimal)

int m = -1; // bloom size (c * n)
// size of universe = 2147483647 (2^31 - 1)
const int p = 2147483647; // also the prime for the prime nums
int universe[p] = {0};
std::vector<int> stored(n);

std::vector<int> a; // type 1
std::vector<int> b; // type 2
std::vector<int> seeds; // varies depending on k --> type 2
std::mt19937 mt(time(nullptr)); // seed gen
std::uniform_int_distribution<int> uniformDist(0, p-1); // a and b gen
std::vector<std::chrono::duration<double, std::milli>> primeTimeBloom;
std::vector<std::chrono::duration<double, std::milli>> seedTimeBloom;

// final outputs
// hash tests
std::vector<std::chrono::duration<double, std::milli>> finalAvgHashTimePrime;
std::vector<std::chrono::duration<double, std::milli>> finalAvgHashTimeSeed;
std::vector<std::vector<int>> finalDistributionsPrime(1, std::vector<int>(0, 0));
std::vector<std::vector<int>> finalDistributionsSeed(1, std::vector<int>(0, 0));

// bloom tests
std::vector<double> finalFalsePosRatesPrime = {0};
std::vector<double> finalFalsePosRatesSeed = {0};

std::vector<hashFunc> hashesPrimes;
std::vector<hashFunc> hashesSeeds;

double avgFalsePosPrime = 0.0;
double avgFalsePosSeed = 0.0;

void testHashing();
void testBlooms();
void setSeeds();
void setAandB();
void buildUniverse();
void setHashes(Bloom &bPrime, Bloom &bSeed);
void insertData(Bloom &bPrime, Bloom &bSeed);
void testFalsePos(Bloom &bPrime, Bloom &bSeed);
void writeHash();
void writeBloom();

int main() {
    int test = 1;

    // output data into a csv or smth, parse/plot with python

    std::cout << "Enter part of report to test (1 = hashing, 2 = bloom, 3 = both)";
    std::cin >> test;

    switch(test) {
        case 1:
            testHashing();
            break;
        case 2:
            testBlooms();
            break;
        case 3:
            testHashing();
            testBlooms();
            break;
    }


    return 0;
}

// All

void setSeeds() {
    for (int i = 0; i < seeds.size(); i++) {
        seeds[i] = mt();
    }
}

void setAandB() {
    int aNum = 1;
    for (int i = 0; i < a.size(); i++) {
        aNum = uniformDist(mt);
        while (aNum == 0) { // a cannot be 0!
            aNum = uniformDist(mt);
        }
        a[i] = aNum;
        b[i] = uniformDist(mt);
    }
}

void setHashes(Bloom &bPrime, Bloom &bSeed) {
    for (int i = 0; i < a.size(); i++) {
        hashFunc tempHashPrime(a[i], b[i], p, m);
        hashFunc tempHashSeed(seeds[i], m);
        
        hashesPrimes[i] = tempHashPrime;
        hashesSeeds[i] = tempHashSeed;
    }
    bPrime.hashFunctions = hashesPrimes;
    bSeed.hashFunctions = hashesSeeds;
}

// insert from 0 --> U-1 (0-p-1)
void buildUniverse() {
    for (int i = 0; i < p; i++) {
        universe[i] = i;
    }
}


// Hash

// insert w/ random data --> use chrono to test time --> csv data
void testHashing() {
    // ten tests
    
    for (int i = 0; i < 10; i++) {
        std::vector<int> inserted;
        inserted.resize(1000); // n

        std::vector<int> primeIndices;
        primeIndices.resize(4000); // c = 4
        std::vector<std::chrono::duration<double, std::milli>> primeTime;
        int aNum = uniformDist(mt);
        while (aNum == 0) { // a cannot be 0!
            aNum = uniformDist(mt);
        }
        int a = aNum;
        int b = uniformDist(mt);
        hashFunc prime(a, b, p, 4000); // c = 4

        std::vector<int> seedIndices;
        seedIndices.resize(4000);
        std::vector<std::chrono::duration<double, std::milli>> seedTime;
        hashFunc seed(mt(), 4000);

        // measures time of each hash
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        for (int i = 0; i < 1000; i++) {
            inserted[i] = uniformDist(mt);

            start = std::chrono::steady_clock::now();
            primeIndices[i] = prime.hash(inserted[i]);
            end = std::chrono::steady_clock::now();
            duration = end - start;
            auto durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            primeTime.push_back(durationMilli);

            start = std::chrono::steady_clock::now();
            seedIndices[i] = seed.hash(inserted[i]);
            end = std::chrono::steady_clock::now();
            duration = end - start;
            durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
            seedTime.push_back(durationMilli);
        }

        std::cout << "Prime:" << std::endl;
        for (int p = 0; p < primeIndices.size(); p++) {
            std::cout << primeIndices[p] << std::endl;
        }

        std::cout << "Seed:" << std::endl;
        for (int p = 0; p < seedIndices.size(); p++) {
            std::cout << seedIndices[p] << std::endl;
        }
        
        auto primeTimeAvg = primeTime[0];
        for (int k = 1; k < primeTime.size(); k++) {
            primeTimeAvg += primeTime[k];
        }
        primeTimeAvg = primeTimeAvg/primeTime.size();
        finalAvgHashTimePrime.push_back(primeTimeAvg);

        auto seedTimeAvg = seedTime[0];
        for (int k = 1; k < seedTime.size(); k++) {
            seedTimeAvg += seedTime[k];
        }
        seedTimeAvg = seedTimeAvg/seedTime.size();
        finalAvgHashTimeSeed.push_back(seedTimeAvg);
        
        finalDistributionsPrime.push_back(primeIndices);
        finalDistributionsSeed.push_back(seedIndices);
    }
    // write to CSV w/ helper func
}

void writeHash() {

}


// Blooms

void insertData(Bloom &bPrime, Bloom &bSeed) {
    // measures time of each hash
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto duration = end - start;
    std::uniform_int_distribution<int> startingRange(0, p-n);
    // avoid dups?
    int startIndex = startingRange(mt);
    for (int i = startIndex; i < n; i++) {
        stored[i] = universe[i];

        start = std::chrono::steady_clock::now();
        bPrime.insert(stored[i]);
        end = std::chrono::steady_clock::now();
        duration = end - start;
        auto durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        primeTimeBloom.push_back(durationMilli);

        start = std::chrono::steady_clock::now();
        bSeed.insert(stored[i]);
        end = std::chrono::steady_clock::now();
        duration = end - start;
        durationMilli = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        seedTimeBloom.push_back(durationMilli);
    }
}

void testFalsePos(Bloom &bPrime, Bloom &bSeed) {
    int currIndex = 0;
    std::unordered_set<int> tested; // could make into a vector lwk
    for (int i = 0; i < 2*n; i++) {
        currIndex = uniformDist(mt);
        while (!(tested.find(universe[currIndex]) != tested.end())) {
            currIndex = uniformDist(mt);
        }
        tested.insert(universe[currIndex]);

        auto it = std::find(stored.begin(), stored.end(), universe[currIndex]);
        if (bPrime.contains(universe[currIndex]) && !(it != stored.end())) {
            avgFalsePosPrime++;
        }
        if (bSeed.contains(universe[currIndex]) && !(it != stored.end())) {
            avgFalsePosSeed++;
        }
        // TODO: compute false pos rate
    }
}

void testBlooms() {   
    for (int i = 0; i < c.size(); i++) { // going thru all c and k
        m = c[i] * k[i];
        Bloom TPrime(m, n);
        Bloom TSeed(m, n);
        seeds.resize(k[i]);
        a.resize(k[i]);
        b.resize(k[i]);
        // find better way
        primeTimeBloom.resize(0);
        seedTimeBloom.resize(0);
        for (int j = 0; j < 10; j++) { // 10 tests each
            // make into vector in which i sum and divide at the end?
            avgFalsePosPrime = 0.0;
            avgFalsePosSeed = 0.0;
            buildUniverse();
            setSeeds();
            setAandB();
            setHashes(TPrime, TSeed);
            insertData(TPrime, TSeed);
            testFalsePos(TPrime, TSeed); // ultimately, choose median (sort...)
        }
    }
}