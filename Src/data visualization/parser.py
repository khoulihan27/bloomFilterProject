# goal: return arrays with data!
# refer to the guidelines!

import csv

# hash reader
with open('dataHash.csv', newline='') as csvfile:
    hashReader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in hashReader:
        for value in row:
            print("hello")
            
        
# bloom reader
with open('dataBloom.csv', newline='') as csvfile:
    hashReader = csv.reader(csvfile, delimiter=' ', quotechar='|')
    for row in hashReader:
        for value in row:
            print("hello")