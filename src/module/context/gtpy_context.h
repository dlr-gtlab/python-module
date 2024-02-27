/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.01.2024
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCONTEXT_H
#define GTPYCONTEXT_H

#include "PythonQtObjectPtr.h"
#include "gtpy_contextinit.h"

class GtpyContext
{
public:
    GtpyContext(gtpy::context::Type type);
    ~GtpyContext();

private:
    QString m_moduleName;
    PythonQtObjectPtr m_module;
};

#endif // GTPYCONTEXT_H
