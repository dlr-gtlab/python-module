/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_collectionitemwidget.h
 * copyright 2009-2019 by DLR
 *
 *  Created on: 16.09.2021
 *  Author: Marvin Noethen (AT-TW)
 *  Tel.: +49 2203 601 2692
 */

#ifndef GTPYCOLLECTIONITEMWIDGET_H
#define GTPYCOLLECTIONITEMWIDGET_H


#include <QDialog>

class QFrame;
class GtCollectionItem;

class GtpyCollectionItemWidget: public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE GtpyCollectionItemWidget(const GtCollectionItem& collectionItem,
                                         QWidget* parent = Q_NULLPTR);

    virtual ~GtpyCollectionItemWidget();
private:
    /// Filter string.
    QString m_filter;

    /**
     * @brief filterString
     * @param str
     * @return
     */
    QString filterString(const QString& str);

    /**
     * @brief newFrame
     * @return
     */
    QFrame* newFrame();

    /**
     * @brief newBox
     * @param title
     * @param val
     * @return
     */
    QFrame* newBox(const QString& title, const QString& content);

    /**
     * @brief newFileBox
     * @param fileNames
     * @return
     */
    QFrame* newFileBox(const QStringList& fileNames);

    /**
     * @brief newFileLayout
     * @param filename
     * @return
     */
    QLayout* newFileLayout(const QString& filename);
};

#endif // GTPYCOLLECTIONITEMWIDGET_H
