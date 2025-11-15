#include "hashFunc.h"
#include <random>

hashFunc::hashFunc() {
    
}

hashFunc::hashFunc(int aIn, int bIn, int pIn, int mIn) {
    a = aIn;
    b = bIn;
    p = pIn;
    m = mIn;
    hashType = PRIME;
}

hashFunc::hashFunc(int sIn, int sizeIn) {
    s = sIn;
    size = sizeIn;
    std::uniform_int_distribution<int> temp(0, size);
    uniformDist = temp;
    hashType = SEED;
}

hashFunc::hashFunc(hashFunc &h) {
    a = h.a;
    b = h.b;
    p = h.p;
    m = h.m;
    s = h.s;
    size = h.size;
    uniformDist = h.uniformDist;
    hashType = h.hashType;
}

int hashFunc::hash(int x) {
    // type 1
    switch(hashType) {
        // type 1
        case PRIME:
            // does the simple prime hash function
            return ((a*x+b)%p)%m;
            break;
        // type 2
        case SEED:
            // gets a random index between 0 and size of H given a random number gen
            std::mt19937 mt(s+x); // seed
            return uniformDist(mt);
            break;
        // in-case type defined as NONE (should never happen)    
        default:
            return -1;
            break;
    }
}