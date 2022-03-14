import sys
import os
import json

################# Variables ################## 
ERROROFFSET  = 9
OUTPUTOFFSET = 10

################# Functions ##################
def exit(msg):
    sys.exit('[' + sys.argv[0] + ' ERROR] ' + msg)

def printToConsole(msg, mark=True):
    if mark:
        print('[' + sys.argv[0] + ' OUTPUT] ' + msg)
    else:
        whiteSpace = (len(sys.argv[0]) + OUTPUTOFFSET) * ' '
        print(whiteSpace + msg)

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--old', '-o', action='append', required=True)
parser.add_argument('--new', '-n', action='append', required=True)
parser.add_argument('--diffoutput', '-d', action='append', required=False)
args = parser.parse_args()

################ Main Program ################

def main():    
    oldFile = args.old[0]
    newFile = args.new[0]
    
    oldVulsList = []
    newVulsDict = {} 
    
    # Read in the old cppcheck output file
    if os.path.isfile(oldFile):
        f = open(oldFile, 'r')
        jsonContent = f.read()
        oldVulsList = json.loads(jsonContent)
    else:
        exit('Old cppcheck output file ( ' + oldFile + ' ) not found!') 
    
    # Read in the new cppcheck output file
    if os.path.isfile(newFile):
        f = open(newFile, 'r')
        jsonContent = f.read()
        vulsList = json.loads(jsonContent)
        for vul in vulsList:
            newVulsDict[vul['fingerprint']] = vul
    else:
        exit('New cppcheck output file ( ' + newFile + ' ) not found!') 

    fixedCount = 0
    
    # Iterate over the old vulnerabilities and check if they still exist. 
    # The vulnerabilities that are still present in newVulsDict after this 
    # loop have been newly created.
    for vul in oldVulsList:
        newVul = newVulsDict.pop(vul['fingerprint'], None)
        # If the vulnerability no longer exists, 
        # increment the counter of fixed vulnerabilities.
        if newVul == None:
            fixedCount += 1

    newCount = len(newVulsDict)
    
    printToConsole('Fixed vulnerabilities        : ' + str(fixedCount))
    printToConsole('Newly created vulnerabilities: ' + str(newCount))
    
    if newCount > 0:
        exit("New vulnerabilities created!")

if __name__ == "__main__":
    main()
