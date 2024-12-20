/* GTlab - Gas Turbine laboratory
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#include <QVariant>

#include <gtest/gtest.h>

#include <gt_calculatorfactory.h>
#include <gt_coredatamodel.h>
#include <gt_coreapplication.h>

#include "gtpy_codegenerator.h"

#include "test_helper.h"


class TestGtpyCodeGenerator : public ::testing::Test
{
protected:
    virtual void SetUp() override
    {
        // create a temporary data model to initialize gtDataModel
        dataModel = new MyCoreDataModel{};

        // initialize calculator data for MyCalculator
        auto testCalcData = GT_CALC_DATA(MyCalculator);
        testCalcData->id = "My GtCalculator";
        testCalcData->version = GtVersionNumber(0, 1);
        testCalcData->status = GtCalculatorDataImpl::PROTOTYPE;
        testCalcData->category = "Test";
        testCalcData->author = "Author";

        // register calculator data in the calculator factory
        gtCalculatorFactory->registerCalculatorData(testCalcData);

        // create the test calculator
        calc = new MyCalculator{};
        calc->setFactory(gtCalculatorFactory);

        // set a custom object name to test if the corresponding Python object
        // is named correctly
        calc->setObjectName("My gt Calculator");

        // get the generator instance
        generator = GtpyCodeGenerator::instance();
    }

    virtual void TearDown() override
    {
        if (calc.data())
        {
            delete calc;
        }

        if (dataModel.data())
        {
            delete dataModel;
        }

        gtCalculatorFactory->unregisterClass(MyCalculator::staticMetaObject);
    }

    QPointer<MyCalculator> calc;
    QPointer<MyCoreDataModel> dataModel;
    GtpyCodeGenerator* generator;
};


TEST_F(TestGtpyCodeGenerator, TestPyCodeDefaultProps)
{
    QString code = generator->calculatorPyCode(calc);

    QString expectedCode{
        R"(my_gt_calculator = MyCalculator("My gt Calculator")
)"};

    EXPECT_EQ(code.toStdString(), expectedCode.toStdString());
}

TEST_F(TestGtpyCodeGenerator, TestPyCodeStrProps)
{
    calc->strProp = "new value";

    QString code = generator->calculatorPyCode(calc);

    QString expectedCode{
        R"(my_gt_calculator = MyCalculator("My gt Calculator")
my_gt_calculator.setStrprop("new value")
)"};

    EXPECT_EQ(code.toStdString(), expectedCode.toStdString());
}

TEST_F(TestGtpyCodeGenerator, TestPyCodeDoubleProp)
{
    calc->doubleProp = 4.2;

    QString code = generator->calculatorPyCode(calc);

    QString expectedCode{
        R"(my_gt_calculator = MyCalculator("My gt Calculator")
my_gt_calculator.setDoubleProp(4.2)
)"};

    EXPECT_EQ(code.toStdString(), expectedCode.toStdString());
}

TEST_F(TestGtpyCodeGenerator, TestPyCodeBoolPropDefaultTrue)
{
    calc->boolPropDefaultTrue = false;

    QString code = generator->calculatorPyCode(calc);

    QString expectedCode{
        R"(my_gt_calculator = MyCalculator("My gt Calculator")
my_gt_calculator.setBoolproptrue(False)
)"};

    EXPECT_EQ(code.toStdString(), expectedCode.toStdString());
}

TEST_F(TestGtpyCodeGenerator, TestPyCodeBoolPropDefaultFalse)
{
    calc->boolPropDefaultFalse = true;

    QString code = generator->calculatorPyCode(calc);

    QString expectedCode{
        R"(my_gt_calculator = MyCalculator("My gt Calculator")
my_gt_calculator.setBoolpropFalse(True)
)"};

    EXPECT_EQ(code.toStdString(), expectedCode.toStdString());
}


TEST_F(TestGtpyCodeGenerator, TestPyCodeObjectLinkProp)
{
    // test link to a non-existent object
    calc->objLinkProp.setVal("{6c43fb0e-f7c0-4c78-bcec-3e66613647ac}");

    QString code = generator->calculatorPyCode(calc);

    QString expectedCode{
        R"(my_gt_calculator = MyCalculator("My gt Calculator")
my_gt_calculator.setObjLinkprop("{6c43fb0e-f7c0-4c78-bcec-3e66613647ac}")
)"};

    EXPECT_EQ(code.toStdString(), expectedCode.toStdString());

    // TODO: test link to a existing object
    /// This is currently not testable because generator->calculatorPyCode(calc)
    /// calls gtDataModel->objectByUuid(uuid) which resolves the object from
    /// the current session. Setting up the session for testing purposes seems
    /// too complex at this point. The code generator would be easier to test
    /// if the functionality were implemented more modularly (e.g. by
    /// transferring it from the singleton class to a namespace). Let's address
    /// this in the next minor release.
}
