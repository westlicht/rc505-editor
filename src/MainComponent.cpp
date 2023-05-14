#include "MainComponent.h"
#include "CommandIDs.h"
#include "CustomLookAndFeel.h"
#include "JuceHeader.h"
#include "LibraryTasks.h"
#include "LibraryView.h"

class MainMultiDocumentPanel : public MultiDocumentPanel
{
public:
    MainMultiDocumentPanel()
    {
        _imageIcon = Image(ImageCache::getFromMemory(BinaryData::iconlarge_png, BinaryData::iconlarge_pngSize));
    }

    virtual bool tryToCloseDocument(Component* component) override
    {
        ignoreUnused(component);
        return true;
    }

    virtual void tryToCloseDocumentAsync(Component* component, std::function<void(bool)> callback) override
    {
        ignoreUnused(component);
        callback(true);
    }

    virtual void paint(Graphics& g) override
    {
        if (getNumDocuments() > 0)
        {
            MultiDocumentPanel::paint(g);
        }
        else
        {
            g.fillAll(Colours::black);
            g.drawImageWithin(_imageIcon, 0, 0, getWidth(), getHeight(), RectanglePlacement::onlyReduceInSize | RectanglePlacement::centred);
        }
    }

private:
    Image _imageIcon;
};

MainComponent::MainComponent()
    : _audioEngine(AudioEngine::instance())
{
    setSize(1400, 800);
    setAudioChannels(2, 2);

    _multiDocumentPanel = std::make_unique<MainMultiDocumentPanel>();
    _multiDocumentPanel->setSize(getWidth(), getHeight());
    _multiDocumentPanel->setBackgroundColour(Colours::white);
    _multiDocumentPanel->setLayoutMode(MultiDocumentPanel::MaximisedWindowsWithTabs);
    addAndMakeVisible(_multiDocumentPanel.get());
    addAndMakeVisible(_tooltipPanel);

    resized();
    documentsChanged();
    mountedVolumeListChanged();
    startTimer(500);
}

MainComponent::~MainComponent()
{
    shutdownAudio();
}

void MainComponent::newLibrary()
{
    auto view = new LibraryView();
    view->library().setName(String::formatted("New Library %d", _newLibraryIndex++));
    _multiDocumentPanel->addDocument(view, Colours::white, true);
    documentsChanged();
}

void MainComponent::openLibrary()
{
    FileChooser fileChooser("Open Library");
    if (fileChooser.browseForDirectory())
    {
        openLibrary(fileChooser.getResult());
    }
}

void MainComponent::saveLibrary()
{
    auto view = activeLibraryView();
    if (view)
    {
        if (view->library().path().exists())
        {
            saveLibrary(view->library(), view->library().path());
        }
        else
        {
            FileChooser fileChooser("Save Library");
            if (fileChooser.browseForDirectory())
            {
                saveLibrary(view->library(), fileChooser.getResult());
            }
        }
    }
}

void MainComponent::saveLibraryAs()
{
    auto view = activeLibraryView();
    if (view)
    {
        FileChooser fileChooser("Save Library As");
        if (fileChooser.browseForDirectory())
        {
            saveLibrary(view->library(), fileChooser.getResult());
        }
    }
}

void MainComponent::closeLibrary()
{
    auto view = activeLibraryView();
    if (view)
    {
        if (!view->library().hasChanged() || allowDiscardChanges(view->library()))
        {
            _multiDocumentPanel->closeDocument(view, false);
            documentsChanged();
        }
    }
}

bool MainComponent::allowQuit()
{
    bool allow = true;
    iterateLibraryViews([&](LibraryView* view)
                        {
        if (view->library().hasChanged() && !allowDiscardChanges(view->library())) {
            allow = false;
        } });
    return allow;
}

void MainComponent::paint(Graphics& g)
{
    g.fillAll(findColour(mainBackgroundColourId));
    g.setColour(findColour(mainBorderColourId));
    g.drawLine(0.f, getHeight() - 30.5f, getWidth(), getHeight() - 30.5f);
}

void MainComponent::resized()
{
    if (_multiDocumentPanel)
    {
        _multiDocumentPanel->setSize(getWidth(), getHeight() - 31);
    }
    _tooltipPanel.setBounds(0, getHeight() - 30, getWidth(), 30);
}

void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    _audioEngine.source().prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    _audioEngine.source().getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    _audioEngine.source().releaseResources();
}

ApplicationCommandTarget* MainComponent::getNextCommandTarget()
{
    return nullptr;
}

void MainComponent::getAllCommands(Array<CommandID>& commands)
{
    commands = Array<CommandID>({ CommandIDs::newLibrary,
                                  CommandIDs::openLibrary,
                                  CommandIDs::saveLibrary,
                                  CommandIDs::saveLibraryAs,
                                  CommandIDs::closeLibrary });
}

void MainComponent::getCommandInfo(CommandID commandID, ApplicationCommandInfo& result)
{
    switch (commandID)
    {
        case CommandIDs::newLibrary:
            result.setInfo("New Library", "Create a new RC505 library", CommandCategories::library, 0);
            result.addDefaultKeypress('n', ModifierKeys::commandModifier);
            break;
        case CommandIDs::openLibrary:
            result.setInfo("Open Library...", "Open RC505 library", CommandCategories::library, 0);
            result.addDefaultKeypress('o', ModifierKeys::commandModifier);
            break;
        case CommandIDs::saveLibrary:
            result.setInfo("Save Library...", "Save RC505 library", CommandCategories::library, 0);
            result.addDefaultKeypress('s', ModifierKeys::commandModifier);
            break;
        case CommandIDs::saveLibraryAs:
            result.setInfo("Save Library as...", "Save RC505 library", CommandCategories::library, 0);
            result.addDefaultKeypress('s', ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
            break;
        case CommandIDs::closeLibrary:
            result.setInfo("Close Library", "Close RC505 library", CommandCategories::library, 0);
            result.addDefaultKeypress('w', ModifierKeys::commandModifier);
            break;
        default:
            break;
    }
}

bool MainComponent::perform(const InvocationInfo& info)
{
    switch (info.commandID)
    {
        case CommandIDs::newLibrary:
            newLibrary();
            break;
        case CommandIDs::openLibrary:
            openLibrary();
            break;
        case CommandIDs::saveLibrary:
            saveLibrary();
            break;
        case CommandIDs::saveLibraryAs:
            saveLibraryAs();
            break;
        case CommandIDs::closeLibrary:
            closeLibrary();
            break;
        default:
            break;
    }
    return true;
}

void MainComponent::mountedVolumeListChanged()
{
    String path = RC505::Library::checkVolumesForRC505();
    if (path.isEmpty())
    {
        return;
    }
    if (AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                     "RC-505 Loop Station detected",
                                     "A RC-505 Loop Station was detected via USB. Do you want to load it's library?\n\n"
                                     "WARNING: For your own safety, create a BACKUP before writing the library back!"))
    {
        openLibrary(File(path));
    }
}

void MainComponent::timerCallback()
{
    Array<LibraryView*> viewsToClose;
    iterateLibraryViews([&](LibraryView* view)
                        {
        if (view->library().path() != File() && !view->library().path().exists()) {
            viewsToClose.add(view);
        } });
    if (viewsToClose.size() > 0)
    {
        for (const auto& view : viewsToClose)
        {
            _multiDocumentPanel->closeDocument(view, false);
        }
        documentsChanged();
    }
}

void MainComponent::openLibrary(const File& path)
{
    auto view = new LibraryView();
    if (LoadLibraryTask::loadLibrary(view->library(), path))
    {
        _multiDocumentPanel->addDocument(view, Colours::white, true);
        documentsChanged();
    }
    else
    {
        delete view;
    }
}

void MainComponent::saveLibrary(RC505::Library& library, const File& path)
{
    SaveLibraryTask::saveLibrary(library, path);
}

bool MainComponent::allowDiscardChanges(RC505::Library& library)
{
    return AlertWindow::showOkCancelBox(AlertWindow::QuestionIcon,
                                        "Unsaved changes",
                                        "The library '" + library.name() + "' contains unsaved changes.\n"
                                                                           "Are you sure you want to discard the changes?");
}

LibraryView* MainComponent::activeLibraryView()
{
    return dynamic_cast<LibraryView*>(_multiDocumentPanel->getActiveDocument());
}

void MainComponent::iterateLibraryViews(std::function<void(LibraryView*)> handler)
{
    for (int i = 0; i < _multiDocumentPanel->getNumDocuments(); ++i)
    {
        if (auto view = dynamic_cast<LibraryView*>(_multiDocumentPanel->getDocument(i)))
        {
            handler(view);
        }
    }
}

void MainComponent::documentsChanged()
{
    if (_multiDocumentPanel->getCurrentTabbedComponent())
    {
        _multiDocumentPanel->getCurrentTabbedComponent()->setOutline(0);
    }
    _tooltipPanel.setDefaultTooltip(_multiDocumentPanel->getNumDocuments() != 0 ? "" : "No library loaded! Create a new library, open an existing library or connect an RC-505 Loop Station via USB ...");
}
