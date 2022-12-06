/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_pythonplotwidget.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 13.08.2019
 *  Author: Stanislaus Reitenbach (AT-TW)
 *  Tel.: +49 2203 601 2907
 */

#include <QHBoxLayout>
#include <QLabel>
#include <QDropEvent>
#include <QMimeData>
#include <QDir>
#include <QUuid>
#include <QSvgWidget>
#include <QSvgRenderer>
#include <QFile>

#include "gt_stylesheets.h"
#include "gtpy_contextmanager.h"
#include "gt_application.h"
#include "gt_logging.h"

#include "gtpy_pythonplotitem.h"
#include "gtpy_pythonplotdata.h"
#include "gtpy_pythonsvgwidget.h"
#include "gtpy_pythonplotconfigdialog.h"

#include "gtpy_pythonplotwidget.h"

GtpyPythonPlotWidget::GtpyPythonPlotWidget(GtpyPythonPlotItem* dm,
        QWidget* parent) :
    GtAbstractPostWidget(parent), m_dm(dm)
{
    setAcceptDrops(true);
    setObjectName("Python Post Plot");

    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);
    lay->setSpacing(0);

    m_labelStart = new QLabel(tr("Python Post Plot"));

    m_labelStart->setAlignment(Qt::AlignCenter);
    m_labelStart->setContentsMargins(0, 0, 0, 0);

#if GT_VERSION < 0x020000
    m_labelStart->setStyleSheet(GtStyleSheets::standardLabel());
#else
    m_labelStart->setStyleSheet(gt::gui::stylesheet::standardLabel());
#endif

    m_labelStart->setFixedHeight(300);
    QFont font;
    font.setPointSize(14);
    m_labelStart->setFont(font);

    lay->addWidget(m_labelStart);

    m_svgWid = new GtpyPythonSvgWidget;
    m_svgWid->hide();
    lay->addWidget(m_svgWid);

    setData(m_dm);

    setLayout(lay);

    adjustSize();

    // fill plot
    initFillingUI();
}


void
GtpyPythonPlotWidget::initFillingUI(GtObject*)
{
    setObjectName(m_dm->objectName());

    gtDebug() << "initializing plot...";

    createSvgPlot();
}

void
GtpyPythonPlotWidget::plotToPdf()
{

}

void
GtpyPythonPlotWidget::printEmbedded(QPrinter* /*printer*/,
                                    QPainter* /*painter*/,
                                    QRectF /*rect*/)
{

}

void
#if GT_VERSION < GT_VERSION_CHECK(2,0, 0)
GtpyPythonPlotWidget::changePlotName(QString s)
#else
GtpyPythonPlotWidget::changePlotName(const QString& s)
#endif
{
    m_dm->setObjectName(s);
}

QString
GtpyPythonPlotWidget::providerName()
{
    return m_dm->objectName();
}

void
GtpyPythonPlotWidget::dragEnterEvent(QDragEnterEvent* event)
{
    GtAbstractPostWidget::dragEnterEvent(event);
}

void
GtpyPythonPlotWidget::dropEvent(QDropEvent* event)
{
    GtAbstractPostWidget::dropEvent(event);
}

void
GtpyPythonPlotWidget::createSvgPlot()
{
    GtpyPythonPlotData* pData = m_dm->findDirectChild<GtpyPythonPlotData*>();

    if (pData == Q_NULLPTR)
    {
        return;
    }

    GtpyContextManager* python = GtpyContextManager::instance();

    QString script = pData->script();

    QDir tempDir = gtApp->applicationTempDir();

    QString figUuid = QUuid::createUuid().toString();
    QString figPath = tempDir.absoluteFilePath("fig_" + figUuid + ".svg");

    script = script.replace("$fig_path$", figPath);

    bool success = python->evalScript(GtpyContextManager::GlobalContext,
                                      script, true);

    gtDebug() << "python evaluation successful: " << success;

    if (QFile(figPath).exists())
    {
        m_labelStart->hide();
        m_svgWid->show();

        m_svgWid->load(figPath);

        //        QSvgRenderer* renderer = m_svgWid->renderer();

        //        gtDebug() << "default size = " << renderer->defaultSize();
        //        gtDebug() << "view box = " << renderer->viewBox();

        //        renderer->setViewBox(QRect(200,150, 200, 100));
    }
}

void
GtpyPythonPlotWidget::showConfigurationMenu(GtObject*)
{
    GtpyPythonPlotData* pData = m_dm->findDirectChild<GtpyPythonPlotData*>();

    GtpyPythonPlotConfigDialog dialog;
    dialog.resize(600, 400);

    if (pData != Q_NULLPTR)
    {
        dialog.setScript(pData->script());
    }

    dialog.exec();

    if (pData == Q_NULLPTR)
    {
        pData = new GtpyPythonPlotData;
        m_dm->appendChild(pData);
    }

    pData->setScript(dialog.pythonScript());

    createSvgPlot();
}

bool
GtpyPythonPlotWidget::canShowConfigurationMenu(GtObject*)
{
    if (m_dm == Q_NULLPTR)
    {
        return false;
    }

    return true;
}

void
GtpyPythonPlotWidget::hideAxisLabels(GtObject*)
{

}

bool
GtpyPythonPlotWidget::canHideAxisLabels(GtObject*)
{
    return false;
}

void
GtpyPythonPlotWidget::showAxisLabels(GtObject*)
{

}

bool
GtpyPythonPlotWidget::canShowAxisLabels(GtObject*)
{
    return false;
}
