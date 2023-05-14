#include "TrackLabel.h"
#include "CustomLookAndFeel.h"
#include "JuceHeader.h"

TrackLabel::TrackLabel(int index)
    : _index(index)
{
}

TrackLabel::~TrackLabel()
{
}

void TrackLabel::paint(Graphics& g)
{
    g.setColour(findColour(mainTextColourId));
    g.setFont(25.f);
    g.drawText(String::formatted("%d", _index + 1), getLocalBounds(), Justification::centred, true);
}

void TrackLabel::mouseDrag(const MouseEvent& event)
{
    ignoreUnused(event);
    var description(Array<var>({ var("Track"), var(_index) }));
    dynamic_cast<DragAndDropContainer*>(getTopLevelComponent())->startDragging(description, this);
}

bool TrackLabel::isInterestedInDragSource(const SourceDetails& dragSourceDetails)
{
    var description = dragSourceDetails.description;
    return description.isArray() && description.size() == 2 && description[0] == "Track";
}

void TrackLabel::itemDropped(const SourceDetails& dragSourceDetails)
{
    int from = int(dragSourceDetails.description[1]);
    int to = _index;
    if (from != to)
    {
        _listeners.call(&Listener::trackMoved, from, to);
    }
}
