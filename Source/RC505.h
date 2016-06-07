#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include <functional>

namespace RC505 {

class Library;
class Patch;

class Property {
public:
    enum SelectedState {
        Unselected,
        Selected,
        PartiallySelected,
    };

    Property(Library *library, const String &name) :
        _library(library),
        _name(name),
        _visible(true),
        _selected(false)
    {
    }

    virtual ~Property()
    {
    }

    const String &name() const { return _name; }

    bool visible() const { return _visible; }
    void setVisible(bool visible) { _visible = visible; }

    virtual bool selected() const { return _selected; }
    virtual void setSelected(bool selected) { _selected = selected; }
    virtual SelectedState selectedState() const { return _selected ? Selected : Unselected; }

    virtual void assign(const Property &other, bool selectedOnly) = 0;

    virtual bool loadFromXml(XmlElement *xml) = 0;
    virtual bool saveToXml(XmlElement *xml) = 0;

protected:
    Library *_library;
    String _name;
    bool _visible;
    bool _selected;
};

class ValueProperty : public Property {
public:
    class Listener {
    public:
        virtual ~Listener() {}
        virtual void valueChanged(ValueProperty *property) = 0;
    };

    ValueProperty(Library *library, const String &name, const String &id) :
        Property(library, name),
        _id(id)
    {
    }

    const String &id() const { return _id; }

    const String &value() const { return _value; }
    void setValue(const String &value) { _value = value; notifyValueChanged(); }

    virtual void assign(const Property &other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected()) {
            setValue(static_cast<const ValueProperty *>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement *xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement()) {
            return false;
        }
        _value = xml->getFirstChildElement()->getText();
        return true;
    }
    
    virtual bool saveToXml(XmlElement *xml) override
    {
        xml->addTextElement(_value);
        return true;
    }

    void addListener(Listener *listener)
    {
        _listeners.add(listener);
    }

    void removeListener(Listener *listener)
    {
        _listeners.remove(listener);
    }

protected:
    void notifyValueChanged();

    String _id;
    String _value;
    ListenerList<Listener> _listeners;
};

class BoolProperty : public ValueProperty {
public:
    BoolProperty(Library *library, const String &name, const String &desc) :
        ValueProperty(library, name, desc)
    {
    }

    const bool value() const { return _value; }
    void setValue(bool value) { _value = value; notifyValueChanged(); }

    virtual void assign(const Property &other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected()) {
            setValue(static_cast<const BoolProperty *>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement *xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement()) {
            return false;
        }
        _value = xml->getFirstChildElement()->getText().getIntValue() != 0;
        return true;
    }
    
    virtual bool saveToXml(XmlElement *xml) override
    {
        xml->addTextElement(String(_value ? 1 : 0));
        return true;
    }

private:
    bool _value;
};

class IntProperty : public ValueProperty {
public:
    struct Type {
        typedef std::function<float(int)> ToFloat;
        typedef std::function<int(float)> FromFloat;
        typedef std::function<String(float)> Formatter;
        ToFloat toFloat;
        FromFloat fromFloat;
        Formatter formatter;
        float min;
        float max;
        float skew;
        Type(ToFloat toFloat, FromFloat fromFloat, Formatter formatter, float min, float max, float skew = 1.f) :
            toFloat(toFloat), fromFloat(fromFloat), formatter(formatter), min(min), max(max), skew(skew)
        {}
    };

    static Type DefaultType;
    static Type MidiType;
    static Type LevelType;
    static Type PanType;
    static Type TempoType;
    static Type ThresholdType;
    static Type RateType;
    static Type BalanceType;
    static Type SamplesType;

    IntProperty(Library *library, const String &name, const String &desc, const Type &type = DefaultType) :
        ValueProperty(library, name, desc),
        _type(type)
    {
    }

    const Type &type() const { return _type; }

    const int value() const { return _value; }
    void setValue(int value) { _value = value; notifyValueChanged(); }
    
    virtual void assign(const Property &other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected()) {
            setValue(static_cast<const IntProperty *>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement *xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement()) {
            return false;
        }
        _value = xml->getFirstChildElement()->getText().getIntValue();
        return true;
    }
    
    virtual bool saveToXml(XmlElement *xml) override
    {
        xml->addTextElement(String(_value));
        return true;
    }

private:
    const Type &_type;
    int _value;
};

class EnumProperty : public ValueProperty {
public:
    EnumProperty(Library *library, const String &name, const String &desc, const StringArray &options) :
        ValueProperty(library, name, desc), _options(options)
    {
    }

    const int value() const { return _value; }
    void setValue(int value) { _value = value; notifyValueChanged(); }

    const StringArray &options() const { return _options; }

    virtual void assign(const Property &other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected()) {
            setValue(static_cast<const EnumProperty *>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement *xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement()) {
            return false;
        }
        _value = xml->getFirstChildElement()->getText().getIntValue();
        return true;
    }
    
    virtual bool saveToXml(XmlElement *xml) override
    {
        xml->addTextElement(String(_value));
        return true;
    }

private:
    StringArray _options;
    int _value;
};

class NameProperty : public ValueProperty {
public:
    NameProperty(Library *library, const String &name, const String &desc) :
        ValueProperty(library, name, desc)
    {
    }

    const String &value() const { return _value; }
    void setValue(const String &value) { _value = value; notifyValueChanged(); }

    virtual void assign(const Property &other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected()) {
            setValue(static_cast<const NameProperty *>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement *xml) override
    {
        _value.clear();
        for (int i = 0; i < 12; ++i) {
            XmlElement *xmlChar = xml->getChildByName(String::formatted("C%02d", i + 1));
            if (!xmlChar) {
                return false;
            }
            _value += String::charToString(xmlChar->getFirstChildElement()->getText().getIntValue());
        }
        _value = _value.trim();
        return true;
    }

    virtual bool saveToXml(XmlElement *xml) override
    {
        for (int i = 0; i < 12; ++i) {
            XmlElement *xmlChar = xml->createNewChildElement(String::formatted("C%02d", i + 1));
            xmlChar->addTextElement(String(i < _value.length() ? int(_value[i]) : 32));
        }
        return true;
    }

private:
    String _value;
};

class Group : public Property {
public:
    Group(Library *library, const String &name) :
        Property(library, name)
    {
    }

    Group &operator=(const Group &other)
    {
        jassert(_children.size() == other._children.size());
        for (int i = 0; i < _children.size(); ++i) {
            _children[i]->assign(*other._children[i], false);
        }
        return *this;
    }

    const OwnedArray<Property> &children() const { return _children; }

    virtual void setSelected(bool selected) override
    {
        for (auto child : _children) {
            child->setSelected(selected);
        }
    }

    virtual SelectedState selectedState() const override
    {
        int unselected = 0, selected = 0;
        for (auto child : _children) {
            switch (child->selectedState()) {
            case Unselected: ++unselected; break;
            case Selected: ++selected; break;
            case PartiallySelected: return PartiallySelected; break;
            }
        }
        if (unselected == 0) return Selected;
        if (selected == 0) return Unselected;
        return PartiallySelected;
    }

    virtual void assign(const Property &other, bool selectedOnly) override
    {
        const Group *group = static_cast<const Group *>(&other);
        jassert(_children.size() == group->_children.size());
        for (int i = 0; i < _children.size(); ++i) {
            _children[i]->assign(*group->_children[i], selectedOnly);
        }
    }

    virtual bool loadFromXml(XmlElement *xml) override
    {
        for (auto child : _children) {
            if (ValueProperty *property = dynamic_cast<ValueProperty *>(child)) {
                if (!property->loadFromXml(xml->getChildByName(property->id()))) {
                    return false;
                }
            }
        }
        return true;
    }
    
    virtual bool saveToXml(XmlElement *xml) override
    {
        for (auto child : _children) {
            if (ValueProperty *property = dynamic_cast<ValueProperty *>(child)) {
                if (!property->saveToXml(xml->createNewChildElement(property->id()))) {
                    return false;
                }
            }
        }
        return true;
    }

protected:
    void addChild(Property *child) {
        _children.add(child);
    }
    
    template<typename T, typename ...Args>
    T *createChild(Args&&... args)
    {
        T *child = new T(_library, std::forward<Args>(args)...);
        _children.add(child);
        return child;
    }
    
    OwnedArray<Property> _children;
};
    
class TrackSettings : public Group {
public:
    BoolProperty *reverse = createChild<BoolProperty>("Reverse", "Rev");
    IntProperty *playLevel = createChild<IntProperty>("Play Level", "PlyLvl", IntProperty::LevelType);
    IntProperty *pan = createChild<IntProperty>("Pan", "Pan", IntProperty::PanType);
    BoolProperty *oneShot = createChild<BoolProperty>("One Shot", "One");
    BoolProperty *trackFx = createChild<BoolProperty>("Track Fx", "TrkFx");
    EnumProperty *playMode = createChild<EnumProperty>("Play Mode", "PlyMod", StringArray({"MULTI","SINGLE"}));
    EnumProperty *startMode = createChild<EnumProperty>("Start Mode", "StrtMod", StringArray({"IMMEDIATE","FADE IN"}));
    EnumProperty *stopMode = createChild<EnumProperty>("Stop Mode", "StpMod", StringArray({"IMMEDIATE","LOOP END"}));
    Property *measMod = createChild<ValueProperty>("MeasMod", "MeasMod"); // TODO ?
    IntProperty *measLen = createChild<IntProperty>("MeasLen", "MeasLen"); // TODO ?
    Property *measBtLp = createChild<ValueProperty>("MeasBtLp", "MeasBtLp"); // TODO ?
    BoolProperty *loopSync = createChild<BoolProperty>("Loop Sync", "LpSync");
    BoolProperty *tempoSync = createChild<BoolProperty>("Tempo Sync", "TmpSync");
    IntProperty *recTmp = createChild<IntProperty>("RecTmp", "RecTmp", IntProperty::TempoType); // TODO ?
    BoolProperty *wavStat = createChild<BoolProperty>("WavStat", "WavStat"); // TODO ?
    IntProperty *wavLen = createChild<IntProperty>("WavLen", "WavLen", IntProperty::SamplesType); // TODO ?

    TrackSettings(Library *library, const String &name) :
        Group(library, name)
    {
        measMod->setVisible(false);
        measLen->setVisible(false);
        measBtLp->setVisible(false);
        recTmp->setVisible(false);
        wavStat->setVisible(false);
        wavLen->setVisible(false);
    }
};

class RhythmSettings : public Group {
    static StringArray PatternNames;
public:
    BoolProperty *switch_ = createChild<BoolProperty>("Switch", "Sw");
    IntProperty *level = createChild<IntProperty>("Level", "Lvl", IntProperty::LevelType);
    EnumProperty *pattern = createChild<EnumProperty>("Pattern", "Ptn", PatternNames);
    Property *beat = createChild<ValueProperty>("Beat", "Bt"); // TODO 4/4 ?
    BoolProperty *lineOut = createChild<BoolProperty>("Line Out", "LineOut");
    BoolProperty *recCount = createChild<BoolProperty>("Rec Count", "RecCnt");
    BoolProperty *playCount = createChild<BoolProperty>("Play Count", "PlyCnt");
    EnumProperty *stop = createChild<EnumProperty>("Stop", "Stp", StringArray({"OFF","LOOPER STOP","REC END"}));

    RhythmSettings(Library *library, const String &name) : Group(library, name) {}
};
    
class MasterSettings : public Group {
public:
    IntProperty *level = createChild<IntProperty>("Level", "Lvl", IntProperty::LevelType);
    IntProperty *tempo = createChild<IntProperty>("Tempo", "Tmp", IntProperty::TempoType);
    IntProperty *comp = createChild<IntProperty>("Comp", "Cs", IntProperty::ThresholdType);
    IntProperty *reverb = createChild<IntProperty>("Reverb", "Rv");
    MasterSettings(Library *library, const String &name) : Group(library, name) {}
};

class RecordSettings : public Group {
public:
    EnumProperty *overdubMode = createChild<EnumProperty>("Overdub Mode", "DubMod", StringArray({"OVERDUB","REPLACE"}));
    EnumProperty *recordAction = createChild<EnumProperty>("Record Action", "RecAct", StringArray({"REC->OVERDUB","REC->PLAY"}));
    RecordSettings(Library *library, const String &name) : Group(library, name) {}
};

class PlaySettings : public Group {
public:
    static IntProperty::Type LoopLengthType;
    static IntProperty::Type FadeTimeType;

    EnumProperty *singlePlayChange = createChild<EnumProperty>("Single Play Change", "SinglePlyChg", StringArray({"IMMEDIATE","LOOP END"}));
    EnumProperty *quantize = createChild<EnumProperty>("Quantize", "LpQtz", StringArray({"REC END","MEASURE","BEAT"}));
    Property *loopMode = createChild<ValueProperty>("Loop Mode", "LpMod"); // TODO ?
    IntProperty *loopLength = createChild<IntProperty>("Loop Length", "LpLen", LoopLengthType);
    Property *allStart = createChild<ValueProperty>("All Start", "AllStrt"); // TODO BITSET
    IntProperty *fadeTime = createChild<IntProperty>("Fade Time", "FadeTim", FadeTimeType);
    EnumProperty *targetTrack = createChild<EnumProperty>("Target Track", "TgtTr", StringArray({"TRACK1","TRACK2","TRACK3","TRACK4","TRACK5"}));
    PlaySettings(Library *library, const String &name) : Group(library, name) {}
};

class AssignmentSettings : public Group {
    static StringArray SourceNames;
    static StringArray TargetNames;
public:
    BoolProperty *switch_ = createChild<BoolProperty>("Switch", "Sw");
    EnumProperty *source = createChild<EnumProperty>("Source", "Src", SourceNames);
    EnumProperty *sourceMode = createChild<EnumProperty>("Source Mode", "SrcMod", StringArray({"MOMENT","TOGGLE"}));
    EnumProperty *target = createChild<EnumProperty>("Target", "Tgt", TargetNames);
    IntProperty *targetMin = createChild<IntProperty>("Target Min", "TgtMin", IntProperty::MidiType);
    IntProperty *targetMax = createChild<IntProperty>("Target Max", "TgtMax", IntProperty::MidiType);
    AssignmentSettings(Library *library, const String &name) : Group(library, name) {}
};

class FilterFxSettings : public Group {
public:
    EnumProperty *type = createChild<EnumProperty>("Type", "FilTyp", StringArray({"LPF","BPF","HPF"}));
    IntProperty *rate = createChild<IntProperty>("Rate", "FilRat", IntProperty::RateType);
    IntProperty *depth = createChild<IntProperty>("Depth", "FilDep");
    IntProperty *resonance = createChild<IntProperty>("Resonance", "FilReso");
    IntProperty *cutoff = createChild<IntProperty>("Cutoff", "FilCut");
    FilterFxSettings(Library *library) : Group(library, "Filter") {}
};

class PhaserFxSettings : public Group {
public:
    IntProperty *rate = createChild<IntProperty>("Rate", "PhRat", IntProperty::RateType);
    IntProperty *depth = createChild<IntProperty>("Depth", "PhDep");
    IntProperty *resonance = createChild<IntProperty>("Resonance", "PhReso");
    IntProperty *level = createChild<IntProperty>("Level", "PhLvl");
    PhaserFxSettings(Library *library) : Group(library, "Phaser") {}
};

class FlangerFxSettings : public Group {
public:
    IntProperty *rate = createChild<IntProperty>("Rate", "FlRat", IntProperty::RateType);
    IntProperty *depth = createChild<IntProperty>("Depth", "FlDep");
    IntProperty *resonance = createChild<IntProperty>("Resonance", "FlReso");
    IntProperty *level = createChild<IntProperty>("Level", "FlLvl");
    FlangerFxSettings(Library *library) : Group(library, "Flanger") {}
};

class SynthFxSettings : public Group {
public:
    IntProperty *frequency = createChild<IntProperty>("Frequency", "SynFreq");
    IntProperty *resonance = createChild<IntProperty>("Resonance", "SynReso");
    IntProperty *decay = createChild<IntProperty>("Decay", "SynDecay");
    IntProperty *balance = createChild<IntProperty>("Balance", "SynBal", IntProperty::BalanceType);
    SynthFxSettings(Library *library) : Group(library, "Synth") {}
};

class LoFiFxSettings : public Group {
    static IntProperty::Type BitDepthType;
    static IntProperty::Type SampleRateType;
public:
    IntProperty *bitDepth = createChild<IntProperty>("BitDepth", "LoFiDep", BitDepthType);
    IntProperty *sampleRate = createChild<IntProperty>("SampleRate", "LoFiSmpl", SampleRateType);
    IntProperty *balance = createChild<IntProperty>("Balance", "LoFiBal", IntProperty::BalanceType);
    LoFiFxSettings(Library *library) : Group(library, "Lo-Fi") {}
};

class GuitarToBassFxSettings : public Group {
public:
    IntProperty *balance = createChild<IntProperty>("Balance", "GToB", IntProperty::BalanceType);
    GuitarToBassFxSettings(Library *library) : Group(library, "Guitar To Bass") {}
};

class TransposeFxSettings : public Group {
    static IntProperty::Type TransType;
public:
    IntProperty *trans = createChild<IntProperty>("Trans", "Trans", TransType);
    TransposeFxSettings(Library *library) : Group(library, "Transpose") {}
};

class RobotFxSettings : public Group {
    static IntProperty::Type GenderType;
public:
    EnumProperty *note = createChild<EnumProperty>("Note", "RoboNote", StringArray({"C","Db","D","Eb","E","F","F#","G","Ab","A","Bb","B"}));
    IntProperty *gender = createChild<IntProperty>("Gender", "RoboGen", GenderType);
    RobotFxSettings(Library *library) : Group(library, "Robot") {}
};

class VocalDistFxSettings : public Group {
    static IntProperty::Type ToneType;
public:
    IntProperty *dist = createChild<IntProperty>("Dist", "DistDist");
    IntProperty *tone = createChild<IntProperty>("Tone", "DistTon", ToneType);
    IntProperty *level = createChild<IntProperty>("Level", "DistLvl");
    VocalDistFxSettings(Library *library) : Group(library, "Vocal Dist") {}
};

class VocoderFxSettings : public Group {
public:
    EnumProperty *carrier = createChild<EnumProperty>("Carrier", "VocoCar", StringArray({"TRACK1","TRACK2","TRACK3","TRACK4","TRACK5"}));
    IntProperty *modSens = createChild<IntProperty>("ModSens", "VocoSens");
    IntProperty *attack = createChild<IntProperty>("Attack", "VocoAtck");
    IntProperty *balance = createChild<IntProperty>("Balance", "VocoBal", IntProperty::BalanceType);
    VocoderFxSettings(Library *library) : Group(library, "Vocoder") {}
};

class DynamicsFxSettings : public Group {
public:
    EnumProperty *type = createChild<EnumProperty>("Type", "DynmTyp", StringArray({"NATURAL COMP","MIXER COMP","LIVE COMP","NATURAL LIM","HARD LIM","JINGL COMP","HARD COMP","SOFT COMP","CLEAN COMP","DANCE COMP","ORCH COMP","VOCAL COMP","ACOUSTIC","ROCK BAND","ORCHESTRA","LOW BOOST","BRIGHTEN","DJs VOICE","PHONE VOX"}));
    IntProperty *dynamics = createChild<IntProperty>("Dynamics", "DynmDynm", IntProperty::ThresholdType);
    DynamicsFxSettings(Library *library) : Group(library, "Dynamics") {}
};

class EqFxSettings : public Group {
public:
    IntProperty *low = createChild<IntProperty>("Low", "EqLo", IntProperty::ThresholdType);
    IntProperty *lowMid = createChild<IntProperty>("Low-Mid", "EqLoMd", IntProperty::ThresholdType);
    IntProperty *highMid = createChild<IntProperty>("High-Mid", "EqHiMd", IntProperty::ThresholdType);
    IntProperty *high = createChild<IntProperty>("High", "EqHi", IntProperty::ThresholdType);
    IntProperty *level = createChild<IntProperty>("Level", "EqLvl", IntProperty::ThresholdType);
    EqFxSettings(Library *library) : Group(library, "Eq") {}
};

class IsolatorFxSettings : public Group {
public:
    EnumProperty *band = createChild<EnumProperty>("Band", "IsoBnd", StringArray({"LOW","MID","HIGH"}));
    IntProperty *rate = createChild<IntProperty>("Rate", "IsoRat", IntProperty::RateType);
    IntProperty *depth = createChild<IntProperty>("Depth", "IsoDep");
    IntProperty *bandLevel = createChild<IntProperty>("BandLevel", "IsoLvl");
    IsolatorFxSettings(Library *library) : Group(library, "Isolator") {}
};

class OctaveFxSettings : public Group {
public:
    EnumProperty *mode = createChild<EnumProperty>("Mode", "OctMod", StringArray({"-1OCT","-2OCT","-1OCT&-2OCT"}));
    IntProperty *octLevel = createChild<IntProperty>("Oct.Level", "OctLvl");
    OctaveFxSettings(Library *library) : Group(library, "Octave") {}
};

class PanFxSettings : public Group {
public:
    EnumProperty *type = createChild<EnumProperty>("Type", "PanTyp", StringArray({"AUTO1","AUTO2","MANUAL"}));
    IntProperty *rate = createChild<IntProperty>("Rate", "PanRat", IntProperty::RateType);
    IntProperty *depth = createChild<IntProperty>("Depth", "PanDep");
    IntProperty *position = createChild<IntProperty>("Position", "PanPos", IntProperty::PanType);
    PanFxSettings(Library *library) : Group(library, "Pan") {}
};

class SlicerFxSettings : public Group {
public:
    EnumProperty *pattern = createChild<EnumProperty>("Pattern", "SlPat", StringArray({"P01","P02","P03","P04","P05","P06","P07","P08","P09","P10","P11","P12","P13","P14","P15","P16","P17","P18","P19","P20"}));
    IntProperty *rate = createChild<IntProperty>("Rate", "SlRat", IntProperty::RateType);
    IntProperty *depth = createChild<IntProperty>("Depth", "SlDep");
    SlicerFxSettings(Library *library) : Group(library, "Slicer") {}
};

class DelayFxSettings : public Group {
    static IntProperty::Type TimeType;
public:
    IntProperty *time = createChild<IntProperty>("Time", "DlyTim", TimeType);
    IntProperty *feedback = createChild<IntProperty>("Feedback", "DlyFb");
    IntProperty *level = createChild<IntProperty>("Level", "DlyLvl");
    DelayFxSettings(Library *library) : Group(library, "Delay") {}
};

class TapeEchoFxSettings : public Group {
public:
    IntProperty *repeatRate = createChild<IntProperty>("RepeatRate", "EchRat");
    IntProperty *intensity = createChild<IntProperty>("Intensity", "EchInt");
    IntProperty *echoLevel = createChild<IntProperty>("EchoLevel", "EchLvl");
    TapeEchoFxSettings(Library *library) : Group(library, "Tape Echo") {}
};

class GranularDelayFxSettings : public Group {
public:
    IntProperty *time = createChild<IntProperty>("Time", "GraTim");
    IntProperty *feedback = createChild<IntProperty>("Feedback", "GraFb");
    IntProperty *level = createChild<IntProperty>("Level", "GraLvl");
    GranularDelayFxSettings(Library *library) : Group(library, "Granular Delay") {}
};

class ChorusFxSettings : public Group {
public:
    IntProperty *rate = createChild<IntProperty>("Rate", "ChoRat");
    IntProperty *depth = createChild<IntProperty>("Depth", "ChoDep");
    IntProperty *level = createChild<IntProperty>("Level", "ChoLvl");
    ChorusFxSettings(Library *library) : Group(library, "Chorus") {}
};

class ReverbFxSettings : public Group {
    static IntProperty::Type TimeType;
public:
    IntProperty *time = createChild<IntProperty>("Time", "RevTim", TimeType);
    IntProperty *level = createChild<IntProperty>("Level", "RevLvl");
    ReverbFxSettings(Library *library) : Group(library, "Reverb") {}
};

class BeatRepeatFxSettings : public Group {
public:
    EnumProperty *type = createChild<EnumProperty>("Type", "RepTyp", StringArray({"FORWARD","REWIND","MIX"}));
    EnumProperty *length = createChild<EnumProperty>("Length", "RepLen", StringArray({"THRU","Whole Note","Half Note","Quarter Note","Eighth Note","Sixteenth Note","32th Note"}));
    BeatRepeatFxSettings(Library *library) : Group(library, "Beat Repeat") {}
};

class BeatShiftFxSettings : public Group {
public:
    EnumProperty *type = createChild<EnumProperty>("Type", "ShftTyp", StringArray({"FUTURE","PAST"}));
    EnumProperty *shift = createChild<EnumProperty>("Shift", "ShftShft", StringArray({"THRU","Sixteenth Note","Eighth Note","Quarter Note","Half Note","Whole Note"}));
    BeatShiftFxSettings(Library *library) : Group(library, "Beat Shift") {}
};

class BeatScatterFxSettings : public Group {
public:
    EnumProperty *type = createChild<EnumProperty>("Type", "ScatTyp", StringArray({"P1","P2","P3","P4"}));
    EnumProperty *length = createChild<EnumProperty>("Length", "ScatLen", StringArray({"THRU","Half Note","Quarter Note","Eighth Note","Sixteenth Note"}));
    BeatScatterFxSettings(Library *library) : Group(library, "Beat Scatter") {}
};

class VinylFlickFxSettings : public Group {
public:
    IntProperty *flick = createChild<IntProperty>("Flick", "Flick");
    VinylFlickFxSettings(Library *library) : Group(library, "Vinyl Flick") {}
};



class InputFxSlotSettings : public Group {
public:
    FilterFxSettings *filter = createChild<FilterFxSettings>();
    PhaserFxSettings *phaser = createChild<PhaserFxSettings>();
    FlangerFxSettings *flanger = createChild<FlangerFxSettings>();
    SynthFxSettings *synth = createChild<SynthFxSettings>();
    LoFiFxSettings *lofi = createChild<LoFiFxSettings>();
    GuitarToBassFxSettings *guitarToBass = createChild<GuitarToBassFxSettings>();
    TransposeFxSettings *transpose = createChild<TransposeFxSettings>();
    RobotFxSettings *robot = createChild<RobotFxSettings>();
    VocalDistFxSettings *vocalDist = createChild<VocalDistFxSettings>();
    VocoderFxSettings *vocoder = createChild<VocoderFxSettings>();
    DynamicsFxSettings *dynamics = createChild<DynamicsFxSettings>();
    EqFxSettings *eq = createChild<EqFxSettings>();
    IsolatorFxSettings *isolator = createChild<IsolatorFxSettings>();
    OctaveFxSettings *octave = createChild<OctaveFxSettings>();
    PanFxSettings *pan = createChild<PanFxSettings>();
    SlicerFxSettings *slicer = createChild<SlicerFxSettings>();
    DelayFxSettings *delay = createChild<DelayFxSettings>();
    TapeEchoFxSettings *tapeEcho = createChild<TapeEchoFxSettings>();
    GranularDelayFxSettings *granularDelay = createChild<GranularDelayFxSettings>();
    ChorusFxSettings *chorus = createChild<ChorusFxSettings>();
    ReverbFxSettings *reverb = createChild<ReverbFxSettings>();

    InputFxSlotSettings(Library *library, const String &name) : Group(library, name) {}

    bool loadFromXml(XmlElement *xml) override
    {
        for (auto child : _children) {
            if (!child->loadFromXml(xml)) {
                return false;
            }
        }
        return true;
    }

    bool saveToXml(XmlElement *xml) override
    {
        for (auto child : _children) {
            if (!child->saveToXml(xml)) {
                return false;
            }
        }
        return true;
    }
};

class InputFxSettings : public Group {
    static StringArray EffectNames;
public:
    BoolProperty *switch_ = createChild<BoolProperty>("Switch", "Sw");
    EnumProperty *selected = createChild<EnumProperty>("Selected", "Sel", StringArray({"Fx A","Fx B","Fx C"}));
    EnumProperty *fxType[3];
    InputFxSlotSettings *fxSlot[3];

    InputFxSettings(Library *library, const String &name) :
        Group(library, name)
    {
        for (int i = 0; i < 3; ++i) {
            fxType[i] = createChild<EnumProperty>(String::formatted("Fx %c Type", 'A' + i), String::formatted("Typ%d", i + 1), EffectNames);
        }
        for (int i = 0; i < 3; ++i) {
            fxSlot[i] = createChild<InputFxSlotSettings>(String::formatted("Fx %c", 'A' + i));
        }
    }
};

class TrackFxSlotSettings : public Group {
public:
    FilterFxSettings *filter = createChild<FilterFxSettings>();
    PhaserFxSettings *phaser = createChild<PhaserFxSettings>();
    FlangerFxSettings *flanger = createChild<FlangerFxSettings>();
    SynthFxSettings *synth = createChild<SynthFxSettings>();
    LoFiFxSettings *lofi = createChild<LoFiFxSettings>();
    GuitarToBassFxSettings *guitarToBass = createChild<GuitarToBassFxSettings>();
    TransposeFxSettings *transpose = createChild<TransposeFxSettings>();
    RobotFxSettings *robot = createChild<RobotFxSettings>();
    VocalDistFxSettings *vocalDist = createChild<VocalDistFxSettings>();
    VocoderFxSettings *vocoder = createChild<VocoderFxSettings>();
    DynamicsFxSettings *dynamics = createChild<DynamicsFxSettings>();
    EqFxSettings *eq = createChild<EqFxSettings>();
    IsolatorFxSettings *isolator = createChild<IsolatorFxSettings>();
    OctaveFxSettings *octave = createChild<OctaveFxSettings>();
    PanFxSettings *pan = createChild<PanFxSettings>();
    SlicerFxSettings *slicer = createChild<SlicerFxSettings>();
    DelayFxSettings *delay = createChild<DelayFxSettings>();
    TapeEchoFxSettings *tapeEcho = createChild<TapeEchoFxSettings>();
    GranularDelayFxSettings *granularDelay = createChild<GranularDelayFxSettings>();
    ChorusFxSettings *chorus = createChild<ChorusFxSettings>();
    ReverbFxSettings *reverb = createChild<ReverbFxSettings>();

    BeatRepeatFxSettings *beatRepeat = createChild<BeatRepeatFxSettings>();
    BeatShiftFxSettings *beatShift = createChild<BeatShiftFxSettings>();
    BeatScatterFxSettings *beatScatter = createChild<BeatScatterFxSettings>();
    VinylFlickFxSettings *vinylFlick = createChild<VinylFlickFxSettings>();

    TrackFxSlotSettings(Library *library, const String &name) : Group(library, name) {}

    bool loadTrackFxFromXml(XmlElement *xml)
    {
        Group *trackFx[] = { filter, phaser, flanger, synth, lofi, guitarToBass, transpose, robot, vocalDist, vocoder, dynamics, eq, isolator, octave, pan, slicer, delay, tapeEcho, granularDelay, chorus, reverb };
        for (auto child : trackFx) {
            if (!child->loadFromXml(xml)) {
                return false;
            }
        }
        return true;
    }

    bool saveTrackFxToXml(XmlElement *xml)
    {
        Group *trackFx[] = { filter, phaser, flanger, synth, lofi, guitarToBass, transpose, robot, vocalDist, vocoder, dynamics, eq, isolator, octave, pan, slicer, delay, tapeEcho, granularDelay, chorus, reverb };
        for (auto child : trackFx) {
            if (!child->saveToXml(xml)) {
                return false;
            }
        }
        return true;
    }

    bool loadBeatFxFromXml(XmlElement *xml)
    {
        Group *beatFx[] = { beatRepeat, beatShift, beatScatter, vinylFlick };
        for (auto child : beatFx) {
            if (!child->loadFromXml(xml)) {
                return false;
            }
        }
        return true;
    }

    bool saveBeatFxToXml(XmlElement *xml)
    {
        Group *beatFx[] = { beatRepeat, beatShift, beatScatter, vinylFlick };
        for (auto child : beatFx) {
            if (!child->saveToXml(xml)) {
                return false;
            }
        }
        return true;
    }
};

class TrackFxSettings : public Group {
    static StringArray EffectNames;
public:
    BoolProperty *switch_ = createChild<BoolProperty>("Switch", "Sw");
    EnumProperty *selected = createChild<EnumProperty>("Selected", "Sel", StringArray({"Fx A","Fx B","Fx C"}));
    EnumProperty *fxType[3];
    TrackFxSlotSettings *fxSlot[3];
    
    TrackFxSettings(Library *library, const String &name) :
        Group(library, name)
    {
        for (int i = 0; i < 3; ++i) {
            fxType[i] = createChild<EnumProperty>(String::formatted("Fx %c Type", 'A' + i), String::formatted("Typ%d", i + 1), EffectNames);
        }
        for (int i = 0; i < 3; ++i) {
            fxSlot[i] = createChild<TrackFxSlotSettings>(String::formatted("Fx %c", 'A' + i));
        }
    }
};

class PatchSettings : public Group {
public:
    RhythmSettings *rhythm = createChild<RhythmSettings>("Rhythm");
    MasterSettings *master = createChild<MasterSettings>("Master");
    RecordSettings *record = createChild<RecordSettings>("Record");
    PlaySettings *play = createChild<PlaySettings>("Play");
    InputFxSettings *inputFx = createChild<InputFxSettings>("Input Fx");
    TrackFxSettings *trackFx = createChild<TrackFxSettings>("Track Fx");
    AssignmentSettings *assignment[8];

    PatchSettings(Library *library) :
        Group(library, "Patch")
    {
        for (int i = 0; i < 8; ++i) {
            assignment[i] = createChild<AssignmentSettings>(String::formatted("Assignment %d", i + 1));
        }
    }

    bool loadFromXml(XmlElement *xml) override
    {
        rhythm->loadFromXml(xml->getChildByName("RHYTHM"));
        master->loadFromXml(xml->getChildByName("MASTER"));
        record->loadFromXml(xml->getChildByName("REC_OPTION"));
        play->loadFromXml(xml->getChildByName("PLAY_OPTION"));
        for (int i = 0; i < 8; ++i) {
            assignment[i]->loadFromXml(xml->getChildByName(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->loadFromXml(xml->getChildByName("INPUT_FX"));
        trackFx->loadFromXml(xml->getChildByName("TRACK_FX"));
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->loadBeatFxFromXml(xml->getChildByName(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            inputFx->fxSlot[i]->loadFromXml(xml->getChildByName(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->loadTrackFxFromXml(xml->getChildByName(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }

    bool saveToXml(XmlElement *xml) override
    {
        rhythm->saveToXml(xml->createNewChildElement("RHYTHM"));
        master->saveToXml(xml->createNewChildElement("MASTER"));
        record->saveToXml(xml->createNewChildElement("REC_OPTION"));
        play->saveToXml(xml->createNewChildElement("PLAY_OPTION"));
        for (int i = 0; i < 8; ++i) {
            assignment[i]->saveToXml(xml->createNewChildElement(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->saveToXml(xml->createNewChildElement("INPUT_FX"));
        trackFx->saveToXml(xml->createNewChildElement("TRACK_FX"));
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->saveBeatFxToXml(xml->createNewChildElement(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            inputFx->fxSlot[i]->saveToXml(xml->createNewChildElement(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->saveTrackFxToXml(xml->createNewChildElement(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }
};

class SetupSettings : public Group {
    static IntProperty::Type MemNumType;
    static IntProperty::Type LcdContrastType;
public:
    IntProperty *memNum = createChild<IntProperty>("Memory", "MemNum", MemNumType);
    IntProperty *lcdContrast = createChild<IntProperty>("LCD Contrast", "LcdContrast", LcdContrastType);
    BoolProperty *phantomPower = createChild<BoolProperty>("Phantom Power", "PhantomPwr");
    EnumProperty *dispMode = createChild<EnumProperty>("Display", "DispMod", StringArray({"MEMORY NUMBER","LEVEL","REVERSE","1SHOT","MULTI","REMAIN"}));
    EnumProperty *indMod = createChild<EnumProperty>("Indicator", "IndMod", StringArray({"STATUS","LOOP POSITION","LEVEL"}));
    BoolProperty *autoOff = createChild<BoolProperty>("Auto Off", "AutoOff");
    SetupSettings(Library *library, const String &name) : Group(library, name) {}
};

class InputOutputSettings : public Group {
public:
    IntProperty *lineOutLevel = createChild<IntProperty>("Line Out Level", "LineOutLvl", IntProperty::LevelType);
    BoolProperty *inputLineOut = createChild<BoolProperty>("Input Line Out", "InLineOut");
    EnumProperty *outLevelSelect = createChild<EnumProperty>("Out Level Select", "OutLvlSel", StringArray({"LINE+PHONES","PHONES"}));
    InputOutputSettings(Library *library, const String &name) : Group(library, name) {}
};

class UsbSettings : public Group {
public:
    EnumProperty *usbMode = createChild<EnumProperty>("USB Mode", "Mod", StringArray({"STORAGE","AUDIO/MIDI"}));
    EnumProperty *usbAAudio = createChild<EnumProperty>("USB Audio", "AuRouting", StringArray({"LOOP IN","SUB MIX","LINE OUT"}));
    IntProperty *usbInLevel = createChild<IntProperty>("USB IN Level", "InLvl", IntProperty::LevelType);
    IntProperty *usbOutLevel = createChild<IntProperty>("USB OUT Level", "OutLvl", IntProperty::LevelType);
    UsbSettings(Library *library, const String &name) : Group(library, name) {}
};

class MidiSettings : public Group {
public:
    EnumProperty *rxChannel = createChild<EnumProperty>("MIDI Rx Channel", "RxCh", StringArray({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16"}));
    BoolProperty *omni = createChild<BoolProperty>("MIDI Omni", "Omni");
    EnumProperty *txChannel = createChild<EnumProperty>("MIDI Tx Channel", "TxCh", StringArray({"1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","RX"}));
    EnumProperty *sync = createChild<EnumProperty>("MIDI Sync", "Sync", StringArray({"AUTO","INTERNAL"}));
    EnumProperty *syncSource = createChild<EnumProperty>("MIDI Sync Source", "InSel", StringArray({"USB (AUTO)","MIDI"}));
    BoolProperty *pcOut = createChild<BoolProperty>("MIDI PC Out", "PcOut");
    MidiSettings(Library *library, const String &name) : Group(library, name) {}
};

class SystemSettings : public Group {
public:
    SetupSettings *setup = createChild<SetupSettings>("Setup");
    InputOutputSettings *inputOutput = createChild<InputOutputSettings>("Input / Output");
    UsbSettings *usb = createChild<UsbSettings>("USB");
    MidiSettings *midi = createChild<MidiSettings>("MIDI");
    TrackSettings *tracks[5] = {
        createChild<TrackSettings>("Track 1"),
        createChild<TrackSettings>("Track 2"),
        createChild<TrackSettings>("Track 3"),
        createChild<TrackSettings>("Track 4"),
        createChild<TrackSettings>("Track 5")
    };
    RhythmSettings *rhythm = createChild<RhythmSettings>("Rhythm");
    NameProperty *name = createChild<NameProperty>("Name", "NAME");
    MasterSettings *master = createChild<MasterSettings>("Master");
    RecordSettings *record = createChild<RecordSettings>("Record");
    PlaySettings *play = createChild<PlaySettings>("Play");
    AssignmentSettings *assignment[8] = {
        createChild<AssignmentSettings>("Assignment 1"),
        createChild<AssignmentSettings>("Assignment 2"),
        createChild<AssignmentSettings>("Assignment 3"),
        createChild<AssignmentSettings>("Assignment 4"),
        createChild<AssignmentSettings>("Assignment 5"),
        createChild<AssignmentSettings>("Assignment 6"),
        createChild<AssignmentSettings>("Assignment 7"),
        createChild<AssignmentSettings>("Assignment 8")
    };
    InputFxSettings *inputFx = createChild<InputFxSettings>("Input Fx");
    TrackFxSettings *trackFx = createChild<TrackFxSettings>("Track Fx");

    SystemSettings(Library *library) :
        Group(library, "System")
    {
        name->setVisible(false);
    }

    bool loadFromXml(XmlElement *xml) override
    {
        setup->loadFromXml(xml->getChildByName("SETUP"));
        inputOutput->loadFromXml(xml->getChildByName("INPUT_OUTPUT"));
        usb->loadFromXml(xml->getChildByName("USB"));
        midi->loadFromXml(xml->getChildByName("MIDI"));
        for (int i = 0; i < 5; ++i) {
            tracks[i]->loadFromXml(xml->getChildByName(String::formatted("TRACK%d", i + 1)));
        }
        rhythm->loadFromXml(xml->getChildByName("RHYTHM"));
        name->loadFromXml(xml->getChildByName("NAME"));
        master->loadFromXml(xml->getChildByName("MASTER"));
        record->loadFromXml(xml->getChildByName("REC_OPTION"));
        play->loadFromXml(xml->getChildByName("PLAY_OPTION"));
        for (int i = 0; i < 8; ++i) {
            assignment[i]->loadFromXml(xml->getChildByName(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->loadFromXml(xml->getChildByName("INPUT_FX"));
        trackFx->loadFromXml(xml->getChildByName("TRACK_FX"));
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->loadBeatFxFromXml(xml->getChildByName(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            inputFx->fxSlot[i]->loadFromXml(xml->getChildByName(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->loadTrackFxFromXml(xml->getChildByName(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }

    bool saveToXml(XmlElement *xml) override
    {
        setup->saveToXml(xml->createNewChildElement("SETUP"));
        inputOutput->saveToXml(xml->createNewChildElement("INPUT_OUTPUT"));
        usb->saveToXml(xml->createNewChildElement("USB"));
        midi->saveToXml(xml->createNewChildElement("MIDI"));
        for (int i = 0; i < 5; ++i) {
            tracks[i]->saveToXml(xml->createNewChildElement(String::formatted("TRACK%d", i + 1)));
        }
        rhythm->saveToXml(xml->createNewChildElement("RHYTHM"));
        name->saveToXml(xml->createNewChildElement("NAME"));
        master->saveToXml(xml->createNewChildElement("MASTER"));
        record->saveToXml(xml->createNewChildElement("REC_OPTION"));
        play->saveToXml(xml->createNewChildElement("PLAY_OPTION"));
        for (int i = 0; i < 8; ++i) {
            assignment[i]->saveToXml(xml->createNewChildElement(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->saveToXml(xml->createNewChildElement("INPUT_FX"));
        trackFx->saveToXml(xml->createNewChildElement("TRACK_FX"));
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->saveBeatFxToXml(xml->createNewChildElement(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            inputFx->fxSlot[i]->saveToXml(xml->createNewChildElement(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i) {
            trackFx->fxSlot[i]->saveTrackFxToXml(xml->createNewChildElement(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }
};

// ----------------------------------------------------------------------------
// Track
// ----------------------------------------------------------------------------

class Track {
public:
    enum WaveState {
        WaveEmpty,      // no wave file assigned to track
        WaveOriginal,   // original wave file assigned to track (stored in _waveFile)
        WaveChanged,    // changed wave file assigned (stored in _audioBuffer)
    };

    Track(Library *library, Patch &patch, int index);

    int index() const;
    void setIndex(int index);

    TrackSettings *trackSettings() { return &_trackSettings; }

    const WaveState waveState() const { return _waveState; }
    const File &waveFile() const { return _waveFile; }

    const AudioSampleBuffer &audioBuffer() const;
    void setAudioBuffer(const AudioSampleBuffer &audioBuffer);
    void clearAudioBuffer();

    bool loadFromXml(XmlElement *xml);
    bool saveToXml(XmlElement *xml);

private:
    Library *_library;
    Patch &_patch;
    int _index;
    TrackSettings _trackSettings;

    WaveState _waveState;
    File _waveFile;
    mutable AudioSampleBuffer _audioBuffer;
};

// ----------------------------------------------------------------------------
// Patch
// ----------------------------------------------------------------------------

class Patch {
public:
    static const int NumTracks = 5;

    Patch(Library *library);
    Patch(const RC505::Patch &patch);

    int index() const;
    void setIndex(int index);

    const OwnedArray<Track> &tracks() const { return _tracks; }
    PatchSettings *patchSettings() { return &_patchSettings; }
    NameProperty *patchName() { return &_patchName; }

    String name() const;
    void setName(const String &name);

    void moveTrack(int from, int to);

    bool loadFromXml(XmlElement *xml);
    bool saveToXml(XmlElement *xml);

private:
    Library *_library;
    int _index;
    OwnedArray<Track> _tracks;
    PatchSettings _patchSettings;
    NameProperty _patchName;
};

// ----------------------------------------------------------------------------
// Library
// ----------------------------------------------------------------------------

class Library {
public:
    static const int NumPatches = 99;

    class Listener {
    public:
        virtual ~Listener() {}
        virtual void beforeLibraryLoaded() {}
        virtual void afterLibraryLoaded() {}
        virtual void propertyValueChanged(ValueProperty *property) {}
    };

    Library();

    const File &rootPath() const { return _rootPath; }
    const File &dataPath() const { return _dataPath; }
    const File &wavePath() const { return _wavePath; }

    const OwnedArray<Patch> &patches() const { return _patches; }
    SystemSettings *systemSettings() { return &_systemSettings; }

    void movePatches(const Array<Patch *> &patches, int insertIndex);

    void addListener(Listener *listener) { _listeners.add(listener); }
    void removeListener(Listener *listener) { _listeners.remove(listener); }

    bool load(const File &path);
    bool save(const File &path);

    static String checkVolumesForRC505();
    
private:
    bool loadMemory(const File &path);
    bool saveMemory(const File &path);

    bool loadSystem(const File &path);
    bool saveSystem(const File &path);

    bool saveWaveFiles(bool inplace);

    void notifyPropertyValueChanged(ValueProperty *property);

    File _rootPath;
    File _dataPath;
    File _wavePath;

    OwnedArray<Patch> _patches;
    SystemSettings _systemSettings;
    ListenerList<Listener> _listeners;

    friend class ValueProperty;
    friend class Patch;
    friend class Track;
};
    

} // namespace RC505
