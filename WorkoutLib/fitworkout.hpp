#pragma once
#include "workout.hpp"

#include <memory>

#include <fit_encode.hpp>
#include <fit_file_id_mesg.hpp>
#include "fit_date_time.hpp"
#include <fit_workout_mesg.hpp>
#include <fit_workout_step_mesg.hpp>
#include <fit_decode.hpp>

using namespace Workouts;
class FitInterval : public Interval
{
public:
  FitInterval(std::ostream& file, WorkoutType intervalType, uint32_t index, fit::Encode *encoder, ValueRange value, Duration duration)
      : Interval(file, intervalType, value, duration), m_index(index), m_encoder(encoder) {}

  void writeCommon() override;
  void writeAbsoluteWatt() override;
  void writePercentFTP() override;
  void writeAbsoluteHeartRate() override;
  void writePercentMaxHeartRate() override;

protected:
  std::unique_ptr<fit::WorkoutStepMesg> m_workoutStepMesg;

private:
  uint32_t m_index;
  fit::Encode *m_encoder;
  static constexpr int AbsolutePowerOffset = 1000;
  static constexpr int AbsoluteHeartRateOffset = 100;
};

class FitWorkout : public Workout
{
public:
  explicit FitWorkout(const std::string &file, std::string workoutName, std::string notes)
      : Workout(file, workoutName, notes)
  {
    writeWorkout();
  }

  ~FitWorkout()
  {
    writeToFile();
    m_encoder->Close();
  }

  std::size_t createInterval(WorkoutType type, ValueRange value, Duration duration) override
  {
    m_intervals.emplace_back(
        std::make_shared<FitInterval>(m_file, type, m_intervals.size(), m_encoder.get(), value, duration));
    return m_intervals.size() - 1;
  }

private:
  void writeWorkout() override;

private:
  std::unique_ptr<fit::Encode> m_encoder;
};