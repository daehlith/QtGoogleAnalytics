/*
 * Copyright (c) 2014 Thomas Daehling <doc@methedrine.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include "QtGA/QtGA.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

/*!
 * \brief The QtGA class allows an application to send information to a Google Analytics backend.
 */
/*!
 * \brief QtGA Constructs a QtGA object that is the center of the Qt Google Analytics integration and sets parent as the parent object.
 *
 * \note If parent is an instance of QNetworkAccessManager then this instance will be used for communicating with the Google Analytics endpoint.
 */
/*!
 * \brief QtGA Constructs a QtGA object that is the center of the Qt Google Analytics integration
 *
 * The configuration specified via config will be used for configuring Google Analytics endpoint, and the parent object will be
 * set to parent.
 *
 * \note If parent is an instance of QNetworkAccessManager then this instance will be used for communicating with the Google Analytics endpoint.
 */
/*!
 * \brief setNetworkAccessManager specifies the QNetworkAccessManager to use.
 * \note Since QtGA requires a valid QNetworkAccessManager instance this QtGA will be reparented to the specified instance.
 */
/*!
 * \brief networkAccessManager returns a pointer to the QNetworkAccessManager that is being used.
 */
/*!
 * \brief startSession indicates that the next tracking event is going to be the start of an analytics session.
 */

/*!
 * \brief endSession indicates that the next tracking event is going to be the end of an analytics session.
 */
/*!
 * \brief track Sends the provided event to the google analytics endpoint.
 *
 * \note The return value only indicates that an event was sent, not that it was successfully tracked on the Google Analytics backend.
 */

struct ParameterInfo
{
    QtGAHit::Parameter parameter;
    const char* textualRepresentation;
    QtGAValue::Type valueType;
    int maxLength;
    QtGAHit::Types supportedTypes;
    bool required;
};

// Need some LUTs for parameter validation:
// HitParameter | Textual Representation | ValueType | Max Length | Supported HitTypes | required
// TODO map required/optional flag with explicit checks?
ParameterInfo s_parameterInfo[] = {
    { QtGAHit::ProtocolVersion, "v", QtGAValue::Text, -1, QtGAHit::AllTypes, true },
    { QtGAHit::TrackingID, "tid", QtGAValue::Text, -1, QtGAHit::AllTypes, true },
    { QtGAHit::AnonymizeIP, "aip", QtGAValue::Boolean, -1, QtGAHit::AllTypes, false },
    { QtGAHit::QueueTime, "qt", QtGAValue::Integer, -1, QtGAHit::AllTypes, false },
    { QtGAHit::CacheBuster, "z", QtGAValue::Boolean, -1, QtGAHit::AllTypes, false },
    { QtGAHit::ClientID, "cid", QtGAValue::Text, -1, QtGAHit::AllTypes, true },
    { QtGAHit::SessionControl, "sc", QtGAValue::Text, -1, QtGAHit::AllTypes, false },
    { QtGAHit::DocumentReferrer, "dr", QtGAValue::Text, 2048, QtGAHit::AllTypes, false },
    { QtGAHit::CampaignName, "cn", QtGAValue::Text, 100, QtGAHit::AllTypes, false },
    { QtGAHit::CampaignSource, "cs", QtGAValue::Text, 100, QtGAHit::AllTypes, false },
    { QtGAHit::CampaignMedium, "cm", QtGAValue::Text, 50, QtGAHit::AllTypes, false },
    { QtGAHit::CampaignKeyword, "ck", QtGAValue::Text, 500, QtGAHit::AllTypes, false },
    { QtGAHit::CampaignContent, "cc", QtGAValue::Text, 500, QtGAHit::AllTypes, false },
    { QtGAHit::CampaignID, "ci", QtGAValue::Text, 100, QtGAHit::AllTypes, false },
    { QtGAHit::GoogleAdWordsID, "gclid", QtGAValue::Text, -1, QtGAHit::AllTypes, false },
    { QtGAHit::GoogleDisplayAdsID, "dclid", QtGAValue::Text, -1, QtGAHit::AllTypes, false },
    { QtGAHit::ScreenResolution, "sr", QtGAValue::Text, 20, QtGAHit::AllTypes, false },
    { QtGAHit::ViewportSize, "vp", QtGAValue::Text, 20, QtGAHit::AllTypes, false },
    { QtGAHit::DocumentEncoding, "de", QtGAValue::Text, 20, QtGAHit::AllTypes, false },
    { QtGAHit::ScreenColors, "sd", QtGAValue::Text, 20, QtGAHit::AllTypes, false },
    { QtGAHit::UserLanguage, "ul", QtGAValue::Text, 20, QtGAHit::AllTypes, false },
    { QtGAHit::JavaEnabled, "je", QtGAValue::Boolean, -1, QtGAHit::AllTypes, false },
    { QtGAHit::FlashVersion, "fl", QtGAValue::Text, 20, QtGAHit::AllTypes, false },
    { QtGAHit::HitType, "t", QtGAValue::Text, -1, QtGAHit::AllTypes, true }, // must be in QtGAHit::Types enum
    { QtGAHit::NonInteractionHit, "ni", QtGAValue::Boolean, -1, QtGAHit::AllTypes, false },
    { QtGAHit::DocumentLocationUrl, "dl", QtGAValue::Text, 2048, QtGAHit::AllTypes, false },
    { QtGAHit::DocumentHostName, "dh", QtGAValue::Text, 100, QtGAHit::AllTypes, false },
    { QtGAHit::DocumentPath, "dp", QtGAValue::Text, 2048, QtGAHit::AllTypes, false },
    { QtGAHit::DocumentTitle, "dt", QtGAValue::Text, 1500, QtGAHit::AllTypes, false },
    { QtGAHit::ContentDescription, "cd", QtGAValue::Text, 2048, QtGAHit::AllTypes, false },
    { QtGAHit::LinkID, "linkid", QtGAValue::Text, -1, QtGAHit::AllTypes, false },
    { QtGAHit::ApplicationName, "an", QtGAValue::Text, 100, QtGAHit::AllTypes, false },    // Only visible in app views (profiles)
    { QtGAHit::ApplicationVersion, "av", QtGAValue::Text, 100, QtGAHit::AllTypes, false }, // Only visible in app views (profiles)
    { QtGAHit::EventCategory, "ec", QtGAValue::Text, 150, QtGAHit::Event, false }, // must not be empty
    { QtGAHit::EventAction, "ea", QtGAValue::Text, 500, QtGAHit::Event, false },
    { QtGAHit::EventLabel, "el", QtGAValue::Text, 500, QtGAHit::Event, false },
    { QtGAHit::EventValue, "ev", QtGAValue::Integer, -1, QtGAHit::Event, false }, // must not be negative
    { QtGAHit::TransactionID, "ti", QtGAValue::Text, 500, QtGAHit::Transaction|QtGAHit::Item, true }, // value should be the same for transaction and item hits associated with it
    { QtGAHit::TransactionAffiliation, "ta", QtGAValue::Text, 500, QtGAHit::Transaction, false },
    { QtGAHit::TransactionRevenue, "tr", QtGAValue::Currency, -1, QtGAHit::Transaction, false }, // value should include shipping and tax costs
    { QtGAHit::TransactionShipping, "ts", QtGAValue::Currency, -1, QtGAHit::Transaction, false },
    { QtGAHit::TransactionTax, "tt", QtGAValue::Currency, -1, QtGAHit::Transaction, false },
    { QtGAHit::ItemName, "in", QtGAValue::Text, 500, QtGAHit::Item, true },
    { QtGAHit::ItemPrice, "ip", QtGAValue::Currency, -1, QtGAHit::Item, false }, // price per single item / unit
    { QtGAHit::ItemQuantity, "iq", QtGAValue::Integer, -1, QtGAHit::Item, false },
    { QtGAHit::ItemCode, "ic", QtGAValue::Text, 500, QtGAHit::Item, false }, // SKU / item code
    { QtGAHit::ItemCategory, "iv", QtGAValue::Text, 500, QtGAHit::Item, false },
    { QtGAHit::CurrencyCode, "cu", QtGAValue::Text, 10, QtGAHit::Item, false }, // ISO 4217 currency code
    { QtGAHit::SocialNetwork, "sn", QtGAValue::Text, 50, QtGAHit::Social, true },
    { QtGAHit::SocialAction, "sa", QtGAValue::Text, 50, QtGAHit::Social, true },
    { QtGAHit::SocialActionTarget, "st", QtGAValue::Text, 2048, QtGAHit::Social, true },
    { QtGAHit::UserTimingCategory, "utc", QtGAValue::Text, 150, QtGAHit::Timing, false },
    { QtGAHit::UserTimingVariableName, "utv", QtGAValue::Text, 500, QtGAHit::Timing, false },
    { QtGAHit::UserTimingTime, "utt", QtGAValue::Integer, -1, QtGAHit::Timing, false }, // in milliseconds!
    { QtGAHit::UserTimingLabel, "utl", QtGAValue::Text, 500, QtGAHit::Timing, false },
    { QtGAHit::PageLoadTime, "plt", QtGAValue::Integer, -1, QtGAHit::Timing, false }, // in milliseconds
    { QtGAHit::DnsTime, "dns", QtGAValue::Integer, -1, QtGAHit::Timing, false }, // in milliseconds
    { QtGAHit::PageDownloadTime, "pdt", QtGAValue::Integer, -1, QtGAHit::Timing, false }, // in milliseconds
    { QtGAHit::RedirectResponseTime, "rrt", QtGAValue::Integer, -1, QtGAHit::Timing, false }, // in milliseconds
    { QtGAHit::TcpConnectTime, "tcp", QtGAValue::Integer, -1, QtGAHit::Timing, false }, // in milliseconds
    { QtGAHit::ServerResponseTime, "srt", QtGAValue::Integer, -1, QtGAHit::Timing, false }, // in milliseconds
    { QtGAHit::ExceptionDescription, "exd", QtGAValue::Text, 150, QtGAHit::Exception, false },
    { QtGAHit::IsExceptionFatal, "exf", QtGAValue::Boolean, -1, QtGAHit::Exception, false },
    // FIXME: custom dimension / metric should be exposed differently!
    { QtGAHit::CustomDimension, "cd[1-9][0-9]*", QtGAValue::Text, 150, QtGAHit::AllTypes, false }, // max 20 (Premium: 200), name suffix must be positive integer between 1 and 200 incl.
    { QtGAHit::CustomMetric, "cm[1-9][0-9]*", QtGAValue::Integer, -1, QtGAHit::AllTypes, false }, // max 20 (Premium: 200), name suffix must be positive integer between 1 and 200 incl.
    // FIXME: experiment ID and variant should be exposed differently!
//    { QtGAHit::ExperimentID, "xid", QtGAValue::Text, 40, QtGAHit::AllTypes, false }, // should be in conjunction with ExperimentVariant
//    { QtGAHit::ExperimentVariant, "xvar", QtGAValue::Text, -1, QtGAHit::AllTypes, false }
};


QtGA::QtGA(QObject *parent) : QObject(parent), m_session(NoSession)
{
    QNetworkAccessManager* obj = qobject_cast<QNetworkAccessManager*>(parent);
    if (obj)
    {
        m_nam = obj;
    }
}

void QtGA::setNetworkAccessManager(QNetworkAccessManager *manager)
{
    if ( manager )
    {
        setParent(manager);
        m_nam = manager;
    }
}

const QNetworkAccessManager* QtGA::networkAccessManager() const
{
    return m_nam;
}

void QtGA::startSession()
{
    m_session = StartSession;
}

void QtGA::endSession()
{
    m_session = EndSession;
}

//QtGA::AnalyticsError QtGA::track(HitTypes type, const QVariantMap &parameters, bool interactive)
//{
//    if ( m_nam == nullptr )
//    {
//        return ErrorNoNetworkManager;
//    }

//    QNetworkRequest req(m_config.endpoint());
//    req.setHeader(QNetworkRequest::UserAgentHeader, m_config.userAgent());
//    QUrlQuery query;
//    QByteArray data = query.toString(QUrl::FullyEncoded).toLatin1();
//    m_nam->post( req, data );

//    return NoError;
//}

/*!
 * \brief setConfiguration sets the configuration to use.
 */
/*!
 * \brief configuration returns the QtGAConfiguration that will be used when communicating with the Google Analytics endpoint.
 */
QtGAHit::QtGAHit(const QtGAConfiguration &config) : m_config(config)
{
}

void QtGAHit::setConfiguration(const QtGAConfiguration &config)
{
    m_config = config;
}

QtGAConfiguration QtGAHit::configuration() const
{
    return m_config;
}

void QtGAHit::setExperiment(const QString& id, const QString& variant)
{
    // FIXME validation!
    m_parameters[QtGAHit::ExperimentID] = id;
    m_parameters[QtGAHit::ExperimentVariant] = variant;
}

QString QtGAHit::experimentID() const
{
    QString ret;
    if (m_parameters.contains(QtGAHit::ExperimentID))
    {
        ret = m_parameters[QtGAHit::ExperimentID].toString();
    }
    return ret;
}

QString QtGAHit::experimentVariant() const
{
    QString ret;
    if (m_parameters.contains(QtGAHit::ExperimentVariant))
    {
        ret = m_parameters[QtGAHit::ExperimentVariant].toString();
    }
    return ret;
}

void QtGAHit::addParameter(QtGAHit::Parameter parameter, const QtGAValue& value)
{
    // FIXME validation!
    m_parameters[parameter] = value;
}

QtGAHit::ParameterMap QtGAHit::parameters() const
{
    return m_parameters;
}
