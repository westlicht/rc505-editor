#include "../JuceLibraryCode/JuceHeader.h"
#include "PropertyView.h"

// ----------------------------------------------------------------------------
// ValuePropertyView
// ----------------------------------------------------------------------------

ValuePropertyView::ValuePropertyView(RC505::ValueProperty *property) :
    _property(nullptr)
{
    addAndMakeVisible(_label);
    setProperty(property);
}

ValuePropertyView::~ValuePropertyView()
{
    setProperty(nullptr);
}

void ValuePropertyView::setProperty(RC505::ValueProperty *property)
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

void ValuePropertyView::resized()
{
    _label.setSize(getWidth(), getHeight());
}

void ValuePropertyView::updateValue()
{
    if (_property) {
        _label.setText(_property->value(), dontSendNotification);
    }
}

void ValuePropertyView::valueChanged(RC505::ValueProperty *property)
{
    updateValue();
}

// ----------------------------------------------------------------------------
// BoolPropertyView
// ----------------------------------------------------------------------------

BoolPropertyView::BoolPropertyView(RC505::BoolProperty *property) :
    _property(nullptr)
{
    _toggleButton.addListener(this);
    addAndMakeVisible(_toggleButton);
    setProperty(property);
}

BoolPropertyView::~BoolPropertyView()
{
    setProperty(nullptr);
}

void BoolPropertyView::setProperty(RC505::BoolProperty *property)
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

void BoolPropertyView::resized()
{
    _toggleButton.setSize(getWidth(), getHeight());
}

void BoolPropertyView::updateValue()
{
    if (_property) {
        _toggleButton.setToggleState(_property->value(), dontSendNotification);
    }
}

void BoolPropertyView::valueChanged(RC505::ValueProperty *property)
{
    updateValue();
}

void BoolPropertyView::buttonClicked(Button *button)
{
    if (_property) {
        _property->setValue(_toggleButton.getToggleState());
    }
}

// ----------------------------------------------------------------------------
// IntPropertyView
// ----------------------------------------------------------------------------

IntPropertyView::IntPropertyView(RC505::IntProperty *property) :
    _property(nullptr)
{
    _slider.addListener(this);
    addAndMakeVisible(_slider);
    setProperty(property);
}

IntPropertyView::~IntPropertyView()
{
    setProperty(nullptr);
}

void IntPropertyView::setProperty(RC505::IntProperty *property)
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

void IntPropertyView::resized()
{
    _slider.setSize(getWidth(), getHeight());
}

void IntPropertyView::updateValue()
{
    if (_property) {
        _slider.setSliderStyle(Slider::LinearBar);
        _slider.setRange(_property->type().min, _property->type().max);
        _slider.setSkewFactor(_property->type().skew);
        _slider.setValue(_property->type().toFloat(_property->value()), dontSendNotification);
    }
}

void IntPropertyView::valueChanged(RC505::ValueProperty *property)
{
    updateValue();
}

void IntPropertyView::sliderValueChanged(Slider *slider)
{
    if (_property) {
        _property->setValue(_property->type().fromFloat(_slider.getValue()));
    }
}

// ----------------------------------------------------------------------------
// EnumPropertyView
// ----------------------------------------------------------------------------

EnumPropertyView::EnumPropertyView(RC505::EnumProperty *property) :
    _property(nullptr)
{
    _comboBox.addListener(this);
    addAndMakeVisible(_comboBox);
    setProperty(property);
}

EnumPropertyView::~EnumPropertyView()
{
    setProperty(nullptr);
}

void EnumPropertyView::setProperty(RC505::EnumProperty *property)
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

void EnumPropertyView::resized()
{
    _comboBox.setSize(getWidth(), getHeight());
}

void EnumPropertyView::updateValue()
{
    if (_property) {
        _comboBox.clear();
        _comboBox.addItemList(_property->options(), 1);
        _comboBox.setSelectedItemIndex(_property->value(), dontSendNotification);
    }
}

void EnumPropertyView::valueChanged(RC505::ValueProperty *property)
{
    updateValue();
}

void EnumPropertyView::comboBoxChanged(ComboBox *comboBox)
{
    if (_property) {
        _property->setValue(_comboBox.getSelectedItemIndex());
    }
}

// ----------------------------------------------------------------------------
// NamePropertyView
// ----------------------------------------------------------------------------

NamePropertyView::NamePropertyView(RC505::NameProperty *property) :
    _property(nullptr)
{
    _textEditor.setInputRestrictions(12);
    _textEditor.addListener(this);
    addAndMakeVisible(_textEditor);
    setProperty(property);
}

NamePropertyView::~NamePropertyView()
{
    setProperty(nullptr);
}

void NamePropertyView::setProperty(RC505::NameProperty *property)
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

void NamePropertyView::resized()
{
    _textEditor.setSize(getWidth(), getHeight());
}

void NamePropertyView::updateValue()
{
    if (_property) {
        _textEditor.setText(_property->value(), dontSendNotification);
    }
}

void NamePropertyView::valueChanged(RC505::ValueProperty *property)
{
    updateValue();
}

void NamePropertyView::textEditorTextChanged(TextEditor &textEditor)
{
    if (_property) {
        _property->setValue(_textEditor.getText());
    }
}
