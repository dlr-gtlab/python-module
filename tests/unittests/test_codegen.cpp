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

TEST(TestCodegen, TestPyObjectPath)
{
    MyPackage p;
    p.setObjectName("MyPackageName");

    MyObject first;
    first.setObjectName("FirstObj");
    p.appendChild(&first);

    MyObject sec;
    sec.setObjectName("Second Object");
    first.appendChild(&sec);

    MyObject searchedObj;
    searchedObj.setObjectName("SearchedObject");
    sec.appendChild(&searchedObj);

    auto path = gtpy::codegen::pyObjectPath(&searchedObj);
    EXPECT_EQ(path.toStdString(),
              "MyPackageName.FirstObj.findGtChild(\"Second Object\")"
              ".SearchedObject");
}

TEST(TestCodegen, TestPySetterName)
{
    auto ident = gtpy::codegen::pySetterName("Prop");
    EXPECT_EQ(ident.toStdString(), "setProp");

    ident = gtpy::codegen::pySetterName("prop");
    EXPECT_EQ(ident.toStdString(), "setProp");

    ident = gtpy::codegen::pySetterName("My Prop");
    EXPECT_EQ(ident.toStdString(), "setMyProp");

    ident = gtpy::codegen::pySetterName("my PROP");
    EXPECT_EQ(ident.toStdString(), "setMyPROP");

    ident = gtpy::codegen::pySetterName("my 123");
    EXPECT_EQ(ident.toStdString(), "setMy123");

    ident = gtpy::codegen::pySetterName("my /*-+$%&/()=?\"");
    EXPECT_EQ(ident.toStdString(), "setMy");

    ident = gtpy::codegen::pySetterName("");
    EXPECT_EQ(ident.toStdString(), "");

    ident = gtpy::codegen::pySetterName(" /*-+$%&/()=?\"");
    EXPECT_EQ(ident.toStdString(), "");
}

TEST(TestCodegen, TestPyPropSetterName)
{
    MyProperty p;
    p.setIdent("propIdent");

    auto setterName = gtpy::codegen::pyPropSetterName(&p);
    EXPECT_EQ(setterName.toStdString(), "setPropIdent");

    p.setIdent("");

    setterName = gtpy::codegen::pyPropSetterName(&p);
    EXPECT_EQ(setterName.toStdString(), "");

    setterName = gtpy::codegen::pyPropSetterName(nullptr);
    EXPECT_EQ(setterName.toStdString(), "");
}
