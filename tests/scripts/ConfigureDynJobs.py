import sys
import os

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
parser.add_argument('--templatefile', '-t', action='append', required=True)
parser.add_argument('--outputfile', '-o', action='append', required=True)
parser.add_argument('--replace', '-r', nargs=2, action='append')
args = parser.parse_args()

################ Main Program ################

def main():    
    templateFile = args.templatefile[0]
    outputFile = args.outputfile[0]
    job = ''
    
    if os.path.isfile(templateFile):
        f = open(templateFile, "r")
        job = f.read()
        f.close()
    else:
        exit('Template file ( ' + templateFile + ' ) not found!')  
    
    for arg in args.replace:
        replace = '${' + arg[0] + '}'
        value = arg[1]

        job = job.replace(replace, value)

    try:
        with open(outputFile, 'a') as output:
            output.write(job + '\n\n')            
    except IOError:
        exit('Output file ( ' + outputFile + ' ) could not be generated!') 
    
    if os.path.isfile(outputFile):
        f = open(outputFile, "r")
        printToConsole(outputFile + ' configured!')
        print('\n' + f.read())
        f.close()


if __name__ == "__main__":
    main()