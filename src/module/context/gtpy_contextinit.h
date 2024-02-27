/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context_init.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 30.01.2024
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPY_CONTEXTINIT_H
#define GTPY_CONTEXTINIT_H

#include <functional>
#include <map>

#include "PythonQtObjectPtr.h"

namespace gtpy {

namespace context {

enum Type {
    BatchContext = 0,
    GlobalContext,
    ScriptEditorContext,
    CalculatorRunContext,
    TaskEditorContext,
    TaskRunContext,
    CollectionContext
};

namespace init {

void batchContext(PythonQtObjectPtr&);

void taskRunContext(PythonQtObjectPtr& con);

using InitFunc = std::function<void(PythonQtObjectPtr&)>;
InitFunc routine(Type type);

} // namespace init

} // namespace context

} // namespace gtpy


#endif // GTPY_CONTEXTINIT_H
