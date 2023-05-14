#pragma once

#include "JuceHeader.h"

class PlayButton : public Button
{
public:
    enum ColourIds
    {
        playColourId = 0x2360000,
        stopColourId = 0x2360001,
    };

    PlayButton()
        : Button("Play")
    {
    }

    bool isPlaying() const { return _playing; }
    void setPlaying(bool playing)
    {
        _playing = playing;
        repaint();
    }

protected:
    virtual void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override
    {
        getLookAndFeel().drawButtonBackground(g, *this, getLookAndFeel().findColour(TextButton::buttonColourId), isMouseOverButton, isButtonDown);

        int cx = getWidth() / 2;
        int cy = getHeight() / 2;
        if (_playing)
        {
            // draw stop icon
            g.setColour(findColour(stopColourId));
            g.fillRect(cx - 5, cy - 5, 10, 10);
        }
        else
        {
            // draw play icon
            Path path;
            path.startNewSubPath(cx - 5, cy - 5);
            path.lineTo(cx + 5, cy);
            path.lineTo(cx - 5, cy + 5);
            path.closeSubPath();
            g.setColour(findColour(playColourId));
            g.fillPath(path);
        }
    }

private:
    bool _playing;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayButton)
};
