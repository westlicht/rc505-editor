#include "../JuceLibraryCode/JuceHeader.h"
#include "MainWindow.h"

MainWindow::MainWindow(String name) :
    DocumentWindow(name, Colours::lightgrey, DocumentWindow::allButtons)
{
    //_library.save(File("/Users/freak/Desktop/ROLAND"));

    setUsingNativeTitleBar(true);
    _mainComponent = new MainComponent(_library);
    setContentOwned(_mainComponent, true);
    setResizable (true, true);

    centreWithSize (getWidth(), getHeight());
    setVisible (true);
    
    #if 0
    if (!_library.load(File("/Users/freak/Desktop/ROLAND"))) {
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Invalid Library Folder");
    }
    #endif

    mountedVolumeListChanged();
    //FileChooser fc("Select ROLAND directory");
    //fc.browseForDirectory();
}

MainWindow::~MainWindow()
{
    clearContentComponent();
}

void MainWindow::closeButtonPressed()
{
    // This is called when the user tries to close this window. Here, we'll just
    // ask the app to quit when this happens, but you can change this to do
    // whatever you need.
    JUCEApplication::getInstance()->systemRequestedQuit();
}

/* Note: Be careful if you override any DocumentWindow methods - the base
   class uses a lot of them, so by overriding you might break its functionality.
   It's best to do all your work in your content component instead, but if
   you really have to override any DocumentWindow methods, make sure your
   subclass also calls the superclass's method.
*/

void MainWindow::mountedVolumeListChanged()
{
    String path = RC505::Library::checkVolumesForRC505();
    if (path.isEmpty()) {
        return;
    }
    if (AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                     "BOSS RC505 detected",
                                     "A BOSS RC505 was detected. Do you want to load it's library?\n\n"
                                     "WARNING: For your own safety, please create a BACKUP before editing the content!\n\n"
                                     "NOTE: Data transfer over USB is rather slow. You might be better off copying all\n"
                                     "content to a local directory, do the changes locally, and write everything back.",
                                     String::empty, String::empty,
                                     nullptr, nullptr)) {
        _library.load(File(path));
    }
}

void MainWindow::openLibrary()
{
    FileChooser fileChooser("Select 'ROLAND' folder...");
    if (fileChooser.browseForDirectory()) {
        File dir = fileChooser.getResult();
        Logger::outputDebugString(dir.getFullPathName());
    }
    if (!_library.load(File("/Users/freak/Desktop/ROLAND"))) {
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Invalid Library Folder");
    }

}

void MainWindow::saveLibrary()
{
    _library.save(_library.rootPath());
    //_library.save(File("/Users/freak/Desktop/ROLAND"));
}

void MainWindow::closeLibrary()
{

}
