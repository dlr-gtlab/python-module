/* GTlab - Gas Turbine laboratory
 * Source File: ResourcesModule
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 04.11.2025
 * Author: Marvin Nöthen (DLR AT-TWK)
 */

#include "gtest/gtest.h"

/// This is a test fixture that does a init for each test
class MyTest : public ::testing::Test
{
    void SetUp() override
    {

    }

    void TearDown() override
    {

    }
};

// example test
TEST_F(MyTest, test_0)
{
    EXPECT_TRUE(true);
}
