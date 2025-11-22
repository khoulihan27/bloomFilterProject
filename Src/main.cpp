#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <random>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <vector>
#include <unordered_set>

#include "bloom.h"
#include "hashFunc.h"

const int n = 10000; // size of S

// indices of each array map to one another for ease
std::vector<int> c = {2, 4, 6, 7, 10}; // const multiplied to n, fix this first
// k = num hashes, tune to find optimal for c (ceiling of c ln 2 is MOST optimal), test a wide range
// change this, fix c and then sweep k values (test MANY k's with each C)

int m = -1; // bloom size (c * n)
// size of universe = 2^31 - 1
const int p = 2147483647; // 2^31 - 1
std::unordered_set<int> stored(n);

std::vector<int> a; // type 1
std::vector<int> b; // type 2
std::vector<int> seeds; // varies depending on k --> type 2
std::mt19937 mt(time(nullptr)); // seed gen
std::uniform_int_distribution<int> uniformDist(0, p-1); // universe data gen, a and b gen
std::vector<double> primeTimeBloom;
std::vector<double> seedTimeBloom;

// final outputs
// hash tests
std::vector<double> finalAvgHashTimePrime;
std::vector<double> finalAvgHashTimeSeed;
std::vector<std::vector<int>> finalDistributionsPrime(0, std::vector<int>(0, 0));
std::vector<std::vector<int>> finalDistributionsSeed(0, std::vector<int>(0, 0));

// bloom tests
// each vector is the MEDIAN (OR AVERAGE??) false pos rate per k for THAT c --> {2, 5, 10, 15, 20} (index 0 -> c = 2)
// each index of those vectors corresponds to a k value where index 0 is k = 1

std::vector<std::vector<double>> finalFalsePosRatesPrime;
std::vector<std::vector<double>> finalFalsePosRatesSeed;

std::vector<hashFunc> hashesPrimes;
std::vector<hashFunc> hashesSeeds;

double falsePosPrime = 0.0;
double falsePosSeed = 0.0;

void testHashing();
void testBlooms();
void setSeeds();
void setAandB();
void setHashes(Bloom &bPrime, Bloom &bSeed);
void insertLinearData(Bloom &bPrime, Bloom &bSeed);
void insertRandomData(Bloom &bPrime, Bloom &bSeed);
void testFalsePos(Bloom &bPrime, Bloom &bSeed);
void writeHash(std::vector<int> primeIndices, std::vector<int> primeIndicesLinear, std::vector<double> primeTime, std::vector<double> primeTimeLinear,
                std::vector<int> seedIndices, std::vector<int> seedIndicesLinear, std::vector<double> seedTime, std::vector<double> seedTimeLinear,
                std::vector<int> storedRand, std::vector<int> storedLinear);
void writeBlooms();

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
        seeds[i] = uniformDist(mt);
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
        hashesPrimes.emplace_back(a[i], b[i], p, m);
        hashesSeeds.emplace_back(seeds[i], m);
    }
    bPrime.hashFunctions = hashesPrimes;
    bSeed.hashFunctions = hashesSeeds;
}


// Hash

// insert w/ random data --> use chrono to test time --> csv data
// test with linear data?
void testHashing() {
    // ten tests
    
    std::vector<int> primeIndices;
    std::vector<int> primeIndicesLinear;
    std::vector<double> primeTime;
    std::vector<double> primeTimeLinear;
    std::vector<int> seedIndices; 
    std::vector<int> seedIndicesLinear;
    std::vector<double> seedTime;
    std::vector<double> seedTimeLinear;
    std::vector<int> storedRand;
    std::vector<int> storedLinear;
        double a = (double)uniformDist(mt);
        while (a == 0) { // a cannot be 0!
            a = (double)uniformDist(mt);
        }
        double b = (double)uniformDist(mt);
        hashFunc prime(a, b, p, 10*n); // c = 10 
        hashFunc seed(mt(), 10*n);

        // measures time of each hash
        auto start = std::chrono::steady_clock::now();
        auto end = std::chrono::steady_clock::now();
        auto duration = end - start;
        
        // rand data
        int currData = 0;
        for (int i = 0; i < n; i++) {
            currData = uniformDist(mt);
            storedRand.push_back(currData);

            start = std::chrono::steady_clock::now();
            primeIndices.push_back(prime.hash(currData));
            end = std::chrono::steady_clock::now();
            duration = end - start;
            auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
            primeTime.push_back(durationMicro.count());

            start = std::chrono::steady_clock::now();
            seedIndices.push_back(seed.hash(currData));
            end = std::chrono::steady_clock::now();
            duration = end - start;
            durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
            seedTime.push_back(durationMicro.count());
        }

        // linear data first even n
        for (int i = 1; i < 2*n; i++) {
            if (i % 2 != 0) {
                i++;
            }
            storedLinear.push_back(i);

            start = std::chrono::steady_clock::now();
            primeIndicesLinear.push_back(prime.hash(i));
            end = std::chrono::steady_clock::now();
            duration = end - start;
            auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
            primeTimeLinear.push_back(durationMicro.count());

            start = std::chrono::steady_clock::now();
            seedIndicesLinear.push_back(seed.hash(i));
            end = std::chrono::steady_clock::now();
            duration = end - start;
            durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
            seedTimeLinear.push_back(durationMicro.count());
        }

        double primeTimeAvg = primeTime[0] + primeTimeLinear[0];
        for (int k = 1; k < primeTime.size(); k++) {
            primeTimeAvg += (primeTime[k]);
        }
        primeTimeAvg = primeTimeAvg/primeTime.size();
        finalAvgHashTimePrime.push_back(primeTimeAvg);

        double seedTimeAvg = seedTime[0];
        for (int k = 1; k < seedTime.size(); k++) {
            seedTimeAvg += seedTime[k];
        }
        seedTimeAvg = seedTimeAvg/seedTime.size();
        finalAvgHashTimeSeed.push_back(seedTimeAvg);
        
        finalDistributionsPrime.push_back(primeIndices);
        finalDistributionsSeed.push_back(seedIndices);
    // write to CSV w/ helper func  

    for (int o = 0; o < finalAvgHashTimePrime.size(); o++) {
        std::cout << finalAvgHashTimePrime[o] << std::endl;
    }
    for (int o = 0; o < finalAvgHashTimeSeed.size(); o++) {
        std::cout << finalAvgHashTimeSeed[o] << std::endl;
    }

    writeHash(primeIndices, primeIndicesLinear, primeTime, primeTimeLinear,
                seedIndices, seedIndicesLinear, seedTime, seedTimeLinear,
                storedRand, storedLinear);
}

void writeHash(std::vector<int> primeIndices, std::vector<int> primeIndicesLinear, std::vector<double> primeTime, std::vector<double> primeTimeLinear,
                std::vector<int> seedIndices, std::vector<int> seedIndicesLinear, std::vector<double> seedTime, std::vector<double> seedTimeLinear,
                std::vector<int> storedRand, std::vector<int> storedLinear) {
                std::fstream hashFilePrime, hashFileSeed;
                
                hashFilePrime.open("Data/hashPrime.csv", std::fstream::out);
                hashFileSeed.open("Data/hashSeed.csv", std::fstream::out);

                for (int i = 0; i < primeIndices.size(); i++) {
                    hashFilePrime << storedRand[i] << "," << storedLinear[i] << "," << primeIndices[i] << "," << primeIndicesLinear[i] << "," << primeTime[i] << "," << primeTimeLinear[i] << "," << "\n";
                    hashFileSeed << storedRand[i] << "," << storedLinear[i] << "," << seedIndices[i] << "," << seedIndicesLinear[i] << "," << seedTime[i] << "," << seedTimeLinear[i] << "," << "\n";
                }

                hashFilePrime.close();
                hashFileSeed.close();

                


}


// Blooms

void insertLinearData(Bloom &bPrime, Bloom &bSeed) {
    // measures time of each hash
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto duration = end - start;
    std::uniform_int_distribution<> startingRange(0, p-n);
    // avoid dups?
    int startIndex = startingRange(mt);
    std::cout << p-n << std::endl;
    std::cout << startIndex << std::endl;
    int storedIndex = 0;
    for (int i = startIndex; i < (startIndex + n); i++) {
        stored.insert(i);
        storedIndex++;

        start = std::chrono::steady_clock::now();
        bPrime.insert(i);
        end = std::chrono::steady_clock::now();
        duration = end - start;
        auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        primeTimeBloom.push_back(durationMicro.count());

        start = std::chrono::steady_clock::now();
        bSeed.insert(i);
        end = std::chrono::steady_clock::now();
        duration = end - start;
        durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        seedTimeBloom.push_back(durationMicro.count());
    }
}

void insertRandomData(Bloom &bPrime, Bloom &bSeed) {
    // measures time of each hash
    auto start = std::chrono::steady_clock::now();
    auto end = std::chrono::steady_clock::now();
    auto duration = end - start;
    int currData = 0;
    for (int i = 0; i < n; i++) {
        currData = uniformDist(mt);
        stored.insert(currData);
        // std::cout << "stored: " << stored[i] << std::endl;

        start = std::chrono::steady_clock::now();
        bPrime.insert(currData);
        end = std::chrono::steady_clock::now();
        duration = end - start;
        auto durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        primeTimeBloom.push_back(durationMicro.count());

        start = std::chrono::steady_clock::now();
        bSeed.insert(currData);
        end = std::chrono::steady_clock::now();
        duration = end - start;
        durationMicro = std::chrono::duration_cast<std::chrono::microseconds>(duration);
        seedTimeBloom.push_back(durationMicro.count());
    }
}

void testFalsePos(Bloom &bPrime, Bloom &bSeed) {
    int currVal = 0;
    double trueNegPrime = 0.0;
    double trueNegSeed = 0.0;
    for (int i = 0; i < 50000; i++) {
        // std::cout << i << std::endl;
        currVal = uniformDist(mt);

        auto it = stored.find(currVal);
        if (bPrime.contains(currVal) && it == stored.end()) {
            falsePosPrime++;
        }
        else if (!bPrime.contains(currVal) && it == stored.end()) {
            trueNegPrime++;
        }
        if (bSeed.contains(currVal) && it == stored.end()) {
            falsePosSeed++;
        }
        else if (!bSeed.contains(currVal) && it == stored.end()) {
            trueNegSeed++;
        }

        //std::cout << "val: " << currVal << ": in stored: " <<  !(it != stored.end()) << "; in bloom prime: " << bPrime.contains(currVal) << std::endl;
        //std::cout << "val: " << currVal << ": in stored: " <<  !(it != stored.end()) << "; in bloom seed: " << bSeed.contains(currVal) << std::endl;
        
    }
    falsePosPrime = falsePosPrime/(falsePosPrime + trueNegPrime);
    falsePosSeed = falsePosSeed/(falsePosSeed + trueNegSeed);
}


void testBlooms() {   
    for (int i = 0; i < c.size(); i++) { // going thru all c
        std::cout << "Testing c = " << c[i] << std::endl;
        std::vector<double> medianFPRPrime = {0};
        std::vector<double> medianFPRSeed = {0};
        for (int k = 1; k < 22; k++) {
            std::vector<double> kFPRPrime = {0};
            std::vector<double> kFPRSeed = {0};
            m = c[i] * n;
            seeds.resize(k);
            a.resize(k);
            b.resize(k);
            std::cout << "Testing k = " << k << std::endl;
            for (int j = 0; j < 11; j++) { // 10 tests per k value
                // make into vector in which i sum and divide at the end?
                Bloom TPrime(m, n);
                Bloom TSeed(m, n);
                falsePosPrime = 0.0;
                falsePosSeed = 0.0;
                std::unordered_set<int> newStored;
                stored = newStored;
                setSeeds();
                setAandB();
                setHashes(TPrime, TSeed);
                // insert linear also?
                insertRandomData(TPrime, TSeed);
                testFalsePos(TPrime, TSeed);
                kFPRPrime.push_back(falsePosPrime);
                kFPRSeed.push_back(falsePosSeed);
                hashesPrimes.resize(0);
                hashesSeeds.resize(0);
                primeTimeBloom.resize(0);
                seedTimeBloom.resize(0);
            }
            std::sort(kFPRPrime.begin(), kFPRPrime.end());
            medianFPRPrime.push_back(kFPRPrime[(kFPRPrime.size() / 2) + 1]);
            std::sort(kFPRSeed.begin(), kFPRSeed.end());
            medianFPRSeed.push_back(kFPRSeed[(kFPRSeed.size() / 2) + 1]);
            std::cout << kFPRPrime[(kFPRPrime.size() / 2) + 1] << std::endl;
            std::cout << kFPRSeed[(kFPRSeed.size() / 2) + 1] << std::endl;
        }
        finalFalsePosRatesPrime.push_back(medianFPRPrime);
        finalFalsePosRatesSeed.push_back(medianFPRSeed);
        std::cout << finalFalsePosRatesPrime[0][21] << std::endl;
    }
    writeBlooms();
}

void writeBlooms() {
    std::ofstream hashFilePrime, hashFileSeed;
    hashFilePrime.open("Data/bloomPrime.csv", std::ofstream::out);
    hashFileSeed.open("Data/bloomSeed.csv", std::ofstream::out);

    for (int i = 0; i < c.size(); i++) {
        for (int k = 1; k < 22; k++) {
            hashFilePrime << c[i] << "," << k << "," << finalFalsePosRatesPrime[i][k] << "," << "\n";
            hashFileSeed << c[i] << "," << k << "," << finalFalsePosRatesSeed[i][k] << "," << "\n";
    
        }    
    }
    /*
    hashFilePrime << -1 << "\n";
    hashFileSeed << -1 << "\n";
    hashFilePrime << "LINEAR\n";
    hashFileSeed << "LINEAR\n";
    for (int i = 0; i < primeIndicesLinear.size(); i++) {
        hashFilePrime << storedLinear[i] << "," << primeIndicesLinear[i] << "," << primeTimeLinear[i] << "," << "\n";
        hashFileSeed << storedLinear[i] << "," << seedIndicesLinear[i] << "," << seedTimeLinear[i] << "," << "\n";
    }
    */
    hashFilePrime.close();
    hashFileSeed.close();
}