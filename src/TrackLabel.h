#pragma once

#include "JuceHeader.h"

class TrackLabel : public Component,
                   public SettableTooltipClient,
                   public DragAndDropContainer,
                   public DragAndDropTarget
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void trackMoved(int from, int to) = 0;
    };

    TrackLabel(int index);
    ~TrackLabel() override;

    void addListener(Listener* listener) { _listeners.add(listener); }
    void removeListener(Listener* listener) { _listeners.remove(listener); }

    // Component
    virtual void paint(Graphics& g) override;
    virtual void mouseDrag(const MouseEvent& event) override;

    // DragAndDropTarget
    virtual bool isInterestedInDragSource(const SourceDetails& dragSourceDetails) override;
    virtual void itemDropped(const SourceDetails& dragSourceDetails) override;

private:
    int _index;
    ListenerList<Listener> _listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TrackLabel)
};
