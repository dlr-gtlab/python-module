/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_codegen.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 19.12.2024
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <QRegularExpression>

#include "gt_xmlexpr.h"
#include "gt_datamodel.h"
#include "gt_package.h"
#include "gt_calculator.h"
#include "gt_objectlinkproperty.h"
#include "gt_stringproperty.h"
#include "gt_objectmemento.h"
#include "gt_calculatorfactory.h"
#include "gt_objectmementodiff.h"

#include "gtpy_contextmanager.h"
#include "gtpy_regexp.h"

#include "gtpy_codegen.h"

namespace
{

inline QString quot(const QString& val)
{
    return QString{"\"%1\""}.arg(val);
}

void generatePythonObjectPath(GtObject* obj, QStringList& objPath)
{
    if (!obj) return;

    QString objGetter  = obj->objectName();

    // if the object name contains special characters use findGtChild
    // with the object name as a parameter to get the object
    if (!gtpy::re::validation::isValidPythonIdentifier(objGetter))
    {
        objGetter = GtpyContextManager::instance()->findChildFuncName() +
                    "(" + quot(obj->objectName()) + ")";
    }

    objPath.prepend(objGetter);

    // since packages are accessible directly via the Python context
    // terminate recursion after resolving the package getter
    if (qobject_cast<GtPackage*>(obj)) return;

    generatePythonObjectPath(obj->parentObject(), objPath);
}

GtObjectMemento defaultCalcMemento(GtCalculator* calc)
{
    // check if calculator and calculator data are valid
    if (!calc) return GtObjectMemento{};

    const auto calcData = gtCalculatorFactory->calculatorData(
        calc->metaObject()->className());

    if (!calcData->isValid()) return GtObjectMemento{};

    // create a new calculator instance to get the default memento
    std::unique_ptr<GtCalculator> defaultCalc(qobject_cast<GtCalculator*>(
        calcData->metaData().newInstance()));

    if (!defaultCalc) return GtObjectMemento{};

    defaultCalc->setFactory(gtCalculatorFactory);
    defaultCalc->setUuid(calc->uuid());

    return defaultCalc->toMemento();
}

}

QString
gtpy::codegen::pyIdentifier(const QString& str)
{
    // remove leading and trailing whitespace
    auto ident = str.trimmed();

    // remove special characters except underscores
    ident.replace(QRegularExpression{"[^A-Za-z0-9 _]"}, "");

    if (ident.isEmpty())
    {
        gtWarning() << QObject::tr(
                           "A valid Python identifier cannot be created based "
                           "on '%1'. The given string is either empty or "
                           "contains only special characters."
                           ).arg(str);

        return {};
    }

    // replace whitespaces with a single underscore
    ident.replace(QRegularExpression{"\\s+"}, "_");

    // match capital letter sequences optionally preceded or followed by an
    // underscore
    QRegularExpression capLetters{"[A-Z]+"};
    QRegularExpressionMatchIterator iterator = capLetters.globalMatch(ident);

    int shift = 0;

    // iterate over the capital letter sequences
    while (iterator.hasNext())
    {
        auto match = iterator.next();

        // adjust the start and end positions of the match
        // necessary because ident is modified during the loop
        int startPos = match.capturedStart() + shift;
        int endPos = match.capturedEnd() + shift;

        // convert uppercase letters to lowercase
        auto matchedStr = match.captured(0).toLower();

        bool singleLetter = (matchedStr.size() == 1);

        // if the match is not at the start of the string and there is not
        // already an underscore before the match, prepend an underscore
        if (startPos > 0 && ident.at(startPos - 1) != "_")
        {
            matchedStr.prepend("_");
        }

        // if the match is not at the end of the string and it consists of
        // more than one letter, append an underscore
        if (!singleLetter && endPos < ident.length() &&
            ident.at(endPos) != "_")
        {
            matchedStr.append("_");
        }

        ident.replace(startPos, endPos - startPos, matchedStr);

        shift += matchedStr.length() - (endPos - startPos);
    }

    // if the resulting identifier starts with a digit, prepend an underscore
    if (!ident.isEmpty() && ident[0].isDigit())
    {
        ident.prepend("_");
    }

    assert(gtpy::re::validation::isValidPythonIdentifier(ident));

    return ident;
}

QString
gtpy::codegen::pyObjectIdentifier(GtObject* obj)
{
    if (!obj) return {};

    // create the Python object identifier from the object name property
    // or fall back to the class name if the object name is empty
    const auto& objName = obj->objectName();

    auto pyObjIdent = pyIdentifier(objName);
    if (!pyObjIdent.isEmpty()) return pyObjIdent;

    const auto& className = obj->metaObject()->className();
    return pyIdentifier(className);
}

QString
gtpy::codegen::pyObjectPath(GtObject* obj)
{
    QStringList objPath{};
    generatePythonObjectPath(obj, objPath);
    return objPath.join('.');
}

QString
gtpy::codegen::pySetterName(const QString& str)
{
    QString setterName{str};

    // remove special characters
    setterName.replace(gtpy::re::exceptLettersAndNumbers(), "");

    if (setterName.isEmpty())
    {
        gtWarning() << QObject::tr(
                           "A valid setter method name cannot be created "
                           "based on '%1'. The given string is either empty or "
                           "contains only special characters."
            ).arg(str);

        return {};
    }

    // capitalize the first letter and prepend 'set'
    setterName.replace(0, 1, setterName.at(0).toUpper());
    setterName.prepend("set");

    return setterName;
}

QString
gtpy::codegen::pyPropSetterName(GtAbstractProperty* prop)
{
    if (!prop) return {};

    return pySetterName(prop->ident());
}

QString
gtpy::codegen::propValToPyCode(GtAbstractProperty* prop)
{
    QString val{};

    if (qobject_cast<GtObjectLinkProperty*>(prop))
    {
        const auto& uuid = prop->valueToVariant().toString();
        val = pyObjectIdentifier(gtDataModel->objectByUuid(uuid));

        if (val.isEmpty())
        {
            val = quot(uuid);
        }
    }
    else if (qobject_cast<GtStringProperty*>(prop) ||
             dynamic_cast<GtProperty<QString>*>(prop))
    {
        val = quot(GtpyContextManager::instance()->qvariantToPyStr(
            prop->valueToVariant()));
    }
    else
    {
        val = GtpyContextManager::instance()->qvariantToPyStr(
            prop->valueToVariant());
    }

    return val;
}

QString
gtpy::codegen::calcToPyCode(GtCalculator* calc)
{
    if (!calc) return {};

    const auto& defaultMemento = defaultCalcMemento(calc);

    assert(!defaultMemento.isNull());

    const auto& objName = calc->objectName();
    const auto& className = calc->metaObject()->className();

    // create the Python object name from the calculator object name
    // or fall back to the class name if the object name is empty
    auto pyObjIdent = pyObjectIdentifier(calc);

    // Python code to create calculator object
    QString pyCode{"%1 = %2(%3)\n"};
    pyCode = pyCode.arg(pyObjIdent).arg(className).arg(quot(objName));

    // diff default and current configuration of the calculator
    GtObjectMementoDiff diff(defaultMemento, calc->toMemento());

    if (diff.numberOfDiffSteps() == 0) return pyCode;

    QDomNodeList changeElements = diff.documentElement().elementsByTagName(
        gt::xml::S_DIFF_PROP_CHANGE_TAG);

    // generate Python setter calls for properties that have changed
    // compared to their corresponding default values
    QString setPropCodeTemplate{"%1.%2(%3)\n"};
    setPropCodeTemplate = setPropCodeTemplate.arg(pyObjIdent);

    for (int i = 0; i < changeElements.count(); ++i)
    {
        QDomElement element = changeElements.at(i).toElement();

        if (element.isNull()) continue;

        auto* prop = calc->findProperty(element.attribute(gt::xml::S_NAME_TAG));

        const auto& setterName = pyPropSetterName(prop);

        if (setterName.isEmpty()) continue;

        const auto& val = propValToPyCode(prop);

        pyCode.append(setPropCodeTemplate
                          .arg(setterName)
                          .arg(propValToPyCode(prop)));
    }

    // TODO: evluate if it is still necessary to generate Python
    // code for helper classes

    return pyCode;
}
