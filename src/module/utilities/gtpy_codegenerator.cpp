/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_codegenerator.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 18.11.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QMetaProperty>
#include <QRegularExpression>

#include "gt_application.h"
#include "gt_calculator.h"
#include "gt_objectmementodiff.h"
#include "gt_datamodel.h"
#include "gt_objectlinkproperty.h"
#include "gt_calculatorhelperfactory.h"

#include "gtpy_contextmanager.h"
#include "gtpy_codegen.h"

#include "gtpy_codegenerator.h"

GtpyCodeGenerator::GtpyCodeGenerator(QObject* parent) : QObject(parent)
{

}

GtpyCodeGenerator*
GtpyCodeGenerator::instance()
{
    static GtpyCodeGenerator* retval = nullptr;

    if (retval == nullptr)
    {
        retval = new GtpyCodeGenerator(gtApp);
    }

    return retval;
}

QString
GtpyCodeGenerator::calculatorPyCode(GtCalculator* calc)
{
    auto pyCode = gtpy::codegen::calcToPyCode(calc);

    // TODO: check if the concept of helper classes is still relevant
    auto helperPy = helperPyCode(
        calc, gtpy::codegen::pyObjectIdentifier(calc));

    if (!helperPy.isEmpty())
    {
        pyCode += ("\n" + helperPy);
    }

    return pyCode;
}

QString
GtpyCodeGenerator::helperPyCode(GtObject* obj, const QString& pyObjName)
{
    if (obj == nullptr)
    {
        return QString();
    }

    QString pyCode;

    QStringList helperNames = gtCalculatorHelperFactory->connectedHelper(
                                  obj->metaObject()->className());

    if (helperNames.isEmpty())
    {
        return QString();
    }

    QList<GtObject*> children = obj->findDirectChildren<GtObject*>();

    bool initRun = true;

    foreach (GtObject* child, children)
    {
        if (child != nullptr)
        {
            QString childClassName = child->metaObject()->className();

            if (helperNames.contains(childClassName))
            {
                QString helperObjName = child->objectName();

                if (helperObjName.isEmpty())
                {
                    helperObjName = childClassName;
                }

                QList<GtObject*> sameNamedChildren =
                    obj->findDirectChildren<GtObject*>(helperObjName);

                int count = 1;

                foreach (GtObject* sameNamedChild, sameNamedChildren)
                {
                    if (sameNamedChild != child)
                    {
                        sameNamedChild->setObjectName(helperObjName +
                                                      QString::number(count));
                        count++;
                    }
                }

                QRegExp regExp("[^A-Za-z0-9]+");

                int pos = regExp.indexIn(helperObjName);

                while (pos >= 0)
                {
                    helperObjName = helperObjName.remove(pos, 1);
                    pos = regExp.indexIn(helperObjName);
                }

                helperObjName.replace(0, 1, helperObjName.at(0).toLower());

                if (initRun)
                {
                    initRun = false;
                }
                else
                {
                    pyCode += "\n";
                }

                pyCode += (helperObjName + " = " + pyObjName + ".create" +
                           childClassName + "(\"" +
                           child->objectName() + "\")\n");

                const QMetaObject* metaobject = child->metaObject();

                for (int i = 0; i < metaobject->propertyCount(); i ++)
                {
                    QMetaProperty metaproperty = metaobject->property(i);
                    const char* name = metaproperty.name();

                    QString nameStr = QString::fromUtf8(name);

                    if (nameStr != "objectName")
                    {
                        QVariant value = child->property(name);

                        QString typeName = QString::fromUtf8(
                                               child->property(name).
                                               typeName());

                        if (typeName == "QString")
                        {
                            pyCode += (helperObjName + "." + nameStr + " = \"" +
                                       GtpyContextManager::instance()->
                                       qvariantToPyStr(value) + "\"\n");
                        }
                        else
                        {
                            pyCode += (helperObjName + "." + nameStr + " = " +
                                       GtpyContextManager::instance()->
                                       qvariantToPyStr(value) + "\n");
                        }
                    }
                }

                QList<GtAbstractProperty*> props = child->properties();

                foreach (GtAbstractProperty* prop, props)
                {
                    if (prop != nullptr)
                    {
                        QString ident = prop->ident();

                        QString val = gtpy::codegen::propValToPyCode(prop);

                        pyCode += (helperObjName + "." +
                                   GtpyContextManager::instance()->
                                   setPropertyValueFuncName() + "(\""
                                   + ident + "\", " + val + ")\n");

                    }
                }

                QString childHelperCode = helperPyCode(child, helperObjName);

                if (!childHelperCode.isEmpty())
                {
                    pyCode += "\n";
                    pyCode += childHelperCode;
                }
            }
        }
    }

    return pyCode;
}
