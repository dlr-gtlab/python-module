/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_module.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 17.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPY_MODULE_H
#define GTPY_MODULE_H

#include <memory>

#include <QString>
#include <QObject>

#include "PythonQtPythonInclude.h"

#include "gtpypp.h"

class GtpyModule
{
public:
    enum EvalOption {
        EvalFile = Py_file_input,
        EvalSingleString = Py_single_input
    };

    GtpyModule(const GtpyModule& other) noexcept = delete;
    GtpyModule& operator=(const GtpyModule& other) noexcept = delete;

    GtpyModule(GtpyModule&&) noexcept;
    GtpyModule& operator=(GtpyModule&&) noexcept;

    virtual ~GtpyModule();

    bool evalScript(const QString& script, EvalOption option = EvalFile) const;

    void addObject(const QString& pyIdent, QObject* obj) const;

    void addVariable(const QString& pyIdent, const QVariant& value) const;

    void removeVariable(const QString& pyIdent) const;

    const QString& moduleName() const;

    PyPPObject module() const;

    bool isValid() const;

protected:
    explicit GtpyModule(const QString& moduleName);

    bool addFunctions(PyMethodDef* def);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // GTPY_MODULE_H
