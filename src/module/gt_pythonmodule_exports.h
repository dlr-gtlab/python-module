/* GTlab - Gas Turbine laboratory
 * Source File: gt_pythonmodule_exports.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2019
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef GT_PYTHONMODULE_EXPORTS_H
#define GT_PYTHONMODULE_EXPORTS_H

#if defined(WIN32)
  #if defined (GT_PYTHON_DLL)
    #define GT_PYTHON_EXPORT __declspec (dllexport)
  #else
    #define GT_PYTHON_EXPORT __declspec (dllimport)
  #endif
#else
    #define GT_PYTHON_EXPORT
#endif

#endif // GT_PYTHONMODULE_EXPORTS_H
