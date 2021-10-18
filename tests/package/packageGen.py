import sys
import os
import shutil
import xml.etree.ElementTree as ET
from datetime import date

################# Variables ##################
ERROROFFSET  = 9
OUTPUTOFFSET = 10

#Placeholders
PHRELEASESTATUS   = '{RELEASESTATUS}'
PHFRAMEWORK_MAJOR = '{FRAMEWORK_MAJOR}'
PHFRAMEWORK_MINOR = '{FRAMEWORK_MINOR}'
PHRELEASEDATE     = '{RELEASEDATE}'
PHTAGNAME         = '{TAGNAME}'

#Input values
PACKAGETEMPLATEXML = ''
RELEASESTATUS      = ''
FRAMEWORK_MAJOR    = ''
FRAMEWORK_MINOR    = ''
TAGNAME            = ''

################# Functions ##################  
def exit(msg):
    print('[' + sys.argv[0] + ' ERROR] ' + msg)
    sys.exit(1)

def printToConsole(msg, mark=True):
    if mark:
        print('[' + sys.argv[0] + ' OUTPUT] ' + msg)
    else:
        whiteSpace = (len(sys.argv[0]) + OUTPUTOFFSET) * ' '
        print(whiteSpace + msg)

def missingArgument(msg):
    whiteSpace = (len(sys.argv[0]) + ERROROFFSET) * ' '
    msg += '\n' + whiteSpace + '|--> package_template.xml'
    msg += '\n' + whiteSpace + '|--> Release status'
    msg += '\n' + whiteSpace + '|--> Framework major version'
    msg += '\n' + whiteSpace + '|--> Framework minor version'
    msg += '\n' + whiteSpace + '|--> Tag name'
    exit(msg)

def normalizePath(path):
    norm = path.replace('\n','')
    norm = norm.replace('\\', '/')
    splitted = os.path.splitdrive(norm)

    return splitted[1]

def checkArguments():
    if len(sys.argv) == 1:
        missingArgument('Five required arguments are missing:')
    elif len(sys.argv) == 2:
        missingArgument('Four required arguments are missing:')
    elif len(sys.argv) == 3:
        missingArgument('Three required arguments are missing:')
    elif len(sys.argv) == 4:
        missingArgument('Two required arguments are missing:')
    elif len(sys.argv) == 5:
        missingArgument('A required argument is missing:')
    elif len(sys.argv) > 6:
        missingArgument('Too many arguments were given. Five argument are required.')

    global PACKAGETEMPLATEXML
    PACKAGETEMPLATEXML = sys.argv[1]

    global RELEASESTATUS
    RELEASESTATUS = sys.argv[2]

    global FRAMEWORK_MAJOR
    FRAMEWORK_MAJOR = sys.argv[3]
  
    global FRAMEWORK_MINOR
    FRAMEWORK_MINOR = sys.argv[4]
    
    global TAGNAME
    TAGNAME = sys.argv[5]

    PACKAGETEMPLATEXML  = normalizePath(PACKAGETEMPLATEXML)
    
    if not os.path.exists(PACKAGETEMPLATEXML):
        exit(PACKAGETEMPLATEXML + ' does not exists!')

def replace(file, toReplace, value):
    fin = open(file, "rt")
    data = fin.read()
    data = data.replace(toReplace, value)
    fin.close()

    fin = open(file, "wt")
    fin.write(data)
    fin.close()

def getRoot(filePath):
    tree = ET.parse(filePath)
    root = tree.getroot()
    
    if root is None:
        exit('The XML file is invalid!')   
    
    return root

def getElement(root, tagPath):
    tagHierarchy = tagPath.split('/')
    
    if len(tagHierarchy) == 0:
        exit('Given hierarchy of tag is invalid!(' + filePath + ')')
    
    rootName = tagHierarchy.pop(0)
    
    if not root.tag == rootName:
        exit('Root is not named \'' + rootName + '\', but \'' + root.tag + '\'')
    
    tagElement = root
    for tagName in tagHierarchy:
        if tagElement is not None:
            tagElement = tagElement.find(tagName)
    
    return tagElement


def getXmlValue(filePath, tagPath, allowFailure=False):
    root = getRoot(filePath)
    
    tagElement = getElement(root, tagPath)
   
    if tagElement is None:
        if not allowFailure:
            exit('Given hierarchy of tag is invalid! (' + filePath + ')\n'
            '                      |--> Tag \'' + tagPath + '\' can not be found!')
        return None

    return tagElement.text

################ Main Program ################

def main():
    checkArguments()
    
    packageXml = '.\package.xml'
    
    #Copy template file
    shutil.copyfile(PACKAGETEMPLATEXML, packageXml)
    
    replace(packageXml, PHRELEASESTATUS, RELEASESTATUS)
    replace(packageXml, PHFRAMEWORK_MAJOR, FRAMEWORK_MAJOR)
    replace(packageXml, PHFRAMEWORK_MINOR, FRAMEWORK_MINOR)
    
    today = str(date.today())
    
    replace(packageXml, PHRELEASEDATE, today)
    
    tagName = TAGNAME.replace("-", ".", 2)

    replace(packageXml, PHTAGNAME, tagName)
    
    #Generate package path
    dest = getXmlValue(packageXml, 'Package/Destination')
    dest = dest.split('_')[-1]
    
    packageName = getXmlValue(packageXml, 'Package/Name')
    
    path = './components/' + RELEASESTATUS + '/' + FRAMEWORK_MAJOR + '_' + FRAMEWORK_MINOR + '/' + dest + '/packages/' + packageName + '/meta/'
    
    #Create package directory
    try:
        os.makedirs(path)
        shutil.move(packageXml, path)
    except FileExistsError:
        exit("Directory " + path + " could not be created")

if __name__ == "__main__":
    main()