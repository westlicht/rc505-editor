#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "RC505.h"

class PropertyView : public Component {
public:
    virtual void updateValue() = 0;
};

class ValuePropertyView : public PropertyView, public RC505::ValueProperty::Listener {
public:
    ValuePropertyView(RC505::ValueProperty *property = nullptr) :
        _property(nullptr)
    {
        addAndMakeVisible(_label);
        setProperty(property);
    }

    ~ValuePropertyView()
    {
        setProperty(nullptr);
    }

    const Label &label() const { return _label; }
          Label &label()       { return _label; }

    RC505::ValueProperty *property() { return _property; }

    void setProperty(RC505::ValueProperty *property)
    {
        if (_property) {
            _property->removeListener(this);
        }
        _property = property;
        if (_property) {
            _property->addListener(this);
        }
        updateValue();
    }

    virtual void updateValue() override
    {
        if (_property) {
            _label.setText(_property->value(), dontSendNotification);
        }
    }

    virtual void resized() override
    {
        _label.setSize(getWidth(), getHeight());
    }

    virtual void valueChanged(RC505::ValueProperty *property) override
    {
        updateValue();
    }

private:
    RC505::ValueProperty *_property;
    Label _label;
};

class BoolPropertyView : public PropertyView, public RC505::ValueProperty::Listener, public Button::Listener {
public:
    BoolPropertyView(RC505::BoolProperty *property = nullptr) :
        _property(nullptr)
    {
        _toggleButton.addListener(this);
        addAndMakeVisible(_toggleButton);
        setProperty(property);
    }

    ~BoolPropertyView()
    {
        setProperty(nullptr);
    }

    const ToggleButton &toggleButton() const { return _toggleButton; }
          ToggleButton &toggleButton()       { return _toggleButton; }

    RC505::BoolProperty *property() { return _property; }

    void setProperty(RC505::BoolProperty *property)
    {
        if (_property) {
            _property->removeListener(this);
        }
        _property = property;
        if (_property) {
            _property->addListener(this);
        }
        updateValue();
    }

    virtual void updateValue() override
    {
        if (_property) {
            _toggleButton.setToggleState(_property->value(), dontSendNotification);
        }
    }

    virtual void buttonClicked(Button *button) override
    {
        if (_property) {
            _property->setValue(_toggleButton.getToggleState());
        }
    }

    virtual void resized() override
    {
        _toggleButton.setSize(getWidth(), getHeight());
    }

    virtual void valueChanged(RC505::ValueProperty *property) override
    {
        updateValue();
    }

private:
    RC505::BoolProperty *_property;
    ToggleButton _toggleButton;
};

class IntPropertyView : public PropertyView, public RC505::ValueProperty::Listener, public Slider::Listener {
private:
    class CustomSlider : public Slider {
    public:
        typedef std::function<String(float)> Formatter;

        void setProperty(RC505::IntProperty *property)
        {
            _property = property;
        }

        virtual String getTextFromValue(double value) override
        {
            return _property ? _property->type().formatter(float(value)) : String::empty;
        }

    private:
        RC505::IntProperty *_property = nullptr;
    };

public:
    IntPropertyView(RC505::IntProperty *property = nullptr) :
        _property(nullptr)
    {
        _slider.addListener(this);
        addAndMakeVisible(_slider);
        setProperty(property);
    }

    ~IntPropertyView()
    {
        setProperty(nullptr);
    }

    const Slider &slider() const { return _slider; }
          Slider &slider()       { return _slider; }

    RC505::IntProperty *property() { return _property; }

    void setProperty(RC505::IntProperty *property)
    {
        if (_property) {
            _property->removeListener(this);
        }
        _property = property;
        _slider.setProperty(property);
        if (_property) {
            _property->addListener(this);
        }
        updateValue();
    }

    virtual void updateValue() override
    {
        if (_property) {
            _slider.setSliderStyle(Slider::LinearBar);
            _slider.setRange(_property->type().min, _property->type().max);
            _slider.setSkewFactor(_property->type().skew);
            _slider.setValue(_property->type().toFloat(_property->value()), dontSendNotification);
        }
    }

    virtual void sliderValueChanged(Slider *slider) override
    {
        if (_property) {
            _property->setValue(_property->type().fromFloat(_slider.getValue()));
        }
    }

    virtual void resized() override
    {
        _slider.setSize(getWidth(), getHeight());
    }

    virtual void valueChanged(RC505::ValueProperty *property) override
    {
        updateValue();
    }

private:
    RC505::IntProperty *_property;
    CustomSlider _slider;
};

class EnumPropertyView : public PropertyView, public RC505::ValueProperty::Listener, public ComboBox::Listener {
public:
    EnumPropertyView(RC505::EnumProperty *property = nullptr) :
        _property(nullptr)
    {
        _comboBox.addListener(this);
        addAndMakeVisible(_comboBox);
        setProperty(property);
    }

    ~EnumPropertyView()
    {
        setProperty(nullptr);
    }

    const ComboBox &comboBox() const { return _comboBox; }
          ComboBox &comboBox()       { return _comboBox; }

    RC505::EnumProperty *property() { return _property; }

    void setProperty(RC505::EnumProperty *property)
    {
        if (_property) {
            _property->removeListener(this);
        }
        _property = property;
        if (_property) {
            _property->addListener(this);
        }
        updateValue();
    }

    virtual void updateValue() override
    {
        if (_property) {
            _comboBox.clear();
            _comboBox.addItemList(_property->options(), 1);
            _comboBox.setSelectedItemIndex(_property->value(), dontSendNotification);
        }
    }

    virtual void comboBoxChanged(ComboBox *comboBox) override
    {
        if (_property) {
            _property->setValue(_comboBox.getSelectedItemIndex());
        }
    }

    virtual void resized() override
    {
        _comboBox.setSize(getWidth(), getHeight());
    }

    virtual void valueChanged(RC505::ValueProperty *property) override
    {
        updateValue();
    }

private:
    RC505::EnumProperty *_property;
    ComboBox _comboBox;
};

class NamePropertyView : public PropertyView, public RC505::ValueProperty::Listener, public TextEditor::Listener {
public:
    NamePropertyView(RC505::NameProperty *property = nullptr) :
        _property(nullptr)
    {
        _textEditor.setInputRestrictions(12);
        _textEditor.addListener(this);
        addAndMakeVisible(_textEditor);
        setProperty(property);
    }

    ~NamePropertyView()
    {
        setProperty(nullptr);
    }

    const TextEditor &textEditor() const { return _textEditor; }
          TextEditor &textEditor()       { return _textEditor; }

    RC505::NameProperty *property() { return _property; }

    void setProperty(RC505::NameProperty *property)
    {
        if (_property) {
            _property->removeListener(this);
        }
        _property = property;
        if (_property) {
            _property->addListener(this);
        }
        updateValue();
    }

    virtual void updateValue() override
    {
        if (_property) {
            _textEditor.setText(_property->value(), dontSendNotification);
        }
    }

    virtual void textEditorTextChanged(TextEditor &textEditor) override
    {
        if (_property) {
            _property->setValue(_textEditor.getText());
        }
    }

    virtual void resized() override
    {
        _textEditor.setSize(getWidth(), getHeight());
    }

    virtual void valueChanged(RC505::ValueProperty *property) override
    {
        updateValue();
    }

private:
    RC505::NameProperty *_property;
    TextEditor _textEditor;
};
