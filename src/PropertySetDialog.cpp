#include "PropertySetDialog.h"
#include "CustomLookAndFeel.h"
#include "JuceHeader.h"

PropertySetDialog::PropertySetDialog(RC505::Group* group)
{
    addAndMakeVisible(_propertySetTreeView);
    addAndMakeVisible(_buttonCancel);
    addAndMakeVisible(_buttonOk);
    _propertySetTreeView.setGroup(group);
    _buttonCancel.setButtonText("Cancel");
    _buttonCancel.addListener(this);
    _buttonOk.setButtonText("Ok");
    _buttonOk.addListener(this);
}

void PropertySetDialog::paint(Graphics& g)
{
    g.fillAll(findColour(mainBackgroundColourId));
}

void PropertySetDialog::resized()
{
    _propertySetTreeView.setSize(getWidth(), getHeight() - 50);
    _buttonCancel.setBounds(10, getHeight() - 40, getWidth() / 2 - 15, 30);
    _buttonOk.setBounds(getWidth() / 2 + 5, getHeight() - 40, getWidth() / 2 - 15, 30);
}

void PropertySetDialog::buttonClicked(Button* button)
{
    if (button == &_buttonCancel)
    {
        if (DialogWindow* dialogWindow = findParentComponentOfClass<DialogWindow>())
        {
            dialogWindow->exitModalState(0);
        }
    }
    if (button == &_buttonOk)
    {
        if (DialogWindow* dialogWindow = findParentComponentOfClass<DialogWindow>())
        {
            dialogWindow->exitModalState(1);
        }
    }
}
