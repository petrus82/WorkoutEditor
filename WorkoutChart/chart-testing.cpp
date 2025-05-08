#include <QTest>
#include <QApplication>
#include <QQuickView>
#include <QPainter>
#include <QLoggingCategory>

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
            QQuickPaintedItem::setWidth(800);
            QQuickPaintedItem::setHeight(800);
            initialize();
        }

    private slots:
        void addIntervalTest();
        void addStepTest();
        void removeStepTest();
        void removeIntervalTest();

    private:
        // To Simulate paint event execution, it is not executed while running QTest
        std::unique_ptr<QPainter> painter = std::make_unique<QPainter>();
    };
    void ButtonTests::addIntervalTest() {
        qDebug() << "QWARN QPainter is from non active painter, not relevant";
        qDebug() << "Check if AddIntervalButton is enabled.";
        QCOMPARE(WorkoutChart::isAddIntervalEnabled(), true);
        qDebug() << "Check if AddStepButton is disabled.";
        QCOMPARE(WorkoutChart::isAddStepEnabled(), false);
        qDebug() << "Check if RemoveIntervalButton is disabled.";
        QCOMPARE(WorkoutChart::isRemoveIntervalEnabled(), false);
        qDebug() << "Check if RemoveStepButton is disabled.";
        QCOMPARE(WorkoutChart::isRemoveStepEnabled(), false);
        qDebug() << "Check call to onIntervalAdd.";
        QVERIFY_THROWS_NO_EXCEPTION(WorkoutChart::onIntervalAdd());
        WorkoutChart::paintSelection(painter.get());
        WorkoutChart::setDuration("01:00");
        WorkoutChart::setIntensity(100);
        qDebug() << "Check intensity of newly added step";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(0)->getIntensity(), 100);
        qDebug() << "Check duration of newly added step";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(0)->getDuration(), 1);
        qDebug() << "Check selection of newly added step";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(0)->isSelected(), true);
        QCOMPARE(WorkoutChart::getIntensity(), 100);
        QCOMPARE(WorkoutChart::getDuration(), "01:00");
        qDebug() << "Check if AddIntervalButton is enabled.";
        QCOMPARE(WorkoutChart::isAddIntervalEnabled(), true);
        qDebug() << "Check if AddStepButton is enabled.";
        QCOMPARE(WorkoutChart::isAddStepEnabled(), true);
        qDebug() << "Check if RemoveIntervalButton is enabled.";
        QCOMPARE(WorkoutChart::isRemoveIntervalEnabled(), true);
        qDebug() << "Check if RemoveStepButton is enabled.";
        QCOMPARE(WorkoutChart::isRemoveStepEnabled(), true);
    }
    void ButtonTests::addStepTest() {
        qDebug() << "Check call to onStepAdd.";
        QVERIFY_THROWS_NO_EXCEPTION(WorkoutChart::onStepAdd());
        WorkoutChart::paintSelection(painter.get());
        WorkoutChart::setDuration("02:00");
        WorkoutChart::setIntensity(200);
        qDebug() << "Verify newly created values.";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(1)->getIntensity(), 200);
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(1)->getDuration(), 2);
        WorkoutChart::paintSelection(painter.get());
        qDebug() << "Check that previous step is disabled.";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(0)->isSelected(), false);
        qDebug() << "Check that newly created step is selected";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(1)->isSelected(), true);
    }
    void ButtonTests::removeStepTest() {
        qDebug() << "Check call to removeStep";
        QVERIFY_THROWS_NO_EXCEPTION(WorkoutChart::onStepRemove());
        qDebug() << "Check that only 1 step exists, i.e. one was removed";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->steps().count(), 1);
        qDebug() << "Check that the previous step is selected";
        QCOMPARE(WorkoutChart::intervals()->atIntervals(0)->step(0)->isSelected(), true);
        qDebug() << "Check that m_activeSelection.step points to last step";
        QCOMPARE(WorkoutChart::m_activeSelection.step, WorkoutChart::intervals()->atIntervals(0)->at(0));
        qDebug() << "Check Interval remove Button is enabled";
        QCOMPARE(WorkoutChart::isRemoveIntervalEnabled(), true);
        qDebug() << "Check Step remove Button is enabled";
        QCOMPARE(WorkoutChart::isRemoveStepEnabled(), true);
        qDebug() << "Check Add Interval Button is enabled";
        QCOMPARE(WorkoutChart::isAddIntervalEnabled(), true);
        qDebug() << "Check Add Step Button is enabled";
        QCOMPARE(WorkoutChart::isAddStepEnabled(), true);
    }
    void ButtonTests::removeIntervalTest() {
        qDebug() << "Check call to onIntervalRemove";
        QVERIFY_THROWS_NO_EXCEPTION(WorkoutChart::onIntervalRemove());
        qDebug() << "Check that all intervals were removed";
        QCOMPARE(WorkoutChart::intervals()->count(), 0);
        qDebug() << "Check that interval selection is nullptr";
        QVERIFY(WorkoutChart::m_activeSelection.interval == nullptr);
        qDebug() << "Check that step selection is nullptr";
        QVERIFY(WorkoutChart::m_activeSelection.step == nullptr);
        qDebug() << "Check Interval remove Button is disabled";
        QCOMPARE(WorkoutChart::isRemoveIntervalEnabled(), false);
        qDebug() << "Check Step remove Button is disabled";
        QCOMPARE(WorkoutChart::isRemoveStepEnabled(), false);
        qDebug() << "Check Add Interval Button is enabled";
        QCOMPARE(WorkoutChart::isAddIntervalEnabled(), true);
        qDebug() << "Check Add Step Button is disabled";
        QCOMPARE(WorkoutChart::isAddStepEnabled(), false);
    }

    QTEST_MAIN(ButtonTests)
#endif

#include "chart-testing.moc"