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

#include "gtpy_codegen.h"

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
};


TEST(TestCodegen, TestPyIdentifier)
{
    auto ident = gtpy::codegen::pyIdentifier("gtlab");
    EXPECT_EQ(ident.toStdString(), "gtlab");

    ident = gtpy::codegen::pyIdentifier("_gtlab");
    EXPECT_EQ(ident.toStdString(), "_gtlab");

    ident = gtpy::codegen::pyIdentifier("gtlab_");
    EXPECT_EQ(ident.toStdString(), "gtlab_");

    ident = gtpy::codegen::pyIdentifier("GTlab");
    EXPECT_EQ(ident.toStdString(), "gt_lab");

    ident = gtpy::codegen::pyIdentifier("gt lab");
    EXPECT_EQ(ident.toStdString(), "gt_lab");

    ident = gtpy::codegen::pyIdentifier("GT Lab");
    EXPECT_EQ(ident.toStdString(), "gt_lab");

    ident = gtpy::codegen::pyIdentifier("GtLaB");
    EXPECT_EQ(ident.toStdString(), "gt_la_b");

    ident = gtpy::codegen::pyIdentifier("gT Lab");
    EXPECT_EQ(ident.toStdString(), "g_t_lab");

    ident = gtpy::codegen::pyIdentifier("gT LAb");
    EXPECT_EQ(ident.toStdString(), "g_t_la_b");

    ident = gtpy::codegen::pyIdentifier("GT 123 Lab");
    EXPECT_EQ(ident.toStdString(), "gt_123_lab");

    ident = gtpy::codegen::pyIdentifier("GT/*-+,!§$%&/()=?)\" !123! #Lab");
    EXPECT_EQ(ident.toStdString(), "gt_123_lab");

    ident = gtpy::codegen::pyIdentifier("   gt LAB   ");
    EXPECT_EQ(ident.toStdString(), "gt_lab");

    // And now forget everything you have read before.
    // Here is the one and only correct way to write it — no exceptions!
    ident = gtpy::codegen::pyIdentifier("GTlab");
    EXPECT_EQ(ident.toStdString(), "gt_lab");
}

TEST(TestCodegen, TestPyObjectIdentifier)
{
    MyObject o;
    o.setObjectName("MyObjectName");

    auto ident = gtpy::codegen::pyObjectIdentifier(&o);
    EXPECT_EQ(ident.toStdString(), "my_object_name");

    // test if the class name is used to generate the identifier when the
    // object name is empty
    o.setObjectName("");
    ident = gtpy::codegen::pyObjectIdentifier(&o);
    EXPECT_EQ(ident.toStdString(), "my_object");
}
