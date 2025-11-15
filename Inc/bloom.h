#ifndef BLOOM_H_
#define BLOOM_H_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "hashFunc.h"

class Bloom {
    public:
        Bloom(int mIn, int nIn);
        void addHash(std::vector<hashFunc> hashIn);
        void insert(int x);
        bool contains(int x);
        std::vector<hashFunc> hashFunctions;

    private:
        std::vector<bool> H;
        int m; // size, if c needed --> m/n
        int n;
};




#endif