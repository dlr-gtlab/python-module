#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

BUILD_BATCH     =   $$(BUILDBATCH)
BUILD_UNITTESTS =   $$(BUILDUNITTESTS)

MAJOR_VERSION =    $$(MAJOR_VERSION)

PY_MAJOR_VERSION = $$(PY_MAJOR)
PY_MINOR_VERSION = $$(PY_MINOR)

# path to dev tools
DEV_TOOLS = $$(DEVTOOLS)
USE_HDF5 =  $$(USE_HDF5)

# path to GTlab Core repository
GTLAB_CORE_PATH    = $${DEV_TOOLS}
GTLAB_LOGGING_PATH = $${DEV_TOOLS}
GENH5_PATH   = $${DEV_TOOLS}

# Third Party
PYTHON_PATH      = $${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_$${PY_MAJOR_VERSION}$${PY_MINOR_VERSION}
PYTHON_QT_PATH   = $${DEV_TOOLS}/ThirdPartyLibraries/PythonQt/PythonQt_$${PY_MAJOR_VERSION}$${PY_MINOR_VERSION}
GOOGLE_TEST_PATH = $${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest
HDF5_PATH =        $${DEV_TOOLS}/ThirdPartyLibraries/hdf5


# Deployment
GTLAB_ENVIRONMENT_PATH = 



