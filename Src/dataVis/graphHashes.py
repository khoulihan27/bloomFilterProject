# goal: return arrays with data!
# refer to the guidelines!

import csv
import matplotlib
import matplotlib.pyplot as plt
import math
import numpy as np
    
randomStored = list()
linearStored = list()

primeRandIndices = list()
primeRandTime = list() # microsec
primeLinearIndices = list()
primeLinearTime = list() # microsec

seedRandIndices = list()
seedRandTime = list() # microsec
seedLinearIndices = list()
seedLinearTime = list() # microsec
    
# parses prime hash data
def parsePrime():
   # prime reader
    print("parsing prime")
    i = 0
    r = 1
    with open('Data/hashPrime.csv', mode='r') as csvfile:
        hashReader = csv.reader(csvfile)
        for row in hashReader:
            i = 0
            print(r)
            for value in row:
                match i:
                    case 0:
                        randomStored.append(float(value))
                    case 1:
                        linearStored.append(float(value))
                    case 2:
                        primeRandIndices.append(float(value))
                    case 3:
                        primeLinearIndices.append(float(value))
                    case 4:
                        primeRandTime.append(float(value))
                    case 5:
                        primeLinearTime.append(float(value))  
                i+=1 
            r+=1   
            
# parses seed hash data
def parseSeed():
    # seed reader
    i = 0
    with open('Data/hashSeed.csv', mode='r') as csvfile:
        hashReader = csv.reader(csvfile)
        for row in hashReader:
            i = 0
            for value in row:
                match i:
                    # rand and linear alr stored, no need to look again
                    case 2:
                        seedRandIndices.append(float(value))
                    case 3:
                        seedLinearIndices.append(float(value))
                    case 4:
                        seedRandTime.append(float(value))
                    case 5:
                        seedLinearTime.append(float(value))
                i += 1   
   
# plots indices for prime and seeded hashing         
def plotHashes():
    print("plotting hashes")
    fig, ax = plt.subplots(2, 2, figsize=(15,10))
    
    ax[0][0].scatter(randomStored, primeRandIndices, color='blue', s=1)
    ax[0][0].set_title("Prime Modulo Hashing (Random Data)")
    ax[0][0].autoscale()
    ax[0][1].scatter(linearStored, primeLinearIndices, color='red', s=1)
    ax[0][1].set_title("Prime Modulo Hashing (Linear Data)")
    ax[0][1].autoscale()
    
    
    ax[1][0].scatter(randomStored, seedRandIndices, color='green', s=1)
    ax[1][0].set_title("Seeded Hashing (Random Data)")
    ax[1][0].autoscale()
    ax[1][1].scatter(linearStored, seedLinearIndices, color='black', s=1)
    ax[1][1].set_title("Seeded Hashing (Linear Data)")
    ax[1][1].autoscale()
    fig.tight_layout()

    print("finished plotting")

    plt.savefig("Graphs/hashes.png")
    plt.clf()
         
# plots runtimes of seeded and prime hashing   
def plotTime():
    print("plotting time")
    fig, ax = plt.subplots(1, 2, figsize=(10,5))
    ax[0].scatter(randomStored, primeRandTime, color='orange', label='Prime Modulo', s=2)
    ax[0].scatter(randomStored, seedRandTime, color='green', label='Seeded Hash', s=2)
    ax[0].set_title("Time per Hash in ms (Random)")
    ax[0].set_ylim(0, 30)
    ax[0].legend()
    ax[1].scatter(linearStored, primeLinearTime, color='red', label='Prime Modulo', s=2)
    ax[1].scatter(linearStored, seedLinearTime, color='blue', label='Seeded Hash', s=2)
    ax[1].set_title("Time per Hash in ms (Linear)")
    ax[1].set_ylim(0, 30)
    ax[1].legend()
    fig.tight_layout()
    
    plt.savefig("Graphs/hashTimes.png")
    
    

def main():
    parsePrime()
    parseSeed()

    np.array(randomStored)
    np.array(linearStored)
    
    np.array(primeRandIndices)
    np.array(primeLinearIndices)
    np.array(primeRandTime)
    np.array(primeLinearTime)
    
    np.array(seedRandIndices)
    np.array(seedLinearIndices)
    np.array(seedRandTime)
    np.array(seedLinearTime)
    
    plotHashes()
    plotTime()
    
    exit

main()