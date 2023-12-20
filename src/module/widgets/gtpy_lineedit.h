/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_lineedit.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 24.10.2023
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYLINEEDIT_H
#define GTPYLINEEDIT_H

#include "gt_globals.h"

#if GT_VERSION >= GT_VERSION_CHECK(2, 0, 0)
#include "gt_lineedit.h"
/**
 * @brief The GtpyLineEdit class
 */
class GtpyLineEdit : public GtLineEdit
{
    Q_OBJECT

public:
    /**
     * @brief GtpyLineEdit
     * @param parent
     */
    explicit GtpyLineEdit(QWidget* parent = nullptr);
};

#else
#include <QLineEdit>
/**
 * @brief The GtpyLineEdit class
 */
class GtpyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    /**
     * @brief GtpyLineEdit
     * @param parent
     */
    explicit GtpyLineEdit(QWidget* parent = nullptr);

protected:

    /**
     * @brief focusInEvent overloaded to emit the focusIn event
     * @param event
     */
    void focusInEvent(QFocusEvent* event) override;

    /**
     * @brief focusOutEvent overloaded to emit the focusOut events
     * @param event
     */
    void focusOutEvent(QFocusEvent* event) override;

signals:

    /**
     * @brief focusIn emitted once the lineedit receives the focus for input
     */
    void focusIn();

    /**
     * @brief focusOut emitted once the lineedit looses focus and contains any
     * text
     */
    void focusOut();

    /**
     * @brief clearFocusOut emitted once the lineedit looses focus and does not
     * contain any text
     */
    void clearFocusOut();
};

#endif



#endif // GTPYLINEEDIT_H
