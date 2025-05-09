
#include "workoutchart.hpp"

namespace WORKOUT_CHART {
    QQmlListProperty<Step> Interval::stepList() 
    {
        return {this, this,
                &Interval::appendFunction,
                &Interval::countFunction,
                &Interval::atFunction,
                &Interval::clearFunction};
    }

    void Interval::recalculateDuration() {
        m_duration = 0;
        for (uint index = 1; index <= m_steps.size(); index++) {
            if (index >= m_from) {
                m_duration += m_repeats * m_steps[index - 1]->getDuration();
            }
            else {
                m_duration += m_steps[index]->getDuration();
            }
        }
    }
    Step* Interval::step(uint index)
    {
        if (index < 0 || index >= m_steps.size())
        {
            return nullptr;
        }
        return m_steps[index].get();
    }

    void Interval::appendFunction(QQmlListProperty<Step> *property, Step *newStep) 
    {
        Interval *instance = qobject_cast<Interval *>(property->object);
        if (instance)
        {
            instance->appendStep(newStep);
        }
    }
    long long Interval::countFunction(QQmlListProperty<Step> *property)
    {
        Interval *instance = qobject_cast<Interval *>(property->object);
        if (instance)
        {
            return instance->count();
        }
        else
        {
            return -1;
        }
    }
    Step* Interval::atFunction(QQmlListProperty<Step> *property, long long index)
    {
        Interval *instance = qobject_cast<Interval *>(property->object);
        if (instance)
        {
            return instance->at(index);
        }
        else
            return nullptr;
    }
    void Interval::clearFunction(QQmlListProperty<Step> *property)
    {
        Interval *instance = qobject_cast<Interval *>(property->object);
        if (instance)
        {
            instance->clearSteps();
        }
    }

    QQmlListProperty<Interval> IntervalListModel::intervalList()
    {
        return {this, this,
                &IntervalListModel::appendFunction,
                &IntervalListModel::countFunction,
                &IntervalListModel::atFunction,
                &IntervalListModel::clearFunction};
    }
    Interval* IntervalListModel::appendInterval() {
        m_intervals.emplace_back(std::make_unique<Interval>());
        WorkoutChart* myWorkoutParent {qobject_cast<WorkoutChart*>(QObject::parent())};
        QMetaObject::invokeMethod(myWorkoutParent, "onIntervalsChanged", Qt::AutoConnection);
        return m_intervals.back().get();
    }
    void IntervalListModel::appendInterval(Interval* interval) {
        m_intervals.emplace_back(std::unique_ptr<Interval>(interval));
        WorkoutChart* myWorkoutParent {qobject_cast<WorkoutChart*>(QObject::parent())};
        QMetaObject::invokeMethod(myWorkoutParent, "onIntervalsChanged", Qt::AutoConnection);
    }

    void IntervalListModel::appendFunction(QQmlListProperty<Interval> *property, Interval *newInterval)
    {
        IntervalListModel *instance = qobject_cast<IntervalListModel *>(property->object);
        if (instance)
        {
            instance->appendInterval(newInterval);
        }
    }
    long long IntervalListModel::countFunction(QQmlListProperty<Interval> *property)
    {
        IntervalListModel *instance = qobject_cast<IntervalListModel *>(property->object);
        if (instance)
        {
            return instance->count();
        }
        else
        {
            return -1;
        }
    }
    Interval* IntervalListModel::atFunction(QQmlListProperty<Interval> *property, long long index)
    {
        IntervalListModel *instance = qobject_cast<IntervalListModel *>(property->object);
        if (instance)
        {
            return instance->atIntervals(index);
        }
        else
            return nullptr;
    }
    void IntervalListModel::clearFunction(QQmlListProperty<Interval> *property)
    {
        IntervalListModel *instance = qobject_cast<IntervalListModel *>(property->object);
        if (instance)
        {
            instance->clearIntervals();
        }
    }
    void WorkoutChart::paint (QPainter* painter) {
        if (m_paintExtent == PaintExtent::everything) {
            paintEverything(painter);
        }
        else {
            paintSelection(painter);
        }
    }

    void WorkoutChart::paintEverything(QPainter* painter) {
        if (!m_intervals) {
            initialize();
        }
        uint left = margin;
        uint right {};
        uint height {};
        uint width {};

        QRectF background(QQuickPaintedItem::boundingRect());
        painter->fillRect(background, *m_backgroundColor.get());

        if(m_maxDuration == 0 || m_maxIntensity == 0) {
            for (const auto& interval : m_intervals->intervals()) {
                for (const auto& step : interval->steps()) {
                    if (m_maxIntensity < step->getIntensity()) {
                        m_maxIntensity = step->getIntensity();
                    }
                }
                m_maxDuration += interval->getDuration();
            }
        }
        calculateScaling();
        auto drawStep = [&](Step* step, uint repeat) {
            qreal duration = step->getDuration();
            int intensity = step->getIntensity();
            QRect rect;
            right = left + duration * m_scalingFactorWidth;
            height = intensity * m_scalingFactorHeight;
            width = duration * m_scalingFactorWidth;
            uint top (getBottom() - height);
            rect.setLeft(left);
            rect.setTop(top);
            rect.setRight(right);
            rect.setBottom(getBottom());
            QColor intervalColor {step->getColor(repeat)};
            
            painter->fillRect(rect, intervalColor);
            step->setSpace(left, right, repeat);
            step->top = top;
            left += width;
        };
        for (const auto& interval : m_intervals->intervals()) {
            uint repeats {interval->getRepeats()};
            interval->left = left;
            for (const auto& step : interval->steps()) {
                drawStep(step, 1);
            }
            // draw the repeats
            if (repeats > 1) {
                for (uint repeat = 2; repeat <= repeats; ++repeat) {
                    for (uint index = interval->getFrom(); index <= interval->count(); ++index) {
                        drawStep(interval->steps().at(index - 1), repeat);
                    }
                }
            }
            interval->right = right;
        }
    }
    void WorkoutChart::initialize() {
        auto object = this->findChild<QObject*>("intervalList");
        if (object == 0) {
            m_intervals = std::make_unique<IntervalListModel>(parent());
        } else {
            m_intervals = std::unique_ptr<IntervalListModel>(qobject_cast<IntervalListModel*>(object));
        }
        
        QObject::connect(this, &WorkoutChart::selectionPainted, this, &WorkoutChart::onSelectionFinished);
    }

    void WorkoutChart::paintSelection(QPainter* painter) {

        QColor stepColor; 
        if (m_paintSelectionNeeded) {
            stepColor = m_activeSelection.step->getColor(m_activeSelection.repeat);
            // if selection, then adjust hue
            if (stepColor.lightness() > 127) {
                stepColor = stepColor.darker(150);
            } 
            else {
                stepColor = stepColor.lighter(130);
            }
            painter->fillRect(m_selectionRect, stepColor);
            emit selectionPainted();
        }
        else {
            // repaint without hue adjustement for deselection paint
            stepColor = m_oldSelection.step->getColor(m_oldSelection.repeat);
            painter->fillRect(m_selectionRect, stepColor);
            m_oldSelection = m_activeSelection;
            if (m_activeSelection.interval == nullptr) {
                setRemoveIntervalEnabled(false);
            }
            if (m_activeSelection.step == nullptr) {
                setRemoveStepEnabled(false);
            }
        }
    }
    void WorkoutChart::setActiveStep(uint x_clicked) {
        // If a selection is active, it is stored in m_selection
        for (const auto& interval : m_intervals->intervals()) {
            if (interval->left <= x_clicked &&
                interval->right >= x_clicked
            ) {
                m_activeSelection.interval = interval;
                setRemoveIntervalEnabled(true);
                setAddStepEnabled(true);
                for (const auto& step : interval->steps()) {
                    int repeatIndex {step->isInSpace(x_clicked)};
                    if (repeatIndex >= 0) {
                        m_activeSelection.step = step;
                        m_activeSelection.repeat = repeatIndex + 1;
                        setRemoveStepEnabled(true);
                        select();
                        return;
                    }
                }
            }
        }
    }
    /*
        ***Selection Logic:***
        If no selection, make a new one
        If click on an unselected Step, make a new selection and delete old selection
        If click on selected Step, delete Selection

        ***Reasons:***
        Qt is optimizing out two consecutive calls to update.
        Its only possible to repaint one rectangle without destroying other rectangles.
        To solve those problems, painting is split into selection painting and unselection painting
        Control of what to do when is done using m_paintSelectionNeeded.

        ***Implementation of selection logic:***
        - if no m_oldSelection:
            ----------------------------------
            select function:
            - m_paintSelectionNeeded to true
            - selectionColor on m_activeSelection 
            ------------------------------------
            onSelectionFinished slot:
            - no unselection if no m_oldSelection
            - m_oldSelection = m_activeSelection

        - if m_oldSelection and m_oldSelection != m_activeSelection:
            ----------------------------------
            select function:
            - m_paintSelectionNeeded to true
            - selectionColor on m_activeSelection
            ------------------------------------
            onSelectionFinished slot:
            - m_paintSelectionNeeded to false
            - unselection and unselectionColor on m_oldSelection
            - m_oldSelection = m_activeSelection

        - if m_activeSelection on m_oldSelection and m_oldSelection isSelected false:
            ----------------------------------
            select function:
            - m_paintSelectionNeeded to true
            - selectionColor on m_activeSelection
            ------------------------------------
            onSelectionFinished slot:
            - no unselection because m_activeSelection on m_oldSelection and m_oldSelection isSelected = false
            - m_oldSelection = m_activeSelection
        
        - if m_activeSelection on m_oldSelection and m_oldSelection isSelected true:
            ----------------------------------
            select function:
            - m_paintSelection to false
            - no selection on m_activeSelection
            ------------------------------------
            onSelectionFinished slot:
            - unselection and unselectionColor on m_activeSelection 
            - m_oldSelection = m_activeSelection
    */
    void WorkoutChart::select()
    {
        if(m_oldSelection.step == nullptr) {
            // No old selection, make new selection
            m_paintSelectionNeeded = true;
            m_activeSelection.step->setSelected(true);
            prepareSelection(&m_activeSelection);
        }
        else if (m_oldSelection.step && !isNewEqOldSelection()) {
            // Old selection elsewhere from new selection, make new selection, deselect old
            m_paintSelectionNeeded = true;
            m_activeSelection.step->setSelected(true);
            prepareSelection(&m_activeSelection);
        }
        else if (isNewEqOldSelection() && !m_oldSelection.step->isSelected()) {
            //New selection on old selection which has been deselected, select again
            m_paintSelectionNeeded = true;
            m_activeSelection.step->setSelected(true);
            prepareSelection(&m_activeSelection);
        }
        else if (isNewEqOldSelection() && m_oldSelection.step->isSelected()) {
            //New selection on old selection which has been selected, deselect
            m_paintSelectionNeeded = false;
            m_activeSelection.step->setSelected(false);
            prepareSelection(&m_activeSelection);
        }
    }
    void WorkoutChart::onSelectionFinished()
    {
        if (m_oldSelection.step == nullptr) {
            // No old selection, done!
            m_oldSelection = m_activeSelection;
        }
        else if (isNewEqOldSelection() == false) {
            // Deselect old selection now
            m_paintSelectionNeeded = false;
            m_oldSelection.step->setSelected(false);
            prepareSelection(&m_oldSelection);
        }
        else if (isNewEqOldSelection() == true) {
            // New selection on old selection, no deselection required, done here
            m_oldSelection = m_activeSelection;
        }
        emit intensityChanged();
        emit durationChanged();
    }
    void WorkoutChart::prepareSelection (Selection* selection) {
        m_selectionRect.setTop(selection->step->top);
        m_selectionRect.setLeft(selection->step->getSpace(selection->repeat).first);
        m_selectionRect.setBottom(getBottom());
        m_selectionRect.setRight(selection->step->getSpace(selection->repeat).second);
        m_paintExtent = PaintExtent::selection;
        QQuickPaintedItem::update(m_selectionRect);
    }
    void WorkoutChart::onIntervalAdd() {
        m_activeSelection.interval = m_intervals->appendInterval();
        setAddStepEnabled(true);
        setRemoveIntervalEnabled(true);
        onStepAdd();
        select();
    }
    int WorkoutChart::getIntervalIndex() {
        int index {};
        for (const auto& interval : m_intervals->intervals()) {
            if (interval == m_activeSelection.interval) {
                return index;
            }
            ++index;
        }
        return -1;
    }
    int WorkoutChart::getStepIndex() {
        if (m_activeSelection.interval == nullptr) return -1;
        int index {0};
        for (const auto& step : m_activeSelection.interval->steps()) {
            if (step == m_activeSelection.step) return index;
            ++index;
        }   
        return -1;
    }
    void WorkoutChart::onIntervalRemove() {
        if (m_activeSelection.interval == nullptr) {
            throw std::runtime_error("This should not happen, no selection!");
        }
        for (const auto& step : m_activeSelection.interval->steps()) {
            m_activeSelection.step = step;
            onStepRemove(true);
        }
        m_intervals->removeInterval(getIntervalIndex());
        m_oldSelection = Selection();
        int remainingIntervals {m_intervals->count()};
        if (remainingIntervals == 0) {
            m_activeSelection = Selection();
            setRemoveStepEnabled(false);
        } 
        else {
            m_activeSelection.interval = m_intervals->atIntervals(remainingIntervals - 1);
            auto steps {m_activeSelection.interval->steps().count()};
            m_activeSelection.step = m_activeSelection.interval->at(steps - 1);
            m_activeSelection.repeat = m_activeSelection.interval->getRepeats();
            select();
        }
        setRemoveIntervalEnabled(false);
        setAddStepEnabled(false);
        updateChart();
    }
    void WorkoutChart::onStepAdd() {
        if (m_activeSelection.interval == nullptr) {
            // either there is no interval or there is none selected
            if (m_intervals->intervals().count() > 0) {
                // select last valid interval
                m_activeSelection.interval = m_intervals->intervals().back();
            }
            else {
                // no interval yet, make one
                onIntervalAdd();
            }
        }
        m_activeSelection.step = m_activeSelection.interval->appendStep();
        m_activeSelection.repeat = 1;
        setRemoveStepEnabled(true);
        select();
    }
    void WorkoutChart::onStepRemove(bool isCalledFromInterval) {
        if (m_activeSelection.interval == nullptr) return;
        int stepIndex {getStepIndex()};
        if (stepIndex >= 0) {
            m_activeSelection.interval->removeStep(stepIndex);
        }
        m_oldSelection = Selection();
        if (stepIndex > 0) {
            m_activeSelection.step = m_activeSelection.interval->at(stepIndex - 1);
            select();
        } else {
            // no steps left, remove interval
            if (!isCalledFromInterval) {
                onIntervalRemove();
                m_activeSelection = Selection();
            }
            setRemoveIntervalEnabled(false);
            setRemoveStepEnabled(false);
        }
        updateChart();
    }
    void WorkoutChart::onRepeatChanged() {
        m_activeSelection.step->setRepeats(getRepeats());
    }

    void WorkoutChart::onFromChanged() {
        m_activeSelection.interval->setFrom(getFrom());
    }

    void WorkoutChart::onOkClicked() {

    }
} // WORKOUT_CHART namespace