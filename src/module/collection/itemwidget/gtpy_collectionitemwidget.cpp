/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionitemwidget.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include <QVBoxLayout>
#include <QFrame>
#include <QTabWidget>
#include <QScrollArea>
#include <QLabel>
#include <QMainWindow>
#include <QApplication>

#include "gt_collectionitem.h"

#include "gtpy_stylesheet_compat.h"
#include "gtpy_icons_compat.h"

#include "gtpy_collectionitemwidget.h"

GtpyCollectionItemWidget::GtpyCollectionItemWidget(const
        GtCollectionItem& collectionItem, QWidget* parent) : QWidget(parent)
{
    setAttribute(Qt::WA_DeleteOnClose);

    foreach (QWidget* wid, QApplication::topLevelWidgets())
    {
        QMainWindow* mainWin = qobject_cast<QMainWindow*>(wid);

        if (mainWin)
        {
            setWindowModality(Qt::NonModal);
            setParent(mainWin);
            setWindowFlags(Qt::Dialog
                           | Qt::WindowCloseButtonHint);
            break;
        }
    }

    setWindowTitle(collectionItem.ident());
    setWindowIcon(GTPY_ICON(info));

    QVBoxLayout* lay = new QVBoxLayout;
    lay->setContentsMargins(0, 0, 0, 0);

    QFrame* frame = new QFrame;
    frame->setObjectName(QStringLiteral("frame"));
    frame->setAutoFillBackground(true);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Raised);
    frame->setStyleSheet(GTPY_STYLESHEET(backgroundFrame));

    QVBoxLayout* frameLay = new QVBoxLayout;
    frameLay->setContentsMargins(10, 10, 10, 10);

    QTabWidget* tabWidget = new QTabWidget;

    QWidget* overviewWidget = new QWidget;

    QVBoxLayout* overviewLay = new QVBoxLayout;

    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setStyleSheet(
        "QAbstractScrollArea { background-color: transparent; } "
        "QWidget#scrollAreaWidgetContents{"
        " background-color: transparent;} ");
    scrollArea->setFrameStyle(QScrollArea::NoFrame);
    scrollArea->setWidgetResizable(true);

    QWidget* scrollWidget = new QWidget;
    //scrollWidget->setStyleSheet("background-color:white;");

    QVBoxLayout* scrollLay = new QVBoxLayout;
    scrollLay->setContentsMargins(0, 0, 0, 0);

    // title widget
    QFrame* titleWidget = newBox(tr("Name:"),
                                 collectionItem.ident());
    scrollLay->addWidget(titleWidget);

    // description widget
    QFrame* descWidget = newBox(tr("Description:"),
                                collectionItem.description());
    scrollLay->addWidget(descWidget);

    // version widget
    QFrame* versWidget = newBox(tr("Version:"),
                                QString::number(collectionItem.version()));
    scrollLay->addWidget(versWidget);

    // collection specific properties
    foreach (const QString& str, collectionItem.propertyIds())
    {
        QFrame* specWidget = newBox(str,
                                    collectionItem.property(str).toString());
        scrollLay->addWidget(specWidget);
    }

    // files widget
    QFrame* filesWidget = newFileBox(collectionItem.files());
    scrollLay->addWidget(filesWidget);

    scrollLay->addSpacerItem(new QSpacerItem(10, 20, QSizePolicy::Minimum,
                             QSizePolicy::Expanding));

    scrollWidget->setLayout(scrollLay);
    scrollArea->setWidget(scrollWidget);

    overviewLay->addWidget(scrollArea);

    overviewWidget->setLayout(overviewLay);

    tabWidget->addTab(overviewWidget, GTPY_ICON(list), tr("Overview"));

    QWidget* docWidget = new QWidget;

    tabWidget->addTab(docWidget, GTPY_ICON(questionmark), tr("Documentation"));

    tabWidget->setTabEnabled(1, false);

    frameLay->addWidget(tabWidget);

    frame->setLayout(frameLay);

    lay->addWidget(frame);

    setLayout(lay);

    resize(600, 400);
}

QString
GtpyCollectionItemWidget::filterString(const QString& str)
{
    if (m_filter.isEmpty())
    {
        return str;
    }

    QString retval = str;
    QString newStr = QStringLiteral("<font color=red>") + m_filter +
                     QStringLiteral("</font>");

    retval = retval.replace(m_filter, newStr);

    return retval;
}

QFrame*
GtpyCollectionItemWidget::newFrame()
{
    QFrame* frame = new QFrame(this);

    frame->setAutoFillBackground(true);
    QPalette p = frame->palette();
    //p.setColor(frame->backgroundRole(), Qt::white);
    frame->setPalette(p);
    frame->setFrameShape(QFrame::Box);
    frame->setFrameShadow(QFrame::Sunken);

    return frame;
}

QFrame*
GtpyCollectionItemWidget::newBox(const QString& title, const QString& content)
{
    QFrame* box = newFrame();

    QVBoxLayout* boxLayout = new QVBoxLayout;

    QLabel* boxTitle = new QLabel(QStringLiteral("<b>") + title +
                                  QStringLiteral("</b>"));
    QLabel* boxContent = new QLabel(filterString(content));
    boxLayout->addWidget(boxTitle);
    boxLayout->addWidget(boxContent);

    box->setLayout(boxLayout);

    return box;
}

QFrame*
GtpyCollectionItemWidget::newFileBox(const QStringList& fileNames)
{
    QFrame* box = newFrame();

    QVBoxLayout* boxLayout = new QVBoxLayout;

    QLabel* boxTitle = new QLabel(QStringLiteral("<b>") + tr("Files") +
                                  QStringLiteral("</b>"));
    boxLayout->addWidget(boxTitle);

    foreach (const QString& fileName, fileNames)
    {
        QLayout* fileLay = newFileLayout(fileName);
        boxLayout->addLayout(fileLay);
    }

    box->setLayout(boxLayout);

    return box;
}

QLayout*
GtpyCollectionItemWidget::newFileLayout(const QString& filename)
{
    QHBoxLayout* fileLay = new QHBoxLayout;
    QLabel* iconLabel = new QLabel;
    iconLabel->setMaximumWidth(16);
    iconLabel->setPixmap(GTPY_ICON(file).pixmap(QSize(16, 16)));
    QLabel* fileLabel = new QLabel(filename);
    fileLay->addWidget(iconLabel);
    fileLay->addWidget(fileLabel);

    return fileLay;
}
