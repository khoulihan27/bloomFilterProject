#ifndef HASHFUNC_H_
#define HASHFUNC_H_
#include <random>

enum types {
    PRIME,
    SEED,
    NONE
};

class hashFunc {
    public:
        hashFunc(); // default
        hashFunc(double aIn, double bIn, int pIn, int mIn); // type 1
        hashFunc(int sIn, int sizeIn); // type 2
        hashFunc(const hashFunc &h); // copy
        hashFunc& operator=(const hashFunc &h); // copy operator
        int hash(int x); // performs hash with given x
        types hashType = NONE; // type of hash

    private:
        // type 1 (prime number)
        double a = -1;
        double b = -1;
        int p = -1;
        int m = -1;
        
        // type 2 (seed)
        int s = -1;
        int size = -1; // size of H 
        std::uniform_int_distribution<int> uniformDist;

};


#endif