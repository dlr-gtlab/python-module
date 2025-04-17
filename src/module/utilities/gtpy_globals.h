/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_globals.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 11.05.2020
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYGLOBALS_H
#define GTPYGLOBALS_H

#include <QString>

#define PyObjectAPIReturn PyObject*

namespace GtpyGlobals
{


///StdOut meta data
extern const char* CONTEXT_KEY;
extern const char* OUTPUT_KEY;
extern const char* ERROR_KEY;

struct StdOutMetaData
{
    QString contextName = QString();
    bool output = false;
    bool error = false;
};


#define gtDevMode() (gtApp && gtApp->devMode())

}
#endif // GTPYGLOBALS_H
