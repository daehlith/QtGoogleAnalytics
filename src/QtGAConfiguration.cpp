#include <QtGA/QtGAConfiguration.h>

#include <QtCore/QRegExp>

const QUrl QtGAConfiguration::SecureEndpoint = QUrl("https://ssl.google-analytics.com/collect");
const QUrl QtGAConfiguration::NormalEndpoint = QUrl("https//www.google-analytics.com/collect");
const QString QtGAConfiguration::UserAgent = QString("QtGA/1.0");

/*!
 * \brief The QtGAConfiguration class holds the configuration for a QtGA connection.
 *
 * QtGAConfiguration is used by QtGA to communicate with the Google Analytics backend.
 */
/*!
 * \brief NormalEndpoint is a constant pointing to the default unsecure endpoint for Google Analytics.
 */
/*!
 * \brief SecureEndpoint is a constant pointing to the default SSL encrypted endpoint for Google Analytics.
 */
/*!
 * \brief UserAgent is the default user-agent string used by QtGA.
 */
/*!
 * \brief The RequestType enum determines the HTTP request type used by QtGA
 *
 * By default QtGAConfiguration is using the HTTP POST method for communicating with Google Analytics.
 */
/*!
 * \brief The Version enum provides symbolic synonyms for Google Analytics protocol version numbers
 */
/*!
 * \brief setCacheBusting specifies whether the QtGA backend should use cache busting.
 *
 * Cache busting adds a random parameter to GET requests
 *
 * \note This option has no effect when using POST requests
 */

/*!
 * \brief cacheBusting indicates whether cache busting will be applied.
 */
/*!
 * \brief setTrackingID sets the Google Analytics tracking ID to use.
 *
 * \note In case of an invalid tracking ID being passed in the configuration will not have a tracking ID at all.
 */

/*!
 * \brief trackingID returns the Google Analytics trackingID that is being used.
 */
/*!
 * \brief isSecure indicates whether the specified endpoint uses the HTTPS protocol.
 * \note This function will always return false if Qt was built without SSL support.
 */
/*!
 * \brief setEndpoint specifies the URL to the Google Analytics endpoint to use.
 *
 * By default QtGA will use the endpoint specified by QtGAConfiguration::NORMAL_ENDPOINT.
 *
 * \note If Qt was built without SSL support then https URLs cannot be used.
 */
/*!
 * \brief endpoint Returns the configured Google Analytics endpoint.
 */
/*!
 * \brief setUserAgent sets the user-agent string to use when communicating with Google Analytics
 *
 * By default QtGAConfiguration::USERAGENT is being used
 */
/*!
 * \brief userAgent returns the user-agent that is being used when communicating with Google Analytics
 */
/*!
 * \brief setAnonymized enables IP anonymization.
 *
 * When present, the IP address of the sender will be anonymized.
 */
/*!
 * \brief anonymized Indicates if IP anonymization is used when communicating with Google Analytics.
 */
/*!
 * \brief setClientID sets the globally unique clientID to use with Google Analytics for this run.
 * This anonymously identifies a particular user, device, or browser instance. For the web, this is generally stored as a first-party
 * cookie with a two-year expiration. For mobile apps, this is randomly generated for each particular instance of an application install.
 * The value of this field should be a random UUID (version 4) as described in http://www.ietf.org/rfc/rfc4122.txt
 */
/*!
 * \brief clientID returns the UUID
 */

QtGAConfiguration::QtGAConfiguration()
    : m_endpoint(QtGAConfiguration::NormalEndpoint), m_requestType(QtGAConfiguration::DefaultRequest),
      m_userAgent(QtGAConfiguration::UserAgent), m_cacheBusting(false), m_anonymizeIP(false)
{

}

void QtGAConfiguration::setTrackingID(const QString& trackingID)
{
    QRegExp validTrackingID("\\bUA-\\d{4,10}-\\d{1,4}\\b");
    if ( validTrackingID.exactMatch(trackingID) )
    {
        m_trackingID = trackingID;
    }
    else
    {
        m_trackingID = QString("");
    }
}

const QString& QtGAConfiguration::trackingID() const
{
    return m_trackingID;
}

bool QtGAConfiguration::isSecure() const
{
#ifndef QT_NO_SSL
    return m_endpoint.scheme() == "https";
#else
    return false;
#endif
}

void QtGAConfiguration::setEndpoint(const QUrl &endpoint)
{
#ifdef QT_NO_SSL
    if (endpoint.scheme() == "https")
    {
        return
    }
#endif
    if (endpoint.isValid())
    {
        m_endpoint = endpoint;
    }
}

QUrl QtGAConfiguration::endpoint() const
{
    return m_endpoint;
}

void QtGAConfiguration::setIPAnonymization(bool anonymous)
{
    m_anonymizeIP = anonymous;
}

bool QtGAConfiguration::anonymizeIP() const
{
    return m_anonymizeIP;
}

void QtGAConfiguration::setUserAgent(const QString& userAgent)
{
    // FIXME validation is missing.
    // According to HTTP then a valid user agent is:
    // 1*( product | comment )
    // Where:
    //  product         = token ["/" product-version]
    //  product-version = token
    //
    //  OCTET          = <any 8-bit sequence of data>
    //  CHAR           = <any US-ASCII character (octets 0 - 127)>
    //  CTL            = <any US-ASCII control character (octets 0 - 31) and DEL (127)>
    //  CRLF           = CR LF
    //  SP             = <US-ASCII SP, space (32)>
    //  HT             = <US-ASCII HT, horizontal-tab (9)>
    //  LWS            = [CRLF] 1*( SP | HT )
    //  TEXT           = <any OCTET except CTLs, but including LWS>
    //  token          = 1*<any CHAR except CTLs or separators>
    //  separators     = "(" | ")" | "<" | ">" | "@"
    //                   | "," | ";" | ":" | "\" | <">
    //                   | "/" | "[" | "]" | "?" | "="
    //                   | "{" | "}" | SP | HT
    //  comment        = "(" *( ctext | quoted-pair | comment ) ")"
    //  ctext          = <any TEXT excluding "(" and ")">
    //  quoted-pair    = "\" CHAR
    m_userAgent = userAgent;
}

QString QtGAConfiguration::userAgent() const
{
    return m_userAgent;
}

void QtGAConfiguration::setClientID(const QUuid &clientID)
{
    if ( ( !clientID.isNull() ) && ( clientID.version() == QUuid::Random ) )
    {
        m_clientID = clientID;
    }
}

QUuid QtGAConfiguration::clientID() const
{
    return m_clientID;
}

void QtGAConfiguration::setCacheBusting(bool enabled)
{
    m_cacheBusting = enabled;
}

bool QtGAConfiguration::cacheBusting() const
{
    return m_cacheBusting;
}
