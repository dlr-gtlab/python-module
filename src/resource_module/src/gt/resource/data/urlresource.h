/* GTlab - Gas Turbine laboratory
 * Source File: urlresource.h
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#ifndef URLRESOURCE_H
#define URLRESOURCE_H

#include <QUrl>

#include <gt_object.h>

namespace gt
{

namespace resource
{

namespace data
{

class UrlResource : public GtObject
{
    Q_OBJECT

public:
    explicit UrlResource(const QUrl& url);

    ~UrlResource() override;

    QUrl url() const;

protected:
    virtual QUrl toUrl(const QString& urlStr) const;

    void setUrlLocked(bool readOnly);

signals:
    void urlChanged();

private:
    struct Impl;
    std::unique_ptr<Impl> m_pimpl;
};

} // namespace data

} // namespace resource

} // namespace gt

#endif // URLRESOURCE_H
