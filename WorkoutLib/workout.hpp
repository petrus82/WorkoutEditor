#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <vector>
#include <filesystem>
#include <fstream>

namespace Workouts {

  enum class WorkoutType
  {
    AbsoluteWatt,
    PercentFTP,
    AbsoluteHeartRate,
    PercentMaxHeartRate
  };

  static std::string intervalTypeToString(WorkoutType type)
  {
    switch (type)
    {
    case WorkoutType::AbsoluteWatt:
      return "AbsoluteWatt";
    case WorkoutType::PercentFTP:
      return "PercentFTP";
    case WorkoutType::AbsoluteHeartRate:
      return "AbsoluteHeartRate";
    case WorkoutType::PercentMaxHeartRate:
      return "PercentMaxHeartRate";
    default:
      return "Unknown";
    }
  }
  using uintType = uint16_t;
  struct Duration
  {
    uintType Minutes;
    uintType Seconds;
  };

  struct ValueRange
  {
    uintType From;
    uintType To;
  };


  struct Repeat
  {
    uintType From;
    uintType To;
    uintType Times;
  };

  enum class FileType
  {
    Fit,
    Plan,
    Erg,
    Mrc
  };

  class Interval
  {
  public:
    explicit Interval(std::ostream& file, WorkoutType type, ValueRange value, Duration duration)
        : m_file(file), m_type(type), m_value(value), m_duration(duration) {}

    virtual ~Interval() = default;

    [[nodiscard]] float writeToFile(float startTime);

  protected:
    virtual void writeCommon() = 0;
    virtual void writeAbsoluteWatt() = 0;
    virtual void writePercentFTP() = 0;
    virtual void writeAbsoluteHeartRate() = 0;
    virtual void writePercentMaxHeartRate() = 0;

  protected:
    float m_startTime{};
    std::ostream &m_file;
    WorkoutType m_type;
    ValueRange m_value;
    Duration m_duration;
  };

  class Workout
  {
  public:
    explicit Workout(std::filesystem::path filename, std::string workoutName, std::string notes)
        : m_workoutName(workoutName), m_notes(notes)
    {
      m_file.open(filename, std::ios::out | std::ios::in | std::ios::trunc | std::ios::binary);
    }
    explicit Workout(std::string &filename, std::string workoutName, std::string notes)
        : Workout(std::filesystem::path(filename), workoutName, notes) {}

    virtual ~Workout() = default;

    virtual std::size_t createInterval(WorkoutType type, ValueRange value, Duration duration) = 0;
    void createRepeat(Repeat repeat, uint8_t times = 1);
    void writeToFile();

  protected:
    virtual void writeWorkout() = 0;

  protected:
    std::fstream m_file;
    std::string m_workoutName;
    std::string m_notes;
    std::vector<std::shared_ptr<Interval>> m_intervals;
    std::vector<Repeat> m_repeats;
  };
} // Workout namespace