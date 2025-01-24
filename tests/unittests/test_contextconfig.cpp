/* GTlab - Gas Turbine laboratory
 * Source File: test_contextconfig.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 20.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include <PythonQtPythonInclude.h>

#include <QRegularExpression>

#include "test_helper.h"

#include <gtpypp.h>
#include <gtpy_constants.h>
#include <gtpy_code.h>
#include <gtest/gtest.h>

struct TestContextConfigParams {
    GtpyContextManager::Context type;
    QRegularExpression contextNameRegex;
};

class TestDefaultContextConfig : public ::testing::TestWithParam<TestContextConfigParams> {};

TEST_P(TestDefaultContextConfig, DefaultFunctionality)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    // check if the context object is valid
    ASSERT_TRUE(ctxObj);

    GTPY_GIL_SCOPE

    // QString contextName{PyModule_GetName(ctxObj.get())};

    // // check if the module name matches the expected pattern
    // EXPECT_TRUE(params.contextNameRegex.match(contextName).hasMatch());

    // check if the sys.modules entry is None
    // auto modulesDict = PyPPImport_GetModuleDict();

    // auto module = PyPPDict_GetItem(modulesDict,
    //                                contextName.toStdString().data());
    // EXPECT_TRUE(module.get() && module.get() == Py_None);

    // check if the context provides the projectPath() function
    auto projectPathFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::constants::PROJECT_PATH_F_NAME);

    ASSERT_TRUE(projectPathFunc);
    EXPECT_TRUE(PyPPCallable_Check(projectPathFunc));

    // check if the context provides the footprint() function
    auto footprintFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::constants::PROJECT_PATH_F_NAME);

    ASSERT_TRUE(footprintFunc);
    EXPECT_TRUE(PyPPCallable_Check(footprintFunc));

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
    // check if the context provides the shared functions API
    auto sharedFuncGetter = PyPPObject_GetAttr(
        ctxObj, gtpy::constants::SHARED_FUNC_F_NAME);

    ASSERT_TRUE(sharedFuncGetter);
    EXPECT_TRUE(PyPPCallable_Check(sharedFuncGetter));

    auto callSharedFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::constants::SHARED_FUNC_F_NAME);

    ASSERT_TRUE(callSharedFunc);
    EXPECT_TRUE(PyPPCallable_Check(callSharedFunc));

    auto sharedFuncIds = PyPPObject_GetAttr(
        ctxObj, gtpy::constants::SHARED_FUNC_IDS_F_NAME);

    ASSERT_TRUE(sharedFuncIds);
    EXPECT_TRUE(PyPPCallable_Check(sharedFuncIds));
#endif
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestDefaultContextConfig,
    ::testing::Values(
        TestContextConfigParams{GtpyContextManager::BatchContext,
                                QRegularExpression{"^BatchContext_\\d+$"}},
        TestContextConfigParams{GtpyContextManager::GlobalContext,
                                QRegularExpression{"^GlobalContext_\\d+$"}},
        TestContextConfigParams{GtpyContextManager::ScriptEditorContext,
                                QRegularExpression{"^ScriptEditorContext_\\d+$"}},
        TestContextConfigParams{GtpyContextManager::CalculatorRunContext,
                                QRegularExpression{"^CalculatorRunContext_\\d+$"}},
        TestContextConfigParams{GtpyContextManager::TaskEditorContext,
                                QRegularExpression{"^TaskEditorContext_\\d+$"}},
        TestContextConfigParams{GtpyContextManager::TaskRunContext,
                                QRegularExpression{"^TaskRunContext_\\d+$"}},
        TestContextConfigParams{GtpyContextManager::CollectionContext,
                                QRegularExpression{"^CollectionContext_\\d+$"}}
        )
    );

struct TestContextParams {
    GtpyContextManager::Context type;
};

class TestDefaultModulesProvided : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestDefaultModulesProvided, ProvidesDefaultModules)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check if the context provides the QtCore module
    auto qtCoreModule = PyPPObject_GetAttr(
        ctxObj, gtpy::code::modules::QTCORE);

    EXPECT_TRUE(qtCoreModule);
    EXPECT_TRUE(PyPPModule_Check(qtCoreModule));

    // check if the context provides the GtClasses module
    auto gtClassesModule = PyPPObject_GetAttr(
        ctxObj, gtpy::code::modules::GT_CLASSES);

    EXPECT_TRUE(gtClassesModule);
    EXPECT_TRUE(PyPPModule_Check(gtClassesModule));

    // check if the context provides the GtpyExtendedWrapper class
    auto extendedWrapperClass = PyPPObject_GetAttr(
        ctxObj, gtpy::code::classes::GTPY_EXTENDED_WRAPPER);

    EXPECT_TRUE(extendedWrapperClass);
    EXPECT_TRUE(PyType_Check(extendedWrapperClass.get()));
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestDefaultModulesProvided,
    ::testing::Values(
        TestContextParams{GtpyContextManager::BatchContext},
        TestContextParams{GtpyContextManager::GlobalContext},
        TestContextParams{GtpyContextManager::ScriptEditorContext},
        TestContextParams{GtpyContextManager::CalculatorRunContext},
        TestContextParams{GtpyContextManager::TaskEditorContext},
        TestContextParams{GtpyContextManager::TaskRunContext}
        )
    );

class TestDefaultModulesNotProvided : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestDefaultModulesNotProvided, DoesNotProvideDefaultModules)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check that the context not provides the QtCore module
    auto hasQtCoreModule = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::modules::QTCORE);

    EXPECT_FALSE(hasQtCoreModule);

    // check that the context not provides the GtClasses module
    auto hasGtClassesModule = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::modules::GT_CLASSES);

    EXPECT_FALSE(hasGtClassesModule);

    // check that the context not provides the GtpyExtendedWrapper class
    auto hasExtendedWrapperClass = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::classes::GTPY_EXTENDED_WRAPPER);

    EXPECT_FALSE(hasExtendedWrapperClass);
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestDefaultModulesNotProvided,
    ::testing::Values(
        TestContextParams{GtpyContextManager::CollectionContext}
        )
    );

class TestLoggingFuncsProvided : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestLoggingFuncsProvided, ProvidesLoggingFuncs)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check if the context provides the logging functions
    auto gtDebugFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::code::funcs::GT_DEBUG);

    ASSERT_TRUE(gtDebugFunc);
    EXPECT_TRUE(PyPPCallable_Check(gtDebugFunc));

    auto gtInfoFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::code::funcs::GT_INFO);

    ASSERT_TRUE(gtInfoFunc);
    EXPECT_TRUE(PyPPCallable_Check(gtInfoFunc));

    auto gtWaringFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::code::funcs::GT_WARNING);

    ASSERT_TRUE(gtWaringFunc);
    EXPECT_TRUE(PyPPCallable_Check(gtWaringFunc));

    auto gtErrorFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::code::funcs::GT_ERROR);

    ASSERT_TRUE(gtErrorFunc);
    EXPECT_TRUE(PyPPCallable_Check(gtErrorFunc));

    auto gtFatalFunc = PyPPObject_GetAttr(
        ctxObj, gtpy::code::funcs::GT_FATAL);

    ASSERT_TRUE(gtFatalFunc);
    EXPECT_TRUE(PyPPCallable_Check(gtFatalFunc));
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestLoggingFuncsProvided,
    ::testing::Values(
        TestContextParams{GtpyContextManager::BatchContext},
        TestContextParams{GtpyContextManager::GlobalContext},
        TestContextParams{GtpyContextManager::ScriptEditorContext},
        TestContextParams{GtpyContextManager::CalculatorRunContext},
        TestContextParams{GtpyContextManager::TaskEditorContext},
        TestContextParams{GtpyContextManager::TaskRunContext}
        )
    );

class TestLoggingFuncsNotProvided : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestLoggingFuncsNotProvided, DoesNotProvideLoggingFuncs)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check that the context does not provide the logging functions
    auto hasGtDebugFunc = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::funcs::GT_DEBUG);

    EXPECT_FALSE(hasGtDebugFunc);

    auto hasGtInfoFunc = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::funcs::GT_INFO);

    EXPECT_FALSE(hasGtInfoFunc);

    auto hasGtWaringFunc = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::funcs::GT_WARNING);

    EXPECT_FALSE(hasGtWaringFunc);

    auto hasGtErrorFunc = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::funcs::GT_ERROR);

    EXPECT_FALSE(hasGtErrorFunc);

    auto hasGtFatalFunc = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::funcs::GT_FATAL);

    EXPECT_FALSE(hasGtFatalFunc);
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestLoggingFuncsNotProvided,
    ::testing::Values(
        TestContextParams{GtpyContextManager::CollectionContext}
        )
    );

class TestLogToAppConsoleEnabled : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestLogToAppConsoleEnabled, LoggingIsEnabled)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check if logging to the application console is enabled
    auto logToAppConsole = PyPPObject_GetAttr(
        ctxObj, gtpy::code::attrs::LOGGING_ENABLED);

    ASSERT_TRUE(logToAppConsole);
    ASSERT_TRUE(PyPPBool_Check(logToAppConsole));

    EXPECT_EQ(logToAppConsole.get(), Py_True);
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestLogToAppConsoleEnabled,
    ::testing::Values(
        TestContextParams{GtpyContextManager::BatchContext},
        TestContextParams{GtpyContextManager::GlobalContext},
        TestContextParams{GtpyContextManager::CalculatorRunContext},
        TestContextParams{GtpyContextManager::TaskRunContext}
        )
    );

class TestLogToAppConsoleDisabled : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestLogToAppConsoleDisabled, LoggingIsDisabled)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check if logging to the application console is disabled
    auto hasLogToAppConsoleAttr = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::attrs::LOGGING_ENABLED);

    ASSERT_FALSE(hasLogToAppConsoleAttr);
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestLogToAppConsoleDisabled,
    ::testing::Values(
        TestContextParams{GtpyContextManager::ScriptEditorContext},
        TestContextParams{GtpyContextManager::TaskEditorContext},
        TestContextParams{GtpyContextManager::CollectionContext}
        )
    );

class TestGtCalculatorsProvided : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestGtCalculatorsProvided, ProvidesGtCalculators)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check if the context provides the GtCalculator module
    auto gtCalculatorsModule = PyPPObject_GetAttr(
        ctxObj, gtpy::code::modules::GT_CALCULATORS);

    ASSERT_TRUE(gtCalculatorsModule);
    EXPECT_TRUE(PyPPModule_Check(gtCalculatorsModule));
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestGtCalculatorsProvided,
    ::testing::Values(
        TestContextParams{GtpyContextManager::TaskEditorContext},
        TestContextParams{GtpyContextManager::TaskRunContext}
        )
    );

class TestGtCalculatorsNotProvided : public ::testing::TestWithParam<TestContextParams> {};

TEST_P(TestGtCalculatorsNotProvided , DoesNotProvideGtCalculators)
{
    const auto& params = GetParam();

    TestPythonContext context{params.type};
    auto ctxObj = context.obj();

    GTPY_GIL_SCOPE

    // check if the context provides the GtCalculator module
    auto gtCalculatorsModule = PyObject_HasAttrString(
        ctxObj.get(), gtpy::code::modules::GT_CALCULATORS);

    EXPECT_FALSE(gtCalculatorsModule);
}

INSTANTIATE_TEST_SUITE_P(
    ContextTypes,
    TestGtCalculatorsNotProvided ,
    ::testing::Values(
        TestContextParams{GtpyContextManager::BatchContext},
        TestContextParams{GtpyContextManager::GlobalContext},
        TestContextParams{GtpyContextManager::ScriptEditorContext},
        TestContextParams{GtpyContextManager::CalculatorRunContext},
        TestContextParams{GtpyContextManager::CollectionContext}
        )
    );

// TODO: Test that BatchContext and GlobalContext provide the functions
// openProject(), currentProject(), init() and switchSession().
