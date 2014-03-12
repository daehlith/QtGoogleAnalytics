#ifndef QTGACONFIGURATION_H
#define QTGACONFIGURATION_H

#include "QtGA_global.h"

#include <QString>
#include <QUrl>
#include <QUuid>

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

#endif // QTGACONFIGURATION_H
