/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_convert.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 15.11.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gtpy_convert.h"

#include "PythonQtConversion.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_structproperty.h"
#endif

#include "gtpy_gilscope.h"


#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

PyPPObject
gtpy::convert::fromPropertyStructContainer(const GtPropertyStructContainer& con)
{
    GTPY_GIL_SCOPE

    auto result = PyPPDict_New();

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
                auto valObj = PyPPObject::fromQVariant(val);

                /// sets name and val to the result dict
                PyPPDict_SetItem(result, name.toUtf8().data(), valObj);
            }
        }
    }

    return result;
}

#endif
