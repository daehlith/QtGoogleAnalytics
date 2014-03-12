#include <QString>

#include <gtest/gtest.h>
#include <gtest/gtest-spi.h>

#include <QtGA/QtGA.h>

TEST(QtGAConfiguration_Tests, tracking_id_test)
{
    const QString longTrackingID("UA-9876543210-1234");
    const QString shortTrackingID("UA-4321-0");
    const QString invalidTrackingID("FOO-BAR-BAZ");

    QtGAConfiguration configuration;
    configuration.setTrackingID(longTrackingID);
    EXPECT_EQ(longTrackingID, configuration.trackingID());
    configuration.setTrackingID(shortTrackingID);
    EXPECT_EQ(shortTrackingID, configuration.trackingID());
    configuration.setTrackingID(invalidTrackingID);
    EXPECT_EQ(QString(""), configuration.trackingID());
}

TEST(QtGAConfiguration_Tests, connector_test)
{
    QtGAConfiguration configuration;
    EXPECT_FALSE(configuration.isSecure());
    configuration.setEndpoint(QtGAConfiguration::SECURE_ENDPOINT);
#ifndef QT_NO_SSL
    EXPECT_TRUE(configuration.isSecure());
#else
    EXPECT_NONFATAL_FAILURE(EXPECT_TRUE(configuration.isSecure()), "Qt was built without SSL support");
#endif
}

TEST(QtGAConfiguration_Tests, endpoint_tests)
{
    QtGAConfiguration configuration;
    EXPECT_EQ(QtGAConfiguration::NORMAL_ENDPOINT, configuration.endpoint());
    configuration.setEndpoint(QtGAConfiguration::SECURE_ENDPOINT);
#ifndef QT_NO_SSL
    EXPECT_EQ(QtGAConfiguration::SECURE_ENDPOINT, configuration.endpoint());
#else
    EXPECT_NONFATAL_FAILURE(EXPECT_EQ(QtGAConfiguration::SECURE_ENDPOINT, configuration.endpoint()), "Qt was built without SSL support");
#endif
}

TEST(QtGAConfiguration_Tests, client_id_tests)
{
    // The value of this field should be a random UUID (version 4) as described in http://www.ietf.org/rfc/rfc4122.txt
    QtGAConfiguration configuration;
    EXPECT_TRUE(configuration.clientID().isNull());
    const QUuid expected = QUuid("f47ac10b-58cc-4372-a567-0e02b2c3d479");
    configuration.setClientID(expected);
    EXPECT_EQ(expected, configuration.clientID());
    // An invalid clientID should not change the existing value
    const QUuid noVersion4Uuid = QUuid("f47ac10b-58cc-5372-b567-0e02b2c3d479");
    configuration.setClientID(noVersion4Uuid);
    EXPECT_EQ(expected, configuration.clientID());
}

TEST(QtGAConfiguration_Tests, cache_busting_tests)
{
    QtGAConfiguration configuration;
    EXPECT_FALSE(configuration.cacheBusting());
    configuration.setCacheBusting(true);
    EXPECT_TRUE(configuration.cacheBusting());
    // TODO check for "z" being appended to the final URL
}
