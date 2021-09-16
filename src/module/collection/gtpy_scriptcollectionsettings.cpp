/* GTlab - Gas Turbine laboratory
 * Source File: gtpyscriptcollectionsettings.cpp
 * copyright 2009-2019 by DLR
 *
 *  Created on: 12.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#include "gt_collectionitem.h"
#include "gt_localcollectionwidget.h"

#include "gtpy_collectionwidget.h"
#include "gtpy_collectionbrowser.h"

#include "gtpy_scriptcollectionsettings.h"

GtpyScriptCollectionSettings::GtpyScriptCollectionSettings()
{

}

QWidget*
GtpyScriptCollectionSettings::localCollectionWidget(QWidget* parent)
{
    return new GtpyCollectionWidget(parent);
}

void
GtpyScriptCollectionSettings::setLocalCollectionData(QWidget* widget,
        QList<GtCollectionItem> items)
{
    GtpyCollectionWidget* localWidget = dynamic_cast<GtpyCollectionWidget*>(
                                            widget);

    if (localWidget == Q_NULLPTR)
    {
        return;
    }

    localWidget->setItems(items);
}

GtAbstractBrowserWidget*
GtpyScriptCollectionSettings::browserWidget(QWidget* parent)
{
    return new GtpyCollectionBrowser(parent);
}

void
GtpyScriptCollectionSettings::setBrowserWidgetData(
    GtAbstractBrowserWidget* widget,
    QList<GtCollectionNetworkItem> installedItems,
    QList<GtCollectionNetworkItem> availableItems,
    QList<GtCollectionNetworkItem> updataAvailableItems)
{
    GtpyCollectionBrowser* wid = qobject_cast<GtpyCollectionBrowser*>(widget);

    if (wid == Q_NULLPTR)
    {
        return;
    }

    wid->setItems(installedItems, availableItems, updataAvailableItems);
}
