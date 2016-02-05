#include "RC505.h"
#include "Utils.h"

namespace RC505 {

void ValueProperty::notifyValueChanged()
{
    _listeners.call(&Listener::valueChanged, this);
    if (_library) {
        _library->notifyPropertyValueChanged(this);
    }
}

IntProperty::Type IntProperty::DefaultType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { return String(int(std::floor(f))); },
    0.f,
    100.f
);

IntProperty::Type IntProperty::MidiType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { return String(int(std::floor(f))); },
    0.f,
    127.f
);

IntProperty::Type IntProperty::LevelType(
    [] (int i) { return float(i*2); },
    [] (float f) { return int(std::floor(f*0.5f)); },
    [] (float f) { return String(int(std::floor(f*0.5f)) * 2); },
    0.f,
    200.f
);

IntProperty::Type IntProperty::PanType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) {
        int i = int(std::floor(f));
        if (i < 50) {
            return "L" + String(50 - i);
        } else if (i > 50) {
            return "R" + String(i - 50);
        } else {
            return String("CENTER");
        }
    },
    0.f,
    100.f
);

IntProperty::Type IntProperty::TempoType(
    [] (int i) { return i*0.1f; },
    [] (float f) { return int(std::floor(f*10.f)); },
    [] (float f) { return String(float(f),1) + " BPM"; },
    40.f,
    250.f
);

IntProperty::Type IntProperty::ThresholdType(
    [] (int i) { return i-20.f; },
    [] (float f) { return int(std::floor(f+20.f)); },
    [] (float f) { return String(int(std::floor(f))) + " dB"; },
    -20.f,
    20.f
);

IntProperty::Type IntProperty::RateType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { 
        int i = int(std::floor(f));
        switch (i) {
        case 101: return String("4 MEASURE");
        case 102: return String("2 MEASURE");
        case 103: return String("1 MEASURE");
        case 104: return String("Half Note");
        case 105: return String("Quarter Note");
        case 106: return String("Eighth Note");
        case 107: return String("Sixteenth Note");
        default: return String(i);
        }
    },
    0.f,
    107.f
);

IntProperty::Type IntProperty::BalanceType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) {
        int i = int(std::floor(f));
        return String(100 - i) + ":" + String(i);
    },
    0.f,
    100.f
);

IntProperty::Type IntProperty::SamplesType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { return String(int(std::floor(f))); },
    0.f,
    1000000000.f
);

IntProperty::Type PlaySettings::LoopLengthType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { return String(int(std::floor(f))); },
    0.f,
    1000.f
);

IntProperty::Type PlaySettings::FadeTimeType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { return String(int(std::floor(f))) + " MEASURES"; },
    1.f,
    64.f
);

IntProperty::Type LoFiFxSettings::BitDepthType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { 
        int i = int(std::floor(f));
        switch (i) {
        case 0: return String("OFF");
        default: return String(16 - i);
        }
    },
    0.f,
    15.f
);

IntProperty::Type LoFiFxSettings::SampleRateType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { 
        int i = int(std::floor(f));
        switch (i) {
        case 0: return String("OFF");
        default: return String("1/") + String(i + 1);
        }
    },
    0.f,
    31.f
);

IntProperty::Type TransposeFxSettings::TransType(
    [] (int i) { return i-12.f; },
    [] (float f) { return int(std::floor(f+12.f)); },
    [] (float f) { return String(int(std::floor(f))); },
    -12.f,
    12.f
);

IntProperty::Type RobotFxSettings::GenderType(
    [] (int i) { return i-10.f; },
    [] (float f) { return int(std::floor(f+10.f)); },
    [] (float f) { return String(int(std::floor(f))); },
    -10.f,
    10.f
);

IntProperty::Type VocalDistFxSettings::ToneType(
    [] (int i) { return i-50.f; },
    [] (float f) { return int(std::floor(f+50.f)); },
    [] (float f) { return String(int(std::floor(f))); },
    -50.f,
    50.f
);

IntProperty::Type DelayFxSettings::TimeType(
    [] (int i) { return float(i); },
    [] (float f) { return int(std::floor(f)); },
    [] (float f) { 
        int i = int(std::floor(f));
        switch (i) {
        case 1001: return String("Sixteenth Note");
        case 1002: return String("Eighth Note");
        case 1003: return String("Quarter Note");
        case 1004: return String("Half Note");
        default: return String(i);
        }
    },
    1.f,
    1004.f
);

IntProperty::Type ReverbFxSettings::TimeType(
    [] (int i) { return (i+1)*0.1f; },
    [] (float f) { return int(std::floor(f*10.f)-1); },
    [] (float f) { return String(float(f),1); },
    0.1f,
    10.0f
);

IntProperty::Type SetupSettings::MemNumType(
    [] (int i) { return i+1.f; },
    [] (float f) { return int(std::floor(f-1.f)); },
    [] (float f) { return String(int(std::floor(f))); },
    1.f,
    99.f
);

IntProperty::Type SetupSettings::LcdContrastType(
    [] (int i) { return i+1.f; },
    [] (float f) { return int(std::floor(f-1.f)); },
    [] (float f) { return String(int(std::floor(f))); },
    1.f,
    16.f
);


StringArray RhythmSettings::PatternNames({"Simple Beat 1","Simple Beat 2","Simple Beat 3","Simple Beat 4","Simple Beat 5","Downbeat Snare","Rock 1","Rock 2","Rock 3","Rock 4","Rock 5","Latin Rock 1","Latin Rock 2","Groove Beat 1","Groove Beat 2","Groove Beat 3","Groove Beat 4","Groove Beat 5","Shuffle 1","Shuffle 2","Shuffle 3","Shuffle 4","Shuffle 5","Shuffle 6","1/2 Shuffle 1","1/2 Shuffle 2","16th Shuffle 1","16th Shuffle 2","Shuffle Reggae","Pop 1","Pop 2","Pop 3","Funk 1","Funk 2","Funk 3","Fusion 1","Fusion 2","Fusion 3","Swing","Bossa 1","Bossa 2","Bossa 3","Samba 1","Samba 2","Samba 3","Clave 1","Clave 2","909 Beat","909 Clap","R&B 1","R&B 2","Hi-Hat","Kick & Hi-Hat 1","Kick & Hi-Hat 2","Conga & Hi-Hat","Conga & Maracas","Metronome 1","Metronome 2"});
StringArray AssignmentSettings::SourceNames({"EXP PEDAL","CTL1 PEDAL","CTL2 PEDAL","INPUT FX KNOB","TRACK FX KNOB","TR1 REC/DUB","TR2 REC/DUB","TR3 REC/DUB","TR4 REC/DUB","TR5 REC/DUB","TR1 PLAY/STOP","TR2 PLAY/STOP","TR3 PLAY/STOP","TR4 PLAY/STOP","TR5 PLAY/STOP","SYNC START/STOP","#CC 1","#CC 2","#CC 3","#CC 4","#CC 5","#CC 6","#CC 7","#CC 8","#CC 9","#CC 10","#CC 11","#CC 12","#CC 13","#CC 14","#CC 15","#CC 16","#CC 17","#CC 18","#CC 19","#CC 20","#CC 21","#CC 22","#CC 23","#CC 24","#CC 25","#CC 26","#CC 27","#CC 28","#CC 29","#CC 30","#CC 31","#CC 64","#CC 65","#CC 66","#CC 67","#CC 68","#CC 69","#CC 70","#CC 71","#CC 72","#CC 73","#CC 74","#CC 75","#CC 76","#CC 77","#CC 78","#CC 79","#CC 80","#CC 81","#CC 82","#CC 83","#CC 84","#CC 85","#CC 86","#CC 87","#CC 88","#CC 89","#CC 90","#CC 91","#CC 92","#CC 93","#CC 94","#CC 95"});
StringArray AssignmentSettings::TargetNames({"TR1 PLAY LEVEL","TR2 PLAY LEVEL","TR3 PLAY LEVEL","TR4 PLAY LEVEL","TR5 PLAY LEVEL","TR1 PAN","TR2 PAN","TR3 PAN","TR4 PAN","TR5 PAN","TGT TR REC/PLAY","TGT TR STOP","TGT TR CLEAR","TGT TR UNDO/REDO","TGT TR PLAY LEV","ALL START/STOP","UNDO/REDO","RHYTHM LEVEL","RHYYHM PATTERN","MEMORY LEVEL","TEMPO (TAP)","MASTER COMP","MASTER REVERB","OVERDUB MODE","INPUT FX ON/OFF","TRACK FX ON/OFF","INPUT FX CONTROL","TRACK FX CONTROL","INPUT FX TYP INC","INPUT FX TYP DEC","TRACK FX TYP INC","TRACK FX TYP DEC","INPUT FX INC","INPUT FX DEC","TRACK FX INC","TRACK FX DEC","MEMORY INC","MEMORY DEC","INPUT LEVEL","#CC 1","#CC 2","#CC 3","#CC 4","#CC 5","#CC 6","#CC 7","#CC 8","#CC 9","#CC 10","#CC 11","#CC 12","#CC 13","#CC 14","#CC 15","#CC 16","#CC 17","#CC 18","#CC 19","#CC 20","#CC 21","#CC 22","#CC 23","#CC 24","#CC 25","#CC 26","#CC 27","#CC 28","#CC 29","#CC 30","#CC 31","#CC 64","#CC 65","#CC 66","#CC 67","#CC 68","#CC 69","#CC 70","#CC 71","#CC 72","#CC 73","#CC 74","#CC 75","#CC 76","#CC 77","#CC 78","#CC 79","#CC 80","#CC 81","#CC 82","#CC 83","#CC 84","#CC 85","#CC 86","#CC 87","#CC 88","#CC 89","#CC 90","#CC 91","#CC 92","#CC 93","#CC 94","#CC 95"});
StringArray InputFxSettings::EffectNames({"FILTER","PHASER","FLANGER","SYNTH","LO-FI","GUITAR TO BASS","TRANSPOSE","ROBOT","VOCAL DIST","VOCODER","DYNAMICS","EQ","ISOLATOR","OCTAVE","PAN","SLICER","DELAY","TAPE ECHO","GRANUAR DELAY","CHORUS","REVERB"});
StringArray TrackFxSettings::EffectNames({"FILTER","PHASER","FLANGER","SYNTH","LO-FI","GUITAR TO BASS","TRANSPOSE","ROBOT","VOCAL DIST","VOCODER","DYNAMICS","EQ","ISOLATOR","OCTAVE","PAN","SLICER","DELAY","TAPE ECHO","GRANUAR DELAY","CHORUS","REVERB","BEAT REPEAT","BEAT SHIFT","BEAT SCATTER","VINYL FLICK"});

// ----------------------------------------------------------------------------
// Track
// ----------------------------------------------------------------------------

Track::Track(Library *library, Patch &patch, int index) :
    _library(library),
    _patch(patch),
    _index(index),
    _trackSettings(library, "Track")
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

const AudioSampleBuffer &Track::audioBuffer() const
{
    if (_waveState == WaveOriginal && _audioBuffer.getNumSamples() == 0) {
        Utils::readAudioFile(_waveFile, _audioBuffer);
    }
    return _audioBuffer;
}

void Track::setAudioBuffer(const AudioSampleBuffer &audioBuffer)
{
    _waveState = WaveChanged;
    _audioBuffer = audioBuffer;
    _trackSettings.wavStat->setValue(_audioBuffer.getNumSamples() > 0);
    _trackSettings.wavLen->setValue(_audioBuffer.getNumSamples());
}

void Track::clearAudioBuffer()
{
    _waveState = WaveEmpty;
    _audioBuffer.setSize(0, 0);
    _trackSettings.wavStat->setValue(false);
    _trackSettings.wavLen->setValue(0);
}

bool Track::loadFromXml(XmlElement *xml)
{
    bool result = _trackSettings.loadFromXml(xml);

    // find wave file
    File wavePath = File::addTrailingSeparator(_library->wavePath().getFullPathName()) + String::formatted("%03d_%d", _patch.index() + 1, _index + 1);
    Array<File> files;
    wavePath.findChildFiles(files, File::findFiles, false, "*.WAV");
    if (files.size() > 0) {
        _waveState = WaveOriginal;
        _waveFile = files[0];
    } else {
        _waveState = WaveEmpty;
        _waveFile = File::addTrailingSeparator(wavePath.getFullPathName()) + String::formatted("%03d_%d.WAV", _patch.index() + 1, _index + 1);
    }

    return result;
}

bool Track::saveToXml(XmlElement *xml)
{
    return _trackSettings.saveToXml(xml);
}

// ----------------------------------------------------------------------------
// Patch
// ----------------------------------------------------------------------------

Patch::Patch(Library *library) :
    _library(library),
    _patchSettings(library),
    _patchName(library, "Name", "NAME")
{
    for (int i = 0; i < NumTracks; ++i) {
        _tracks.add(new Track(library, *this, i));
    }
}

Patch::Patch(const RC505::Patch &patch) :
    _library(patch._library),
    _patchSettings(patch._library),
    _patchName(patch._library, "Name", "NAME")
{
    for (int i = 0; i < NumTracks; ++i) {
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

void Patch::setName(const String &name)
{
    _patchName.setValue(name);
}

void Patch::moveTrack(int from, int to)
{
    _tracks.swap(from, to);
}

bool Patch::loadFromXml(XmlElement *xml)
{
    _index = xml->getIntAttribute("id");
    for (int i = 0; i < NumTracks; ++i) {
        _tracks[i]->loadFromXml(xml->getChildByName(String::formatted("TRACK%d", i + 1)));
    }
    _patchName.loadFromXml(xml->getChildByName("NAME"));
    _patchSettings.loadFromXml(xml);
    return true;
}

bool Patch::saveToXml(XmlElement *xml)
{
    xml->setAttribute("id", _index);
    for (int i = 0; i < NumTracks; ++i) {
        _tracks[i]->saveToXml(xml->createNewChildElement(String::formatted("TRACK%d", i + 1)));
    }
    _patchName.saveToXml(xml->createNewChildElement("NAME"));
    _patchSettings.saveToXml(xml);
    return true;
}

// ----------------------------------------------------------------------------
// Library
// ----------------------------------------------------------------------------

Library::Library() :
    _systemSettings(this)
{
}

void Library::movePatches(const Array<Patch *> &patches, int insertIndex)
{
    if (patches.size() > 0) {
        for (auto patch : patches) {
            int fromIndex = _patches.indexOf(patch);
            if (insertIndex > fromIndex) {
                --insertIndex;
            }
            jassert(fromIndex >= 0);
            _patches.move(fromIndex, insertIndex);
            insertIndex = _patches.indexOf(patch) + 1;
        }
    }
    for (int i = 0; i < _patches.size(); ++i) {
        _patches[i]->setIndex(i);
    }
}

bool Library::load(const File &path)
{
    _listeners.call(&Listener::beforeLibraryLoaded);

    _rootPath = path;
    _dataPath = File::addTrailingSeparator(path.getFullPathName()) + "DATA";
    _wavePath = File::addTrailingSeparator(path.getFullPathName()) + "WAVE";
    if (!_dataPath.exists() || !_dataPath.isDirectory()) {
        return false;
    }
    if (!_wavePath.exists() || !_wavePath.isDirectory()) {
        return false;
    }

    File memoryPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "MEMORY.RC0";
    if (!loadMemory(memoryPath)) {
        Logger::outputDebugString("Failed to load memory settings from '" + memoryPath.getFullPathName() + "'");
        return false;
    }
    File systemPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "SYSTEM.RC0";
    if (!loadSystem(systemPath)) {
        Logger::outputDebugString("Failed to load system settings from '" + systemPath.getFullPathName() + "'");
        return false;
    }

    _listeners.call(&Listener::afterLibraryLoaded);
    
    return true;
}

bool Library::save(const File &path)
{
    bool inplace = (path == _rootPath);
    _rootPath = path;
    _dataPath = File::addTrailingSeparator(path.getFullPathName()) + "DATA";
    _wavePath = File::addTrailingSeparator(path.getFullPathName()) + "WAVE";
    if (!_dataPath.exists() || !_dataPath.isDirectory()) {
        return false;
    }
    if (!_wavePath.exists() || !_wavePath.isDirectory()) {
        return false;
    }

    File memoryPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "MEMORY.RC0";
    if (!saveMemory(memoryPath)) {
        Logger::outputDebugString("Failed to save memory settings to '" + memoryPath.getFullPathName() + "'");
        return false;
    }
    File systemPath = File::addTrailingSeparator(_dataPath.getFullPathName()) + "SYSTEM.RC0";
    if (!saveSystem(systemPath)) {
        Logger::outputDebugString("Failed to save system settings to '" + systemPath.getFullPathName() + "'");
        return false;
    }

    if (!saveWaveFiles(inplace)) {
        return false;
    }

    return true;
}

bool Library::loadMemory(const File &path)
{
    ScopedPointer<XmlElement> xml(XmlDocument::parse(path));
    if (!xml) {
        return false;
    }
    if (!xml->hasTagName("database")) {
        return false;
    }
    _patches.clear();
    for (int i = 0; i < xml->getNumChildElements(); ++i) {
        XmlElement *xmlChild = xml->getChildElement(i);
        if (!xmlChild->hasTagName("mem") || !xmlChild->hasAttribute("id")) {
            return false;
        }
        Patch *patch = new Patch(this);
        if (!patch->loadFromXml(xmlChild)) {
            return false;
        }
        _patches.add(patch);
    }
    return true;
}

bool Library::saveMemory(const File &path)
{
    ScopedPointer<XmlElement> xml = new XmlElement("database");
    xml->setAttribute("name", "RC-505");
    xml->setAttribute("revision", 1);
    for (auto patch : _patches) {
        if (!patch->saveToXml(xml->createNewChildElement("mem"))) {
            return false;
        }
    }
    return xml->writeToFile(path, "");
}

bool Library::loadSystem(const File &path)
{
    ScopedPointer<XmlElement> xml(XmlDocument::parse(path));
    if (!xml) {
        return false;
    }
    if (!xml->hasTagName("database")) {
        return false;
    }
    return _systemSettings.loadFromXml(xml->getChildByName("sys"));
}

bool Library::saveSystem(const File &path)
{
    ScopedPointer<XmlElement> xml = new XmlElement("database");
    xml->setAttribute("name", "RC-505");
    xml->setAttribute("revision", 1);
    if (!_systemSettings.saveToXml(xml->createNewChildElement("sys"))) {
        return false;
    }
    return xml->writeToFile(path, "");
}

bool Library::saveWaveFiles(bool inplace)
{
    auto finalWaveDir = [&] (int patchIndex, int trackIndex) {
        return File(File::addTrailingSeparator(_wavePath.getFullPathName()) + 
            String::formatted("%03d_%d", patchIndex + 1, trackIndex + 1));
    };

    auto temporaryWaveDir = [&] (int patchIndex, int trackIndex) {
        return File(File::addTrailingSeparator(_wavePath.getFullPathName()) + 
            String::formatted("temp_%03d_%d", patchIndex + 1, trackIndex + 1));
    };

    auto temporaryWaveFile = [&] (int patchIndex, int trackIndex) {
        return  File(File::addTrailingSeparator(temporaryWaveDir(patchIndex, trackIndex).getFullPathName()) + 
            String::formatted("%03d_%d.WAV", patchIndex + 1, trackIndex + 1));
    };

    // create folders x_xxx and temp_x_xxx
    for (int patchIndex = 0; patchIndex < NumPatches; ++patchIndex) {
        for (int trackIndex = 0; trackIndex < Patch::NumTracks; ++trackIndex) {
            finalWaveDir(patchIndex, trackIndex).createDirectory();
            temporaryWaveDir(patchIndex, trackIndex).createDirectory();
        }
    }

    // store wave files to temporary directories
    for (int patchIndex = 0; patchIndex < NumPatches; ++patchIndex) {
        for (int trackIndex = 0; trackIndex < Patch::NumTracks; ++trackIndex) {
            auto track = _patches[patchIndex]->tracks()[trackIndex];
            switch (track->waveState()) {
            case Track::WaveEmpty:
                // do nothing
                break;
            case Track::WaveOriginal:
                // move/copy files
                if (!inplace || !track->waveFile().moveFileTo(temporaryWaveFile(patchIndex, trackIndex))) {
                    track->waveFile().copyFileTo(temporaryWaveFile(patchIndex, trackIndex));
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
    for (int patchIndex = 0; patchIndex < NumPatches; ++patchIndex) {
        for (int trackIndex = 0; trackIndex < Patch::NumTracks; ++trackIndex) {
            finalWaveDir(patchIndex, trackIndex).deleteRecursively();
            temporaryWaveDir(patchIndex, trackIndex).moveFileTo(finalWaveDir(patchIndex, trackIndex));
        }
    }

    return true;
}

void Library::notifyPropertyValueChanged(ValueProperty *property)
{
    _listeners.call(&Listener::propertyValueChanged, property);
}

} // namespace RC505
