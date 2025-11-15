#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include "bloom.h"

Bloom::Bloom(int mIn, int nIn) {
    m = mIn;
    n = nIn;
    H.resize(m);
}

void Bloom::addHash(std::vector<hashFunc> hashIn) {
    hashFunctions = hashIn;
}

void Bloom::insert(int x) {
    int currIndex = 0;
    for (int i = 0; i < hashFunctions.size(); i++) {
        currIndex = hashFunctions[i].hash(x);
        H[currIndex] = true;
    }
}

bool Bloom::contains(int x) {
    int currIndex = 0;
    for (int i = 0; i < hashFunctions.size(); i++) {
        currIndex = hashFunctions[i].hash(x);
        if (H[currIndex] == false) {
            return false;
        }
    }
    return true;
}

