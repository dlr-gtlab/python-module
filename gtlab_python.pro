#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( local_settings.pri )

TEMPLATE = subdirs
CONFIG += console
CONFIG += c++11

SUBDIRS += module
module.subdir = src/module

#### BATCH ####
contains(BUILD_BATCH, true) {
    message("BUILD_BATCH = true")
    SUBDIRS += batch
    batch.subdir = src/batch
    batch.depends = module
}

#### UNIT TESTS ####
contains(BUILD_UNITTESTS, true) {
    message("BUILD_UNITTESTS = true")
    SUBDIRS += unittests
    unittests.subdir = tests/unittests
    unittests.depends = module
}
######################################################################
