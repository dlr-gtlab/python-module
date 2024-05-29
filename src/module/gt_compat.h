/* GTlab - Gas Turbine laboratory
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 */

#ifndef GT_COMPAT_H
#define GT_COMPAT_H

#define GT_MODULENAME() GT_MODULE_ID

#if GT_VERSION < 0x020000
#define GT_IMPLEMENT_MODULE_ \
Q_INTERFACES(GtModuleInterface) \
    QString ident() const final { return GT_MODULENAME(); }

#define GT_MODULE_NO_JSON_() \
Q_PLUGIN_METADATA(IID GT_MODULE_ID)\
    GT_IMPLEMENT_MODULE_

#define GT_MODULE_WITH_JSON_(JSON_FILE) \
    Q_PLUGIN_METADATA(IID GT_MODULE_ID FILE JSON_FILE)\
    GT_IMPLEMENT_MODULE_

#define FUNC_CHOOSER(_f1, _f2, ...) _f2
#define FUNC_RECOMPOSER(argsWithParentheses) FUNC_CHOOSER argsWithParentheses
#define CHOOSE_FROM_ARG_COUNT(...) FUNC_RECOMPOSER((__VA_ARGS__, GT_MODULE_WITH_JSON_, ))
#define NO_ARG_EXPANDER() ,GT_MODULE_NO_JSON_
#define MACRO_CHOOSER(...) CHOOSE_FROM_ARG_COUNT(NO_ARG_EXPANDER __VA_ARGS__ ())

/**
 * To create a gtlab module, use this function at the beginning of the class
 *
 * Usage
 *
 * class MyModule : public QObject, public GtModuleInterface
 {
    Q_OBJECT

GT_MODULE()
// or GT_MODULE("my_module.json") // if you like to pass metadata to the module

*/
#define GT_MODULE(...) MACRO_CHOOSER(__VA_ARGS__)(__VA_ARGS__)

#endif // GTVERSION < 2.0

#endif // GT_COMPAT_H
