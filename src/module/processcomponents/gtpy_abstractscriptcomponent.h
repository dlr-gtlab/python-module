/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractscriptcomponent.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 09.08.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOMPONENTASSISTANT_H
#define GTPYCOMPONENTASSISTANT_H

#include "gt_stringproperty.h"
#include "gt_intproperty.h"
#include "gt_boolproperty.h"

#include "gtpy_contextmanager.h"

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
    const GtPropertyStructContainer& inputArgs();

    /**
     * @brief Returns the output arguments as property struct container.
     * @return Output arguments property struct container.
     */
    const GtPropertyStructContainer& outputArgs();
#endif

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
     * @param type Context type.
     * @return True, if the evaluation was successful.
     */
    bool evalScritp(const GtpyContextManager::Context& type);

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
