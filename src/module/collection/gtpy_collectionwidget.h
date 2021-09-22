/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionwidget.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 12.08.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONWIDGET_H
#define GTPYCOLLECTIONWIDGET_H

#include <QWidget>

class GtpyCollectionLocalWidget;
class GtCollectionItem;

class GtpyScriptEditor;

/**
 * @brief The GtpyCollectionWidget class
 */
class GtpyCollectionWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GtpyCollectionWidget
     * @param parentParent widget.
     */
    explicit GtpyCollectionWidget(QWidget* parent = Q_NULLPTR);

    /**
     * @brief Sets collection items list.
     * @param items Collection items list
     */
    void setItems(const QList<GtCollectionItem>& items);
private:
    ///default local widget
    GtpyCollectionLocalWidget* m_defaultLocalWidget;

    /// Python Editor
    GtpyScriptEditor* m_editor;
private slots:
    /**
     * @brief Gets the data of the select item and set them to the plot.
     * @param index Index of the item.
     */
    void onItemSelected(const QModelIndex& index);
};

#endif // GTPYCOLLECTIONWIDGET_H
