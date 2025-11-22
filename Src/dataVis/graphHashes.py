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
            
def plotHashes():
    print("plotting hashes")
    fig, ax = plt.subplots(1, 2, figsize=(10,5))
    
    ax[0].scatter(randomStored, primeRandIndices, color='blue', s=1)
    ax[0].set_title("Prime Modulo Hashing (Random Data)")
    ax[0].autoscale()
    
    ax[1].scatter(randomStored, seedRandIndices, color='green', s=1)
    ax[1].set_title("Seeded Hashing (Random Data)")
    ax[1].autoscale()
    fig.tight_layout()

    print("finished plotting")

    plt.savefig("Graphs/hashes.png")
    plt.clf()
            
def plotTime():
    print("plotting time")
    plt.figure()
    plt.scatter(randomStored, primeRandTime, color='orange', label='Prime Modulo', s=2)
    plt.scatter(randomStored, seedRandTime, color='green', label='Seeded Hash', s=2)
    plt.title("Time per Hash in ms (Random)")
    plt.ylim(0, 30)
    plt.legend()
    
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
    
    # graph stuff here

main()