/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractscriptcomponent.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 09.08.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYCOMPONENTASSISTANT_H
#define GTPYCOMPONENTASSISTANT_H

#include "gt_globals.h"
#include "gt_stringproperty.h"
#include "gt_intproperty.h"
#include "gt_boolproperty.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_propertystructcontainer.h"
#endif

class GtPackage;
class GtObjectPath;
class GtObjectPathProperty;

class GtpyAbstractScriptComponent
{
public:
    /**
     * @brief Destructor
     */
    virtual ~GtpyAbstractScriptComponent();

    /**
     * @brief Returns current script.
     * @return Current script.
     */
    QString script() const;

    /**
     * @brief Sets new script.
     * @param New script.
     */
    void setScript(const QString &script);

    /**
     * @brief Returns whether tabs are replaced by spaces.
     * @return True if tabs are replaced by spaces.
     */
    bool replaceTabBySpaces() const;

    /**
     * @brief Sets whether tabs are replaced by spaces.
     * @param replaceTabBySpaces If this parameter is true tabs will be
     * replaced by spaces.
     */
    void setReplaceTabBySpaces(bool replaceTabBySpaces);

    /**
     * @brief Returns the tab size.
     * @return Tab size
     */
    int tabSize() const;

    /**
     * @brief Sets the tab size to the given tabSize.
     * @param tabSize Tab size value.
     */
    void setTabSize(int tabSize);

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    /**
     * @brief Returns the input arguments as property struct container.
     * @return Input arguments property struct container.
     */
    const GtPropertyStructContainer& inputArgs() const;

    /**
     * @brief Returns the input argument identified by the given argName.
     * @param argName Name of the input argument for which the value should be
     * returned.
     * @return The value of the input arguement identified by argName.
     */
    QVariant inputArg(const QString& argName) const;

    /**
     * @brief Sets the value of the input argument identified by the given
     * argName to the given value.
     * @param argName Name of the input argument for which the value should be
     * set.
     * @param value New argument value.
     */
    bool setInputArg(const QString& argName, const QVariant& value);

    /**
     * @brief Returns the output arguments as property struct container.
     * @return Output arguments property struct container.
     */
    const GtPropertyStructContainer& outputArgs() const;

    /**
     * @brief Returns the output argument identified by the given argName.
     * @param argName Name of the output argument for which the value should be
     * returned.
     * @return The value of the output arguement identified by argName.
     */
    QVariant outputArg(const QString& argName) const;
#endif

    /**
     * @brief Returns the script property.
     * @return Constant reference to the script property.
     */
    const GtStringProperty& scriptProp() const;

protected:
    /**
     * @brief GtpyComponentAssistant
     */
    GtpyAbstractScriptComponent();

    /// Python thread id
    long m_pyThreadId;

    /// Replace Tab By Spaces.
    GtBoolProperty m_replaceTabBySpaces;

    /// Tab size.
    GtIntProperty m_tabSize;

    /// Script.
    GtStringProperty m_script;

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    /// Input argument struct container.
    GtPropertyStructContainer m_inputArgs;

    /// Output argument struct container.
    GtPropertyStructContainer m_outputArgs;
#endif

    /// Dynamic properties regarding project modules
    QList<GtObjectPathProperty*> m_dynamicPathProps;

    /**
     * @brief Creates a context of the given type and adds all available
     * packages and the input and output property struct container to the
     * context. Finally, it starts the evaluation of the script.
     * @param contextId Id of the context where the script should be executed.
     * @return True, if the evaluation was successful.
     */
    bool evalScript(int contextId);

private:
    /**
     * @brief Must be implemented by classes derived from this class.
     * Has to return the data package identified by the given object path.
     * @param pkgPath Object path
     * @return data package
     */
    virtual GtPackage* dataPackage(const GtObjectPath& pkgPath) = 0;
};

#endif // GTPYCOMPONENTASSISTANT_H
