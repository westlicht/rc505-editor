#pragma once

#include "JuceHeader.h"

#include "RC505.h"

// ----------------------------------------------------------------------------
// PropertyView
// ----------------------------------------------------------------------------

class PropertyView : public Component,
                     public SettableTooltipClient
{
public:
    virtual ~PropertyView() {}
    virtual void updateValue() = 0;
};

// ----------------------------------------------------------------------------
// ValuePropertyView
// ----------------------------------------------------------------------------

class ValuePropertyView : public PropertyView,
                          public RC505::ValueProperty::Listener
{
public:
    ValuePropertyView(RC505::ValueProperty* property = nullptr);
    ~ValuePropertyView() override;

    const Label& label() const { return _label; }
    Label& label() { return _label; }

    RC505::ValueProperty* property() { return _property; }

    void setProperty(RC505::ValueProperty* property);

    // Component
    virtual void resized() override;

    // PropertyView
    virtual void updateValue() override;

    // RC505::ValueProperty::Listener
    virtual void valueChanged(RC505::ValueProperty* property) override;

private:
    RC505::ValueProperty* _property;
    Label _label;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ValuePropertyView)
};

// ----------------------------------------------------------------------------
// BoolPropertyView
// ----------------------------------------------------------------------------

class BoolPropertyView : public PropertyView,
                         public RC505::ValueProperty::Listener,
                         public Button::Listener
{
public:
    BoolPropertyView(RC505::BoolProperty* property = nullptr);
    ~BoolPropertyView() override;

    const ToggleButton& toggleButton() const { return _toggleButton; }
    ToggleButton& toggleButton() { return _toggleButton; }

    RC505::BoolProperty* property() { return _property; }

    void setProperty(RC505::BoolProperty* property);

    // Component
    virtual void resized() override;

    // PropertyView
    virtual void updateValue() override;

    // RC505::ValueProperty::Listener
    virtual void valueChanged(RC505::ValueProperty* property) override;

    // Button::Listener
    virtual void buttonClicked(Button* button) override;

private:
    RC505::BoolProperty* _property;
    ToggleButton _toggleButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BoolPropertyView)
};

// ----------------------------------------------------------------------------
// BitSetPropertyView
// ----------------------------------------------------------------------------

class BitSetPropertyView : public PropertyView,
                           public RC505::ValueProperty::Listener,
                           public Button::Listener
{
public:
    BitSetPropertyView(RC505::BitSetProperty* property = nullptr);
    ~BitSetPropertyView() override;

    const ToggleButton& toggleButton(int index) const { return *_toggleButtons[index]; }
    ToggleButton& toggleButton(int index) { return *_toggleButtons[index]; }

    RC505::BitSetProperty* property() { return _property; }

    void setProperty(RC505::BitSetProperty* property);

    // Component
    virtual void resized() override;

    // PropertyView
    virtual void updateValue() override;

    // RC505::ValueProperty::Listener
    virtual void valueChanged(RC505::ValueProperty* property) override;

    // Button::Listener
    virtual void buttonClicked(Button* button) override;

private:
    RC505::BitSetProperty* _property;
    OwnedArray<ToggleButton> _toggleButtons;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitSetPropertyView)
};

// ----------------------------------------------------------------------------
// IntPropertyView
// ----------------------------------------------------------------------------

class IntPropertyView : public PropertyView,
                        public RC505::ValueProperty::Listener,
                        public Slider::Listener
{
private:
    class CustomSlider : public Slider
    {
    public:
        typedef std::function<String(float)> Formatter;

        void setProperty(RC505::IntProperty* property)
        {
            _property = property;
        }

        virtual String getTextFromValue(double value) override
        {
            return _property ? _property->type().formatter(float(value)) : String();
        }

    private:
        RC505::IntProperty* _property = nullptr;
    };

public:
    IntPropertyView(RC505::IntProperty* property = nullptr);
    ~IntPropertyView() override;

    const Slider& slider() const { return _slider; }
    Slider& slider() { return _slider; }

    RC505::IntProperty* property() { return _property; }

    void setProperty(RC505::IntProperty* property);

    // Component
    virtual void resized() override;

    // PropertyView
    virtual void updateValue() override;

    // RC505::ValueProperty:::Listener
    virtual void valueChanged(RC505::ValueProperty* property) override;

    // Slider::Listener
    virtual void sliderValueChanged(Slider* slider) override;

private:
    RC505::IntProperty* _property;
    CustomSlider _slider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(IntPropertyView)
};

// ----------------------------------------------------------------------------
// EnumPropertyView
// ----------------------------------------------------------------------------

class EnumPropertyView : public PropertyView,
                         public RC505::ValueProperty::Listener,
                         public ComboBox::Listener
{
public:
    EnumPropertyView(RC505::EnumProperty* property = nullptr);
    ~EnumPropertyView() override;

    const ComboBox& comboBox() const { return _comboBox; }
    ComboBox& comboBox() { return _comboBox; }

    RC505::EnumProperty* property() { return _property; }

    void setProperty(RC505::EnumProperty* property);

    // Component
    virtual void resized() override;

    // PropertyView
    virtual void updateValue() override;

    // RC505::ValueProperty::Listener
    virtual void valueChanged(RC505::ValueProperty* property) override;

    // ComboBox::Listener
    virtual void comboBoxChanged(ComboBox* comboBox) override;

private:
    RC505::EnumProperty* _property;
    ComboBox _comboBox;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(EnumPropertyView)
};

// ----------------------------------------------------------------------------
// NamePropertyView
// ----------------------------------------------------------------------------

class NamePropertyView : public PropertyView,
                         public RC505::ValueProperty::Listener,
                         public TextEditor::Listener
{
public:
    NamePropertyView(RC505::NameProperty* property = nullptr);
    ~NamePropertyView() override;

    const TextEditor& textEditor() const { return _textEditor; }
    TextEditor& textEditor() { return _textEditor; }

    RC505::NameProperty* property() { return _property; }

    void setProperty(RC505::NameProperty* property);

    // Component
    virtual void resized() override;

    // PropertyView
    virtual void updateValue() override;

    // RC505::ValueProperty::Listener
    virtual void valueChanged(RC505::ValueProperty* property) override;

    // TextEditor::Listener
    virtual void textEditorTextChanged(TextEditor& textEditor) override;

private:
    RC505::NameProperty* _property;
    TextEditor _textEditor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(NamePropertyView)
};
