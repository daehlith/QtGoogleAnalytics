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

#ifndef QTGA_H
#define QTGA_H

#include "QtGA_global.h"
#include "QtGAConfiguration.h"

#include <QObject>

class QNetworkAccessManager;

class QT_GA_EXPORTS QtGA : public QObject
{
    Q_OBJECT

    Q_ENUMS(HitTypes)
    Q_ENUMS(HitParameter)
    Q_ENUMS(ParameterType)
    Q_ENUMS(AnalyticsError)

public:

    // Need to support the supported datatypes somehow:
    // Text = QString
    // Currency = QtGACurrency? QString with a validator?
    // Boolean = bool
    // Integer = qint64

    // Need some LUTs for parameter validation:
    // HitParameter | Textual Representation | ValueType | Default Value | Max Length | Supported HitTypes

    enum HitTypes
    {
        PageView = 1<<0,
        AppView = 1<<1,
        Event = 1<<2,
        Transaction = 1<<3,
        Item = 1<<4,
        Social = 1<<5,
        Exception = 1<<6,
        Timing = 1<<7,
    };

    enum HitParameter
    {
        // General
        ProtocolVersion = 0,
        TrackingID,
        WebPropertyID = TrackingID,
        AnonymizeIP,
        QueueTime,
        CacheBuster,
        // Visitor
        ClientID,
        // Session
        SessionControl,
        // Traffic Sources
        DocumentReferrer,
        CampaignName,
        CampaignSource,
        CampaignMedium,
        CampaignKeyword,
        CampaignContent,
        CampaignID,
        GoogleAdWordsID,
        GoogleDisplayAdsID,
        // System Info
        ScreenResolution,
        ViewportSize,
        DocumentEncoding,
        ScreenColors,
        UserLanguage,
        JavaEnabled,
        FlashVersion,
        // Hit
        HitType,
        NonInteractionHit,
        // Content Information
        DocumentLocationUrl,
        DocumentHostName,
        DocumentPath,
        DocumentTitle,
        ContentDescription,
        LinkID,
        // App Tracking
        ApplicationName,
        ApplicationVersion,
        // Event Tracking
        EventCategory,
        EventAction,
        EventLabel,
        EventValue,
        // E-Commerce
        TransactionID,
        TransactionAffiliation,
        TransactionRevenue,
        TransactionShipping,
        TransactionTax,
        ItemName,
        ItemPrice,
        ItemQuantity,
        ItemCode,
        ItemCategory,
        CurrencyCode,
        // Social Interactions
        SocialNetwork,
        SocialAction,
        SocialActionTarget,
        // Timing
        UserTimingCategory,
        UserTimingVariableName,
        UserTimingTime,
        UserTimingLabel,
        PageLoadTime,
        DnsTime,
        PageDownloadTime,
        RedirectResponseTime,
        TcpConnectTime,
        ServerResponseTime,
        // Exceptions
        ExceptionDescription,
        IsExceptionFatal,
        // Custom Dimensions/Metrics
        CustomDimension,
        CustomMetric,
        // Content Experiments
        ExperimentID,
        ExperimentVariant,
    };

    enum ParameterType
    {
        Text = 0,
        Currency,
        Boolean,
        Integer
    };

    enum AnalyticsError
    {
        NoError = 0,
        ErrorNoNetworkManager,
        ErrorMissingRequiredParameter,
        ErrorPayloadTooLarge,
    };

    explicit QtGA(QObject* parent=nullptr);

    QtGA(const QtGAConfiguration& config, QObject *parent=nullptr);

    void setConfiguration(const QtGAConfiguration& config);
    QtGAConfiguration configuration();

    void setNetworkAccessManager(QNetworkAccessManager* manager);
    const QNetworkAccessManager* networkAccessManager() const;

    void startSession();
    void endSession();

    AnalyticsError track(HitTypes type, const QVariantMap& parameters, bool interactive = false);

private:

    enum Session
    {
        NoSession = 0,
        StartSession,
        EndSession,
    };

    QtGAConfiguration m_config;
    QNetworkAccessManager* m_nam;
    Session m_session;
};

Q_DECLARE_METATYPE(QtGA::AnalyticsError)

#endif // QTGA_H
