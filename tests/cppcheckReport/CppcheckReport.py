import sys
import os
import json

################# Variables ##################
ERROROFFSET  =  (len(sys.argv[0]) + 10) * ' ' 
OUTPUTOFFSET =  (len(sys.argv[0]) + 10) * ' ' 

INVALIDFILEERRORMSG = "The input files are invalid! Download the artifacts and check them.\n" + ERROROFFSET + "" \
                      "NOTE: This error can occur if a new unstable branch was created. In this case, \n" + ERROROFFSET + "" \
                      "no older cppcheck result file can be found. On the next commit, \n" + ERROROFFSET + "" \
                      "this error should no longer occur."

INVALIDFILEERRORCODE = 3

################# Functions ##################
def exit(msg, code=1):  
    print('[' + sys.argv[0] + ' ERROR]  ' + msg)
    sys.exit(code)

def printToConsole(msg, mark=True):
    if mark:
        print('[' + sys.argv[0] + ' OUTPUT] ' + msg)
    else:
        #whiteSpace = (len(sys.argv[0]) + OUTPUTOFFSET) * ' '
        print(OUTPUTOFFSET + msg)

# Reads the given cppcheck json file and returns its contents 
# in a dictionary.
def readCppcheckJsonFile(file):
    vulsDict = {} 
    try:
        if os.path.isfile(file):
            f = open(file, 'r')
            jsonContent = f.read()
            vulsList = json.loads(jsonContent)
            for vul in vulsList:
                # The ID of a vulnerability is the combination of its fingerprint 
                # and the line and column in which the vulnerability occurs.  
                line  = vul['location']['positions']['begin']['line']
                col   = vul['location']['positions']['begin']['column']
                vulId = str(vul['fingerprint']) + '_' + str(line) + '_' + str(col)
                
                vulsDict[vulId] = vul
        else:
            exit('Cppcheck JSON file ( ' + file + ' ) not found!')
    except:
        exit(INVALIDFILEERRORMSG, INVALIDFILEERRORCODE)
    
    
    return vulsDict

# Takes the dictionary of new vulnerabilities and the number of fixed vulnerabilities
# and prints a report on the console.
# It returns True if no new vulnerabilities were created. Otherwise it returns False.
def report(newVuls, fixed):
    newCount = len(newVuls)
    
    printToConsole('-------------- CppCheck Report --------------')
    printToConsole('Fixed vulnerabilities        : ' + str(fixed), mark=False)
    printToConsole('Newly created vulnerabilities: ' + str(newCount), mark=False)
    printToConsole('', mark=False)
    
    count = 0
    
    for vulId, vul in newVuls.items():
        count += 1
        path  = vul['location']['path']
        line  = vul['location']['positions']['begin']['line']
        col   = vul['location']['positions']['begin']['column']
        
        printToConsole('New vulnerability #' + str(count), mark=False)
        printToConsole('Fingerpring: ' + str(vul['fingerprint']), mark=False)
        printToConsole('Path       : ' + path, mark=False)
        printToConsole('Line       : ' + str(line), mark=False)
        printToConsole('Column     : ' + str(col) + '\n', mark=False)
        printToConsole('Description: ' + vul['description'], mark=False)
        printToConsole('', mark=False)
        
    printToConsole('---------------------------------------------', mark=False)
    
    return not bool(newCount)

# Takes the results of two cppcheck runs and compares them.
# The input parameters must be dictionaries containing the vulnerabilities 
# as json objects. To ensure that this function works correctly, use 
# readCppcheckJsonFile(file) to create the input parameters.
# It returns True if no new vulnerabilities were created. Otherwise it returns False.
def compareCppcheckResults(oldVulsDict, newVulsDict):
    fixedCount = 0
    
    # Iterate over the old vulnerabilities and check if they still exist. 
    # The vulnerabilities that are still present in newVulsDict after this 
    # loop have been newly created.
    for vulId, vul in oldVulsDict.items():
        newVul = newVulsDict.pop(vulId, None)
        # If the vulnerability no longer exists, 
        # increment the counter of fixed vulnerabilities.
        if newVul == None:
            fixedCount += 1
    
    return report(newVulsDict, fixedCount)

import argparse
parser = argparse.ArgumentParser()
parser.add_argument('--old', '-o', action='append', required=True)
parser.add_argument('--new', '-n', action='append', required=True)
parser.add_argument('--diffoutput', '-d', action='append', required=False)
args = parser.parse_args()

################ Main Program ################

def main():
    # Get the file paths from arguments
    oldFile = args.old[0]
    newFile = args.new[0]
    
    # Read in the files
    oldVulsDict = readCppcheckJsonFile(oldFile)
    newVulsDict = readCppcheckJsonFile(newFile)
    
    # Compare vulnerabilities and give an error message 
    # if new vulnerabilities have been created.
    if not compareCppcheckResults(oldVulsDict, newVulsDict):
        exit("New vulnerabilities created!")

if __name__ == "__main__":
    main()
