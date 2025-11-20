# goal: return arrays with data!
# refer to the guidelines!

import csv
import matplotlib
import math
    
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
    i = 0
    with open('hashPrime.csv', newline='') as csvfile:
        hashReader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in hashReader:
        i = 0
        for value in row:
            match i:
                case 0:
                    randomStored.insert(value)
                case 1:
                    linearStored.insert(value)
                case 2:
                    primeRandIndices.insert(value)
                case 3:
                    primeLinearIndices.insert(value)
                case 4:
                    primeRandTime.insert(value)
                case 5:
                    primeLinearTime.insert(value)  
            i+=1    
            
    
def parseSeed():
    # seed reader
    i = 0
    with open('hashSeed.csv', newline='') as csvfile:
        hashReader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in hashReader:
        i = 0
        for value in row:
            match i:
                # rand and linear alr stored, no need to look again
                case 2:
                    seedRandIndices.insert(value)
                case 3:
                    seedLinearIndices.insert(value)
                case 4:
                    seedRandTime.insert(value)
                case 5:
                    seedLinearTime.insert(value)
            i += 1   
            
def main():
    parsePrime()
    parseSeed()
    # graph stuff here