/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotitem.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTPY_PYTHONPLOTITEM_H
#define GTPY_PYTHONPLOTITEM_H

#include "gt_abstractchartprovider.h"

/**
 * @brief The GtpyPythonPlotItem class
 */
class GtpyPythonPlotItem : public GtPostTemplateItem
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     */
    Q_INVOKABLE GtpyPythonPlotItem();

    /**
     * @brief Overloaded function to create user interface of post plot item.
     * @param dm Post datamodel item.
     * @param parent Parent widget.
     * @return Post widget.
     */
    GtAbstractPostWidget* createUI(GtPostTemplateItem* dm,
                                   QWidget* parent) Q_DECL_OVERRIDE;

};

#endif // GTPY_PYTHONPLOTITEM_H
