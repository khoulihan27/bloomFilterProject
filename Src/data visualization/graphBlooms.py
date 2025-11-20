# goal: return arrays with data!
# refer to the guidelines!

import csv
import matplotlib
import math
    
c = list()
k = list()

primeFPRRand = list()
primeFPRLinear = list()

seedFPRRand = list()
seedFPRLinear = list()
    
def parsePrime():
   # prime reader
   # if doing linear, include case
    i = 0
    with open('bloomPrime.csv', newline='') as csvfile:
        hashReader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in hashReader:
        i = 0
        for value in row:
            match i:
                case 0:  
                    c.insert(value)
                case 1:
                    k.insert(value)
                case 2:
                    primeFPRRand.insert(value)
            i+=1    
            
    
def parseSeed():
    # seed reader
    # if doing linear, include case
    i = 0
    with open('bloomSeed.csv', newline='') as csvfile:
        hashReader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in hashReader:
        i = 0
        for value in row:
            match i:
                # finding c and k not needed!
                case 2:
                    seedFPRRand.insert(value)
            i+=1       
            
def main():
    parsePrime()
    parseSeed()
    # graph stuff here
            