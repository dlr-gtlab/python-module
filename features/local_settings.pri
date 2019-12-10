#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

BUILD_BATCH     =   true
BUILD_UNITTESTS =   false

# python version (273|373)
PY_VERSION = 373

# path to dev tools
DEV_TOOLS = PATH/TO/DEVTOOLS

#   applicationDir: build in directory TARGET_DIRECTORY
#                   or into modules subdir for GTlab Modules
#   localDir: build locale in build-directory
#
# MODES: (applicationDir|localDir)
BUILD_TARGET = localDir

TARGET_DIRECTORY = PATH/TO/TARGET/DIRECTORY

#
#   devTools: use Libs and headers for GTLab Core and modules from
#             DEV_TOOLS
#
#   nightlyBuild: use Libs and headers for GTLab Core and modules from
#                 NIGHTLYBUILD_DIR
#
#   repository: use Libs and headers for other modules from REPO_PATHS
#               and for core from NIGHTLYBUILD_DIR
#
# MODES: (devTools|nightlyBuild|repository)
GTLAB_LIBS = devTools

NIGHTLY_BUILD_DIR = PATH/TO/NIGHTLYBUILDS

# path to GTlab Core repository
GTLAB_CORE = PATH/TO/GTLAB/CORE/REPOSITORY


