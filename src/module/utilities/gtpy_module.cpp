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
#include <QMutexLocker>

#include "gtpy_regexp.h"
#include "gtpy_gilscope.h"

#include "gt_logging.h"

#include "gtpy_module.h"

struct GtpyModule::Impl
{
    QString moduleName{};
    PyPPObject module{};
    static QMutex evalMutex;

    ~Impl()
    {
        if (!module) return;

        GTPY_GIL_SCOPE

        // When adding functions to a module using PyModule_AddFunctions(),
        // each function increments the reference count of the module before
        // being added to the module's dictionary (__dict__). As a result, the
        // reference count of the module becomes greater than expected.
        //
        // When this destructor is called, the reference count of the module
        // should ideally decrease from 1 to 0. However, this is not possible
        // as long as there are objects in the module's dictionary that hold
        // references to the module itself.
        //
        // To simplify memory management and ensure the module can be properly
        // deallocated, we clear the module's __dict__ at this point. After
        // clearing the dictionary, the module's reference count should drop
        // to 1.
        if (auto dict = PyPPModule_GetDict(module)) PyPPDict_Clear(dict);

        // Prevent a GIL scope assertion by decrementing the reference count
        // here. The assertion occurs when ~PyPPObjectT() decrements the
        // reference count to 0, which triggers the deallocation of the
        // corresponding PyObject. This deallocation requires the GIL to be
        // locked. But ~PyPPObjectT() currently does not lock the GIL.
        //
        // To avoid the assertion, we explicitly decrement the reference count
        // here.
        assert(module->ob_refcnt == 1);
        Py_XDECREF(module.release());
    }
};

QMutex GtpyModule::Impl::evalMutex{};

GtpyModule::GtpyModule(const QString& moduleName) :
    pimpl(std::make_unique<Impl>())
{
    GTPY_GIL_SCOPE

    pimpl->moduleName = moduleName;
    pimpl->module = PyPPObject::Borrow(
        PythonQt::self()->createModuleFromScript(pimpl->moduleName));
}

GtpyModule::GtpyModule(GtpyModule&&) noexcept = default;
GtpyModule& GtpyModule::operator=(GtpyModule&&) noexcept = default;

GtpyModule::~GtpyModule() = default;

bool
GtpyModule::eval(const QString& code, InputType type) const
{
    GTPY_GIL_SCOPE

    if (code.isEmpty()) return true;

    // TODO: Ensure the error handling in this method is thread-safe.
    // PythonQt::evalScript() clears the error status before the script is
    // evaluated. This could lead to problems with error handling if the method
    // is called in multiple threads. The current implementation is based on
    // the previous implementation of GtpyContextManager::evalScript(). Since
    // no thread-safety issues with GtpyContextManager::evalScript() have been
    // observed so far, it might be acceptable to keep this implementation.
    // However, remember to verify the thread safety again.
    bool hadError = false;

    PythonQt::self()->evalScript(pimpl->module.get(), code, type);

    QMutexLocker locker{&Impl::evalMutex};

    hadError = PythonQt::self()->hadError();
    PythonQt::self()->clearError();

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

void
GtpyModule::addVariable(const QString& pyIdent, const QVariant& value) const
{
    if (!gtpy::re::validation::isValidPythonIdentifier(pyIdent))
    {
        gtError() << "Invalid Python identifier given ( " << pyIdent << " )."
                  << "The value " << value
                  << " cannot be added to the Python module.";
        return;
    }

    PythonQt::self()->addVariable(pimpl->module.get(), pyIdent, value);
}

void
GtpyModule::removeVariable(const QString& pyIdent) const
{
    PythonQt::self()->removeVariable(pimpl->module.get(), pyIdent);
}

const QString&
GtpyModule::moduleName() const
{
    return pimpl->moduleName;
}

bool
GtpyModule::addFunctions(PyMethodDef* def)
{
    return PyPPModule_AddFunctions(pimpl->module, def);
}

PyPPObject
GtpyModule::module() const
{
    return pimpl->module;
}
