#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

BUILD_BATCH     =   true
BUILD_UNITTESTS =   true

PY_VERSION = 37
MAJOR_VERSION =    2


# path to dev tools
DEV_TOOLS = PATH/TO/DEVTOOLS

# path to GTlab Core repository
GTLAB_CORE_PATH    = $${DEV_TOOLS}
GTLAB_LOGGING_PATH = $${DEV_TOOLS}
GTLAB_H5_PATH =      $${DEV_TOOLS}

# Third Party
PYTHON_PATH      = $${DEV_TOOLS}\ThirdPartyLibraries\Python\Python_$${PY_VERSION}
PYTHON_QT_PATH   = $${DEV_TOOLS}\ThirdPartyLibraries\PythonQt\PythonQt_$${PY_VERSION}
GOOGLE_TEST_PATH = $${DEV_TOOLS}\ThirdPartyLibraries\GoogleTest
HDF5_PATH =        $${DEV_TOOLS}/ThirdPartyLibraries/hdf5

# Deployment
GTLAB_ENVIRONMENT_PATH =
