#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class TrackLabel : public Component, public DragAndDropContainer, public DragAndDropTarget {
public:
    class Listener {
    public:
        virtual ~Listener() {}
        virtual void trackMoved(int from, int to) = 0;
    };

    TrackLabel(int index);
    ~TrackLabel();

    void addListener(Listener *listener) { _listeners.add(listener); }
    void removeListener(Listener *listener) { _listeners.remove(listener); }

    void paint(Graphics &g);

    void mouseDrag(const MouseEvent &event) override;

    bool isInterestedInDragSource(const SourceDetails &dragSourceDetails) override;
    void itemDropped(const SourceDetails &dragSourceDetails) override;

private:
    int _index;
    ListenerList<Listener> _listeners;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TrackLabel)
};
