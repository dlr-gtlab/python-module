/* GTlab - Gas Turbine laboratory
 *
 * SPDX-FileCopyrightText: 2023 German Aerospace Center (DLR)
 *
 * Author: Martin Siggel (AT-TWK)
 */

#ifndef GTPYTHREADSCOPE_H
#define GTPYTHREADSCOPE_H

#include "PythonQtPythonInclude.h"

class GtpyThreadScope
{
public:
    GtpyThreadScope()
        : state(PyEval_SaveThread())
    {}

    ~GtpyThreadScope()
    {
        PyEval_RestoreThread(state);
    }

private:
    PyThreadState* state;
};

#endif // GTPYTHREADSCOPE_H
