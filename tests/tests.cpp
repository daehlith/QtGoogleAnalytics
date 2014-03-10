#include <QString>

#include <gtest/gtest.h>

#include <QtGA/QtGA.h>

TEST(QtGA_Tests, tracking_id_test)
{
    const QString longTrackingID("UA-987654321-1234");
    const QString shortTrackingID("UA-4321-0");
    const QString invalidTrackingID("FOO-BAR-BAZ");

    QtGA ga;
    ga.setTrackingID(longTrackingID);
    EXPECT_EQ(longTrackingID, ga.trackingID());
    ga.setTrackingID(shortTrackingID);
    EXPECT_EQ(shortTrackingID, ga.trackingID());
    ga.setTrackingID(invalidTrackingID);
    EXPECT_EQ(QString(""), ga.trackingID());
}
