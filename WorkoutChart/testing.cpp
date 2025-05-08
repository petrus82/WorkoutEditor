#include <gtest/gtest.h>
#include <fit_encode.hpp>

#include "fitworkout.hpp"
#include "mainui.cpp"

/*
class TestFitAbsoluteInterval : public testing::Test, public FitInterval
{
public:
    TestFitAbsoluteInterval()
        : FitInterval(stream, IntervalType::AbsoluteHeartRate, 0, initEncoder(), initValue(), initDuration()) {}
    fit::Encode* initEncoder() {
        encoder = new fit::Encode(fit::ProtocolVersion::V20);
        return encoder;
    }
    ValueRange initValue() {
        static uint8_t valueF = 2;
        static uint8_t valueT = 0;
        value.From = valueF;
        value.To = valueT;
        valueF = 150;
        valueT = 170;
        return value;
    }
    Duration initDuration() {
        duration.Minutes = 5;
        duration.Seconds = 0;
        return duration;
    }
    void SetUp() override {
        
    }
    void TearDown() override
    {
        delete encoder;
    }
    static constexpr int heartRateOffset = 100;
    static constexpr int powerOffSet = 1000;

private:
    fit::Encode* encoder;
    std::stringstream stream;
    Duration duration;
    ValueRange value;
};
TEST_F(TestFitAbsoluteInterval, WriteHeartRateZoneTest)
{
    writeCommon();
    writeAbsoluteHeartRate();
    ASSERT_EQ(m_workoutStepMesg->GetTargetHrZone(), 2);
}
TEST_F(TestFitAbsoluteInterval, WriteAbsoluteHeartRateTest)
{
    writeCommon();
    writeAbsoluteHeartRate();
    ASSERT_EQ(m_workoutStepMesg->GetCustomTargetHeartRateHigh(), 170 + heartRateOffset);
}
TEST_F(TestFitAbsoluteInterval, WriteAbsolutePowerTest)
{
    writeCommon();
    writeAbsoluteWatt();
    ASSERT_EQ(m_workoutStepMesg->GetCustomTargetPowerHigh(), 170 + powerOffSet);
}

class TestFitRelativeInterval : public testing::Test, public FitInterval
{
public:
    TestFitRelativeInterval()
        : FitInterval(stream, IntervalType::PercentMaxHeartRate, 0, initEncoder(), initValue(), initDuration()) {}
    fit::Encode* initEncoder() {
        encoder = new fit::Encode(fit::ProtocolVersion::V20);
        return encoder;
    }
    ValueRange initValue() {
        static uint8_t valueF = 2;
        static uint8_t valueT = 0;
        value.From = valueF;
        value.To = valueT;
        valueF = 85;
        valueT = 95;
        return value;
    }
    Duration initDuration() {
        duration.Minutes = 5;
        duration.Seconds = 0;
        return duration;
    }
    void SetUp() override {
        
    }
    void TearDown() override
    {
        delete encoder;
    }

private:
    fit::Encode* encoder;
    std::stringstream stream;
    Duration duration;
    ValueRange value;
};
TEST_F(TestFitRelativeInterval, WriteRelativeHeartRateZoneTest)
{
    writeCommon();
    writeAbsoluteHeartRate();
    ASSERT_EQ(m_workoutStepMesg->GetTargetHrZone(), 2);
}
TEST_F(TestFitRelativeInterval, WriteRelativeHeartRateTest)
{
    writeCommon();
    writePercentMaxHeartRate();
    ASSERT_EQ(m_workoutStepMesg->GetCustomTargetHeartRateHigh(), 95);
}
TEST_F(TestFitRelativeInterval, WriteRelativePowerTest)
{
    writeCommon();
    writePercentFTP();
    ASSERT_EQ(m_workoutStepMesg->GetCustomTargetPowerHigh(), 95);
}

class UIBindingsTest : public ::testing::Test {
 protected:
  UIBindings uiBindings;

  void SetUp() override {
    // Initialize uiBindings with some sample data
    uiBindings.m_positionVector = {{0, true}, {1, false}, {2, true}, {3, false}};
    uiBindings.m_repeatsVector = {{0, 2, 2}, {1, 3, 2}};
    uiBindings.m_barSet = new QBarSet("");
    uiBindings.m_removeIntervalButton = new QQuickItem();
  }

  void TearDown() override {
    delete uiBindings.m_barSet;
    delete uiBindings.m_removeIntervalButton;
  }
};

TEST_F(UIBindingsTest, RemoveRepeatThrowsExceptionWhenNoSelection) {
  uiBindings.m_selectedIndex = -1;
  EXPECT_THROW(uiBindings.removeRepeat(), std::runtime_error);
}

TEST_F(UIBindingsTest, RemoveRepeatFindsStartPositionCorrectly) {
  uiBindings.m_selectedIndex = 2;
  uiBindings.removeRepeat();
  EXPECT_EQ(1, uiBindings.startPosition);
}

TEST_F(UIBindingsTest, RemoveRepeatRemovesCorrectElements) {
  uiBindings.m_selectedIndex = 2;
  uiBindings.removeRepeat();
  EXPECT_EQ(2, uiBindings.m_positionVector.size());
  EXPECT_EQ(1, uiBindings.m_repeatsVector.size());
}

TEST_F(UIBindingsTest, RemoveRepeatUpdatesButtonAndIndexCorrectly) {
  uiBindings.m_selectedIndex = 2;
  uiBindings.removeRepeat();
  EXPECT_FALSE(uiBindings.m_removeIntervalButton->isEnabled());
  EXPECT_EQ(-1, uiBindings.m_selectedIndex);
}

TEST_F(UIBindingsTest, RemoveRepeatThrowsExceptionWhenBarSetIsNullptr) {
  uiBindings.m_barSet = nullptr;
  uiBindings.m_selectedIndex = 2;
  EXPECT_THROW(uiBindings.removeRepeat(), std::runtime_error);
} */