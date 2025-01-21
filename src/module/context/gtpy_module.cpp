/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_module.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QMutex>

#include "gtpypp.h"
#include "gtpy_regexp.h"
#include "gtpy_gilscope.h"

#include "gt_logging.h"

#include "gtpy_module.h"

struct GtpyModule::Impl
{
    QString moduleName{};
    PyPPObject module{};
    QMutex evalMutex{};
};

GtpyModule::GtpyModule(GtpyModule&&) noexcept = default;
GtpyModule& GtpyModule::operator=(GtpyModule&&) noexcept = default;

GtpyModule::GtpyModule(const QString& moduleName) :
    pimpl(std::make_unique<Impl>())
{
    GTPY_GIL_SCOPE

    pimpl->moduleName = moduleName;
    pimpl->module = PyPPObject::Borrow(
        PythonQt::self()->createModuleFromScript(pimpl->moduleName));
}

bool
GtpyModule::addFunctions(PyMethodDef* def)
{
    return PyPPModule_AddFunctions(pimpl->module, def);
}

GtpyModule::~GtpyModule()
{
    GTPY_GIL_SCOPE

    // prevent a GIL scope assertion by decrementing the reference count here
    // TODO: Analyze why the assertion occurs when ~PyPPObjectT() handles
    // the reference count decrement.
    if (pimpl) Py_XDECREF(pimpl->module.release());
}

bool
GtpyModule::evalScript(const QString& script, EvalOption option) const
{
    GTPY_GIL_SCOPE

    assert(pimpl->module.get());

    bool hadError = false;

    if (!script.isEmpty())
    {
        PythonQt::self()->evalScript(pimpl->module.get(), script, option);

        // TODO: Check if mutex is used correctly
        pimpl->evalMutex.lock();
        hadError = PythonQt::self()->hadError();
        PythonQt::self()->clearError();
        pimpl->evalMutex.unlock();
    }

    return !hadError;
}

void
GtpyModule::addObject(const QString& pyIdent, QObject* obj) const
{
    if (!obj)
    {
        gtError() << "nullptr cannot be added to the Python module.";
        return;
    }

    if (!gtpy::re::validation::isValidPythonIdentifier(pyIdent))
    {
        gtError() << "Invalid Python identifier given ( " << pyIdent << " )."
                  << "The object " << obj
                  << " cannot be added to the Python module.";
        return;
    }

    PythonQt::self()->addObject(pimpl->module.get(), pyIdent, obj);
}

const QString&
GtpyModule::moduleName() const
{
    return pimpl->moduleName;
}
