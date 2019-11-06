/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotwidget.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#ifndef GTPY_PYTHONPLOTWIDGET_H
#define GTPY_PYTHONPLOTWIDGET_H

#include "gt_abstractpostwidget.h"

#include <QPointer>

class QLabel;
class GtpyPythonSvgWidget;
class GtpyPythonPlotItem;

/**
 * @brief The GtpyPythonPlotWidget class
 */
class GtpyPythonPlotWidget : public GtAbstractPostWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param dm Post datamodel item.
     * @param parent Parent widget.
     */
    GtpyPythonPlotWidget(GtpyPythonPlotItem* dm, QWidget* parent = Q_NULLPTR);


public slots:
    /**
     * @brief Overloaded function to initialize user interface.
     */
    void initFillingUI(GtObject* = Q_NULLPTR) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to create pdf plot.
     */
    void plotToPdf() Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to print post widget content.
     * @param printer
     * @param painter
     * @param rect
     */
    void printEmbedded(QPrinter* printer,
                       QPainter* painter,
                       QRectF rect) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to change post plot identification string.
     * @param s New identification string.
     */
    void changePlotName(QString s) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to return identification string of provider.
     * @return Provider identification string.
     */
    QString providerName() Q_DECL_OVERRIDE;

protected:

    /**
     * @brief dragEnterEvent
     * @param event
     */
    virtual void dragEnterEvent(QDragEnterEvent* event) Q_DECL_OVERRIDE;

    /**
     * @brief dropEvent
     * @param event
     */
    virtual void dropEvent(QDropEvent* event) Q_DECL_OVERRIDE;

private:
    /// comet post data
    QPointer<GtpyPythonPlotItem> m_dm;

    /// information label
    QLabel* m_labelStart;

    /// svg widget
    GtpyPythonSvgWidget* m_svgWid;

    /**
     * @brief Creates svg plot.
     */
    void createSvgPlot();

private slots:
    /**
     * @brief Overloaded function to open a configuration menu.
     */
    void showConfigurationMenu(GtObject*) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to define whether a configuration menu exsits.
     * @return
     */
    bool canShowConfigurationMenu(GtObject*) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to hide axis labels.
     */
    void hideAxisLabels(GtObject*) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to define whether functionality to hide axis
     * labels exists.
     * @return
     */
    bool canHideAxisLabels(GtObject*) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to show axis labels.
     */
    void showAxisLabels(GtObject*) Q_DECL_OVERRIDE;

    /**
     * @brief Overloaded function to define whether functionality to show axis
     * labels exists.
     * @return
     */
    bool canShowAxisLabels(GtObject*) Q_DECL_OVERRIDE;

};

#endif // GTPY_PYTHONPLOTWIDGET_H
