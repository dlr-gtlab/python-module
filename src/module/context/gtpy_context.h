/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 10.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYCONTEXT_H
#define GTPYCONTEXT_H

#include <QString>

#include "gtpy_module.h"

class GtpyContext : public GtpyModule
{
    friend class GtpyContextManager;

public:
    enum Type
    {
        BatchContext = 0,
        GlobalContext,
        ScriptEditorContext,
        CalculatorRunContext,
        TaskEditorContext,
        TaskRunContext,
        CollectionContext
    };

    explicit GtpyContext(Type type);
};

#endif // GTPYCONTEXT_H
