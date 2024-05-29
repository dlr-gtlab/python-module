/* GTlab - Gas Turbine laboratory
 * Source File: gtpyscriptcollectionsettings.cpp
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 12.08.2021
 * Author: Marvin Noethen (AT-TW)
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


#if GT_VERSION >= 0x020000
void
GtpyScriptCollectionSettings::setBrowserWidgetData(
    GtAbstractBrowserWidget* widget,
    const QList<GtCollectionNetworkItem>& installedItems,
    const QList<GtCollectionNetworkItem>& availableItems,
    const QList<GtCollectionNetworkItem>& updataAvailableItems)
#else
void
GtpyScriptCollectionSettings::setBrowserWidgetData(
    GtAbstractBrowserWidget* widget,
    QList<GtCollectionNetworkItem> installedItems,
    QList<GtCollectionNetworkItem> availableItems,
    QList<GtCollectionNetworkItem> updataAvailableItems)
#endif
{
    GtpyCollectionBrowser* wid = qobject_cast<GtpyCollectionBrowser*>(widget);

    if (wid == Q_NULLPTR)
    {
        return;
    }

    wid->setItems(installedItems, availableItems, updataAvailableItems);
}
