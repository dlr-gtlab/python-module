/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_convert.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.11.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "PythonQtConversion.h"

#include "gt_structproperty.h"

#include "gtpy_gilscope.h"
#include "gtpy_convert.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

PyObject*
gtpy::convert::fromPropertyStructContainer(const GtPropertyStructContainer& con)
{
    GTPY_GIL_SCOPE

    PyObject* result = PyDict_New();

    /// iterate through the container entries
    for (const auto& entry : con)
    {
        auto name = entry.getMemberVal<QString>("name");

        if (!name.isEmpty())
        {
            bool ok{true};
            auto val = entry.getMemberValToVariant("value", &ok);

            if (ok)
            {
                /// convert value to PyObject*
                auto* valObj = PythonQtConv::QVariantToPyObject(val);

                /// sets name and val to the result dict
                PyDict_SetItemString(result, name.toUtf8().data(), valObj);

                /// decrement the reference count for valObj
                Py_DECREF(valObj);
            }
        }
    }

    return result;
}

#endif
