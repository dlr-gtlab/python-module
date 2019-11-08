#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/              

# path to dev tools
DEV_TOOLS = D:\devel\GTlab-DevTools

# path to GTlab project
GTLAB_PROJECT = D:\devel\MNO_PythonRefactoring

# path to GtLab
GTLAB_PATH = D:\GTlab

# build settings
#BUILD_UNITTESTS =   true

# compile mode
#This variable determines whether the header files of the dev tools or a GTlab
#project are used. It also determines in which build directory the binaries
#will be stored. There are four modes:
#
# module       --> header files of dev tools are used, a build directory which 
#                  contains the binaries will be created at the current 
#                  project directory
# moduleTests  --> header files of the GTlab project specified in GTLAB_PROJECT
#                  are used, a build directory which contains the binaries will
#                  be created at the current project directory
# gtlab        --> header files of dev tools are used, the binaries will be 
#                  stored in the bin directory at GTLAB_PATH
# gtlabProject --> header files of the GTlab project specified in GTLAB_PROJECT
#                  are used, the binaries will be stored in the build\modules\ 
#                  directory at GTLAB_PROJECT
#
COMPILE_MODE = modules