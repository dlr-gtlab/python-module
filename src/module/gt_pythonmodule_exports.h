/* GTlab - Gas Turbine laboratory
 * Source File: gt_pythonmodule_exports.h
 * copyright 2009-2018 by DLR
 *
 *  Created on: 12.08.2019
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
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
