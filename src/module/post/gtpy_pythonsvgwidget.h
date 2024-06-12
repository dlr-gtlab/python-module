/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonsvgwidget.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.08.2019
 * Author: Stanislaus Reitenbach (DLR AT-TWK)
 */

#ifndef GTPY_PYTHONSVGWIDGET_H
#define GTPY_PYTHONSVGWIDGET_H

#include <QSvgWidget>

class QRubberBand;

/**
 * @brief The GtpyPythonSvgWidget class
 */
class GtpyPythonSvgWidget : public QSvgWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent Parent widget.
     */
    explicit GtpyPythonSvgWidget(QWidget* parent = nullptr);


    /**
     * @brief mousePressEvent
     * @param event
     */
    virtual void mousePressEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    /**
     * @brief mouseMoveEvent
     * @param event
     */
    virtual void mouseMoveEvent(QMouseEvent* event) Q_DECL_OVERRIDE;

    /**
     * @brief mouseReleaseEvent
     * @param event
     */
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    /**
     * @brief hasHeightForWidth
     * @return
     */
    virtual bool hasHeightForWidth() const Q_DECL_OVERRIDE;

    /**
     * @brief Function to keep aspect ratio of widget (maybe depricated in
     * higher qt versions)
     * @param w
     * @return
     */
    virtual int heightForWidth(int w) const Q_DECL_OVERRIDE;

private:
    /// Rubber band
    QRubberBand* m_rubberBand;

    /// Rubber band origin
    QPoint m_origin;

};

#endif // GTPY_PYTHONSVGWIDGET_H
