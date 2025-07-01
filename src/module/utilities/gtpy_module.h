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

#include <PythonQtPythonInclude.h>

#include <gtpypp.h>

#include <gt_pythonmodule_exports.h>

/**
 * @brief The GtpyModule class represents a C++ interface for a Python module.
 * A Python module corresponds to an importable *.py file.
 */
class GT_PYTHON_EXPORT GtpyModule
{
public:
    /**
     * @enum InputType
     * @brief Specifies the form of the input string to be processed during
     * Python script evaluation.
     */
    enum InputType
    {
        Script  = Py_file_input, /// multi-line input
        SingleStatement = Py_single_input /// single-line input
    };

    /**
     * @brief GtpyModule Constructor.
     * @param moduleName The name of the Python module to be created.
     */
    explicit GtpyModule(const QString& moduleName);

    GtpyModule(const GtpyModule& other) noexcept = delete;
    GtpyModule& operator=(const GtpyModule& other) noexcept = delete;

    GtpyModule(GtpyModule&&) noexcept;
    GtpyModule& operator=(GtpyModule&&) noexcept;

    virtual ~GtpyModule();

    /**
     * @brief Evaluates a Python script or statement within the scope of the
     * Python module. Any variables, functions or classes defined within the
     * module during evaluation will be available for subsequent calls of this
     * method.
     * @param code The Python code to be evaluated.
     * @param type Specifies the form of the input to be processed.
     * @return True if the script or statement was successfully evaluated,
     * otherwise false.
     */
    bool eval(const QString& code, InputType type = Script) const;

    /**
     * @brief Adds the given QObject to the Python module. The object becomes
     * accessible from Python code under the provided identifier. The
     * identifier must be a valid Python object name. If the object is a
     * nullptr or the identifier is invalid, the object is not added to the
     * module.
     * @param pyIdent The Python identifier for the object.
     * @param obj The QObject to be added to the Python module.
     */
    void addObject(const QString& pyIdent, QObject* obj) const;

    /**
     * @brief Adds a variable to the Python module. The variable becomes
     * accessible from Python code under the provided identifier. The
     * identifier must be a valid Python variable name. If the identifier is
     * invalid, the variable is not added to the module. If the value is
     * invalid, the variable is assigned to None.
     * @param pyIdent The Python identifier for the variable.
     * @param value The value to be assigned to the Python variable.
     */
    void addVariable(const QString& pyIdent, const QVariant& value) const;

    /**
     * @brief Removes the variable identified by the provided Python
    * identifier from the Python module. After removal, the variable will no
    * longer be accessible from Python code.
    * @param pyIdent The Python identifier of the variable to be removed.
     */
    void removeVariable(const QString& pyIdent) const;

    /**
     * @brief Returns the name of the Python module.
     * @return The name of the Python module.
     */
    const QString& moduleName() const;

    /**
     * @brief Adds a set of functions defined by the provided PyMethodDef
     * array to the Python module associated with this instance. The functions
     * will be accessible from Python code under the names defined in the
     * PyMethodDef structure.
     *
     * @param def A pointer to an array of PyMethodDef structures that define
     * the functions to be added to the Python module.
     * @return True if the functions were successfully added, otherwise false.
     */
    bool addFunctions(PyMethodDef* def);

protected:

    /**
     * @brief Returns the Python module object that is associated with the
     * current instance of GtpyModule. It provides direct access to the
     * underlying PyPPObject representing the module.
     *
     * Note: This method was introduced to integrate the GtpyContext class
     * into the GtpyContextManager without changing the API of the
     * GtpyContextManager. It will probably be removed with the next major
     * release.
     * @return The PyPPObject representing the Python module associated with
     * this instance.
     */
    PyPPObject module() const;

private:
    struct Impl;
    std::unique_ptr<Impl> pimpl;
};

#endif // GTPY_MODULE_H
