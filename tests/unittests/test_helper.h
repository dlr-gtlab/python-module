/* GTlab - Gas Turbine laboratory
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */


#ifndef TEST_HELPER_H
#define TEST_HELPER_H

#include <QStringList>

#include <gt_coredatamodel.h>
#include <gt_object.h>
#include <gt_package.h>
#include <gt_calculator.h>
#include <gt_doubleproperty.h>
#include <gt_intproperty.h>
#include <gt_boolproperty.h>
#include <gt_objectlinkproperty.h>

#include <gtpy_contextmanager.h>


class MyObject : public GtObject
{
    Q_OBJECT
public:
    MyObject()
    {
        setObjectName("MyObjectName");
    }
};

class MyPackage : public GtPackage
{
    Q_OBJECT
public:
    MyPackage()
    {
        setObjectName("MyPackageName");
    }
};

class MyProperty : public GtProperty<QString>
{
    Q_OBJECT
public:
    MyProperty()
    {
        setObjectName("MyProperty");
    }

    QVariant valueToVariant(const QString& unit,
                            bool* success = nullptr) const override
    {
        return {};
    }

    bool setValueFromVariant(const QVariant& val,
                             const QString& unit) override
    {
        return false;
    }

    void setIdent(const QString& ident)
    {
        m_id = ident;
    }
};

class MyCoreDataModel : public GtCoreDatamodel
{
public:
    MyCoreDataModel() : GtCoreDatamodel()
    {

    }
};

class FirstCalculatorHelper : public GtObject
{
    Q_OBJECT

    Q_PROPERTY(QString strVal MEMBER strVal)

public:
    FirstCalculatorHelper()
    {
        registerProperty(strProp);
    }


    QString strVal{"strVal"};
    GtStringProperty strProp{"str prop", "name", "brief", "strPropVal"};
};

class SecondCalculatorHelper : public GtObject
{
    Q_OBJECT

    Q_PROPERTY(double doubleVal MEMBER doubleVal)

public:
    SecondCalculatorHelper()
    {
        registerProperty(doubleProp);

        doubleProp = 42.42;
    }

    double doubleVal = 4.2;
    GtDoubleProperty doubleProp{"double prop", "name", "brief"};
};

class SubHelper : public GtObject
{
    Q_OBJECT

    Q_PROPERTY(int intVal MEMBER intVal)

public:
    SubHelper()
    {
        registerProperty(intProp);
    }

    int intVal = 0;
    GtIntProperty intProp{"int prop", "name", "brief"};
};

class MyCalculator : public GtCalculator
{
    Q_OBJECT

public:
    Q_INVOKABLE MyCalculator()
    {
        setObjectName("MyGtCalculator");

        registerProperty(strProp);
        registerProperty(intProp);
        registerProperty(doubleProp);
        registerProperty(boolPropDefaultTrue);
        registerProperty(boolPropDefaultFalse);
        registerProperty(objLinkProp);
    }

    bool run() override
    {
        return true;
    }

    GtStringProperty strProp{"str prop", "name", "brief", "value"};
    GtIntProperty intProp{"int prop", "name", "brief"};
    GtDoubleProperty doubleProp{"double prop", "name", "brief"};
    GtBoolProperty boolPropDefaultTrue{"bool prop true", "name", "brief", true};
    GtBoolProperty boolPropDefaultFalse{"bool prop false", "name", "brief",
                                        false};
    GtObjectLinkProperty objLinkProp{"obj link prop", "name", "brief", this,
                                     QStringList{} << GT_CLASSNAME(GtObject),
                                     true};
};

struct TestPythonContext
{
    explicit TestPythonContext(GtpyContextManager::Context type = GtpyContextManager::ScriptEditorContext)
    {
        GtpyContextManager::instance()->initContexts();
        context_id = GtpyContextManager::instance()->createNewContext(type);
    }

    ~TestPythonContext()
    {
        GtpyContextManager::instance()->deleteContext(context_id);
    }

    int id() const
    {
        return context_id;
    }

    PyPPObject obj() const
    {
        return PyPPObject::Borrow(GtpyContextManager::instance()->contextPointer(context_id));
    }

    int context_id;
};

#endif // TEST_HELPER_H
