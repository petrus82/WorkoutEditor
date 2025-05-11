#include "workout.hpp"
#include <memory>

#include <QObject>
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QPainter>
#include <QList>
#include <QAbstractListModel>
#include <QtQmlIntegration/qqmlintegration.h>

namespace WORKOUT_CHART {
Q_NAMESPACE

    struct Zone {
        constexpr explicit Zone(const uint lowerIntensity, const uint upperIntensity,const std::string& color)
            : LowerIntensity(lowerIntensity), UpperIntensity(upperIntensity), Color(color) {}
        uint LowerIntensity {};
        uint UpperIntensity {};
        std::string Color {};
    };

    static constexpr const std::array<Zone, 5> getKarvonen (const uint restingHeartRate, const uint maxHeartRate) {
        const uint heartRateReserve {maxHeartRate - restingHeartRate};
        return {
            Zone(heartRateReserve * 0.55, heartRateReserve * 0.59, "blue"),
            Zone(heartRateReserve * 0.6, heartRateReserve * 0.69, "green"),
            Zone(heartRateReserve * 0.7, heartRateReserve * 0.79, "yellow"),
            Zone(heartRateReserve * 0.8, heartRateReserve * 0.89, "orange"),
            Zone(heartRateReserve * 0.9, heartRateReserve, "red")
        };
    }
    static constexpr const std::string getHeartRateColor (uint heartRate, uint restingHeartRate, uint maxHeartRate) {
        const auto zones {getKarvonen(restingHeartRate, maxHeartRate)};
        for (const auto& zone: zones) {
            if (heartRate >= zone.LowerIntensity && heartRate <= zone.UpperIntensity) {
                return zone.Color;
            }
        }
        return "";
    }
    static constexpr std::string getHeartRateColor (const uint heartRatePercentage) {
        static constexpr std::array<std::pair<uint, uint>, 5> ranges = {
            {{55, 59}, {60, 69}, {70, 79}, {80, 89}, {90, 100}}
        };
        static constexpr std::array<std::string, 5> colors = {
            "blue", "green", "yellow", "orange", "red"
        };

        for (size_t i = 0; i < ranges.size(); ++i) {
            if (heartRatePercentage >= ranges[i].first && heartRatePercentage <= ranges[i].second) {
                return colors[i];
            }
        }
        // This should not happen
        return "";
    }
    template <uint nrZones>
    static constexpr const std::array<Zone, nrZones> getPowerZones(const uint ftp) {
        if constexpr (nrZones == 6) {
            return {
                Zone(0, ftp * 0.55, "lightgray"),
                Zone(ftp * 0.56, ftp * 0.75, "blue"),
                Zone(ftp * 0.76, ftp * 0.9, "lightblue"),
                Zone(ftp * 0.91, ftp * 1.05, "green"),
                Zone(ftp * 1.06, ftp * 1.2, "yellow"),
                Zone(ftp * 1.21, 2000, "orange")
            };
        } else {
            return {
                Zone(0, ftp * 0.55, "lightgray"),
                Zone(ftp * 0.56, ftp * 0.75, "blue"),
                Zone(ftp * 0.76, ftp * 0.87, "lightblue"),
                Zone(ftp * 0.88, ftp * 0.94, "green"),
                Zone(ftp * 0.95, ftp * 1.05, "yellow"),
                Zone(ftp * 1.06, ftp * 1.2, "orange"),
                Zone(ftp * 1.21, 2000, "red")
            };
        }
    }

    template <uint nrZones>
    static constexpr const std::string getPowerZoneColor (const uint power, const uint ftp) {
        const auto zones {getPowerZones<nrZones>(ftp)};
        for (const auto& zone: zones) {
            if (power >= zone.LowerIntensity && power <= zone.UpperIntensity) {
                return zone.Color;
            }
        }
        return "black";
    }
    template <uint nrZones>
    static constexpr const std::string getPowerZoneColor (const uint powerPercentage) {
        static constexpr auto ranges = [](){
            if constexpr (nrZones == 6) {
                return std::array<std::pair<uint, uint>, 6>{{{0, 55}, {56, 75}, {76, 90}, {91, 105}, {106, 120}, {121, 200}}};
            } else if constexpr (nrZones == 7) {
                return std::array<std::pair<uint, uint>, 7>{{{0, 55}, {56, 75}, {76, 87}, {88, 94}, {95, 105}, {106, 120}, {121, 200}}};
            }
        }();

        static constexpr auto colors = [](){
            if constexpr (nrZones == 6) {
                return std::array<std::string, 6>{{"lightgray", "blue", "lightblue", "green", "yellow", "orange"}};
            } else if constexpr (nrZones == 7) {
                return std::array<std::string, 7>{{"lightgray", "blue", "lightblue", "green", "yellow", "orange", "red"}};
            }
        }();

        for (size_t i = 0; i < ranges.size(); ++i) {
            if (powerPercentage >= ranges[i].first && powerPercentage <= ranges[i].second) {
                return colors[i];
            }
        }
        // This should not happen
        return "";
    }

    class WorkoutChart;
    class Step : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        Q_PROPERTY(uint intensity READ getIntensity WRITE setIntensity)
        Q_PROPERTY(qreal duration READ getDuration WRITE setDuration)
        Q_PROPERTY(QColor color READ getColor)
        Q_PROPERTY(bool isSelected READ isSelected WRITE setSelected);
        Q_CLASSINFO("DefaultProperty", "data")

    public:
        Step() {};

        uint getIntensity() const { return m_intensity; }
        void setIntensity(uint intensity) { m_intensity = intensity; }

        qreal getDuration() const { return m_duration; }
        void setDuration(qreal duration) { m_duration = duration; }

        QColor getColor(uint repeat = 1);
        bool isSelected() const { return m_isSelected; }
        void setSelected(bool isSelected) { m_isSelected = isSelected; }

        void setSpace(uint left, uint right, uint repeat) {
            if (repeat <= m_mySpaces.size()) {
                m_mySpaces[repeat-1].first = left;
                m_mySpaces[repeat -1].second = right;
            } else {
                m_mySpaces.emplace_back(std::pair<int,int>(left, right));
            }
        }
        int isInSpace(uint x) {
            int left {};
            int right {static_cast<int>(m_mySpaces.size())};
            while (left < right || left == 0) {
                uint mid = left + (right - left) / 2;
                if (x >= m_mySpaces[mid].first && x <= m_mySpaces[mid].second) {
                    return mid;
                } else if (x < m_mySpaces[mid].first) {
                    // if the value is not inside m_mySpaces[mid]
                    // and x < left value, then look below mid
                    // because the values inside m_mySpaces are continously incrementing
                    right = mid -1;
                } else {
                    // otherwise go for higher values
                    left = mid + 1;
                }
            }
            return -1;
        }
        std::pair<int,int> getSpace(uint repeat) {
            if (repeat < 1) throw std::runtime_error("repeat has to be at least 1");
            return m_mySpaces.at(repeat - 1);
        }
        void setRepeats(uint repeat) {
            if (repeat < m_mySpaces.size()) {
                m_mySpaces.resize(repeat);
            }
        }
        uint top{};
    
    private:
        WorkoutChart* getWorkoutPtr();

    private:
        uint m_intensity{};
        qreal m_duration{};

        // first value is left border, second value right border
        // All elements with index > 0 represent repeats
        std::vector<std::pair<int, int>> m_mySpaces {std::pair<int,int>(0, 0)};
        bool m_isSelected {false};
    };

    class Interval : public QObject
    {
        Q_OBJECT
        QML_ELEMENT
        Q_PROPERTY(QQmlListProperty<Step> stepList READ stepList)
        Q_PROPERTY(uint repeat READ getRepeats WRITE setRepeats NOTIFY repeatsChanged)
        Q_PROPERTY(uint from READ getFrom WRITE setFrom NOTIFY fromChanged)
        Q_PROPERTY(qreal duration READ getDuration)
        Q_CLASSINFO("DefaultProperty", "steps")
        
    public:
        Q_INVOKABLE explicit Interval(QObject *parent = nullptr)
            : QObject(parent) {}

        QQmlListProperty<Step> stepList();

        QList<Step*> steps() const {
            QList<Step*> rawPtrList;
            rawPtrList.reserve(m_steps.size());
            for (const auto& step : m_steps) {
                rawPtrList.append(step.get());
            }
            return rawPtrList;
        }
        Step* appendStep() {
            m_steps.emplace_back(std::make_unique<Step>());
            return m_steps.back().get();
        }
        void appendStep(Step *step) {
            m_steps.emplace_back(std::unique_ptr<Step>(step));
        }
        int count() const {return m_steps.size();}
        Step *at(long long index) const { return m_steps.at(index).get();}
        void clearSteps()
        {
            m_steps.clear();
            emit stepsChanged();
        }

        void removeStep(int index)
        {
            if (index >= 0 && index < m_steps.size()) {
                m_steps.erase(m_steps.begin() + index);
                emit stepsChanged();
            }
        }
        uint getRepeats() const {
            return m_repeats;
        }
        void setRepeats(uint repeats) {
            if (repeats > 1) {
                for (const auto& step : m_steps) {
                    step->setRepeats(repeats);
                }
                m_repeats = repeats;
                emit repeatsChanged();
                recalculateDuration();
            }
        }
        uint getFrom() const {return m_from;}
        void setFrom(uint index) {
            if (index > 0) {
                m_from = index;
                emit fromChanged();
            }
        }
        qreal getDuration() {
            if (m_duration == 0) recalculateDuration();
            return m_duration;
        }

        uint left{};
        uint right{};
        uint top{};
    public slots:
        Step* step(uint index);

    signals:
        void stepsChanged();
        void repeatsChanged();
        void fromChanged();

    private:
        static void appendFunction(QQmlListProperty<Step> *property, Step *newStep);

        static long long countFunction(QQmlListProperty<Step> *property);
        static Step *atFunction(QQmlListProperty<Step> *property, long long index);
        static void clearFunction(QQmlListProperty<Step> *property);
        void recalculateDuration();

    private:
        uint m_repeats {1};
        qreal m_duration {};
        uint m_from {1};
        std::vector<std::unique_ptr<Step>> m_steps;
    };
    

    class IntervalListModel : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QQmlListProperty<Interval> intervalList READ intervalList)
        Q_CLASSINFO("DefaultProperty", "intervals")
        QML_ELEMENT
    public:
        Q_INVOKABLE explicit IntervalListModel(QObject *parent = nullptr)
            : QObject(parent) {}

        QQmlListProperty<Interval> intervalList();
        QList<Interval*> intervals() const {
            QList<Interval*> rawPtrs;
            rawPtrs.reserve(m_intervals.size());
            for (const auto& interval : m_intervals) {
                rawPtrs.append(interval.get());
            }
            return rawPtrs;
        }
        Interval* appendInterval();
        void appendInterval(Interval *interval);
        
        int count(const QModelIndex &parent = QModelIndex()) const
        {
            return m_intervals.size();
        }
        Interval *atIntervals(long long index) const
        {
            return m_intervals.at(index).get();
        }
        void clearIntervals()
        {
            m_intervals.clear();
            emit intervalsChanged();
        }

        void removeInterval(int index)
        {
            m_intervals.erase(m_intervals.begin() + index);
            emit intervalsChanged();
        }
    public slots:
        Interval* interval(uint index)
        {
            if (index < 0 || index < m_intervals.size())
            {
                return nullptr;
            }
            return m_intervals[index].get();
        }
    signals:
        void intervalsChanged();

    private:
        static void appendFunction(QQmlListProperty<Interval> *property, Interval *newInterval);
        static long long countFunction(QQmlListProperty<Interval> *property);
        static Interval *atFunction(QQmlListProperty<Interval> *property, long long index);
        static void clearFunction(QQmlListProperty<Interval> *property);

    private:
        std::vector<std::unique_ptr<Interval>> m_intervals;
    };
    struct Selection {
        Interval* interval{};
        Step* step {};
        int repeat {};
    };

    class WorkoutChart : public QQuickPaintedItem
    {
        Q_OBJECT
        QML_ELEMENT
        Q_PROPERTY(uint fileType READ getFileType WRITE setFileType NOTIFY fileTypeChanged)
        Q_PROPERTY(QString fileName READ getFileName WRITE setFileName NOTIFY fileNameChanged)
        Q_PROPERTY(QString workoutName READ getWorkoutName WRITE setWorkoutName NOTIFY workoutNameChanged)
        Q_PROPERTY(QString workoutNotes READ getWorkoutNotes WRITE setWorkoutNotes NOTIFY workoutNotesChanged)
        Q_PROPERTY(uint workoutType READ getWorkoutType WRITE setWorkoutType NOTIFY workoutTypeChanged)
        Q_PROPERTY(uint intensity READ getIntensity WRITE setIntensity NOTIFY intensityChanged)
        Q_PROPERTY(uint maxIntensity READ getMaxIntensity WRITE setMaxIntensity NOTIFY maxIntensityChanged)
        Q_PROPERTY(QString duration READ getDuration WRITE setDuration NOTIFY durationChanged())
        Q_PROPERTY(uint maxDuration READ getMaxDuration WRITE setMaxDuration NOTIFY maxDurationChanged)
        Q_PROPERTY(uint ftp READ getFTP WRITE setFTP NOTIFY ftpChanged)
        Q_PROPERTY(uint restingHeartRate READ getRestingHeartRate WRITE setRestingHeartRate NOTIFY restingHeartRateChanged)
        Q_PROPERTY(uint maxHeartRate READ getMaxHeartRate WRITE setMaxHeartRate NOTIFY maxHeartRateChanged)
        Q_PROPERTY(QColor *backgroundColor READ getBackgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(IntervalListModel *intervals READ intervals)
        Q_PROPERTY(uint repeats READ getRepeats WRITE setRepeats NOTIFY repeatsChanged)
        Q_PROPERTY(uint from READ getFrom WRITE setFrom NOTIFY fromChanged)
        Q_PROPERTY(uint maxFrom READ maxFrom)
        Q_PROPERTY(bool addIntervalEnabled READ isAddIntervalEnabled WRITE setAddIntervalEnabled NOTIFY addIntervalEnabledChanged)
        Q_PROPERTY(bool addStepEnabled READ isAddStepEnabled WRITE setAddStepEnabled NOTIFY addStepEnabledChanged)
        Q_PROPERTY(bool removeIntervalEnabled READ isRemoveIntervalEnabled WRITE setRemoveIntervalEnabled NOTIFY removeIntervalEnabledChanged)
        Q_PROPERTY(bool removeStepEnabled READ isRemoveStepEnabled WRITE setRemoveStepEnabled NOTIFY removeStepEnabledChanged)

        Q_PLUGIN_METADATA(IID "WorkoutChart")

    public:
        WorkoutChart(QQuickItem *parent = nullptr)
        {
            QQuickPaintedItem::setAcceptedMouseButtons(Qt::LeftButton);
        }
        uint getFileType() const {return static_cast<uint>(m_fileType);}
        void setFileType(uint fileTypeIndex) {
            m_fileType = static_cast<Workouts::FileType>(fileTypeIndex);
            emit fileTypeChanged();
        }
        QString getFileName () const {return m_fileName;}
        void setFileName(QString fileName) {
            m_fileName = QUrl(fileName).toLocalFile();
            emit fileNameChanged();
        }
        QString getWorkoutName () const {return m_workoutName;}
        void setWorkoutName(QString workoutName) {
            m_workoutName = workoutName;
            emit workoutNameChanged();
        }
        QString getWorkoutNotes () const {return m_workoutNotes;}
        void setWorkoutNotes(QString workoutNotes) {
            m_workoutNotes = workoutNotes;
            emit workoutNotesChanged();
        }
        

        uint getWorkoutType () const {return static_cast<uint>(m_workoutType);}
        void setWorkoutType (uint intervalType) {
            m_workoutType = static_cast<Workouts::WorkoutType>(intervalType);
            emit workoutTypeChanged();
        }
        QColor *getBackgroundColor() const
        {
            return m_backgroundColor.get();
        }

        void setBackgroundColor(QColor *color)
        {
            m_backgroundColor.reset(color);
            m_paintExtent = PaintExtent::everything;
            QQuickPaintedItem::update();
            emit backgroundColorChanged();
            updateChart();
        }

        uint getMaxIntensity() const
        {
            return m_maxIntensity;
        }
        void setIntensity(uint intensity) {
            if (m_activeSelection.step == nullptr && intensity > 1) {
                onStepAdd();
            }
            m_activeSelection.step->setIntensity(intensity);
            emit intensityChanged();
            updateChart();
        }
        uint getIntensity () const {
            if (m_activeSelection.step == nullptr) return 0;
            return m_activeSelection.step->getIntensity();
        }
        void setMaxIntensity(uint value)
        {
            m_maxIntensity = value;
            emit maxIntensityChanged();
            updateChart();
        }
        QString getDuration () const {
            if (m_activeSelection.step == nullptr) return QString("00:00");
            qreal duration {m_activeSelection.step->getDuration()};
            uint totalSeconds {static_cast<uint>(duration * 60)};
            uint minutes {static_cast<uint>(totalSeconds / 60)};
            uint seconds {totalSeconds % 60};
            QString durationString = QString("%1:%2").arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));
            return durationString;
        }
        void setDuration (QString durationText) {
            if (m_activeSelection.step == nullptr && durationText != "00:00") {
                onStepAdd();
            }
            QTime time {QTime::fromString(durationText, "mm:ss")};
            qreal realTime {static_cast<qreal>(time.second()) * 1/60};
            realTime += time.minute();
            m_activeSelection.step->setDuration(realTime);
            emit durationChanged();
            updateChart();
        }
        uint getMaxDuration() const
        {
            return m_maxDuration;
        }

        void setMaxDuration(uint maxDuration)
        {
            m_maxDuration = maxDuration;
            emit maxDurationChanged();
            updateChart();
        }
        uint getFTP() const {return m_ftp;}
        void setFTP (uint ftp) {
            m_ftp = ftp;
            emit ftpChanged();
        }
        
        uint getMaxHeartRate () const {return m_maxHeartRate;}
        void setMaxHeartRate (uint maxHeartRate) {
            m_maxHeartRate = maxHeartRate;
            emit maxHeartRateChanged();
        }
        uint getRestingHeartRate () const {return m_restingHeartRate;}
        void setRestingHeartRate (uint restingHeartRate) {
            m_restingHeartRate = restingHeartRate;
            emit restingHeartRateChanged();
        }
        IntervalListModel *intervals() const
        {
            return m_intervals.get();
        }
        void mousePressEvent(QMouseEvent *event) override
        {
            setActiveStep(event->position().x());
            emit intervalClicked(m_oldSelection.interval, m_oldSelection.step);
        }
        uint getRepeats () const {
            if (m_activeSelection.interval == nullptr) return 1;
            return m_activeSelection.interval->getRepeats();
        }
        void setRepeats(uint repeats) {
            if (repeats < 1) return;
            if (m_activeSelection.interval == nullptr) {
                if (m_intervals->intervals().count() == 0) onIntervalAdd();
            }
            m_activeSelection.interval->setRepeats(repeats);
            emit repeatsChanged();
            updateChart();
        }
        uint getFrom () const {
            if (m_activeSelection.interval == nullptr) return 1;
            return m_activeSelection.interval->getFrom();
        }
        void setFrom(uint from) {
            if (from < 1) return;
            if (m_activeSelection.interval == nullptr) {
                if (m_intervals->intervals().count() == 0) onIntervalAdd();
            }
            m_activeSelection.interval->setFrom(from);
            emit fromChanged();
            updateChart();
        }
        uint maxFrom () const {
            if (m_activeSelection.interval) {
                return m_activeSelection.interval->steps().count();
            }
            else {
                return 1;
            }
        }
        bool isAddIntervalEnabled() const {return m_isAddIntervalEnabled;}
        void setAddIntervalEnabled(bool isAddIntervalEnabled) {
            m_isAddIntervalEnabled = isAddIntervalEnabled;
            emit addIntervalEnabledChanged();
        }
        bool isAddStepEnabled() const {return m_isAddStepEnabled;}
        void setAddStepEnabled(bool isAddStepEnabled) {
            m_isAddStepEnabled = isAddStepEnabled;
            emit addStepEnabledChanged();
        }
        bool isRemoveIntervalEnabled() const {return m_isRemoveIntervalEnabled;}
        void setRemoveIntervalEnabled(bool isRemoveIntervalEnabled) {
            m_isRemoveIntervalEnabled = isRemoveIntervalEnabled;
            emit removeIntervalEnabledChanged();
        }
        bool isRemoveStepEnabled() const {return m_isRemoveStepEnabled;}
        void setRemoveStepEnabled(bool isRemoveStepEnabled) {
            m_isRemoveStepEnabled = isRemoveStepEnabled;
            emit removeStepEnabledChanged();
        }
        void paint(QPainter *painter) override;

    public slots:
        void onIntervalAdd();
        void onIntervalRemove();
        void onStepAdd();
        void onStepRemove(bool isCalledFromInterval = false);
        void onRepeatChanged();
        void onFromChanged();
        void onIntervalsChanged()
        {
            QRect mySize;
            mySize.setX(QQuickPaintedItem::x());
            mySize.setY(QQuickPaintedItem::y());
            mySize.setWidth(QQuickPaintedItem::width());
            mySize.setHeight(QQuickPaintedItem::height());
            m_paintExtent = PaintExtent::everything;
            QQuickPaintedItem::update();
        }
        void onSelectionFinished();
        void onOkClicked();
    signals:
        void intervalClicked(Interval* interval, Step* step);
        void selectionPainted();
        void ftpChanged();
        void restingHeartRateChanged();
        void maxHeartRateChanged();
        void fileTypeChanged();
        void fileNameChanged();
        void workoutNameChanged();
        void workoutNotesChanged();
        void workoutTypeChanged();
        void intensityChanged();
        void maxIntensityChanged();
        void durationChanged();
        void maxDurationChanged();
        void backgroundColorChanged();
        void fromChanged();
        void repeatsChanged();
        void addIntervalEnabledChanged();
        void addStepEnabledChanged();
        void removeIntervalEnabledChanged();
        void removeStepEnabledChanged();

    protected:
        void calculateScaling()
        {
            m_scalingFactorHeight = QQuickItem::height() / (m_maxIntensity + 2 * margin);
            m_scalingFactorWidth = QQuickItem::width() / (m_maxDuration + 2 * margin);
        }
        void setActiveStep(uint x_clicked);
        void select();
        void prepareSelection(Selection* selection);
        void paintEverything(QPainter *painter);
        void paintSelection(QPainter *painter);
        void initialize();
        bool isNewEqOldSelection() {
            if (m_activeSelection.interval != m_oldSelection.interval) {
                return false;
            }
            else if (m_activeSelection.step != m_oldSelection.step) {
                return false;
            }
            else if (m_activeSelection.repeat != m_oldSelection.repeat) {
                return false;
            }
            return true;
        }
        uint getBottom() {
            return QQuickItem::height() - margin;
        }
    protected:
        QRect m_selectionRect{};
        Selection m_oldSelection;
        Selection m_activeSelection;
        bool m_paintSelectionNeeded {true};
    private:
        int getIntervalIndex();
        int getStepIndex();
        void updateChart() {
            m_paintExtent = PaintExtent::everything;
            QQuickPaintedItem::update();
        }
    private:
        enum class PaintExtent
        {
            everything,
            selection
        };

        PaintExtent m_paintExtent{};
        static uint constexpr margin = 5;
        std::unique_ptr<QColor> m_backgroundColor{std::make_unique<QColor>(Qt::lightGray)};
        Workouts::FileType m_fileType {};
        QString m_fileName {};
        QString m_workoutName {};
        QString m_workoutNotes {};
        Workouts::WorkoutType m_workoutType {};
        uint m_maxIntensity{};
        uint m_maxDuration{};
        uint m_ftp {310};
        uint m_restingHeartRate {};
        uint m_maxHeartRate {};
        std::unique_ptr<IntervalListModel> m_intervals{};
        qreal m_scalingFactorHeight{};
        qreal m_scalingFactorWidth{};
        int m_selectionIndex{-1};
        bool m_isAddIntervalEnabled {true};
        bool m_isAddStepEnabled {false};
        bool m_isRemoveIntervalEnabled {false};
        bool m_isRemoveStepEnabled {false};
    };
} // End of WORKOUT_CHART namespace
Q_DECLARE_METATYPE(WORKOUT_CHART::Interval *)