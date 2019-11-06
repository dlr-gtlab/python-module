/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonsvgwidget.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
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
    GtpyPythonSvgWidget(QWidget* parent = Q_NULLPTR);


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
