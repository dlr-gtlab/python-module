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
#include <gt_calculator.h>
#include <gt_doubleproperty.h>
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

class MyCoreDataModel : public GtCoreDatamodel
{
public:
    MyCoreDataModel() : GtCoreDatamodel()
    {

    }

};

class MyCalculator : public GtCalculator
{
    Q_OBJECT

public:
    Q_INVOKABLE MyCalculator()
    {
        setObjectName("MyGtCalculator");

        registerProperty(strProp);
        registerProperty(doubleProp);
        registerProperty(boolPropDefaultTrue);
        registerProperty(boolPropDefaultFalse);
        registerProperty(objLinkProp);
    }

    bool run() override
    {
        return true;
    }

    // vary property idents to test if the corresponding Python setter method
    // is named correctly by the automated code generation
    GtStringProperty strProp{"str prop", "name", "brief", "value"};
    GtDoubleProperty doubleProp{"Double Prop", "name", "brief"};
    GtBoolProperty boolPropDefaultTrue{"boolproptrue", "name", "brief", true};
    GtBoolProperty boolPropDefaultFalse{"  bool  prop  False", "name", "brief",
                                        false};
    GtObjectLinkProperty objLinkProp{"obj Link prop", "name", "brief", this,
                                     QStringList{} << GT_CLASSNAME(GtObject),
                                     true};
};

struct TestPythonContext
{
    explicit TestPythonContext(GtpyContextManager::Context type = GtpyContextManager::ScriptEditorContext) :
        context_id(GtpyContextManager::instance()->createNewContext(type))
    {
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
