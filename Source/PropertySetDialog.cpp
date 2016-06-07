#include "../JuceLibraryCode/JuceHeader.h"
#include "PropertySetDialog.h"

PropertySetDialog::PropertySetDialog(RC505::Group *group)
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

void PropertySetDialog::resized()
{
    _propertySetTreeView.setSize(getWidth(), getHeight() - 40);
    _buttonCancel.setBounds(5, getHeight() - 35, getWidth() / 2 - 10, 30);
    _buttonOk.setBounds(getWidth() / 2 + 5, getHeight() - 35, getWidth() / 2 - 10, 30);
}

void PropertySetDialog::buttonClicked(Button *button)
{
    if (button == &_buttonCancel) {
        if (DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>()) {
            dialogWindow->exitModalState(0);
        }
    }
    if (button == &_buttonOk) {
        if (DialogWindow *dialogWindow = findParentComponentOfClass<DialogWindow>()) {
            dialogWindow->exitModalState(1);
        }
    }
}