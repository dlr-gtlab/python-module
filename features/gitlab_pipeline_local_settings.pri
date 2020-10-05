#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

BUILD_BATCH     =   $$(BUILDBATCH)
BUILD_UNITTESTS =   $$(BUILDUNITTESTS)

PY_VERSION = $$(PYVERSION)

# path to dev tools
DEV_TOOLS = $$(DEVTOOLS)

# path to GTlab Core repository
GTLAB_CORE_PATH    = $${DEV_TOOLS}
GTLAB_LOGGING_PATH = $${DEV_TOOLS}

# Third Party
PYTHON_PATH 	 = $${DEV_TOOLS}/ThirdPartyLibraries/Python/Python_$${PY_VERSION}
PYTHON_QT_PATH 	 = $${DEV_TOOLS}/ThirdPartyLibraries/PythonQt/PythonQt_$${PY_VERSION}
GOOGLE_TEST_PATH = $${DEV_TOOLS}/ThirdPartyLibraries/GoogleTest

# Deployment
GTLAB_ENVIRONMENT_PATH = 


