#include "hashFunc.h"
#include <random>
#include <cmath>

hashFunc::hashFunc() {
    // basic constructor --> does nothing!
}

hashFunc::hashFunc(double aIn, double bIn, int pIn, int mIn) {
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

hashFunc::hashFunc(const hashFunc &h) {
    this->a = h.a;
    this->b = h.b;
    this->p = h.p;
    this->m = h.m;
    this->s = h.s;
    this->size = h.size;
    this->uniformDist = h.uniformDist;
    this->hashType = h.hashType;
}

hashFunc& hashFunc::operator=(const hashFunc &h) {
    if (this != &h) {
        this->a = h.a;
        this->b = h.b;
        this->p = h.p;
        this->m = h.m;
        this->s = h.s;
        this->size = h.size;
        this->uniformDist = h.uniformDist;
        this->hashType = h.hashType;
    }
    return *this;
}

int hashFunc::hash(int x) {
    // type 1
    switch(hashType) {
        // type 1
        case PRIME:
            {
            // does the simple prime hash function
            int primed = static_cast<int>(fmod(fmod(a*x+b, p), m));
            return primed;
            break;
            }
        // type 2
        case SEED:
            {
            // gets a random index between 0 and size of H given a random number gen
            std::mt19937 mt(s+x); // seed
            return uniformDist(mt);
            break;
            }
        // in-case type defined as NONE (should never happen)    
        default:
            {
            return -1;
            break;
            }
    }
}