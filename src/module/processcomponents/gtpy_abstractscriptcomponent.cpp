/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_abstractscriptcomponent.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 09.08.2023
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QRegExpValidator>

#include "gtpy_abstractscriptcomponent.h"

#include "gt_package.h"
#include "gt_objectpath.h"
#include "gt_objectpathproperty.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_structproperty.h"
#include "gt_stringmonitoringproperty.h"
#include "gt_doublemonitoringproperty.h"
#include "gt_intmonitoringproperty.h"
#endif

#if GT_VERSION >= GT_VERSION_CHECK(2, 1, 0)
#include <QRegularExpression>
#endif

#include "gtpy_transfer.h"
#include "gtpy_contextmanager.h"
#include "gtpy_packageiteration.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)

namespace {

bool
setStructContainerValue(GtPropertyStructContainer& con, const QString& argName,
                        const QVariant& value)
{
    auto entry = std::find_if(con.begin(), con.end(),
                              [&argName](const GtPropertyStructInstance& entry){
        return entry.getMemberVal<QString>("name") == argName;
    });

    if (entry == con.end())
    {
        return false;
    }

    return entry->setMemberVal("value", value);
}

QVariant
structContainerValue(const GtPropertyStructContainer& con, const QString& argName)
{
    auto entry = std::find_if(con.begin(), con.end(),
                              [&argName](const GtPropertyStructInstance& entry){
        return entry.getMemberVal<QString>("name") == argName;
    });

    if (entry == con.end())
    {
        return {};
    }

    return entry->getMemberValToVariant("value");
}

gt::PropertyFactoryFunction
makeWildcardStringProperty(QString value = "")
{
#if GT_VERSION >= GT_VERSION_CHECK(2, 1, 0)
    return [=](const QString& id) {
        return new GtStringProperty(id, id, {}, std::move(value),
                                    QRegularExpression(QRegExp{".*"}.pattern()));
    };
#else
    return [=](const QString& id) {
        return new GtStringProperty(id, id, {}, std::move(value),
                                    new QRegExpValidator{QRegExp{".*"}});
    };
#endif
}

}

#endif

GtpyAbstractScriptComponent::GtpyAbstractScriptComponent() :
    m_pyThreadId{-1},
    m_replaceTabBySpaces{"replaceTab", "Replace tab by spaces"},
    m_tabSize{"tabSize", "Tab size"},
    m_script{"script", "Script"}
#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    ,
    m_inputArgs{"input_args"},
    m_outputArgs{"output_args"}
#endif
{

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    auto createStructDef =
            [](const QString& typeName, gt::PropertyFactoryFunction f){
        GtPropertyStructDefinition structDef(typeName);
        structDef.defineMember("name", gt::makeStringProperty());
        structDef.defineMember("value", f);
        return structDef;
    };

    m_inputArgs.registerAllowedType(
                createStructDef("str", makeWildcardStringProperty()));
    m_inputArgs.registerAllowedType(
                createStructDef("int", gt::makeIntProperty(0)));
    m_inputArgs.registerAllowedType(
                createStructDef("float", gt::makeDoubleProperty(0.0)));

    m_outputArgs.registerAllowedType(
                createStructDef("str", gt::makeStringMonitoringProperty()));
    m_outputArgs.registerAllowedType(
                createStructDef("int", gt::makeIntMonitoringProperty(0)));
    m_outputArgs.registerAllowedType(
                createStructDef("float", gt::makeDoubleMonitoringProperty(0.0)));
#endif

    // cppcheck-suppress useInitializationList
    m_tabSize = 4;
    // cppcheck-suppress useInitializationList
    m_replaceTabBySpaces = true;

    m_script.hide();
    m_replaceTabBySpaces.hide();
    m_tabSize.hide();

    gtpy::package::forEachPackage([&](const GtPackage* pkg){
        if (!pkg) return;

        auto pathProp = new GtObjectPathProperty(
                    QStringLiteral(""), QStringLiteral(""), QStringLiteral(""),
                    pkg->objectName(), nullptr,
                    QStringList() << pkg->metaObject()->className());

        pathProp->hide(true);

        m_dynamicPathProps << pathProp;});
}

GtpyAbstractScriptComponent::~GtpyAbstractScriptComponent()
{
    qDeleteAll(m_dynamicPathProps);
}

QString
GtpyAbstractScriptComponent::script() const
{
    return m_script;
}

void
GtpyAbstractScriptComponent::setScript(const QString& script)
{
#if GT_VERSION <= GT_VERSION_CHECK(1, 7, 0)
    m_script = QString{script}.replace("\n", "\r");
#else
    m_script = script;
#endif
}

bool
GtpyAbstractScriptComponent::replaceTabBySpaces() const
{
    return m_replaceTabBySpaces;
}

void
GtpyAbstractScriptComponent::setReplaceTabBySpaces(bool replaceTabBySpaces)
{
    m_replaceTabBySpaces = replaceTabBySpaces;
}

int
GtpyAbstractScriptComponent::tabSize() const
{
    return m_tabSize;
}

void
GtpyAbstractScriptComponent::setTabSize(int tabSize)
{
    m_tabSize = tabSize;
}

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
const GtPropertyStructContainer&
GtpyAbstractScriptComponent::inputArgs() const
{
    return m_inputArgs;
}

QVariant
GtpyAbstractScriptComponent::inputArg(const QString& argName) const
{
    return structContainerValue(m_inputArgs, argName);
}

bool
GtpyAbstractScriptComponent::setInputArg(
        const QString& argName, const QVariant& value)
{
    return setStructContainerValue(m_inputArgs, argName, value);
}

const GtPropertyStructContainer&
GtpyAbstractScriptComponent::outputArgs() const
{
    return m_outputArgs;
}

QVariant
GtpyAbstractScriptComponent::outputArg(const QString& argName) const
{
    return structContainerValue(m_outputArgs, argName);
}
#endif

bool
GtpyAbstractScriptComponent::evalScript(int contextId)
{
    for (auto* pathProp : qAsConst(m_dynamicPathProps))
    {
        gtpy::transfer::gtObjectToPython(
                    contextId, dataPackage(pathProp->path()));
    }

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    gtpy::transfer::propStructToPython(contextId, m_inputArgs);
    gtpy::transfer::propStructToPython(contextId, m_outputArgs);
#endif

    m_pyThreadId = GtpyContextManager::instance()->currentPyThreadId();

    GtpyGlobals::StdOutMetaData metaData =
        GtpyContextManager::instance()->threadDictMetaData();

    gtInfo() << "running script...";

    bool success = GtpyContextManager::instance()->evalScript(
                contextId, script(), true);

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    gtpy::transfer::propStructFromPython(contextId, m_outputArgs);
#endif

    for (auto* pathProp : qAsConst(m_dynamicPathProps))
    {
        gtpy::transfer::removeGtObjectFromPython(
                    contextId, dataPackage(pathProp->path()));
    }

    GtpyContextManager::instance()->deleteContext(contextId, true);

    GtpyContextManager::instance()->setMetaDataToThreadDict(metaData);

    gtInfo() << "...done!";

    return success;
}
