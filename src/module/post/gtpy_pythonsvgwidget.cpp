/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonsvgwidget.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QMouseEvent>
#include <QRubberBand>

#include <QSvgRenderer>

#include "gt_logging.h"

#include "gtpy_pythonsvgwidget.h"

GtpyPythonSvgWidget::GtpyPythonSvgWidget(QWidget* parent) : QSvgWidget(parent),
    m_rubberBand(Q_NULLPTR)
{

}

void
GtpyPythonSvgWidget::mousePressEvent(QMouseEvent* event)
{
    m_origin = event->pos();

    if (m_rubberBand == Q_NULLPTR)
    {
        m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    }

    m_rubberBand->setGeometry(QRect(m_origin, QSize()));
    m_rubberBand->show();

    QSvgWidget::mousePressEvent(event);
}

void
GtpyPythonSvgWidget::mouseMoveEvent(QMouseEvent* event)
{
    if (m_rubberBand != Q_NULLPTR)
    {
        m_rubberBand->setGeometry(QRect(m_origin, event->pos()).normalized());
    }

    QSvgWidget::mouseMoveEvent(event);
}

void
GtpyPythonSvgWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (m_rubberBand != Q_NULLPTR)
    {
        m_rubberBand->hide();

        gtDebug() << "rubberband size = " << m_rubberBand->size();
        gtDebug() << "rubberband position = " << m_rubberBand->pos();

        QSvgRenderer* rend = renderer();

        gtDebug() << "view box = " << rend->viewBox();
        gtDebug() << "size = " << size();

        double x_scale = double(size().width()) /
                         double(rend->defaultSize().width());
        double y_scale = double(size().height()) /
                         double(rend->defaultSize().height());

        gtDebug() << "x_scale = " << x_scale;
        gtDebug() << "y_scale = " << y_scale;

        double x_pos = m_rubberBand->pos().x() / x_scale;
        double y_pos = m_rubberBand->pos().y() / y_scale;
        double w =  m_rubberBand->size().width() / x_scale;
        double h =  m_rubberBand->size().height() / y_scale;

        QRectF newViewBox(x_pos, y_pos, w, h);

        gtDebug() << "new view box = " << newViewBox;

        rend->setViewBox(newViewBox);
        repaint();
    }

    QSvgWidget::mouseReleaseEvent(event);
}

bool
GtpyPythonSvgWidget::hasHeightForWidth() const
{
    return true;
}

int
GtpyPythonSvgWidget::heightForWidth(int w) const
{
    QSvgRenderer* rend = renderer();

    if (rend != Q_NULLPTR)
    {
        double ar = double(rend->defaultSize().height()) /
                    double(rend->defaultSize().width());

        return int(w * ar);
    }

    return QWidget::heightForWidth(w);
}
