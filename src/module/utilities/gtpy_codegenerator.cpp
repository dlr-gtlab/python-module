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

#include "gt_application.h"
#include "gt_calculator.h"
#include "gt_objectmementodiff.h"
#include "gt_datamodel.h"
#include "gt_package.h"
#include "gt_stringproperty.h"
#include "gt_modeproperty.h"
#include "gt_objectlinkproperty.h"
#include "gt_calculatorfactory.h"
#include "gt_calculatorhelperfactory.h"

#include "gtpy_contextmanager.h"

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
    if (calc == nullptr)
    {
        return QString();
    }

    QString className = calc->metaObject()->className();

    GtObjectMemento before;

    if (gtCalculatorFactory->calculatorDataExists(className))
    {
        GtCalculatorData calcData =
            gtCalculatorFactory->calculatorData(className);

        if (!calcData->isValid())
        {
            return QString();
        }

        QObject* newObj = calcData->metaData().newInstance();

        if (newObj == nullptr)
        {
            return QString();
        }

        GtCalculator* defaultCalc = qobject_cast<GtCalculator*>(newObj);

        if (defaultCalc == nullptr)
        {
            delete newObj;
            return QString();
        }

        defaultCalc->setFactory(gtCalculatorFactory);
        defaultCalc->setUuid(calc->uuid());

        before = defaultCalc->toMemento();

        delete defaultCalc;
    }
    else
    {
        return QString();
    }

    QString objName = calc->objectName();

    QRegExp regExp("[^A-Za-z0-9]+");

    QString pyCode;

    QString tempObjName = objName;

    if (tempObjName.isEmpty())
    {
        tempObjName = className;
    }

    int pos = regExp.indexIn(tempObjName);

    while (pos >= 0)
    {
        tempObjName = tempObjName.remove(pos, 1);
        pos = regExp.indexIn(tempObjName);
    }

    tempObjName.replace(0, 1, tempObjName.at(0).toLower());

    pyCode += (tempObjName + " = " + className + "(\"" +
               objName + "\")\n");

    GtObjectMementoDiff diff(before, calc->toMemento());

    QDomElement root = diff.documentElement();

    while (!root.isNull())
    {
        if (root.attribute("uuid") == calc->uuid())
        {
            break;
        }

        root = root.nextSiblingElement();
    }

    if (!root.isNull())
    {
        QDomElement changeElement = root.firstChildElement(
                                        "diff-property-change");

        while (!changeElement.isNull())
        {
            QString ident = changeElement.attribute("name");

            if (ident.isEmpty())
            {
                continue;
            }

            GtAbstractProperty* prop = calc->findProperty(ident);

            if (prop != nullptr)
            {
                int propIdpos = regExp.indexIn(ident);

                while (propIdpos >= 0)
                {
                    ident = ident.remove(propIdpos, 1);
                    propIdpos = regExp.indexIn(ident);
                }

                ident.replace(0, 1, ident.at(0).toUpper());

                ident.insert(0, "set");

                QString val = propValToString(prop);

                pyCode += (tempObjName + "." + ident + "(" + val + ")\n");

                changeElement = changeElement.
                                nextSiblingElement("diff-property-change");
            }
        }
    }

    QString helperPy = helperPyCode(calc, tempObjName);

    if (!helperPy.isEmpty())
    {
        pyCode += ("\n" + helperPy);
    }

    return pyCode;
}

QString
GtpyCodeGenerator::propValToString(GtAbstractProperty* prop)
{
    QString val;

    if (qobject_cast<GtModeProperty*>(prop))
    {
        QString valTemp = GtpyContextManager::instance()->qvariantToPyStr(
                              prop->valueToVariant());

        if (valTemp.isEmpty())
        {
            valTemp = "\"\"";
        }

        val = "\""
              + valTemp
              + "\"";
    }
    else if (qobject_cast<GtObjectLinkProperty*>(prop))
    {
        GtObject* obj = gtDataModel->objectByUuid(prop->valueToVariant().
                        toString());

        if (obj)
        {
            GtPackage* pack = nullptr;

            while (pack == nullptr)
            {
                QString objName = obj->objectName();

                if (!objName.contains(
                            QRegExp(QStringLiteral("^[a-zA-Z0-9_]*$"))))
                {
                    QString funcName =
                        GtpyContextManager::instance()->findChildFuncName();

                    objName = funcName + "(\"" + objName +
                              "\")";
                }

                obj = qobject_cast<GtObject*>(obj->parent());

                if (obj == nullptr)
                {
                    break;
                }

                pack = qobject_cast<GtPackage*>(obj);

                val.insert(0, objName);

                val.insert(0, ".");

                if (pack != nullptr)
                {
                    objName = pack->objectName();

                    if (!objName.contains(
                                QRegExp(QStringLiteral("^[a-zA-Z0-9_]*$"))))
                    {
                        QString funcName =
                            GtpyContextManager::instance()->
                            findChildFuncName();

                        objName = funcName + "(\"" + objName +
                                  "\")";
                    }

                    val.insert(0, objName);
                }
            }

            obj = nullptr;
            pack = nullptr;
        }
        else
        {
            QString valTemp = prop->valueToVariant().toString();

            if (valTemp.isEmpty())
            {
                val = "\"\"";
            }
            else
            {
                val =  GtpyContextManager::instance()->qvariantToPyStr(
                           prop->valueToVariant());
            }
        }
    }
    else if (qobject_cast<GtStringProperty*>(prop))
    {
        val = "\"";
        val +=  GtpyContextManager::instance()->qvariantToPyStr(
                    prop->valueToVariant());
        val += "\"";
    }
    else if (dynamic_cast<GtProperty<QString>*>(prop))
    {
        val = "\"";
        val +=  GtpyContextManager::instance()->qvariantToPyStr(
                    prop->valueToVariant());
        val += "\"";
    }
    else
    {
        val = GtpyContextManager::instance()->qvariantToPyStr(
                  prop->valueToVariant());
    }

    return val;
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

                        QString val = propValToString(prop);

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
