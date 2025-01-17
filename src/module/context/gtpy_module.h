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

#include "PythonQtPythonInclude.h"

class GtpyModule
{
public:
    enum EvalOption {
        EvalFile = Py_file_input,
        EvalSingleString = Py_single_input
    };

    virtual ~GtpyModule();

    // delete the copy constructor and copy assignment operator
    GtpyModule(const GtpyModule&) = delete;
    GtpyModule& operator=(const GtpyModule&) = delete;

    // declare the move constructor and move assignment operator
    GtpyModule(GtpyModule&&) noexcept;
    GtpyModule& operator=(GtpyModule&&) noexcept;

    bool evalScript(const QString& script, EvalOption option = EvalFile) const;

    const QString& moduleName() const;

protected:
    GtpyModule(const QString& moduleName);

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // GTPY_MODULE_H
