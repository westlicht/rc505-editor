#include "RC505.h"
#include "BinaryData.h"
#include "Utils.h"

namespace RC505
{

void ValueProperty::notifyValueChanged()
{
    _listeners.call(&Listener::valueChanged, this);
    if (_library)
    {
        _library->notifyPropertyValueChanged(this);
    }
}

IntProperty::Type IntProperty::DefaultType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    0.f,
    100.f);

IntProperty::Type IntProperty::MidiType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    0.f,
    127.f);

IntProperty::Type IntProperty::LevelType(
    [](int i)
    { return float(i * 2); },
    [](float f)
    { return int(std::floor(f * 0.5f)); },
    [](float f)
    { return String(int(std::floor(f * 0.5f)) * 2); },
    0.f,
    200.f);

IntProperty::Type IntProperty::PanType(
    [](int i)
    { return float(i - 50); },
    [](float f)
    { return int(std::floor(f + 50.f)); },
    [](float f)
    {
        int i = int(std::floor(f));
        if (i < 0)
        {
            return "L" + String(-i);
        }
        else if (i > 0)
        {
            return "R" + String(i);
        }
        else
        {
            return String("CENTER");
        }
    },
    -50.f,
    50.f);

IntProperty::Type IntProperty::TempoType(
    [](int i)
    { return i * 0.1f; },
    [](float f)
    { return int(std::floor(f * 10.f)); },
    [](float f)
    { return String(float(f), 1) + " BPM"; },
    40.f,
    250.f);

IntProperty::Type IntProperty::ThresholdType(
    [](int i)
    { return i - 20.f; },
    [](float f)
    { return int(std::floor(f + 20.f)); },
    [](float f)
    { return String(int(std::floor(f))) + " dB"; },
    -20.f,
    20.f);

IntProperty::Type IntProperty::RateType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    {
        int i = int(std::floor(f));
        switch (i)
        {
            case 101:
                return String("4 Measures");
            case 102:
                return String("2 Measures");
            case 103:
                return String("1 Measures");
            case 104:
                return String("Half Note");
            case 105:
                return String("Dotted Quarter Note");
            case 106:
                return String("Half Note Triplet");
            case 107:
                return String("Quarter Note");
            case 108:
                return String("Dottet Eighth Note");
            case 109:
                return String("Quarter Note Triplet");
            case 110:
                return String("Eighth Note");
            case 111:
                return String("Dotted Sixteenth Note");
            case 112:
                return String("Eighth Note Triplet");
            case 113:
                return String("Sixteenth Note");
            case 114:
                return String("Thirty-second Note");
            default:
                return String(i);
        }
    },
    0.f,
    114.f);

IntProperty::Type IntProperty::StepRateType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    {
        int i = int(std::floor(f));
        switch (i)
        {
            case 0:
                return String("OFF");
            case 102:
                return String("4 Measures");
            case 103:
                return String("2 Measures");
            case 104:
                return String("1 Measures");
            case 105:
                return String("Half Note");
            case 106:
                return String("Dotted Quarter Note");
            case 107:
                return String("Half Note Triplet");
            case 108:
                return String("Quarter Note");
            case 109:
                return String("Dottet Eighth Note");
            case 110:
                return String("Quarter Note Triplet");
            case 111:
                return String("Eighth Note");
            case 112:
                return String("Dotted Sixteenth Note");
            case 113:
                return String("Eighth Note Triplet");
            case 114:
                return String("Sixteenth Note");
            case 115:
                return String("Thirty-second Note");
            default:
                return String(i);
        }
    },
    0.f,
    115.f);

IntProperty::Type IntProperty::DelayTimeType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    {
        int i = int(std::floor(f));
        switch (i)
        {
            case 1001:
                return String("Thirty-second Note");
            case 1002:
                return String("Sixteenth Note");
            case 1003:
                return String("Eighth Note Triplet");
            case 1004:
                return String("Dotted Sixteenth Note");
            case 1005:
                return String("Eighth Note");
            case 1006:
                return String("Quarter Note Triplet");
            case 1007:
                return String("Dottet Eighth Note");
            case 1008:
                return String("Quarter Note");
            case 1009:
                return String("Half Note Tripplet");
            case 1010:
                return String("Dottet Quarter Note");
            case 1011:
                return String("Half Note");
            default:
                return String(i);
        }
    },
    1.f,
    1011.f);

IntProperty::Type IntProperty::BalanceType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    {
        int i = int(std::floor(f));
        return String(100 - i) + ":" + String(i);
    },
    0.f,
    100.f);

IntProperty::Type IntProperty::SamplesType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    0.f,
    1000000000.f);

IntProperty::Type PlaySettings::LoopLengthType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    0.f,
    1000.f);

IntProperty::Type PlaySettings::FadeTimeType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    { return String(int(std::floor(f))) + " Measures"; },
    1.f,
    64.f);

IntProperty::Type LoFiFxSettings::BitDepthType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    {
        int i = int(std::floor(f));
        switch (i)
        {
            case 0:
                return String("OFF");
            default:
                return String(16 - i);
        }
    },
    0.f,
    15.f);

IntProperty::Type LoFiFxSettings::SampleRateType(
    [](int i)
    { return float(i); },
    [](float f)
    { return int(std::floor(f)); },
    [](float f)
    {
        int i = int(std::floor(f));
        switch (i)
        {
            case 0:
                return String("OFF");
            default:
                return String("1/") + String(i + 1);
        }
    },
    0.f,
    31.f);

IntProperty::Type TransposeFxSettings::TransType(
    [](int i)
    { return i - 12.f; },
    [](float f)
    { return int(std::floor(f + 12.f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    -12.f,
    12.f);

IntProperty::Type RobotFxSettings::GenderType(
    [](int i)
    { return i - 10.f; },
    [](float f)
    { return int(std::floor(f + 10.f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    -10.f,
    10.f);

IntProperty::Type VocalDistFxSettings::ToneType(
    [](int i)
    { return i - 50.f; },
    [](float f)
    { return int(std::floor(f + 50.f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    -50.f,
    50.f);

IntProperty::Type ReverbFxSettings::TimeType(
    [](int i)
    { return (i + 1) * 0.1f; },
    [](float f)
    { return int(std::floor(f * 10.f) - 1); },
    [](float f)
    { return String(float(f), 1); },
    0.1f,
    10.0f);

IntProperty::Type SetupSettings::MemNumType(
    [](int i)
    { return i + 1.f; },
    [](float f)
    { return int(std::floor(f - 1.f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    1.f,
    99.f);

IntProperty::Type SetupSettings::LcdContrastType(
    [](int i)
    { return i + 1.f; },
    [](float f)
    { return int(std::floor(f - 1.f)); },
    [](float f)
    { return String(int(std::floor(f))); },
    1.f,
    16.f);

StringArray RhythmSettings::PatternNames({ "Simple Beat 1", "Simple Beat 2", "Simple Beat 3", "Simple Beat 4", "Simple Beat 5", "Downbeat Snare", "Rock 1", "Rock 2", "Rock 3", "Rock 4", "Rock 5", "Latin Rock 1", "Latin Rock 2", "Groove Beat 1", "Groove Beat 2", "Groove Beat 3", "Groove Beat 4", "Groove Beat 5", "Shuffle 1", "Shuffle 2", "Shuffle 3", "Shuffle 4", "Shuffle 5", "Shuffle 6", "1/2 Shuffle 1", "1/2 Shuffle 2", "16th Shuffle 1", "16th Shuffle 2", "Shuffle Reggae", "Pop 1", "Pop 2", "Pop 3", "Funk 1", "Funk 2", "Funk 3", "Fusion 1", "Fusion 2", "Fusion 3", "Swing", "Bossa 1", "Bossa 2", "Bossa 3", "Samba 1", "Samba 2", "Samba 3", "Clave 1", "Clave 2", "909 Beat", "909 Clap", "R&B 1", "R&B 2", "Hi-Hat", "Kick & Hi-Hat 1", "Kick & Hi-Hat 2", "Conga & Hi-Hat", "Conga & Maracas", "Metronome 1", "Metronome 2" });
StringArray AssignmentSettings::SourceNames({ "EXP PEDAL", "CTL1 PEDAL", "CTL2 PEDAL", "INPUT FX KNOB", "TRACK FX KNOB", "TR1 REC/DUB", "TR2 REC/DUB", "TR3 REC/DUB", "TR4 REC/DUB", "TR5 REC/DUB", "TR1 PLAY/STOP", "TR2 PLAY/STOP", "TR3 PLAY/STOP", "TR4 PLAY/STOP", "TR5 PLAY/STOP", "SYNC START/STOP", "#CC 1", "#CC 2", "#CC 3", "#CC 4", "#CC 5", "#CC 6", "#CC 7", "#CC 8", "#CC 9", "#CC 10", "#CC 11", "#CC 12", "#CC 13", "#CC 14", "#CC 15", "#CC 16", "#CC 17", "#CC 18", "#CC 19", "#CC 20", "#CC 21", "#CC 22", "#CC 23", "#CC 24", "#CC 25", "#CC 26", "#CC 27", "#CC 28", "#CC 29", "#CC 30", "#CC 31", "#CC 64", "#CC 65", "#CC 66", "#CC 67", "#CC 68", "#CC 69", "#CC 70", "#CC 71", "#CC 72", "#CC 73", "#CC 74", "#CC 75", "#CC 76", "#CC 77", "#CC 78", "#CC 79", "#CC 80", "#CC 81", "#CC 82", "#CC 83", "#CC 84", "#CC 85", "#CC 86", "#CC 87", "#CC 88", "#CC 89", "#CC 90", "#CC 91", "#CC 92", "#CC 93", "#CC 94", "#CC 95" });

StringArray AssignmentSettings::TargetNames({ "TR1 REC/PLAY", "TR2 REC/PLAY", "TR3 REC/PLAY", "TR4 REC/PLAY", "TR5 REC/PLAY", "TR1 PLAY/STOP", "TR2 PLAY/STOP", "TR3 PLAY/STOP", "TR4 PLAY/STOP", "TR5 PLAY/STOP", "TR1 CLEAR", "TR2 CLEAR", "TR3 CLEAR", "TR4 CLEAR", "TR5 CLEAR", "TR1 UNDO/REDO", "TR2 UNDO/REDO", "TR3 UNDO/REDO", "TR4 UNDO/REDO", "TR5 UNDO/REDO", "TR1 PLAY LEVEL", "TR2 PLAY LEVEL", "TR3 PLAY LEVEL", "TR4 PLAY LEVEL", "TR5 PLAY LEVEL", "TR1 PAN", "TR2 PAN", "TR3 PAN", "TR4 PAN", "TR5 PAN", "TGT TR REC/PLAY", "TGT TR STOP", "TGT TR CLEAR", "TGT TR UNDO/REDO", "TGT TR PLAY LEV", "ALL START/STOP", "ALL CLEAR", "UNDO/REDO", "RHYTHM LEVEL", "RHYYHM PATTERN", "MEMORY LEVEL", "TEMPO (TAP)", "MASTER COMP", "MASTER REVERB", "OVERDUB MODE", "INPUT FX ON/OFF", "TRACK FX ON/OFF", "INPUT FX A CONTROL", "INPUT FX B CONTROL", "INPUT FX C CONTROL", "TRACK FX A CONTROL", "TRACK FX B CONTROL", "TRACK FX C CONTROL", "INPUT FX A TYP INC", "INPUT FX A TYP DEC", "INPUT FX B TYP INC", "INPUT FX B TYP DEC", "INPUT FX C TYP INC", "INPUT FX C TYP DEC", "TRACK FX A TYP INC", "TRACK FX A TYP DEC", "TRACK FX B TYP INC", "TRACK FX B TYP DEC", "TRACK FX C TYP INC", "TRACK FX C TYP DEC", "INPUT FX CONTROL", "TRACK FX CONTROL", "INPUT FX TYP INC", "INPUT FX TYP DEC", "TRACK FX TYP INC", "TRACK FX TYP DEC", "INPUT FX INC", "INPUT FX DEC", "TRACK FX INC", "TRACK FX DEC", "TARGET TRACK INC", "TARGET TRACK DEC", "MEMORY INC", "MEMORY DEC", "INPUT LEVEL", "#CC 1", "#CC 2", "#CC 3", "#CC 4", "#CC 5", "#CC 6", "#CC 7", "#CC 8", "#CC 9", "#CC 10", "#CC 11", "#CC 12", "#CC 13", "#CC 14", "#CC 15", "#CC 16", "#CC 17", "#CC 18", "#CC 19", "#CC 20", "#CC 21", "#CC 22", "#CC 23", "#CC 24", "#CC 25", "#CC 26", "#CC 27", "#CC 28", "#CC 29", "#CC 30", "#CC 31", "#CC 64", "#CC 65", "#CC 66", "#CC 67", "#CC 68", "#CC 69", "#CC 70", "#CC 71", "#CC 72", "#CC 73", "#CC 74", "#CC 75", "#CC 76", "#CC 77", "#CC 78", "#CC 79", "#CC 80", "#CC 81", "#CC 82", "#CC 83", "#CC 84", "#CC 85", "#CC 86", "#CC 87", "#CC 88", "#CC 89", "#CC 90", "#CC 91", "#CC 92", "#CC 93", "#CC 94", "#CC 95" });
StringArray InputFxSettings::SingleFxNames({ "FILTER", "PHASER", "FLANGER", "SYNTH", "LO-FI", "RING MODULATOR", "GUITAR TO BASS", "SLOW GEAR", "TRANSPOSE", "PITCH BEND", "ROBOT", "VOCAL DIST", "VOCODER", "DYNAMICS", "EQ", "ISOLATOR", "OCTAVE", "PAN", "TREMOLO", "SLICER", "DELAY", "PANNING DELAY", "TAPE ECHO", "GRANUAR DELAY", "ROLL", "CHORUS", "REVERB" });
StringArray TrackFxSettings::SingleFxNames({ "FILTER", "PHASER", "FLANGER", "SYNTH", "LO-FI", "RING MODULATOR", "GUITAR TO BASS", "SLOW GEAR", "TRANSPOSE", "PITCH BEND", "ROBOT", "VOCAL DIST", "VOCODER", "DYNAMICS", "EQ", "ISOLATOR", "OCTAVE", "PAN", "TREMOLO", "SLICER", "DELAY", "PANNING DELAY", "TAPE ECHO", "GRANUAR DELAY", "ROLL", "CHORUS", "REVERB", "BEAT REPEAT", "BEAT SHIFT", "BEAT SCATTER", "VINYL FLICK" });

StringArray InputFxSettings::MultiFxNames[3] = {
    StringArray({ "FILTER", "PHASER", "SYNTH", "LO-FI", "RING MODULATOR", "GUITAR TO BASS", "SLOW GEAR", "TRANSPOSE", "PITCH BEND", "ROBOT", "VOCAL DIST", "VOCODER", "DYNAMICS", "EQ", "ISOLATOR", "OCTAVE", "PAN", "TREMOLO", "SLICER" }),
    StringArray({ "FILTER", "PHASER", "FLANGER", "LO-FI", "RING MODULATOR", "VOCAL DIST", "EQ", "ISOLATOR", "PAN", "TREMOLO", "SLICER", "DELAY", "PANNING DELAY", "ROLL", "CHORUS" }),
    StringArray({ "FILTER", "PHASER", "FLANGER", "LO-FI", "RING MODULATOR", "VOCAL DIST", "EQ", "ISOLATOR", "PAN", "TREMOLO", "SLICER", "DELAY", "PANNING DELAY", "TAPE ECHO", "GRANUAR DELAY", "ROLL", "CHORUS", "REVERB" })
};

StringArray TrackFxSettings::MultiFxNames[3] = {
    StringArray({ "FILTER", "PHASER", "SYNTH", "LO-FI", "RING MODULATOR", "GUITAR TO BASS", "SLOW GEAR", "TRANSPOSE", "PITCH BEND", "ROBOT", "VOCAL DIST", "VOCODER", "DYNAMICS", "EQ", "ISOLATOR", "OCTAVE", "PAN", "TREMOLO", "SLICER", "BEAT REPEAT", "BEAT SHIFT", "BEAT SCATTER", "VINYL FLICK" }),
    StringArray({ "FILTER", "PHASER", "FLANGER", "LO-FI", "RING MODULATOR", "VOCAL DIST", "EQ", "ISOLATOR", "PAN", "TREMOLO", "SLICER", "DELAY", "PANNING DELAY", "ROLL", "CHORUS" }),
    StringArray({ "FILTER", "PHASER", "FLANGER", "LO-FI", "RING MODULATOR", "VOCAL DIST", "EQ", "ISOLATOR", "PAN", "TREMOLO", "SLICER", "DELAY", "PANNING DELAY", "TAPE ECHO", "GRANUAR DELAY", "ROLL", "CHORUS", "REVERB" })
};

// ----------------------------------------------------------------------------
// Track
// ----------------------------------------------------------------------------

Track::Track(Library* library, Patch& patch, int index)
    : _library(library)
    , _patch(patch)
    , _index(index)
    , _trackSettings(library, "Track")
{
}

int Track::index() const
{
    return _index;
}

void Track::setIndex(int index)
{
    _index = index;
}

const AudioSampleBuffer& Track::audioBuffer() const
{
    if (_waveState == WaveOriginal && _audioBuffer.getNumSamples() == 0)
    {
        Utils::readAudioFile(_originalWaveFile, _audioBuffer);
    }
    return _audioBuffer;
}

void Track::setAudioBuffer(const AudioSampleBuffer& audioBuffer)
{
    _waveState = WaveChanged;
    _audioBuffer = audioBuffer;
    _trackSettings.wavStat->setValue(_audioBuffer.getNumSamples() > 0);
    _trackSettings.wavLen->setValue(_audioBuffer.getNumSamples());
    _patch.setWaveChanged();
}

void Track::clearAudioBuffer()
{
    _waveState = WaveEmpty;
    _audioBuffer.setSize(0, 0);
    _trackSettings.wavStat->setValue(false);
    _trackSettings.wavLen->setValue(0);
    _patch.setWaveChanged();
}

void Track::saveWaveTo(const File& file) const
{
    switch (_waveState)
    {
        case WaveEmpty:
            break;
        case WaveOriginal:
            _originalWaveFile.copyFileTo(file);
            break;
        case WaveChanged:
            Utils::writeAudioFile(file, _audioBuffer);
            break;
    }
}

bool Track::loadFromXml(XmlElement* xml)
{
    bool result = _trackSettings.loadFromXml(xml);

    // find wave file
    _waveState = WaveEmpty;
    _originalWaveFile = File();

    if (_library->wavePath().exists())
    {
        File wavePath = File::addTrailingSeparator(_library->wavePath().getFullPathName()) + String::formatted("%03d_%d", _patch.index() + 1, _index + 1);
        Array<File> files;
        wavePath.findChildFiles(files, File::findFiles, false, "*.WAV");
        if (files.size() > 0)
        {
            _waveState = WaveOriginal;
            _originalWaveFile = files[0];
        }
    }

    return result;
}

bool Track::saveToXml(XmlElement* xml)
{
    return _trackSettings.saveToXml(xml);
}

// ----------------------------------------------------------------------------
// Patch
// ----------------------------------------------------------------------------

Patch::Patch(Library* library)
    : _library(library)
    , _hasWaveChanged(false)
    , _patchSettings(library)
    , _patchName(library, "Name", "NAME")
{
    for (int i = 0; i < NumTracks; ++i)
    {
        _tracks.add(new Track(library, *this, i));
    }
}

Patch::Patch(const RC505::Patch& patch)
    : _library(patch._library)
    , _patchSettings(patch._library)
    , _patchName(patch._library, "Name", "NAME")
{
    for (int i = 0; i < NumTracks; ++i)
    {
        _tracks.add(new Track(_library, *this, i));
    }
}

int Patch::index() const
{
    return _index;
}

void Patch::setIndex(int index)
{
    _index = index;
}

String Patch::name() const
{
    return _patchName.value();
}

void Patch::setName(const String& name)
{
    _patchName.setValue(name);
}

void Patch::clear()
{
    int oldIndex = _index;
    loadFromXml(_library->_factoryPatchXml.get());
    _index = oldIndex;
    for (const auto& track : _tracks)
    {
        track->clearAudioBuffer();
    }
    _patchName.notifyValueChanged();
    setWaveChanged();
}

void Patch::clearWaveChanged()
{
    _hasWaveChanged = false;
    _library->setChanged();
}

void Patch::setWaveChanged()
{
    _hasWaveChanged = true;
    _patchName.notifyValueChanged();
    _library->setChanged();
}

bool Patch::allTracksEmpty() const
{
    for (const auto& track : _tracks)
    {
        if (track->waveState() != Track::WaveEmpty)
        {
            return false;
        }
    }
    return true;
}

void Patch::moveTrack(int from, int to)
{
    _tracks.swap(from, to);
    setWaveChanged();
}

bool Patch::loadFromXml(XmlElement* xml)
{
    _index = xml->getIntAttribute("id");
    for (int i = 0; i < NumTracks; ++i)
    {
        _tracks[i]->loadFromXml(xml->getChildByName(String::formatted("TRACK%d", i + 1)));
    }
    _patchName.loadFromXml(xml->getChildByName("NAME"));
    _patchSettings.loadFromXml(xml);
    return true;
}

bool Patch::saveToXml(XmlElement* xml)
{
    xml->setAttribute("id", _index);
    for (int i = 0; i < NumTracks; ++i)
    {
        _tracks[i]->saveToXml(xml->createNewChildElement(String::formatted("TRACK%d", i + 1)));
    }
    _patchName.saveToXml(xml->createNewChildElement("NAME"));
    _patchSettings.saveToXml(xml);
    return true;
}

// ----------------------------------------------------------------------------
// Library
// ----------------------------------------------------------------------------

Library::Library()
    : _systemSettings(this)
{
    init();
}

void Library::setName(const String& name)
{
    if (_name != name)
    {
        _name = name;
        notifyLocked([&]()
                     { _listeners.call(&Listener::libraryChanged); });
    }
}

String Library::documentName() const
{
    return _name + (_hasChanged ? " *" : "");
}

void Library::clearChanged()
{
    if (_hasChanged)
    {
        _hasChanged = false;
        notifyLocked([&]()
                     { _listeners.call(&Listener::libraryChanged); });
    }
}

void Library::setChanged()
{
    if (!_hasChanged)
    {
        _hasChanged = true;
        notifyLocked([&]()
                     { _listeners.call(&Listener::libraryChanged); });
    }
}

void Library::movePatches(const Array<Patch*>& patches, int insertIndex)
{
    if (patches.size() > 0)
    {
        for (auto patch : patches)
        {
            int fromIndex = _patches.indexOf(patch);
            if (insertIndex > fromIndex)
            {
                --insertIndex;
            }
            jassert(fromIndex >= 0);
            _patches.move(fromIndex, insertIndex);
            insertIndex = _patches.indexOf(patch) + 1;
        }
    }
    for (int i = 0; i < _patches.size(); ++i)
    {
        const auto& patch = _patches[i];
        if (patch->index() != i)
        {
            patch->setIndex(i);
            if (!patch->allTracksEmpty())
            {
                patch->setWaveChanged();
            }
        }
    }
}

void Library::init()
{
    notifyLocked([&]()
                 { _listeners.call(&Listener::beforeLibraryLoaded); });

    setName("New Library");
    clearChanged();
    loadMemory(String(BinaryData::MEMORY_RC0, BinaryData::MEMORY_RC0Size));
    loadSystem(String(BinaryData::SYSTEM_RC0, BinaryData::SYSTEM_RC0Size));

    _factoryPatchXml = std::make_unique<XmlElement>("mem");
    _patches[0]->saveToXml(_factoryPatchXml.get());

    notifyLocked([&]()
                 { _listeners.call(&Listener::afterLibraryLoaded); });
}

bool Library::load(const File& path, StatusCallback statusCallback)
{
    notifyLocked([&]()
                 { _listeners.call(&Listener::beforeLibraryLoaded); });

    setPath(path);
    if (!_dataPath.exists() || !_dataPath.isDirectory())
    {
        return false;
    }
    if (!_wavePath.exists() || !_wavePath.isDirectory())
    {
        return false;
    }

    statusCallback("Loading memory database ...");
    File memoryPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "MEMORY.RC0";
    if (!loadMemory(memoryPath))
    {
        Logger::outputDebugString("Failed to load memory settings from '" + memoryPath.getFullPathName() + "'");
        return false;
    }

    statusCallback("Loading system database ...");
    File systemPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "SYSTEM.RC0";
    if (!loadSystem(systemPath))
    {
        Logger::outputDebugString("Failed to load system settings from '" + systemPath.getFullPathName() + "'");
        return false;
    }

    setName(path.getFullPathName());
    clearChanged();

    notifyLocked([&]()
                 { _listeners.call(&Listener::afterLibraryLoaded); });

    return true;
}

bool Library::save(const File& path, StatusCallback statusCallback)
{
    notifyLocked([&]()
                 { _listeners.call(&Listener::beforeLibrarySaved); });

    bool inplace = path == _path;
    setPath(path);
    if (!_dataPath.exists())
    {
        _dataPath.createDirectory();
    }
    if (!_wavePath.exists())
    {
        _wavePath.createDirectory();
    }
    if (!_dataPath.isDirectory() || !_wavePath.isDirectory())
    {
        return false;
    }

    statusCallback("Saving memory database ...");
    File memoryPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "MEMORY.RC0";
    if (!saveMemory(memoryPath))
    {
        Logger::outputDebugString("Failed to save memory settings to '" + memoryPath.getFullPathName() + "'");
        return false;
    }

    statusCallback("Saving system database ...");
    File systemPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "SYSTEM.RC0";
    if (!saveSystem(systemPath))
    {
        Logger::outputDebugString("Failed to save system settings to '" + systemPath.getFullPathName() + "'");
        return false;
    }

    if (!saveWaveFiles(inplace, statusCallback))
    {
        return false;
    }

    setName(path.getFullPathName());
    clearChanged();

    notifyLocked([&]()
                 { _listeners.call(&Listener::afterLibrarySaved); });

    return true;
}

void Library::close()
{
    notifyLocked([&]()
                 { _listeners.call(&Listener::libraryClosed); });
}

Library::Info Library::libraryInfo(const File& path)
{
    Info info = { false, -1 };

    File dataPath = File::addTrailingSeparator(path.getFullPathName()) + "DATA";
    File memoryPath = File::addTrailingSeparator(dataPath.getFullPathName()) + "MEMORY.RC0";
    if (dataPath.exists() && memoryPath.exists())
    {
        std::unique_ptr<XmlElement> xml(XmlDocument::parse(memoryPath.loadFileAsString()));
        if (xml && xml->hasAttribute("revision"))
        {
            info.valid = true;
            info.revision = xml->getIntAttribute("revision");
        }
    }

    return info;
}

String Library::checkVolumesForRC505()
{
#if JUCE_MAC
    File volumesPath("/Volumes");
    Array<File> results;
    volumesPath.findChildFiles(results, File::findDirectories, false);
    for (const auto& path : results)
    {
        if (path.getFileName() == "BOSS_RC-505")
        {
            String fullPath = File::addTrailingSeparator(path.getFullPathName()) + "ROLAND";
            if (File(fullPath).exists())
            {
                return fullPath;
            }
        }
    }
#endif
    return String();
}

String Library::tempDirectory()
{
    File path(File::addTrailingSeparator(File::getSpecialLocation(File::tempDirectory).getFullPathName()) + "RC505");
    if (!path.exists() || !path.isDirectory())
    {
        path.deleteFile();
        path.createDirectory();
    }
    return path.getFullPathName();
}

void Library::setPath(const File& path)
{
    _path = path;
    _dataPath = File::addTrailingSeparator(_path.getFullPathName()) + "DATA";
    _wavePath = File::addTrailingSeparator(_path.getFullPathName()) + "WAVE";
}

bool Library::loadMemory(const File& path)
{
    DBG("Loading Memory XML ...");
    return loadMemory(path.loadFileAsString());
}

bool Library::loadMemory(const String& data)
{
    DBG("Parsing Memory XML ...");
    std::unique_ptr<XmlElement> xml(XmlDocument::parse(data));
    if (!xml)
    {
        return false;
    }
    if (!xml->hasTagName("database"))
    {
        return false;
    }
    _patches.clear();
    for (int i = 0; i < xml->getNumChildElements(); ++i)
    {
        XmlElement* xmlChild = xml->getChildElement(i);
        if (!xmlChild->hasTagName("mem") || !xmlChild->hasAttribute("id"))
        {
            return false;
        }
        Patch* patch = new Patch(this);
        if (!patch->loadFromXml(xmlChild))
        {
            return false;
        }
        _patches.add(patch);
    }
    return true;
}

bool Library::saveMemory(const File& path)
{
    DBG("Generating Memory XML ...");
    std::unique_ptr<XmlElement> xml = std::make_unique<XmlElement>("database");
    xml->setAttribute("name", "RC-505");
    xml->setAttribute("revision", Revision);
    for (auto patch : _patches)
    {
        if (!patch->saveToXml(xml->createNewChildElement("mem")))
        {
            return false;
        }
    }
    DBG("Saving Memory XML ...");
    return xml->writeTo(path);
}

bool Library::loadSystem(const File& file)
{
    DBG("Loading System XML ...");
    return loadSystem(file.loadFileAsString());
}

bool Library::loadSystem(const String& data)
{
    DBG("Parsing System XML ...");
    std::unique_ptr<XmlElement> xml(XmlDocument::parse(data));
    if (!xml)
    {
        return false;
    }
    if (!xml->hasTagName("database"))
    {
        return false;
    }
    return _systemSettings.loadFromXml(xml->getChildByName("sys"));
}

bool Library::saveSystem(const File& path)
{
    DBG("Generating System XML ...");
    std::unique_ptr<XmlElement> xml = std::make_unique<XmlElement>("database");
    xml->setAttribute("name", "RC-505");
    xml->setAttribute("revision", Revision);
    if (!_systemSettings.saveToXml(xml->createNewChildElement("sys")))
    {
        return false;
    }
    DBG("Saving System XML ...");
    return xml->writeTo(path);
}

bool Library::saveWaveFiles(bool inplace, StatusCallback statusCallback)
{
    auto finalWaveDir = [&](int patchIndex, int trackIndex)
    {
        return File(File::addTrailingSeparator(_wavePath.getFullPathName()) + String::formatted("%03d_%d", patchIndex + 1, trackIndex + 1));
    };

    auto temporaryWaveDir = [&](int patchIndex, int trackIndex)
    {
        return File(File::addTrailingSeparator(_wavePath.getFullPathName()) + String::formatted("temp_%03d_%d", patchIndex + 1, trackIndex + 1));
    };

    auto temporaryWaveFile = [&](int patchIndex, int trackIndex)
    {
        return File(File::addTrailingSeparator(temporaryWaveDir(patchIndex, trackIndex).getFullPathName()) + String::formatted("%03d_%d.WAV", patchIndex + 1, trackIndex + 1));
    };

    Array<bool> patchWrite;
    int patchWriteCount = 0;
    for (int patchIndex = 0; patchIndex < NumPatches; ++patchIndex)
    {
        bool changed = inplace ? _patches[patchIndex]->hasWaveChanged() : true;
        patchWrite.add(changed);
        patchWriteCount += changed ? 1 : 0;
    }
    DBG(String::formatted("Writing %d patches ...", patchWriteCount));

    // create folders x_xxx and temp_x_xxx
    for (int patchIndex = 0; patchIndex < NumPatches; ++patchIndex)
    {
        if (!patchWrite[patchIndex])
        {
            continue;
        }
        DBG(String::formatted("Creating folders for patch %d ...", patchIndex));
        statusCallback(String::formatted("Preparing patch %d ...", patchIndex + 1));
        for (int trackIndex = 0; trackIndex < Patch::NumTracks; ++trackIndex)
        {
            finalWaveDir(patchIndex, trackIndex).createDirectory();
            temporaryWaveDir(patchIndex, trackIndex).createDirectory();
        }
    }

    // store wave files to temporary directories
    for (int patchIndex = 0; patchIndex < NumPatches; ++patchIndex)
    {
        if (!patchWrite[patchIndex])
        {
            continue;
        }
        DBG(String::formatted("Writing wave files to temporary folder for patch %d ...", patchIndex));
        statusCallback(String::formatted("Writing patch %d ...", patchIndex + 1));
        for (int trackIndex = 0; trackIndex < Patch::NumTracks; ++trackIndex)
        {
            auto track = _patches[patchIndex]->tracks()[trackIndex];
            switch (track->waveState())
            {
                case Track::WaveEmpty:
                    // do nothing
                    break;
                case Track::WaveOriginal:
                    // move/copy files
                    if (!inplace || !track->originalWaveFile().moveFileTo(temporaryWaveFile(patchIndex, trackIndex)))
                    {
                        track->originalWaveFile().copyFileTo(temporaryWaveFile(patchIndex, trackIndex));
                    }
                    break;
                case Track::WaveChanged:
                    // write new file
                    Utils::writeAudioFile(temporaryWaveFile(patchIndex, trackIndex), track->audioBuffer());
                    break;
            }
        }
    }

    // replace original folders with temporary folders
    for (int patchIndex = 0; patchIndex < NumPatches; ++patchIndex)
    {
        if (!patchWrite[patchIndex])
        {
            continue;
        }
        DBG(String::formatted("Moving wave files to original folder for patch %d ...", patchIndex));
        statusCallback(String::formatted("Finalizing patch %d ...", patchIndex + 1));
        for (int trackIndex = 0; trackIndex < Patch::NumTracks; ++trackIndex)
        {
            finalWaveDir(patchIndex, trackIndex).deleteRecursively();
            temporaryWaveDir(patchIndex, trackIndex).moveFileTo(finalWaveDir(patchIndex, trackIndex));
        }
    }

    return true;
}

void Library::notifyPropertyValueChanged(ValueProperty* property)
{
    notifyLocked([&]()
                 { _listeners.call(&Listener::propertyValueChanged, property); });
    setChanged();
}

} // namespace RC505
