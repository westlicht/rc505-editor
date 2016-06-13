#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_LINUX

// Dummy class because JUCE does not provide linux implementation
class MountedVolumeListChangeDetector {
public:
	MountedVolumeListChangeDetector() {}

	virtual void mountedVolumeListChanged() = 0;
};

#endif