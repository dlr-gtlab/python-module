#             ______________      __
#            / ____/_  __/ /___  / /_
#           / / __  / / / / __ `/ __ \
#          / /_/ / / / / / /_/ / /_/ /
#          \____/ /_/ /_/\__,_/_.___/

######################################################################
#### DO NOT CHANGE!
######################################################################

include( local_settings.pri )

#### ARTISTIC STYLE FILE
DISTFILES  += style.astylerc

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

#### SETUP MODULE ####
contains(BUILD_SETUP_MODULE, true) {
    message("BUILD_SETUP_MODULE = true")
    SUBDIRS += setup_module
    setup_module.subdir = src/setup_module
    module.depends = setup_module
}

#### UNIT TESTS ####
contains(BUILD_UNITTESTS, true) {
    message("BUILD_UNITTESTS = true")
    SUBDIRS += unittests
    unittests.subdir = tests/unittests
    unittests.depends = module
}
######################################################################
