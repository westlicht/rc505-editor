#pragma once

#include "JuceHeader.h"

#include <functional>

namespace RC505
{

// this model only support firmware revision 2
static const int Revision = 2;

class Library;
class Patch;

// ----------------------------------------------------------------------------
// Property
// ----------------------------------------------------------------------------

class Property
{
public:
    enum SelectedState
    {
        Unselected,
        Selected,
        PartiallySelected,
    };

    Property(Library* library, const String& name)
        : _library(library)
        , _name(name)
        , _visible(true)
        , _selected(false)
    {
    }

    virtual ~Property()
    {
    }

    const String& name() const { return _name; }

    bool visible() const { return _visible; }
    void setVisible(bool visible) { _visible = visible; }

    virtual bool selected() const { return _selected; }
    virtual void setSelected(bool selected) { _selected = selected; }
    virtual SelectedState selectedState() const { return _selected ? Selected : Unselected; }

    virtual void assign(const Property& other, bool selectedOnly) = 0;

    virtual bool loadFromXml(XmlElement* xml) = 0;
    virtual bool saveToXml(XmlElement* xml) = 0;

protected:
    Library* _library;
    String _name;
    bool _visible;
    bool _selected;
};

// ----------------------------------------------------------------------------
// ValueProperty
// ----------------------------------------------------------------------------

class ValueProperty : public Property
{
public:
    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void valueChanged(ValueProperty* property) = 0;
    };

    ValueProperty(Library* library, const String& name, const String& id)
        : Property(library, name)
        , _id(id)
    {
    }

    const String& id() const { return _id; }

    const String& value() const { return _value; }
    void setValue(const String& value)
    {
        _value = value;
        notifyValueChanged();
    }

    virtual void assign(const Property& other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected())
        {
            setValue(static_cast<const ValueProperty*>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement* xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement())
        {
            return false;
        }
        _value = xml->getFirstChildElement()->getText();
        return true;
    }

    virtual bool saveToXml(XmlElement* xml) override
    {
        xml->addTextElement(_value);
        return true;
    }

    void addListener(Listener* listener)
    {
        _listeners.add(listener);
    }

    void removeListener(Listener* listener)
    {
        _listeners.remove(listener);
    }

protected:
    void notifyValueChanged();

    String _id;
    String _value;
    ListenerList<Listener> _listeners;

    friend class Patch;
};

// ----------------------------------------------------------------------------
// BoolProperty
// ----------------------------------------------------------------------------

class BoolProperty : public ValueProperty
{
public:
    BoolProperty(Library* library, const String& name, const String& desc)
        : ValueProperty(library, name, desc)
    {
    }

    const bool value() const { return _value; }
    void setValue(bool value)
    {
        _value = value;
        notifyValueChanged();
    }

    virtual void assign(const Property& other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected())
        {
            setValue(static_cast<const BoolProperty*>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement* xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement())
        {
            return false;
        }
        _value = xml->getFirstChildElement()->getText().getIntValue() != 0;
        return true;
    }

    virtual bool saveToXml(XmlElement* xml) override
    {
        xml->addTextElement(String(_value ? 1 : 0));
        return true;
    }

private:
    bool _value;
};

// ----------------------------------------------------------------------------
// BitSetProperty
// ----------------------------------------------------------------------------

class BitSetProperty : public ValueProperty
{
public:
    BitSetProperty(Library* library, const String& name, const String& desc, int bits)
        : ValueProperty(library, name, desc)
        , _bits(bits)
    {
    }

    int bits() const { return _bits; }

    const int value() const { return _value; }
    void setValue(int value)
    {
        _value = value;
        notifyValueChanged();
    }

    const bool bitValue(int index)
    {
        jassert(index < _bits);
        return (_value & (1 << index)) != 0;
    }

    void setBitValue(int index, bool value)
    {
        jassert(index < _bits);
        if (value)
        {
            _value |= (1 << index);
        }
        else
        {
            _value &= ~(1 << index);
        }
    }

    virtual void assign(const Property& other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected())
        {
            setValue(static_cast<const BitSetProperty*>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement* xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement())
        {
            return false;
        }
        _value = xml->getFirstChildElement()->getText().getIntValue();
        return true;
    }

    virtual bool saveToXml(XmlElement* xml) override
    {
        xml->addTextElement(String(_value));
        return true;
    }

private:
    int _bits;
    int _value;
};

// ----------------------------------------------------------------------------
// IntProperty
// ----------------------------------------------------------------------------

class IntProperty : public ValueProperty
{
public:
    struct Type
    {
        typedef std::function<float(int)> ToFloat;
        typedef std::function<int(float)> FromFloat;
        typedef std::function<String(float)> Formatter;
        ToFloat toFloat;
        FromFloat fromFloat;
        Formatter formatter;
        float min;
        float max;
        float skew;
        Type(ToFloat toFloat_, FromFloat fromFloat_, Formatter formatter_, float min_, float max_, float skew_ = 1.f)
            : toFloat(toFloat_)
            , fromFloat(fromFloat_)
            , formatter(formatter_)
            , min(min_)
            , max(max_)
            , skew(skew_)
        {
        }
    };

    static Type DefaultType;
    static Type MidiType;
    static Type LevelType;
    static Type PanType;
    static Type TempoType;
    static Type ThresholdType;
    static Type RateType;
    static Type StepRateType;
    static Type DelayTimeType;
    static Type BalanceType;
    static Type SamplesType;

    IntProperty(Library* library, const String& name, const String& desc, const Type& type = DefaultType)
        : ValueProperty(library, name, desc)
        , _type(type)
    {
    }

    const Type& type() const { return _type; }

    const int value() const { return _value; }
    void setValue(int value)
    {
        _value = value;
        notifyValueChanged();
    }

    virtual void assign(const Property& other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected())
        {
            setValue(static_cast<const IntProperty*>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement* xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement())
        {
            return false;
        }
        _value = xml->getFirstChildElement()->getText().getIntValue();
        return true;
    }

    virtual bool saveToXml(XmlElement* xml) override
    {
        xml->addTextElement(String(_value));
        return true;
    }

private:
    const Type& _type;
    int _value;
};

// ----------------------------------------------------------------------------
// EnumProperty
// ----------------------------------------------------------------------------

class EnumProperty : public ValueProperty
{
public:
    EnumProperty(Library* library, const String& name, const String& desc, const StringArray& options)
        : ValueProperty(library, name, desc)
        , _options(options)
    {
    }

    const int value() const { return _value; }
    void setValue(int value)
    {
        _value = value;
        notifyValueChanged();
    }

    const StringArray& options() const { return _options; }

    virtual void assign(const Property& other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected())
        {
            setValue(static_cast<const EnumProperty*>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement* xml) override
    {
        if (!xml->getFirstChildElement() || !xml->getFirstChildElement()->isTextElement())
        {
            return false;
        }
        _value = xml->getFirstChildElement()->getText().getIntValue();
        return true;
    }

    virtual bool saveToXml(XmlElement* xml) override
    {
        xml->addTextElement(String(_value));
        return true;
    }

private:
    StringArray _options;
    int _value;
};

// ----------------------------------------------------------------------------
// NameProperty
// ----------------------------------------------------------------------------

class NameProperty : public ValueProperty
{
public:
    NameProperty(Library* library, const String& name, const String& desc)
        : ValueProperty(library, name, desc)
    {
    }

    const String& value() const { return _value; }
    void setValue(const String& value)
    {
        _value = value;
        notifyValueChanged();
    }

    virtual void assign(const Property& other, bool selectedOnly) override
    {
        if (!selectedOnly || other.selected())
        {
            setValue(static_cast<const NameProperty*>(&other)->value());
        }
    }

    virtual bool loadFromXml(XmlElement* xml) override
    {
        _value.clear();
        for (int i = 0; i < 12; ++i)
        {
            XmlElement* xmlChar = xml->getChildByName(String::formatted("C%02d", i + 1));
            if (!xmlChar)
            {
                return false;
            }
            _value += String::charToString(xmlChar->getFirstChildElement()->getText().getIntValue());
        }
        _value = _value.trim();
        return true;
    }

    virtual bool saveToXml(XmlElement* xml) override
    {
        for (int i = 0; i < 12; ++i)
        {
            XmlElement* xmlChar = xml->createNewChildElement(String::formatted("C%02d", i + 1));
            xmlChar->addTextElement(String(i < _value.length() ? int(_value[i]) : 32));
        }
        return true;
    }

private:
    String _value;
};

// ----------------------------------------------------------------------------
// Group
// ----------------------------------------------------------------------------

class Group : public Property
{
public:
    Group(Library* library, const String& name)
        : Property(library, name)
    {
    }

    Group& operator=(const Group& other)
    {
        jassert(_children.size() == other._children.size());
        for (int i = 0; i < _children.size(); ++i)
        {
            _children[i]->assign(*other._children[i], false);
        }
        return *this;
    }

    const OwnedArray<Property>& children() const { return _children; }

    virtual void setSelected(bool selected) override
    {
        for (auto child : _children)
        {
            child->setSelected(selected);
        }
    }

    virtual SelectedState selectedState() const override
    {
        int unselected = 0, selected = 0;
        for (auto child : _children)
        {
            switch (child->selectedState())
            {
                case Unselected:
                    ++unselected;
                    break;
                case Selected:
                    ++selected;
                    break;
                case PartiallySelected:
                    return PartiallySelected;
                    break;
            }
        }
        if (unselected == 0)
            return Selected;
        if (selected == 0)
            return Unselected;
        return PartiallySelected;
    }

    virtual void assign(const Property& other, bool selectedOnly) override
    {
        const Group* group = static_cast<const Group*>(&other);
        jassert(_children.size() == group->_children.size());
        for (int i = 0; i < _children.size(); ++i)
        {
            _children[i]->assign(*group->_children[i], selectedOnly);
        }
    }

    virtual bool loadFromXml(XmlElement* xml) override
    {
        for (auto child : _children)
        {
            if (ValueProperty* property = dynamic_cast<ValueProperty*>(child))
            {
                if (!property->loadFromXml(xml->getChildByName(property->id())))
                {
                    return false;
                }
            }
        }
        return true;
    }

    virtual bool saveToXml(XmlElement* xml) override
    {
        for (auto child : _children)
        {
            if (ValueProperty* property = dynamic_cast<ValueProperty*>(child))
            {
                if (!property->saveToXml(xml->createNewChildElement(property->id())))
                {
                    return false;
                }
            }
        }
        return true;
    }

protected:
    void addChild(Property* child)
    {
        _children.add(child);
    }

    template <typename T, typename... Args>
    T* createChild(Args&&... args)
    {
        T* child = new T(_library, std::forward<Args>(args)...);
        _children.add(child);
        return child;
    }

    OwnedArray<Property> _children;
};

// ----------------------------------------------------------------------------
// Group classes
// ----------------------------------------------------------------------------

class TrackSettings : public Group
{
public:
    BoolProperty* reverse = createChild<BoolProperty>("Reverse", "Rev");
    IntProperty* playLevel = createChild<IntProperty>("Play Level", "PlyLvl", IntProperty::LevelType);
    IntProperty* pan = createChild<IntProperty>("Pan", "Pan", IntProperty::PanType);
    BoolProperty* oneShot = createChild<BoolProperty>("One Shot", "One");
    BoolProperty* trackFx = createChild<BoolProperty>("Track Fx", "TrkFx");
    EnumProperty* playMode = createChild<EnumProperty>("Play Mode", "PlyMod", StringArray({ "MULTI", "SINGLE" }));
    EnumProperty* startMode = createChild<EnumProperty>("Start Mode", "StrtMod", StringArray({ "IMMEDIATE", "FADE IN" }));
    EnumProperty* stopMode = createChild<EnumProperty>("Stop Mode", "StpMod", StringArray({ "IMMEDIATE", "FADE OUT", "LOOP END" }));
    Property* measMod = createChild<ValueProperty>("MeasMod", "MeasMod"); // TODO ?
    IntProperty* measLen = createChild<IntProperty>("MeasLen", "MeasLen"); // TODO ?
    Property* measBtLp = createChild<ValueProperty>("MeasBtLp", "MeasBtLp"); // TODO ?
    BoolProperty* loopSync = createChild<BoolProperty>("Loop Sync", "LpSync");
    BoolProperty* tempoSync = createChild<BoolProperty>("Tempo Sync", "TmpSync");
    IntProperty* recTmp = createChild<IntProperty>("RecTmp", "RecTmp", IntProperty::TempoType); // TODO ?
    BoolProperty* wavStat = createChild<BoolProperty>("WavStat", "WavStat"); // TODO ?
    IntProperty* wavLen = createChild<IntProperty>("WavLen", "WavLen", IntProperty::SamplesType); // TODO ?

    TrackSettings(Library* library, const String& name)
        : Group(library, name)
    {
        measMod->setVisible(false);
        measLen->setVisible(false);
        measBtLp->setVisible(false);
        recTmp->setVisible(false);
        wavStat->setVisible(false);
        wavLen->setVisible(false);
    }
};

class RhythmSettings : public Group
{
    static StringArray PatternNames;

public:
    BoolProperty* switch_ = createChild<BoolProperty>("Switch", "Sw");
    IntProperty* level = createChild<IntProperty>("Level", "Lvl", IntProperty::LevelType);
    EnumProperty* pattern = createChild<EnumProperty>("Pattern", "Ptn", PatternNames);
    Property* beat = createChild<ValueProperty>("Beat", "Bt"); // TODO 4/4 ?
    BoolProperty* lineOut = createChild<BoolProperty>("Line Out", "LineOut");
    BoolProperty* recCount = createChild<BoolProperty>("Rec Count", "RecCnt");
    BoolProperty* playCount = createChild<BoolProperty>("Play Count", "PlyCnt");
    EnumProperty* stop = createChild<EnumProperty>("Stop", "Stp", StringArray({ "OFF", "LOOPER STOP", "REC END" }));

    RhythmSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class MasterSettings : public Group
{
public:
    IntProperty* level = createChild<IntProperty>("Level", "Lvl", IntProperty::LevelType);
    IntProperty* tempo = createChild<IntProperty>("Tempo", "Tmp", IntProperty::TempoType);
    IntProperty* comp = createChild<IntProperty>("Comp", "Cs", IntProperty::ThresholdType);
    IntProperty* reverb = createChild<IntProperty>("Reverb", "Rv");
    EnumProperty* phonesOut = createChild<EnumProperty>("Phones Out", "PhOut", StringArray({ "MAIN", "MONITOR" }));
    BitSetProperty* phonesOutTracks = createChild<BitSetProperty>("Phones Out Tracks", "PhOutTr", 5);
    MasterSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class RecordSettings : public Group
{
public:
    EnumProperty* overdubMode = createChild<EnumProperty>("Overdub Mode", "DubMod", StringArray({ "OVERDUB", "REPLACE" }));
    EnumProperty* recordAction = createChild<EnumProperty>("Record Action", "RecAct", StringArray({ "REC->OVERDUB", "REC->PLAY" }));
    ValueProperty* recordQuantize = createChild<ValueProperty>("Record Quantize", "RecQtz");
    RecordSettings(Library* library, const String& name)
        : Group(library, name)
    {
        recordQuantize->setVisible(false);
    }
};

class PlaySettings : public Group
{
public:
    static IntProperty::Type LoopLengthType;
    static IntProperty::Type FadeTimeType;

    EnumProperty* singlePlayChange = createChild<EnumProperty>("Single Play Change", "SinglePlyChg", StringArray({ "IMMEDIATE", "LOOP END" }));
    EnumProperty* quantize = createChild<EnumProperty>("Quantize", "LpQtz", StringArray({ "REC END", "MEASURE", "BEAT" }));
    Property* loopMode = createChild<ValueProperty>("Loop Mode", "LpMod"); // TODO ?
    IntProperty* loopLength = createChild<IntProperty>("Loop Length", "LpLen", LoopLengthType);
    BitSetProperty* allStart = createChild<BitSetProperty>("All Start", "AllStrt", 5);
    BitSetProperty* allStop = createChild<BitSetProperty>("All Stop", "AllStp", 5);
    IntProperty* fadeTime = createChild<IntProperty>("Fade Time", "FadeTim", FadeTimeType);
    EnumProperty* targetTrack = createChild<EnumProperty>("Target Track", "TgtTr", StringArray({ "TRACK1", "TRACK2", "TRACK3", "TRACK4", "TRACK5" }));
    PlaySettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class AssignmentSettings : public Group
{
    static StringArray SourceNames;
    static StringArray TargetNames;

public:
    BoolProperty* switch_ = createChild<BoolProperty>("Switch", "Sw");
    EnumProperty* source = createChild<EnumProperty>("Source", "Src", SourceNames);
    EnumProperty* sourceMode = createChild<EnumProperty>("Source Mode", "SrcMod", StringArray({ "MOMENT", "TOGGLE" }));
    EnumProperty* target = createChild<EnumProperty>("Target", "Tgt", TargetNames);
    IntProperty* targetMin = createChild<IntProperty>("Target Min", "TgtMin", IntProperty::MidiType);
    IntProperty* targetMax = createChild<IntProperty>("Target Max", "TgtMax", IntProperty::MidiType);
    AssignmentSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class FilterFxSettings : public Group
{
public:
    EnumProperty* type = createChild<EnumProperty>("Type", "FilTyp", StringArray({ "LPF", "BPF", "HPF" }));
    IntProperty* rate = createChild<IntProperty>("Rate", "FilRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "FilDep");
    IntProperty* resonance = createChild<IntProperty>("Resonance", "FilReso");
    IntProperty* cutoff = createChild<IntProperty>("Cutoff", "FilCut");
    IntProperty* stepRate = createChild<IntProperty>("Step Rate", "FilStpRat", IntProperty::StepRateType);
    FilterFxSettings(Library* library)
        : Group(library, "Filter")
    {
    }
};

class PhaserFxSettings : public Group
{
public:
    IntProperty* rate = createChild<IntProperty>("Rate", "PhRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "PhDep");
    IntProperty* resonance = createChild<IntProperty>("Resonance", "PhReso");
    IntProperty* stepRate = createChild<IntProperty>("Step Rate", "PhStpRat", IntProperty::StepRateType);
    IntProperty* level = createChild<IntProperty>("Level", "PhLvl");
    PhaserFxSettings(Library* library)
        : Group(library, "Phaser")
    {
    }
};

class FlangerFxSettings : public Group
{
public:
    IntProperty* rate = createChild<IntProperty>("Rate", "FlRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "FlDep");
    IntProperty* resonance = createChild<IntProperty>("Resonance", "FlReso");
    IntProperty* stepRate = createChild<IntProperty>("Step Rate", "FlStpRat", IntProperty::StepRateType);
    IntProperty* level = createChild<IntProperty>("Level", "FlLvl");
    FlangerFxSettings(Library* library)
        : Group(library, "Flanger")
    {
    }
};

class SynthFxSettings : public Group
{
public:
    IntProperty* frequency = createChild<IntProperty>("Frequency", "SynFreq");
    IntProperty* resonance = createChild<IntProperty>("Resonance", "SynReso");
    IntProperty* decay = createChild<IntProperty>("Decay", "SynDecay");
    IntProperty* balance = createChild<IntProperty>("Balance", "SynBal", IntProperty::BalanceType);
    SynthFxSettings(Library* library)
        : Group(library, "Synth")
    {
    }
};

class LoFiFxSettings : public Group
{
    static IntProperty::Type BitDepthType;
    static IntProperty::Type SampleRateType;

public:
    IntProperty* bitDepth = createChild<IntProperty>("BitDepth", "LoFiDep", BitDepthType);
    IntProperty* sampleRate = createChild<IntProperty>("SampleRate", "LoFiSmpl", SampleRateType);
    IntProperty* balance = createChild<IntProperty>("Balance", "LoFiBal", IntProperty::BalanceType);
    LoFiFxSettings(Library* library)
        : Group(library, "Lo-Fi")
    {
    }
};

class RingModulatorFxSettings : public Group
{
public:
    IntProperty* frequency = createChild<IntProperty>("Frequency", "RmFreq");
    IntProperty* balance = createChild<IntProperty>("Balance", "RmBal", IntProperty::BalanceType);
    RingModulatorFxSettings(Library* library)
        : Group(library, "Ring Modulator")
    {
    }
};

class GuitarToBassFxSettings : public Group
{
public:
    IntProperty* balance = createChild<IntProperty>("Balance", "GToB", IntProperty::BalanceType);
    GuitarToBassFxSettings(Library* library)
        : Group(library, "Guitar To Bass")
    {
    }
};

class SlowGearFxSettings : public Group
{
public:
    IntProperty* sensitivity = createChild<IntProperty>("Sensitivity", "SgSens");
    IntProperty* riseTime = createChild<IntProperty>("Rise Time", "SgRisTim");
    IntProperty* level = createChild<IntProperty>("Level", "SgLvl");
    SlowGearFxSettings(Library* library)
        : Group(library, "Slow Gear")
    {
    }
};

class TransposeFxSettings : public Group
{
    static IntProperty::Type TransType;

public:
    IntProperty* trans = createChild<IntProperty>("Trans", "Trans", TransType);
    TransposeFxSettings(Library* library)
        : Group(library, "Transpose")
    {
    }
};

class PitchBendFxSettings : public Group
{
public:
    EnumProperty* pitch = createChild<EnumProperty>("Pitch", "PbPit", StringArray({ "-3 Octaves", "-2 Octaves", "-1 Octaves", "+1 Octaves", "+2 Octaves", "+3 Octaves", "+4 Octaves" }));
    IntProperty* bend = createChild<IntProperty>("Bend", "PbBnd");
    PitchBendFxSettings(Library* library)
        : Group(library, "Pitch Bend")
    {
    }
};

class RobotFxSettings : public Group
{
    static IntProperty::Type GenderType;

public:
    EnumProperty* note = createChild<EnumProperty>("Note", "RoboNote", StringArray({ "C", "Db", "D", "Eb", "E", "F", "F#", "G", "Ab", "A", "Bb", "B" }));
    IntProperty* gender = createChild<IntProperty>("Gender", "RoboGen", GenderType);
    RobotFxSettings(Library* library)
        : Group(library, "Robot")
    {
    }
};

class VocalDistFxSettings : public Group
{
    static IntProperty::Type ToneType;

public:
    IntProperty* dist = createChild<IntProperty>("Dist", "DistDist");
    IntProperty* tone = createChild<IntProperty>("Tone", "DistTon", ToneType);
    IntProperty* level = createChild<IntProperty>("Level", "DistLvl");
    VocalDistFxSettings(Library* library)
        : Group(library, "Vocal Dist")
    {
    }
};

class VocoderFxSettings : public Group
{
public:
    EnumProperty* carrier = createChild<EnumProperty>("Carrier", "VocoCar", StringArray({ "TRACK1", "TRACK2", "TRACK3", "TRACK4", "TRACK5" }));
    IntProperty* modSens = createChild<IntProperty>("ModSens", "VocoSens");
    IntProperty* attack = createChild<IntProperty>("Attack", "VocoAtck");
    IntProperty* balance = createChild<IntProperty>("Balance", "VocoBal", IntProperty::BalanceType);
    VocoderFxSettings(Library* library)
        : Group(library, "Vocoder")
    {
    }
};

class DynamicsFxSettings : public Group
{
public:
    EnumProperty* type = createChild<EnumProperty>("Type", "DynmTyp", StringArray({ "NATURAL COMP", "MIXER COMP", "LIVE COMP", "NATURAL LIM", "HARD LIM", "JINGL COMP", "HARD COMP", "SOFT COMP", "CLEAN COMP", "DANCE COMP", "ORCH COMP", "VOCAL COMP", "ACOUSTIC", "ROCK BAND", "ORCHESTRA", "LOW BOOST", "BRIGHTEN", "DJs VOICE", "PHONE VOX" }));
    IntProperty* dynamics = createChild<IntProperty>("Dynamics", "DynmDynm", IntProperty::ThresholdType);
    DynamicsFxSettings(Library* library)
        : Group(library, "Dynamics")
    {
    }
};

class EqFxSettings : public Group
{
public:
    IntProperty* low = createChild<IntProperty>("Low", "EqLo", IntProperty::ThresholdType);
    IntProperty* lowMid = createChild<IntProperty>("Low-Mid", "EqLoMd", IntProperty::ThresholdType);
    IntProperty* highMid = createChild<IntProperty>("High-Mid", "EqHiMd", IntProperty::ThresholdType);
    IntProperty* high = createChild<IntProperty>("High", "EqHi", IntProperty::ThresholdType);
    IntProperty* level = createChild<IntProperty>("Level", "EqLvl", IntProperty::ThresholdType);
    EqFxSettings(Library* library)
        : Group(library, "Eq")
    {
    }
};

class IsolatorFxSettings : public Group
{
public:
    EnumProperty* band = createChild<EnumProperty>("Band", "IsoBnd", StringArray({ "LOW", "MID", "HIGH" }));
    IntProperty* rate = createChild<IntProperty>("Rate", "IsoRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "IsoDep");
    IntProperty* stepRate = createChild<IntProperty>("Step Rate", "IsoStpRat", IntProperty::StepRateType);
    IntProperty* bandLevel = createChild<IntProperty>("BandLevel", "IsoLvl");
    IsolatorFxSettings(Library* library)
        : Group(library, "Isolator")
    {
    }
};

class OctaveFxSettings : public Group
{
public:
    EnumProperty* mode = createChild<EnumProperty>("Mode", "OctMod", StringArray({ "-1OCT", "-2OCT", "-1OCT&-2OCT" }));
    IntProperty* octLevel = createChild<IntProperty>("Oct.Level", "OctLvl");
    OctaveFxSettings(Library* library)
        : Group(library, "Octave")
    {
    }
};

class PanFxSettings : public Group
{
public:
    EnumProperty* type = createChild<EnumProperty>("Type", "PanTyp", StringArray({ "AUTO1", "AUTO2", "MANUAL" }));
    IntProperty* rate = createChild<IntProperty>("Rate", "PanRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "PanDep");
    IntProperty* stepRate = createChild<IntProperty>("Step Rate", "PanStpRat", IntProperty::StepRateType);
    IntProperty* position = createChild<IntProperty>("Position", "PanPos", IntProperty::PanType);
    PanFxSettings(Library* library)
        : Group(library, "Pan")
    {
    }
};

class TremoloFxSettings : public Group
{
public:
    IntProperty* rate = createChild<IntProperty>("Rate", "TrRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "TrDep");
    IntProperty* level = createChild<IntProperty>("Level", "TrLvl");
    TremoloFxSettings(Library* library)
        : Group(library, "Tremolo")
    {
    }
};

class SlicerFxSettings : public Group
{
public:
    EnumProperty* pattern = createChild<EnumProperty>("Pattern", "SlPat", StringArray({ "P01", "P02", "P03", "P04", "P05", "P06", "P07", "P08", "P09", "P10", "P11", "P12", "P13", "P14", "P15", "P16", "P17", "P18", "P19", "P20" }));
    IntProperty* rate = createChild<IntProperty>("Rate", "SlRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "SlDep");
    SlicerFxSettings(Library* library)
        : Group(library, "Slicer")
    {
    }
};

class DelayFxSettings : public Group
{
public:
    IntProperty* time = createChild<IntProperty>("Time", "DlyTim", IntProperty::DelayTimeType);
    IntProperty* feedback = createChild<IntProperty>("Feedback", "DlyFb");
    IntProperty* level = createChild<IntProperty>("Level", "DlyLvl");
    DelayFxSettings(Library* library)
        : Group(library, "Delay")
    {
    }
};

class PanningDelayFxSettings : public Group
{
public:
    IntProperty* time = createChild<IntProperty>("Time", "PanDlyTim", IntProperty::DelayTimeType);
    IntProperty* feedback = createChild<IntProperty>("Feedback", "PanDlyFb");
    IntProperty* level = createChild<IntProperty>("Level", "PanDlyLvl");
    PanningDelayFxSettings(Library* library)
        : Group(library, "Panning Delay")
    {
    }
};

class TapeEchoFxSettings : public Group
{
public:
    IntProperty* repeatRate = createChild<IntProperty>("RepeatRate", "EchRat");
    IntProperty* intensity = createChild<IntProperty>("Intensity", "EchInt");
    IntProperty* echoLevel = createChild<IntProperty>("EchoLevel", "EchLvl");
    TapeEchoFxSettings(Library* library)
        : Group(library, "Tape Echo")
    {
    }
};

class GranularDelayFxSettings : public Group
{
public:
    IntProperty* time = createChild<IntProperty>("Time", "GraTim");
    IntProperty* feedback = createChild<IntProperty>("Feedback", "GraFb");
    IntProperty* level = createChild<IntProperty>("Level", "GraLvl");
    GranularDelayFxSettings(Library* library)
        : Group(library, "Granular Delay")
    {
    }
};

class RollFxSettings : public Group
{
    static IntProperty::Type TimeType;

public:
    IntProperty* time = createChild<IntProperty>("Time", "RllTim", IntProperty::DelayTimeType);
    EnumProperty* mode = createChild<EnumProperty>("Mode", "RllMod", StringArray({ "OFF", "1/2", "1/4", "1/8", "1/16" }));
    IntProperty* feedback = createChild<IntProperty>("Feedback", "RllFb");
    IntProperty* level = createChild<IntProperty>("Level", "RllLvl");
    RollFxSettings(Library* library)
        : Group(library, "Roll")
    {
    }
};

class ChorusFxSettings : public Group
{
public:
    IntProperty* rate = createChild<IntProperty>("Rate", "ChoRat", IntProperty::RateType);
    IntProperty* depth = createChild<IntProperty>("Depth", "ChoDep");
    IntProperty* level = createChild<IntProperty>("Level", "ChoLvl");
    ChorusFxSettings(Library* library)
        : Group(library, "Chorus")
    {
    }
};

class ReverbFxSettings : public Group
{
    static IntProperty::Type TimeType;

public:
    IntProperty* time = createChild<IntProperty>("Time", "RevTim", TimeType);
    IntProperty* level = createChild<IntProperty>("Level", "RevLvl");
    IntProperty* dryLevel = createChild<IntProperty>("Dry Level", "RevDLvl");
    ReverbFxSettings(Library* library)
        : Group(library, "Reverb")
    {
    }
};

class BeatRepeatFxSettings : public Group
{
public:
    EnumProperty* type = createChild<EnumProperty>("Type", "RepTyp", StringArray({ "FORWARD", "REWIND", "MIX" }));
    EnumProperty* length = createChild<EnumProperty>("Length", "RepLen", StringArray({ "THRU", "Whole Note", "Half Note", "Quarter Note", "Eighth Note", "Sixteenth Note", "32th Note" }));
    BeatRepeatFxSettings(Library* library)
        : Group(library, "Beat Repeat")
    {
    }
};

class BeatShiftFxSettings : public Group
{
public:
    EnumProperty* type = createChild<EnumProperty>("Type", "ShftTyp", StringArray({ "FUTURE", "PAST" }));
    EnumProperty* shift = createChild<EnumProperty>("Shift", "ShftShft", StringArray({ "THRU", "Sixteenth Note", "Eighth Note", "Quarter Note", "Half Note", "Whole Note" }));
    BeatShiftFxSettings(Library* library)
        : Group(library, "Beat Shift")
    {
    }
};

class BeatScatterFxSettings : public Group
{
public:
    EnumProperty* type = createChild<EnumProperty>("Type", "ScatTyp", StringArray({ "P1", "P2", "P3", "P4" }));
    EnumProperty* length = createChild<EnumProperty>("Length", "ScatLen", StringArray({ "THRU", "Half Note", "Quarter Note", "Eighth Note", "Sixteenth Note" }));
    BeatScatterFxSettings(Library* library)
        : Group(library, "Beat Scatter")
    {
    }
};

class VinylFlickFxSettings : public Group
{
public:
    IntProperty* flick = createChild<IntProperty>("Flick", "Flick");
    VinylFlickFxSettings(Library* library)
        : Group(library, "Vinyl Flick")
    {
    }
};

class InputFxSlotSettings : public Group
{
public:
    FilterFxSettings* filter = createChild<FilterFxSettings>();
    PhaserFxSettings* phaser = createChild<PhaserFxSettings>();
    FlangerFxSettings* flanger = createChild<FlangerFxSettings>();
    SynthFxSettings* synth = createChild<SynthFxSettings>();
    LoFiFxSettings* lofi = createChild<LoFiFxSettings>();
    RingModulatorFxSettings* ringmod = createChild<RingModulatorFxSettings>();
    GuitarToBassFxSettings* guitarToBass = createChild<GuitarToBassFxSettings>();
    SlowGearFxSettings* slowGear = createChild<SlowGearFxSettings>();
    TransposeFxSettings* transpose = createChild<TransposeFxSettings>();
    PitchBendFxSettings* pitchBend = createChild<PitchBendFxSettings>();
    RobotFxSettings* robot = createChild<RobotFxSettings>();
    VocalDistFxSettings* vocalDist = createChild<VocalDistFxSettings>();
    VocoderFxSettings* vocoder = createChild<VocoderFxSettings>();
    DynamicsFxSettings* dynamics = createChild<DynamicsFxSettings>();
    EqFxSettings* eq = createChild<EqFxSettings>();
    IsolatorFxSettings* isolator = createChild<IsolatorFxSettings>();
    OctaveFxSettings* octave = createChild<OctaveFxSettings>();
    PanFxSettings* pan = createChild<PanFxSettings>();
    TremoloFxSettings* tremolo = createChild<TremoloFxSettings>();
    SlicerFxSettings* slicer = createChild<SlicerFxSettings>();
    DelayFxSettings* delay = createChild<DelayFxSettings>();
    PanningDelayFxSettings* panningDelay = createChild<PanningDelayFxSettings>();
    TapeEchoFxSettings* tapeEcho = createChild<TapeEchoFxSettings>();
    GranularDelayFxSettings* granularDelay = createChild<GranularDelayFxSettings>();
    RollFxSettings* roll = createChild<RollFxSettings>();
    ChorusFxSettings* chorus = createChild<ChorusFxSettings>();
    ReverbFxSettings* reverb = createChild<ReverbFxSettings>();

    InputFxSlotSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }

    bool loadFromXml(XmlElement* xml) override
    {
        for (auto child : _children)
        {
            if (!child->loadFromXml(xml))
            {
                return false;
            }
        }
        return true;
    }

    bool saveToXml(XmlElement* xml) override
    {
        for (auto child : _children)
        {
            if (!child->saveToXml(xml))
            {
                return false;
            }
        }
        return true;
    }
};

class InputFxSettings : public Group
{
    static StringArray SingleFxNames;
    static StringArray MultiFxNames[3];

public:
    EnumProperty* mode = createChild<EnumProperty>("Mode", "Mod", StringArray({ "MULTI", "SINGLE" }));
    BoolProperty* multiMode = createChild<BoolProperty>("Multi Mode", "MltMod");
    BoolProperty* switch_ = createChild<BoolProperty>("Switch", "Sw");
    EnumProperty* selected = createChild<EnumProperty>("Selected", "Sel", StringArray({ "Fx A", "Fx B", "Fx C" }));
    EnumProperty* fxType[3];
    BitSetProperty* multiSwitch;
    EnumProperty* multiFxType[3];
    InputFxSlotSettings* fxSlot[3];

    InputFxSettings(Library* library, const String& name)
        : Group(library, name)
    {
        for (int i = 0; i < 3; ++i)
        {
            fxType[i] = createChild<EnumProperty>(String::formatted("Fx %c Type", 'A' + i), String::formatted("Typ%d", i + 1), SingleFxNames);
        }
        multiMode->setVisible(false);
        multiSwitch = createChild<BitSetProperty>("Multi Switch", "MltSw", 3);
        for (int i = 0; i < 3; ++i)
        {
            multiFxType[i] = createChild<EnumProperty>(String::formatted("Multi Fx %c Type", 'A' + i), String::formatted("MltTyp%d", i + 1), MultiFxNames[i]);
        }
        for (int i = 0; i < 3; ++i)
        {
            fxSlot[i] = createChild<InputFxSlotSettings>(String::formatted("Fx %c", 'A' + i));
        }
    }

    bool saveToXml(XmlElement* xml) override
    {
        multiMode->setValue(mode->value() == 0);
        return Group::saveToXml(xml);
    }
};

class TrackFxSlotSettings : public Group
{
public:
    FilterFxSettings* filter = createChild<FilterFxSettings>();
    PhaserFxSettings* phaser = createChild<PhaserFxSettings>();
    FlangerFxSettings* flanger = createChild<FlangerFxSettings>();
    SynthFxSettings* synth = createChild<SynthFxSettings>();
    LoFiFxSettings* lofi = createChild<LoFiFxSettings>();
    RingModulatorFxSettings* ringmod = createChild<RingModulatorFxSettings>();
    GuitarToBassFxSettings* guitarToBass = createChild<GuitarToBassFxSettings>();
    SlowGearFxSettings* slowGear = createChild<SlowGearFxSettings>();
    TransposeFxSettings* transpose = createChild<TransposeFxSettings>();
    PitchBendFxSettings* pitchBend = createChild<PitchBendFxSettings>();
    RobotFxSettings* robot = createChild<RobotFxSettings>();
    VocalDistFxSettings* vocalDist = createChild<VocalDistFxSettings>();
    VocoderFxSettings* vocoder = createChild<VocoderFxSettings>();
    DynamicsFxSettings* dynamics = createChild<DynamicsFxSettings>();
    EqFxSettings* eq = createChild<EqFxSettings>();
    IsolatorFxSettings* isolator = createChild<IsolatorFxSettings>();
    OctaveFxSettings* octave = createChild<OctaveFxSettings>();
    PanFxSettings* pan = createChild<PanFxSettings>();
    TremoloFxSettings* tremolo = createChild<TremoloFxSettings>();
    SlicerFxSettings* slicer = createChild<SlicerFxSettings>();
    DelayFxSettings* delay = createChild<DelayFxSettings>();
    PanningDelayFxSettings* panningDelay = createChild<PanningDelayFxSettings>();
    TapeEchoFxSettings* tapeEcho = createChild<TapeEchoFxSettings>();
    GranularDelayFxSettings* granularDelay = createChild<GranularDelayFxSettings>();
    RollFxSettings* roll = createChild<RollFxSettings>();
    ChorusFxSettings* chorus = createChild<ChorusFxSettings>();
    ReverbFxSettings* reverb = createChild<ReverbFxSettings>();

    BeatRepeatFxSettings* beatRepeat = createChild<BeatRepeatFxSettings>();
    BeatShiftFxSettings* beatShift = createChild<BeatShiftFxSettings>();
    BeatScatterFxSettings* beatScatter = createChild<BeatScatterFxSettings>();
    VinylFlickFxSettings* vinylFlick = createChild<VinylFlickFxSettings>();

    TrackFxSlotSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }

    bool loadTrackFxFromXml(XmlElement* xml)
    {
        Group* trackFx[] = { filter, phaser, flanger, synth, lofi, ringmod, guitarToBass, slowGear, transpose, pitchBend, robot, vocalDist, vocoder, dynamics, eq, isolator, octave, pan, tremolo, slicer, delay, panningDelay, tapeEcho, granularDelay, roll, chorus, reverb };
        for (auto child : trackFx)
        {
            if (!child->loadFromXml(xml))
            {
                return false;
            }
        }
        return true;
    }

    bool saveTrackFxToXml(XmlElement* xml)
    {
        Group* trackFx[] = { filter, phaser, flanger, synth, lofi, ringmod, guitarToBass, slowGear, transpose, pitchBend, robot, vocalDist, vocoder, dynamics, eq, isolator, octave, pan, tremolo, slicer, delay, panningDelay, tapeEcho, granularDelay, roll, chorus, reverb };
        for (auto child : trackFx)
        {
            if (!child->saveToXml(xml))
            {
                return false;
            }
        }
        return true;
    }

    bool loadBeatFxFromXml(XmlElement* xml)
    {
        Group* beatFx[] = { beatRepeat, beatShift, beatScatter, vinylFlick };
        for (auto child : beatFx)
        {
            if (!child->loadFromXml(xml))
            {
                return false;
            }
        }
        return true;
    }

    bool saveBeatFxToXml(XmlElement* xml)
    {
        Group* beatFx[] = { beatRepeat, beatShift, beatScatter, vinylFlick };
        for (auto child : beatFx)
        {
            if (!child->saveToXml(xml))
            {
                return false;
            }
        }
        return true;
    }
};

class TrackFxSettings : public Group
{
    static StringArray SingleFxNames;
    static StringArray MultiFxNames[3];

public:
    EnumProperty* mode = createChild<EnumProperty>("Mode", "Mod", StringArray({ "MULTI", "SINGLE" }));
    BoolProperty* multiMode = createChild<BoolProperty>("Multi Mode", "MltMod");
    BoolProperty* switch_ = createChild<BoolProperty>("Switch", "Sw");
    EnumProperty* selected = createChild<EnumProperty>("Selected", "Sel", StringArray({ "Fx A", "Fx B", "Fx C" }));
    EnumProperty* fxType[3];
    BitSetProperty* multiSwitch;
    EnumProperty* multiFxType[3];
    TrackFxSlotSettings* fxSlot[3];

    TrackFxSettings(Library* library, const String& name)
        : Group(library, name)
    {
        for (int i = 0; i < 3; ++i)
        {
            fxType[i] = createChild<EnumProperty>(String::formatted("Fx %c Type", 'A' + i), String::formatted("Typ%d", i + 1), SingleFxNames);
        }
        multiMode->setVisible(false);
        multiSwitch = createChild<BitSetProperty>("Multi Switch", "MltSw", 3);
        for (int i = 0; i < 3; ++i)
        {
            multiFxType[i] = createChild<EnumProperty>(String::formatted("Multi Fx %c Type", 'A' + i), String::formatted("MltTyp%d", i + 1), MultiFxNames[i]);
        }
        for (int i = 0; i < 3; ++i)
        {
            fxSlot[i] = createChild<TrackFxSlotSettings>(String::formatted("Fx %c", 'A' + i));
        }
    }

    bool saveToXml(XmlElement* xml) override
    {
        multiMode->setValue(mode->value() == 0);
        return Group::saveToXml(xml);
    }
};

class PatchSettings : public Group
{
public:
    static const int NumAssignments = 16;

    RhythmSettings* rhythm = createChild<RhythmSettings>("Rhythm");
    MasterSettings* master = createChild<MasterSettings>("Master");
    RecordSettings* record = createChild<RecordSettings>("Record");
    PlaySettings* play = createChild<PlaySettings>("Play");
    InputFxSettings* inputFx = createChild<InputFxSettings>("Input Fx");
    TrackFxSettings* trackFx = createChild<TrackFxSettings>("Track Fx");
    AssignmentSettings* assignments[NumAssignments];

    PatchSettings(Library* library)
        : Group(library, "Patch")
    {
        for (int i = 0; i < NumAssignments; ++i)
        {
            assignments[i] = createChild<AssignmentSettings>(String::formatted("Assignment %d", i + 1));
        }
    }

    bool loadFromXml(XmlElement* xml) override
    {
        rhythm->loadFromXml(xml->getChildByName("RHYTHM"));
        master->loadFromXml(xml->getChildByName("MASTER"));
        record->loadFromXml(xml->getChildByName("REC_OPTION"));
        play->loadFromXml(xml->getChildByName("PLAY_OPTION"));
        for (int i = 0; i < NumAssignments; ++i)
        {
            assignments[i]->loadFromXml(xml->getChildByName(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->loadFromXml(xml->getChildByName("INPUT_FX"));
        trackFx->loadFromXml(xml->getChildByName("TRACK_FX"));
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->loadBeatFxFromXml(xml->getChildByName(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            inputFx->fxSlot[i]->loadFromXml(xml->getChildByName(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->loadTrackFxFromXml(xml->getChildByName(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }

    bool saveToXml(XmlElement* xml) override
    {
        rhythm->saveToXml(xml->createNewChildElement("RHYTHM"));
        master->saveToXml(xml->createNewChildElement("MASTER"));
        record->saveToXml(xml->createNewChildElement("REC_OPTION"));
        play->saveToXml(xml->createNewChildElement("PLAY_OPTION"));
        for (int i = 0; i < NumAssignments; ++i)
        {
            assignments[i]->saveToXml(xml->createNewChildElement(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->saveToXml(xml->createNewChildElement("INPUT_FX"));
        trackFx->saveToXml(xml->createNewChildElement("TRACK_FX"));
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->saveBeatFxToXml(xml->createNewChildElement(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            inputFx->fxSlot[i]->saveToXml(xml->createNewChildElement(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->saveTrackFxToXml(xml->createNewChildElement(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }
};

class SetupSettings : public Group
{
    static IntProperty::Type MemNumType;
    static IntProperty::Type LcdContrastType;

public:
    IntProperty* memNum = createChild<IntProperty>("Memory", "MemNum", MemNumType);
    IntProperty* lcdContrast = createChild<IntProperty>("LCD Contrast", "LcdContrast", LcdContrastType);
    BoolProperty* phantomPower = createChild<BoolProperty>("Phantom Power", "PhantomPwr");
    EnumProperty* dispModeV2 = createChild<EnumProperty>("Display", "DispMod", StringArray({ "MEMORY NUMBER", "LEVEL", "REVERSE", "1SHOT", "MULTI", "REMAIN", "FX" }));
    EnumProperty* indModV2 = createChild<EnumProperty>("Indicator", "IndMod", StringArray({ "STATUS", "LOOP POSITION", "POSITION+STATUS", "LEVEL" }));
    BoolProperty* autoOff = createChild<BoolProperty>("Auto Off", "AutoOff");
    BoolProperty* allClear = createChild<BoolProperty>("All Clear", "AllClr");
    BoolProperty* quickClear = createChild<BoolProperty>("Quick Clear", "QuickClr");
    EnumProperty* knobMode = createChild<EnumProperty>("Knob Mode", "KnobMod", StringArray({ "IMMEDIATE", "HOOK" }));
    SetupSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class InputOutputSettings : public Group
{
public:
    IntProperty* nsThreshold = createChild<IntProperty>("NS Threshold", "InNsThrs", IntProperty::LevelType);
    IntProperty* lineOutLevel = createChild<IntProperty>("Line Out Level", "LineOutLvl", IntProperty::LevelType);
    EnumProperty* inputSource = createChild<EnumProperty>("Input Source", "InSrc", StringArray({ "MONO", "STEREO" }));
    BoolProperty* inputLineOut = createChild<BoolProperty>("Input Line Out", "InLineOut");
    EnumProperty* outLevelSelect = createChild<EnumProperty>("Out Level Select", "OutLvlSel", StringArray({ "LINE+PHONES", "PHONES" }));
    InputOutputSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class UsbSettings : public Group
{
public:
    EnumProperty* usbMode = createChild<EnumProperty>("USB Mode", "Mod", StringArray({ "STORAGE", "AUDIO/MIDI" }));
    EnumProperty* usbAAudio = createChild<EnumProperty>("USB Audio", "AuRouting", StringArray({ "LOOP IN", "SUB MIX", "LINE OUT" }));
    IntProperty* usbInLevel = createChild<IntProperty>("USB IN Level", "InLvl", IntProperty::LevelType);
    IntProperty* usbOutLevel = createChild<IntProperty>("USB OUT Level", "OutLvl", IntProperty::LevelType);
    UsbSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class MidiSettings : public Group
{
public:
    EnumProperty* rxChannel = createChild<EnumProperty>("MIDI Rx Channel", "RxCh", StringArray({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" }));
    BoolProperty* omni = createChild<BoolProperty>("MIDI Omni", "Omni");
    EnumProperty* txChannel = createChild<EnumProperty>("MIDI Tx Channel", "TxCh", StringArray({ "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16", "RX" }));
    EnumProperty* sync = createChild<EnumProperty>("MIDI Sync", "Sync", StringArray({ "AUTO", "INTERNAL" }));
    EnumProperty* syncSource = createChild<EnumProperty>("MIDI Sync Source", "InSel", StringArray({ "USB (AUTO)", "MIDI" }));
    BoolProperty* pcOut = createChild<BoolProperty>("MIDI PC Out", "PcOut");
    MidiSettings(Library* library, const String& name)
        : Group(library, name)
    {
    }
};

class SystemSettings : public Group
{
public:
    static const int NumTracks = 5;
    static const int NumAssignments = 16;

    SetupSettings* setup = createChild<SetupSettings>("Setup");
    InputOutputSettings* inputOutput = createChild<InputOutputSettings>("Input / Output");
    UsbSettings* usb = createChild<UsbSettings>("USB");
    MidiSettings* midi = createChild<MidiSettings>("MIDI");
    TrackSettings* tracks[5];
    RhythmSettings* rhythm = createChild<RhythmSettings>("Rhythm");
    NameProperty* name = createChild<NameProperty>("Name", "NAME");
    MasterSettings* master = createChild<MasterSettings>("Master");
    RecordSettings* record = createChild<RecordSettings>("Record");
    PlaySettings* play = createChild<PlaySettings>("Play");
    AssignmentSettings* assignments[NumAssignments];
    InputFxSettings* inputFx = createChild<InputFxSettings>("Input Fx");
    TrackFxSettings* trackFx = createChild<TrackFxSettings>("Track Fx");

    SystemSettings(Library* library)
        : Group(library, "System")
    {
        for (int i = 0; i < NumTracks; ++i)
        {
            tracks[i] = createChild<TrackSettings>(String::formatted("Track %d", i + 1));
        }
        for (int i = 0; i < NumAssignments; ++i)
        {
            assignments[i] = createChild<AssignmentSettings>(String::formatted("Assignment %d", i + 1));
        }
        name->setVisible(false);
    }

    bool loadFromXml(XmlElement* xml) override
    {
        setup->loadFromXml(xml->getChildByName("SETUP"));
        inputOutput->loadFromXml(xml->getChildByName("INPUT_OUTPUT"));
        usb->loadFromXml(xml->getChildByName("USB"));
        midi->loadFromXml(xml->getChildByName("MIDI"));
        for (int i = 0; i < NumTracks; ++i)
        {
            tracks[i]->loadFromXml(xml->getChildByName(String::formatted("TRACK%d", i + 1)));
        }
        rhythm->loadFromXml(xml->getChildByName("RHYTHM"));
        name->loadFromXml(xml->getChildByName("NAME"));
        master->loadFromXml(xml->getChildByName("MASTER"));
        record->loadFromXml(xml->getChildByName("REC_OPTION"));
        play->loadFromXml(xml->getChildByName("PLAY_OPTION"));
        for (int i = 0; i < NumAssignments; ++i)
        {
            assignments[i]->loadFromXml(xml->getChildByName(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->loadFromXml(xml->getChildByName("INPUT_FX"));
        trackFx->loadFromXml(xml->getChildByName("TRACK_FX"));
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->loadBeatFxFromXml(xml->getChildByName(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            inputFx->fxSlot[i]->loadFromXml(xml->getChildByName(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->loadTrackFxFromXml(xml->getChildByName(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }

    bool saveToXml(XmlElement* xml) override
    {
        setup->saveToXml(xml->createNewChildElement("SETUP"));
        inputOutput->saveToXml(xml->createNewChildElement("INPUT_OUTPUT"));
        usb->saveToXml(xml->createNewChildElement("USB"));
        midi->saveToXml(xml->createNewChildElement("MIDI"));
        for (int i = 0; i < NumTracks; ++i)
        {
            tracks[i]->saveToXml(xml->createNewChildElement(String::formatted("TRACK%d", i + 1)));
        }
        rhythm->saveToXml(xml->createNewChildElement("RHYTHM"));
        name->saveToXml(xml->createNewChildElement("NAME"));
        master->saveToXml(xml->createNewChildElement("MASTER"));
        record->saveToXml(xml->createNewChildElement("REC_OPTION"));
        play->saveToXml(xml->createNewChildElement("PLAY_OPTION"));
        for (int i = 0; i < NumAssignments; ++i)
        {
            assignments[i]->saveToXml(xml->createNewChildElement(String::formatted("ASSIGN%d", i + 1)));
        }
        inputFx->saveToXml(xml->createNewChildElement("INPUT_FX"));
        trackFx->saveToXml(xml->createNewChildElement("TRACK_FX"));
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->saveBeatFxToXml(xml->createNewChildElement(String::formatted("BEAT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            inputFx->fxSlot[i]->saveToXml(xml->createNewChildElement(String::formatted("INPUT_FX%d", i + 1)));
        }
        for (int i = 0; i < 3; ++i)
        {
            trackFx->fxSlot[i]->saveTrackFxToXml(xml->createNewChildElement(String::formatted("TRACK_FX%d", i + 1)));
        }
        return true;
    }
};

// ----------------------------------------------------------------------------
// Track
// ----------------------------------------------------------------------------

class Track
{
public:
    enum WaveState
    {
        WaveEmpty, // no wave file assigned to track
        WaveOriginal, // original wave file assigned to track (stored in _originalWaveFile)
        WaveChanged, // changed wave file assigned (stored in _audioBuffer)
    };

    Track(Library* library, Patch& patch, int index);

    int index() const;
    void setIndex(int index);

    TrackSettings* trackSettings() { return &_trackSettings; }

    const WaveState waveState() const { return _waveState; }
    const File& originalWaveFile() const { return _originalWaveFile; }

    const AudioSampleBuffer& audioBuffer() const;
    void setAudioBuffer(const AudioSampleBuffer& audioBuffer);
    void clearAudioBuffer();
    void saveWaveTo(const File& file) const;

    bool loadFromXml(XmlElement* xml);
    bool saveToXml(XmlElement* xml);

private:
    Library* _library;
    Patch& _patch;
    int _index;
    TrackSettings _trackSettings;

    WaveState _waveState;
    File _originalWaveFile;
    mutable AudioSampleBuffer _audioBuffer;
};

// ----------------------------------------------------------------------------
// Patch
// ----------------------------------------------------------------------------

class Patch
{
public:
    static const int NumTracks = 5;

    Patch(Library* library);
    Patch(const RC505::Patch& patch);

    Library* library() { return _library; }

    int index() const;
    void setIndex(int index);

    const OwnedArray<Track>& tracks() const { return _tracks; }
    PatchSettings* patchSettings() { return &_patchSettings; }
    NameProperty* patchName() { return &_patchName; }

    String name() const;
    void setName(const String& name);

    void clear();

    bool hasWaveChanged() const { return _hasWaveChanged; }
    void clearWaveChanged();
    void setWaveChanged();

    bool allTracksEmpty() const;
    void moveTrack(int from, int to);

    bool loadFromXml(XmlElement* xml);
    bool saveToXml(XmlElement* xml);

private:
    Library* _library;
    int _index;
    bool _hasWaveChanged;
    OwnedArray<Track> _tracks;
    PatchSettings _patchSettings;
    NameProperty _patchName;
};

// ----------------------------------------------------------------------------
// Library
// ----------------------------------------------------------------------------

class Library
{
public:
    static const int NumPatches = 99;

    typedef std::function<void(String)> StatusCallback;

    struct Info
    {
        bool valid;
        int revision;
    };

    class Listener
    {
    public:
        virtual ~Listener() {}
        virtual void libraryChanged() {}
        virtual void beforeLibraryLoaded() {}
        virtual void afterLibraryLoaded() {}
        virtual void beforeLibrarySaved() {}
        virtual void afterLibrarySaved() {}
        virtual void libraryClosed() {}
        virtual void propertyValueChanged(ValueProperty* property) { ignoreUnused(property); }
    };

    Library();

    const String& name() const { return _name; }
    void setName(const String& name);
    String documentName() const;

    bool hasChanged() const { return _hasChanged; }
    void clearChanged();
    void setChanged();

    const File& path() const { return _path; }
    const File& dataPath() const { return _dataPath; }
    const File& wavePath() const { return _wavePath; }

    const OwnedArray<Patch>& patches() const { return _patches; }
    SystemSettings* systemSettings() { return &_systemSettings; }

    void movePatches(const Array<Patch*>& patches, int insertIndex);

    void addListener(Listener* listener) { _listeners.add(listener); }
    void removeListener(Listener* listener) { _listeners.remove(listener); }

    void init();
    bool load(const File& path, StatusCallback statusCallback);
    bool save(const File& path, StatusCallback statusCallback);
    void close();

    static Info libraryInfo(const File& path);
    static String checkVolumesForRC505();
    static String tempDirectory();

private:
    void setPath(const File& path);

    bool loadMemory(const File& path);
    bool loadMemory(const String& data);
    bool saveMemory(const File& path);

    bool loadSystem(const File& path);
    bool loadSystem(const String& data);
    bool saveSystem(const File& path);

    bool saveWaveFiles(bool inplace, StatusCallback statusCallback);

    template <typename Func>
    void notifyLocked(Func func)
    {
        const MessageManagerLock messageManagerLock;
        func();
    }

    void notifyPropertyValueChanged(ValueProperty* property);

    String _name;
    bool _hasChanged;

    File _path;
    File _dataPath;
    File _wavePath;

    OwnedArray<Patch> _patches;
    SystemSettings _systemSettings;
    ListenerList<Listener> _listeners;

    std::unique_ptr<XmlElement> _factoryPatchXml;

    friend class ValueProperty;
    friend class Patch;
    friend class Track;
};

} // namespace RC505
