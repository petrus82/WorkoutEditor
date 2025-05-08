#include <QTest>
#include <QApplication>
#include <QQuickView>
#include <QDir>

#include "workoutchart.hpp"

#define BUTTON_TESTS
//#define SELECTION_TESTS

using namespace WORKOUT_CHART;

#ifdef SELECTION_TESTS
    class WorkoutTests : public WorkoutChart {
        Q_OBJECT
    public:
        WorkoutTests() {
            setMaxDuration(60);
            setMaxIntensity(500);
            initialize();
            step1_1->setIntensity(200);
            step1_1->setDuration(5.0);
            step1_1->setColor(QColor("steelblue"));
            step1_1->setSpace(0,10, 1);
            interval1->left = 0;
            interval1->right = 10;
            interval1->appendStep(step1_1.get());
            interval2->setRepeats(3);
            interval2->setFrom(1);
            step2_1->setIntensity(320);
            step2_1->setDuration(5.0);
            step2_1->setColor(QColor("red"));
            step2_1->setSpace(200,210,1);
            step2_1->setSpace(220,230,2);
            step2_1->setSpace(240,250,3);
            interval2->appendStep(step2_1.get());
            step2_2->setIntensity(50);
            step2_2->setDuration(5.0);
            step2_2->setColor(QColor("steelblue"));
            step2_2->setSpace(260, 270, 1);
            step2_2->setSpace(271, 280, 2);
            step2_2->setSpace(281, 290, 3);
            interval2->left = 200;
            interval2->right = 290;
            interval2->appendStep(step2_2.get());
            intervals()->appendInterval(interval1.get());
            intervals()->appendInterval(interval2.get());
            QObject::connect(this, &WorkoutTests::paintFinished, this, &WorkoutChart::onSelectionFinished);
        }
    signals:
        void paintFinished();

    private slots:
        void initialSelectionTest();
        void newSelectionTest();
        void newOnOldTrueSelectionTest();
        void newOnOldFalseSelectionTest();

    private:
        std::unique_ptr<Interval> interval1 = std::make_unique<Interval>();
        std::unique_ptr<Interval> interval2 = std::make_unique<Interval>();
        std::unique_ptr<Step> step1_1 = std::make_unique<Step>();
        std::unique_ptr<Step> step2_1 = std::make_unique<Step>();
        std::unique_ptr<Step> step2_2 = std::make_unique<Step>();
    };

    void WorkoutTests::initialSelectionTest()
    {
        setActiveStep(280); // Simulate Click on Interval index 1, step 2, repeat 2
        QVERIFY(intervals()->atIntervals(1)->step(1)->isSelected());
    }
    void WorkoutTests::newSelectionTest()
    {
        // Test scenario 2: new selection on new location with old selection
        setActiveStep(280);
        emit paintFinished();
        setActiveStep(205); 
        QCOMPARE(intervals()->atIntervals(1)->step(0)->isSelected(), true);
        emit paintFinished();
        QCOMPARE(intervals()->atIntervals(1)->step(1)->isSelected(), false);
    }
    void WorkoutTests::newOnOldTrueSelectionTest()
    {
        // Test scenario 3: new selection on old selection with old selection true
        setActiveStep(5);
        emit paintFinished();
        setActiveStep(280);
        emit paintFinished();
        setActiveStep(280); 
        QCOMPARE(intervals()->atIntervals(1)->step(1)->isSelected(), false);
        emit paintFinished();
        QCOMPARE(intervals()->atIntervals(1)->step(1)->isSelected(), false);
    }
    void WorkoutTests::newOnOldFalseSelectionTest()
    {
        // Test scenario 4: new selection on old selection with old selection false
        setActiveStep(280); 
        QCOMPARE(intervals()->atIntervals(1)->step(1)->isSelected(), true);
        emit paintFinished();
        QCOMPARE(intervals()->atIntervals(1)->step(1)->isSelected(), true);
    }

    QTEST_MAIN(WorkoutTests)
#endif

#ifdef BUTTON_TESTS
    class ButtonTests : public WorkoutChart {
        Q_OBJECT
    public:
        ButtonTests() {
            setMaxDuration(60);
            setMaxIntensity(500);
            initialize();
        }

    private slots:
        void addIntervalTest();
        void addStepTest();
        void removeStepTest();
        void removeIntervalTest();

    private:
        
    };
    void ButtonTests::addIntervalTest() {
        
    }
    void ButtonTests::addStepTest() {

    }
    void ButtonTests::removeStepTest() {

    }
    void ButtonTests::removeIntervalTest() {

    }

    QTEST_MAIN(ButtonTests)
#endif

#include "chart-testing.moc"