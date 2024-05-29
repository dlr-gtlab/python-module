/* GTlab - Gas Turbine laboratory
 * Source File: gtpy_objectmodel.h
 * 
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2024 German Aerospace Center (DLR)
 *
 * Created on: 20.12.2022
 * Author: Marvin Noethen (AT-TW)
 */

#ifndef GTPYOBJECTMODEL_H
#define GTPYOBJECTMODEL_H

#include "gt_pythonmodule_exports.h"

#include "gt_objectmodel.h"

class GT_PYTHON_EXPORT GtpyObjectModel : public GtObjectModel
{
    Q_OBJECT
public:
    explicit GtpyObjectModel(QObject* parent = nullptr);

    explicit GtpyObjectModel(GtObject* root, QObject* parent = nullptr);

    /**
    * @brief Sets the role data for the item at index to value.
    * @param Model index
    * @param Value
    * @param Data role
    * @return Returns true if successful; otherwise returns false.
    */
    virtual bool setData(const QModelIndex& index,
                         const QVariant& value,
                         int role = Qt::EditRole) override;

    /**
     * @brief Returns list of mime types.
     * @return List of mime types.
     */
    QStringList mimeTypes() const override;

    /**
     * @brief Returns an object that contains serialized items of data
     * corresponding to the list of indexes specified.
     * @param List of model indexes
     * @return MimeData
     */
    QMimeData* mimeData(const QModelIndexList& indexes) const override;

protected:
    /**
     * @brief Returns the actions supported by the data in this model.
     * @return Supported drag actions
     */
    Qt::DropActions supportedDragActions() const override;

private:
    /**
     * @brief Creates mime data based ob given object.
     * @param Object
     * @param Whether object should have a new uuid or not
     * @return Mime data of given object
     */
    QMimeData* mimeDataFromObject(GtObject* obj, bool newUuid = false) const;

private slots:
    /**
    * @brief Updates view.
    */
    void update();

signals:
    void processComponentRenamed(QString className, QString oldName,
                                 QString newName);
};

#endif // GTPYOBJECTMODEL_H
