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

#include "gtpy_gilscope.h"
#include "gtpypp.h"

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

const QString&
GtpyModule::moduleName() const
{
    return pimpl->moduleName;
}


