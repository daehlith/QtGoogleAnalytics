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

#include <QObject>
#include <QString>
#include <QUrl>
#include <QUuid>
#include <QVariantMap>

#ifdef Q_OS_WIN
    #ifdef BUILD_SHARED
        #ifdef QT_GA_BUILD
            #define QT_GA_EXPORTS Q_DECL_EXPORT
        #else
            #define QT_GA_EXPORTS Q_DECL_IMPORT
        #endif
    #else
        #define QT_GA_EXPORTS
    #endif
#else
    #define QT_GA_EXPORTS
#endif

class QNetworkAccessManager;

/*!
 * \brief The QtGAConfiguration class holds the configuration for a QtGA connection.
 *
 * QtGAConfiguration is used by QtGA to communicate with the Google Analytics backend.
 */
class QT_GA_EXPORTS QtGAConfiguration
{
public:
    /*!
     * \brief NORMAL_ENDPOINT is a constant pointing to the default unsecure endpoint for Google Analytics.
     */
    static const QUrl NORMAL_ENDPOINT;
    /*!
     * \brief SECURE_ENDPOINT is a constant pointing to the default SSL encrypted endpoint for Google Analytics.
     */
    static const QUrl SECURE_ENDPOINT;
    /*!
     * \brief USERAGENT is the default user-agent string used by QtGA.
     */
    static const QString USERAGENT;

    /*!
     * \brief The RequestType enum determines the HTTP request type used by QtGA
     *
     * By default QtGAConfiguration is using the HTTP POST method for communicating with Google Analytics.
     */
    enum RequestType {
        PostRequest = 0,
        GetRequest,
        DefaultRequest = PostRequest,
    };

    /*!
     * \brief The ProtocolVersion enum provides symbolic synonyms for Google Analytics protocol version numbers
     */
    enum Version {
        QtGA_1_0 = 1,
    };

    QtGAConfiguration();

    /*!
     * \brief setCacheBusting specifies whether the QtGA backend should use cache busting.
     *
     * Cache busting adds a random parameter to GET requests
     *
     * \note This option has no effect when using POST requests
     */
    void setCacheBusting(bool enabled);

    /*!
     * \brief cacheBusting indicates whether cache busting will be applied.
     */
    bool cacheBusting() const;

    /*!
     * \brief setTrackingID sets the Google Analytics tracking ID to use.
     *
     * \note In case of an invalid tracking ID being passed in the configuration will not have a tracking ID at all.
     */
    void setTrackingID(const QString& trackingID);

    /*!
     * \brief trackingID returns the Google Analytics trackingID that is being used.
     */
    const QString& trackingID() const;

    /*!
     * \brief isSecure indicates whether the specified endpoint uses the HTTPS protocol.
     * \note This function will always return false if Qt was built without SSL support.
     */
    bool isSecure() const;

    /*!
     * \brief setEndpoint specifies the URL to the Google Analytics endpoint to use.
     *
     * By default QtGA will use the endpoint specified by QtGAConfiguration::NORMAL_ENDPOINT.
     */
    void setEndpoint(const QUrl& endpoint);
    const QUrl& endpoint() const;

    /*!
     * \brief setUserAgent sets the user-agent string to use when communicating with Google Analytics
     *
     * By default QtGAConfiguration::USERAGENT is being used
     */
    void setUserAgent(const QString& useragent);

    /*!
     * \brief userAgent returns the user-agent that is being used when communicating with Google Analytics
     */
    QString userAgent() const;

    /*!
     * \brief setAnonymized enables IP anonymization.
     *
     * When present, the IP address of the sender will be anonymized.
     */
    void setIPAnonymization(bool anonymous);

    /*!
     * \brief anonymized Indicates if IP anonymization is used when communicating with Google Analytics.
     */
    bool anonymizeIP() const;

    /*!
     * \brief setClientID sets the globally unique clientID to use with Google Analytics for this run.
     * This anonymously identifies a particular user, device, or browser instance. For the web, this is generally stored as a first-party
     * cookie with a two-year expiration. For mobile apps, this is randomly generated for each particular instance of an application install.
     * The value of this field should be a random UUID (version 4) as described in http://www.ietf.org/rfc/rfc4122.txt
     */
    void setClientID(const QUuid& clientID);

    /*!
     * \brief clientID returns the UUID
     */
    QUuid clientID() const;

private:
    QString m_trackingID;
    QUrl m_endpoint;
    RequestType m_requestType;
    QString m_userAgent;
    bool m_cacheBusting;
    bool m_anonymizeIP;
    QUuid m_clientID;
};

/*!
 * \brief The QtGA class allows an application to send information to a Google Analytics backend.
 */
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

    /*!
     * \brief QtGA Constructs a QtGA object that is the center of the Qt Google Analytics integration and sets parent as the parent object.
     *
     * \note If parent is an instance of QNetworkAccessManager then this instance will be used for communicating with the Google Analytics endpoint.
     */
    explicit QtGA(QObject* parent=nullptr);

    /*!
     * \brief QtGA Constructs a QtGA object that is the center of the Qt Google Analytics integration
     *
     * The configuration specified via config will be used for configuring Google Analytics endpoint, and the parent object will be
     * set to parent.
     *
     * \note If parent is an instance of QNetworkAccessManager then this instance will be used for communicating with the Google Analytics endpoint.
     */
    QtGA(const QtGAConfiguration& config, QObject *parent=nullptr);

    /*!
     * \brief setConfiguration sets the configuration to use.
     */
    void setConfiguration(const QtGAConfiguration& config);
    /*!
     * \brief configuration returns the QtGAConfiguration that will be used when communicating with the Google Analytics endpoint.
     */
    QtGAConfiguration configuration();

    /*!
     * \brief setNetworkAccessManager specifies the QNetworkAccessManager to use.
     * \note Since QtGA requires a valid QNetworkAccessManager instance this QtGA will be reparented to the specified instance.
     */
    void setNetworkAccessManager(QNetworkAccessManager* manager);

    /*!
     * \brief networkAccessManager returns a pointer to the QNetworkAccessManager that is being used.
     */
    const QNetworkAccessManager* networkAccessManager() const;

    /*!
     * \brief startSession indicates that the next tracking event is going to be the start of an analytics session.
     */
    void startSession();

    /*!
     * \brief endSession indicates that the next tracking event is going to be the end of an analytics session.
     */
    void endSession();

    /*!
     * \brief track Sends the provided event to the google analytics endpoint.
     *
     * \note The return value only indicates that an event was sent, not that it was successfully tracked on the Google Analytics backend.
     */
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
