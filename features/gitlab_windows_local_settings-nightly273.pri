#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

BUILD_BATCH     =   true
BUILD_UNITTESTS =   false

# python version (273|373)
PY_VERSION = 273

# path to dev tools
DEV_TOOLS = D:\GTlab-DevTools

#   applicationDir: build in directory TARGET_DIRECTORY
#                   or into modules subdir for GTlab Modules
#   localDir: build locale in build-directory
#
# MODES: (applicationDir|localDir)
BUILD_TARGET = applicationDir

TARGET_DIRECTORY = G:\AT-TW\GTlab\NightlyBuild\lib

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
GTLAB_LIBS = nightlyBuild

NIGHTLY_BUILD_DIR = G:\AT-TW\GTlab\NightlyBuild

# path to GTlab Core repository
GTLAB_CORE = D:\devel\MNO_PythonRefactoring


