/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotwidget.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 13.08.2019
 * Author: Stanislaus Reitenbach (DLR AT-TWK)
 */

#ifndef GTPY_PYTHONPLOTWIDGET_H
#define GTPY_PYTHONPLOTWIDGET_H

#include "gt_abstractpostwidget.h"
#include "gt_globals.h"

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
    explicit GtpyPythonPlotWidget(GtpyPythonPlotItem* dm,
                                  QWidget* parent = nullptr);


public slots:
    /**
     * @brief Overloaded function to initialize user interface.
     */
    void initFillingUI(GtObject* = nullptr) override;

    /**
     * @brief Overloaded function to create pdf plot.
     */
    void plotToPdf() override;

    /**
     * @brief Overloaded function to print post widget content.
     * @param printer
     * @param painter
     * @param rect
     */
    void printEmbedded(QPrinter* printer,
                       QPainter* painter,
                       QRectF rect) override;

    /**
     * @brief Overloaded function to change post plot identification string.
     * @param s New identification string.
     */

#if GT_VERSION < GT_VERSION_CHECK(2,0, 0)
    void changePlotName(QString s) override;
#else
    void changePlotName(const QString& s) override;
#endif

    /**
     * @brief Overloaded function to return identification string of provider.
     * @return Provider identification string.
     */
    QString providerName() override;

protected:

    /**
     * @brief dragEnterEvent
     * @param event
     */
    void dragEnterEvent(QDragEnterEvent* event) override;

    /**
     * @brief dropEvent
     * @param event
     */
    void dropEvent(QDropEvent* event) override;

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
    void showConfigurationMenu(GtObject*) override;

    /**
     * @brief Overloaded function to define whether a configuration menu exsits.
     * @return
     */
    bool canShowConfigurationMenu(GtObject*) override;

    /**
     * @brief Overloaded function to hide axis labels.
     */
    void hideAxisLabels(GtObject*) override;

    /**
     * @brief Overloaded function to define whether functionality to hide axis
     * labels exists.
     * @return
     */
    bool canHideAxisLabels(GtObject*) override;

    /**
     * @brief Overloaded function to show axis labels.
     */
    void showAxisLabels(GtObject*) override;

    /**
     * @brief Overloaded function to define whether functionality to show axis
     * labels exists.
     * @return
     */
    bool canShowAxisLabels(GtObject*) override;

};

#endif // GTPY_PYTHONPLOTWIDGET_H
