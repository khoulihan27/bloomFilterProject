# goal: return arrays with data!
# refer to the guidelines!

import csv
import matplotlib
import matplotlib.pyplot as plt
import math
import numpy as np

c = [2, 4, 6, 7, 10]
k = [0] * 21
kIdeal = [0] * 22
    
for i in range(1, 22):
    k[i-1] = i
for i in range(0, 22):
    kIdeal[i] = i

primeRandFPR2 = list()
primeRandFPR4 = list()
primeRandFPR6 = list()
primeRandFPR7 = list()
primeRandFPR10 = list()
primeLinearFPR = list()

seedRandFPR2 = list()
seedRandFPR4 = list()
seedRandFPR6 = list()
seedRandFPR7 = list()
seedRandFPR10 = list()
seedLinearFPR = list()
    
def graphIdeal(c):
    ideal = list()
    for i in range(0, 22):  
        val = float(np.power((1 - np.power(np.e, -i/float(c))), i))
        ideal.append(val)
    return ideal    
    
    

def parsePrime():
   # prime reader
    print("parsing prime")
    i = 0
    kCount = 1
    cCount = c[0]
    cIndex = 0
    r = 1
    with open('Data/bloomPrime.csv', mode='r') as csvfile:
        hashReader = csv.reader(csvfile)
        for row in hashReader:
            i = 0
            if kCount == 22:
                cIndex+=1
                cCount = c[cIndex]
                kCount = 1
            print(r)
            for value in row:
                if value != "RANDOM" or value != "LINEAR":
                    match i:
                        case 2:
                            match cCount:
                                case 2:
                                    primeRandFPR2.append(float(value))
                                case 4:
                                    primeRandFPR4.append(float(value))
                                case 6:
                                    primeRandFPR6.append(float(value))
                                case 7:
                                    primeRandFPR7.append(float(value))
                                case 10:
                                    primeRandFPR10.append(float(value))
                    i+=1
            r+=1   
            kCount+=1 
            
    
def parseSeed():
    # seed reader
    i = 0
    kCount = 1
    cCount = c[0]
    cIndex = 0
    with open('Data/bloomSeed.csv', mode='r') as csvfile:
        hashReader = csv.reader(csvfile)
        for row in hashReader:
            i = 0
            if kCount == 22:
                cIndex+=1
                cCount = c[cIndex]
                kCount = 1
            for value in row:
                if value != "RANDOM" or value != "LINEAR":
                    match i:
                        case 2:
                            match cCount:
                                case 2:
                                    seedRandFPR2.append(float(value))
                                case 4:
                                    seedRandFPR4.append(float(value))
                                case 6:
                                    seedRandFPR6.append(float(value))
                                case 7:
                                    seedRandFPR7.append(float(value))
                                case 10:
                                    seedRandFPR10.append(float(value))
                i+=1
            kCount+=1 
            
def plotBlooms():
    print("plotting hashes")
    fig, ax = plt.subplots(2, 2, figsize=(15,10))
    r = 0
    col = 0
    
    # do graphs per c (4 for now)
    for i in range(0,4):
        match i:
            case 0:
                currData = primeRandFPR2
            case 1:
                currData = primeRandFPR4
            case 2:
                r+=1
                col = 0
                currData = primeRandFPR6
            case 3:
                currData = primeRandFPR7
        currC = c[i]
        ideal = graphIdeal(currC)
        np.array(ideal)
        ax[r][col].plot(k, currData, linestyle='-.', color='red', label='False Positive Rate')
        ax[r][col].plot(kIdeal, ideal, linestyle='-.', color='black', label='Ideal FPR')
        ax[r][col].axvline(x = (float(currC)*(np.log(2))), linestyle='--', color='grey', label="Optimal K: " + str(np.round((float(currC)*(np.log(2))), decimals=2)))
        ax[r][col].set_title("Prime Modulo Hashing (Random Data), c = " + str(c[i]))
        ax[r][col].legend()
        ax[r][col].autoscale()
        col+=1
    fig.tight_layout()
    plt.savefig("Graphs/primeBlooms.png")
    plt.clf()
    
    r = 0
    col = 0
    currData = seedRandFPR2
    fig2, ax2 = plt.subplots(2, 2, figsize=(15,10))
    for i in range(0,4):
        match i:
            case 0:
                currData = seedRandFPR2
            case 1:
                currData = seedRandFPR4
            case 2:
                r+=1
                col = 0
                currData = seedRandFPR6
            case 3:
                currData = seedRandFPR7
        currC = c[i]
        ideal = graphIdeal(currC)
        np.array(ideal)
        ax2[r][col].plot(k, currData, linestyle='-.', color='blue', label="False Positive Rate")
        ax2[r][col].plot(kIdeal, ideal, linestyle='-.', color='green', label='Ideal FPR')
        ax2[r][col].axvline(x = (float(currC)*(np.log(2))), linestyle='--', color='grey', label="Optimal K: " + str(np.round((float(currC)*(np.log(2))), decimals=2)))
        ax2[r][col].set_title("Seed Hashing (Random Data), c = " + str(c[i]))
        ax2[r][col].legend()
        ax2[r][col].autoscale()
        col+=1
        fig2.tight_layout()
    plt.savefig("Graphs/seedBlooms.png")
    plt.clf()
    
    

def main():
    parsePrime()
    parseSeed()
    
    np.array(primeRandFPR2)
    np.array(primeRandFPR4)
    np.array(primeRandFPR6)
    np.array(primeRandFPR7)
    np.array(primeRandFPR10)
    
    np.array(seedRandFPR2)
    np.array(seedRandFPR4)
    np.array(seedRandFPR6)
    np.array(seedRandFPR7)
    np.array(seedRandFPR10)
    
    plotBlooms()
    
    exit
    
    # graph stuff here

main()