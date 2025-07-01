/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_context.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 10.01.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GTPYCONTEXT_H
#define GTPYCONTEXT_H

#include <QString>

#include <gtpy_module.h>
#include <gt_pythonmodule_exports.h>

/**
 * @brief The GtpyContext class represents a C++ interface for a Python module
 * with restrictions and predefined configurations. Unlike a GtpyModule,
 * a GtpyContext cannot be imported by other Python modules. Additionally, it
 * provides predefined Python functions to interact with GTlab, such as opening
 * and modifying projects, sending logging messages to the application console
 * or executing tasks and calculators. The functionality provided by the context
 * depends on the ContextType passed to the constructor.
 */
class GT_PYTHON_EXPORT GtpyContext : public GtpyModule
{
    // The friend relationship allows to integrate GtpyContext into the
    // GtpyContextManager without modifying its API. It will probably be
    // removed in the next major version.
    friend class GtpyContextManager;

public:
    /**
     * @enum ContextType Enum.
     * @brief Specifies the predefined functionality of the GtpyContext
     * instance.
     *
     * The available types are:
     * - BatchContext:
     *   - Imports PythonQt, QCore, GtClasses and GtpyExtendedWrapper
     *   - Imports gtDebug(), gtInfo(), gtWarning(), gtError() and gtFatal()
     *     from GtLogging
     *   - Allows sending logging messages to the application console and the
     *     Python console
     *   - Provides the following methods to interact with GTlab:
     *     - openProject(projectName: str) -> Optional[GtpyExtendedWrapper]
     *     - currentProject() -> Optional[GtpyExtendedWrapper]
     *     - init(id: str = '')
     *     - switchSession(id: str = '')
     * - GlobalContext:
     *   - Same as BatchContext
     *   - Appends an empty string to sys.argv
     * - ScriptEditorContext:
     *   - Imports PythonQt, QCore, GtClasses and GtpyExtendedWrapper
     *   - Imports gtDebug(), gtInfo(), gtWarning(), gtError() and gtFatal()
     *     from GtLogging
     * - CalculatorRunContext:
     *   - Same as ScriptEditorContext
     *   - Allows sending logging messages to the application console and the
     *     Python console
     * - TaskEditorContext:
     *   - Imports PythonQt, QCore, GtClasses and GtpyExtendedWrapper
     *   - Imports gtDebug(), gtInfo(), gtWarning(), gtError() and gtFatal()
     *     from GtLogging
     *   - Imports Calculator classes and the findGtTask(taskName) function
     * - TaskRunContext:
     *   - Same as TaskEditorContext
     *   - Allows sending logging messages to the application console and the
     *     Python console
     * - DefaultContext:
     *   - Context without any special imports or predefined Python functions
     */
    enum ContextType
    {
        BatchContext = 0,
        GlobalContext,
        ScriptEditorContext,
        CalculatorRunContext,
        TaskEditorContext,
        TaskRunContext,
        DefaultContext
    };

    /**
     * @brief GtpyContext Constructor.
     * Creates a uniquely named Python module and removes it from the
     * sys.modules dictionary. The removal makes it impossible to import
     * the module into any other module. Additionally, it adds the following
     * functions, which are available by default in any context:
     *  - projectPath() -> str
     *  - footprint(only_active: bool = True) -> Dict[str, str]
     *
     * For GTlab >= 2.0.0, it also adds:
     *  - shared_function(module_id: str, function_id: str) -> Callable[[Any], Any]
     *  - call_shared_function(module_id: str, function_id: str, args: tuple) -> Tuple[Any, ...]
     *  - shared_function_ids() -> List[Dict[str, str]]
     *
     * @param type The ContextType that defines the functionality of this context.
     */
    explicit GtpyContext(ContextType type);
};

#endif // GTPYCONTEXT_H
