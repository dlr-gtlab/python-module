/* GTlab - Gas Turbine laboratory
 * Source File: urlresource.cpp
 *
 * SPDX-License-Identifier: Apache-2.0
 * SPDX-FileCopyrightText: 2025 German Aerospace Center (DLR)
 *
 * Created on: 10.11.2025
 * Author: Marvin Noethen (DLR AT-TWK)
 */

#include "gt/resource/data/urlresource.h"

#include <QRegularExpressionValidator>

#include <gt_stringproperty.h>

#include "gt/resource/url.h"

namespace gt
{

namespace resource
{

namespace data
{

struct UrlResource::Impl
{
    Impl(const QUrl& url) : urlProp{"URL", "URL", "", "",
                  new QRegularExpressionValidator{QRegularExpression{".*"}}}
    {
        urlProp = gt::resource::url::fullyEncoded(url);
    }

    GtStringProperty urlProp;
};

UrlResource::UrlResource(const QUrl& url) :
    m_pimpl(std::make_unique<Impl>(url))
{
    setUrlLocked(true);

    registerProperty(m_pimpl->urlProp);

    connect(&m_pimpl->urlProp, &GtStringProperty::changed, this,
            &UrlResource::urlChanged);
}

UrlResource::~UrlResource() = default;

QUrl
UrlResource::url() const
{
    return {m_pimpl->urlProp};
}

void
UrlResource::setUrl(const QUrl& url)
{
    QString encoded = gt::resource::url::fullyEncoded(url);
    if (m_pimpl->urlProp == encoded) return;

    m_pimpl->urlProp = encoded;
}

void
UrlResource::setUrlLocked(bool readOnly)
{
    m_pimpl->urlProp.setReadOnly(readOnly);
}

} // namespace data

} // namespace resource

} // namespace gt
