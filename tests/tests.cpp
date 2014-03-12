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
    configuration.setEndpoint(QtGAConfiguration::SecureEndpoint);
#ifndef QT_NO_SSL
    EXPECT_TRUE(configuration.isSecure());
#else
    EXPECT_NONFATAL_FAILURE(EXPECT_TRUE(configuration.isSecure()), "Qt was built without SSL support");
#endif
}

TEST(QtGAConfiguration_Tests, endpoint_tests)
{
    QtGAConfiguration configuration;
    EXPECT_EQ(QtGAConfiguration::NormalEndpoint, configuration.endpoint());

    // specifying an invalid endpoint won't work
    configuration.setEndpoint(QUrl());
    EXPECT_EQ(QtGAConfiguration::NormalEndpoint, configuration.endpoint());

    configuration.setEndpoint(QtGAConfiguration::SecureEndpoint);
#ifndef QT_NO_SSL
    EXPECT_EQ(QtGAConfiguration::SecureEndpoint, configuration.endpoint());
#else
    configuration.setEndpoint(QtGAConfiguration::NormalEndpoint);
    configuration.setEndpoint(QtGAConfiguration::SecureEndpoint);
    EXPECT_NONFATAL_FAILURE(EXPECT_EQ(QtGAConfiguration::SecureEndpoint, configuration.endpoint()), "Qt was built without SSL support");
#endif
}

TEST(QtGAConfiguration_Tests, client_id_tests)
{
    const QString validVersion4Uuid = "f47ac10b-58cc-4372-a567-0e02b2c3d479";
    const QUuid expected = QUuid(validVersion4Uuid);
    const QUuid noVersion4Uuid = QUuid("f47ac10b-58cc-5372-b567-0e02b2c3d479");

    QtGAConfiguration configuration;
    EXPECT_TRUE(configuration.clientID().isNull());

    configuration.setClientID(expected);
    EXPECT_EQ(expected, configuration.clientID());

    // An invalid clientID should not change the existing value
    configuration.setClientID(noVersion4Uuid);
    EXPECT_EQ(expected, configuration.clientID());

    // TODO check that the request contains the cid parameter, and that it matches the expected UUIDv4 string
}

TEST(QtGAConfiguration_Tests, cache_busting_tests)
{
    QtGAConfiguration configuration;
    EXPECT_FALSE(configuration.cacheBusting());
    configuration.setCacheBusting(true);
    EXPECT_TRUE(configuration.cacheBusting());
    // TODO check for "z" being appended to the final URL
    // TODO check for "z" being different for every request
}

TEST(QtGAConfiguration_Tests, IP_anonymization_tests)
{
    QtGAConfiguration configuration;
    EXPECT_FALSE(configuration.anonymizeIP());
    configuration.setIPAnonymization(true);
    EXPECT_TRUE(configuration.anonymizeIP());
    // TODO check for "aip" being appened to the final URL
}

TEST(QtGAConfiguration_Tests, user_agent_tests)
{
    QtGAConfiguration configuration;
    EXPECT_EQ(QtGAConfiguration::UserAgent, configuration.userAgent());
    // TODO check for user agent validation
}
