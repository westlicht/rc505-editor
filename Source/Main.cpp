#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"
#include "Theme.h"
#include "CommandIDs.h"
#include "CustomLookAndFeel.h"

class RC505Application : public JUCEApplication {
public:
    RC505Application() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise(const String& commandLine) override
    {
        if (sendCommandLineToPreexistingInstance()) {
            DBG("Another instance is running - quitting...");
            quit();
            return;
        }

        initCommandManager();

        LookAndFeel::setDefaultLookAndFeel(&_customLookAndFeel);

        _mainMenuModel = new MainMenuModel();
        _mainWindow = new MainWindow(String::formatted("RC-505 Loop Station Editor (Version %s ALPHA)", ProjectInfo::versionString));
        getCommandManager().registerAllCommandsForTarget(&_mainWindow->mainComponent());

#if JUCE_MAC
        MenuBarModel::setMacMainMenu(_mainMenuModel, nullptr);
#else
        _mainWindow->setMenuBar(_mainMenuModel);
#endif


    }

    void shutdown() override
    {
#if JUCE_MAC
        MenuBarModel::setMacMainMenu(nullptr);
#endif

        _mainWindow = nullptr;
        _mainMenuModel = nullptr;

        // clear temporary directory
        File(RC505::Library::tempDirectory()).deleteRecursively();
    }

    void systemRequestedQuit() override
    {
        if (_mainWindow->mainComponent().allowQuit()) {
            quit();
        }
    }

    void anotherInstanceStarted(const String &commandLine) override
    {
    }

    static RC505Application &getApp()
    {
        RC505Application *app = dynamic_cast<RC505Application *>(JUCEApplication::getInstance());
        jassert(app != nullptr);
        return *app;
    }

    static ApplicationCommandManager &getCommandManager()
    {
        ApplicationCommandManager *cm = RC505Application::getApp()._commandManager;
        jassert(cm != nullptr);
        return *cm;
    }

    class MainMenuModel : public MenuBarModel {
    public:
        MainMenuModel()
        {
            setApplicationCommandManagerToWatch(&getCommandManager());
        }

        StringArray getMenuBarNames() override
        {
            return getApp().getMenuNames();
        }

        PopupMenu getMenuForIndex(int /*topLevelMenuIndex*/, const String &menuName) override
        {
            PopupMenu menu;
            getApp().createMenu(menu, menuName);
            return menu;
        }

        void menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/) override
        {
            DBG("menuItemSelected");
            getApp().handleMainMenuCommand(menuItemID);
        }
    };    

    enum {
        recentProjectsBaseID = 100,
        activeDocumentsBaseID = 300,
        colourSchemeBaseID = 1000
    };

    virtual StringArray getMenuNames()
    {
        return StringArray({"File"});
    }

    virtual void createMenu(PopupMenu &menu, const String &menuName)
    {
        if (menuName == "File") {
            createFileMenu(menu);
        } else  {
            jassertfalse;
        }
    }

    virtual void createFileMenu(PopupMenu &menu)
    {
        menu.addCommandItem(_commandManager, CommandIDs::newLibrary);
        menu.addSeparator();

        menu.addCommandItem(_commandManager, CommandIDs::openLibrary);
#if 0
        PopupMenu recentFiles;
        recentFiles.addItem(1, "recent 1");
        recentFiles.addItem(2, "recent 2");
        menu.addSubMenu("Open Recent", recentFiles);
#endif
        menu.addSeparator();

        menu.addCommandItem(_commandManager, CommandIDs::saveLibrary);
        menu.addCommandItem(_commandManager, CommandIDs::saveLibraryAs);
        menu.addSeparator();

        menu.addCommandItem(_commandManager, CommandIDs::closeLibrary);

#if !JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::quit);
#endif
    }

    virtual void handleMainMenuCommand(int menuItemID)
    {
        if (menuItemID >= recentProjectsBaseID && menuItemID < recentProjectsBaseID + 100) {
            // open a file from the "recent files" menu
            //openFile (settings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
        } else {
            //handleGUIEditorMenuCommand(menuItemID);
        }
    }

    class MainWindow : public DocumentWindow,
                       public DragAndDropContainer {
    public:
        MainWindow(String name) :
            DocumentWindow(name, Colours::lightgrey, DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar(true);
            setContentNonOwned(&_mainComponent, true);
            setResizable(true, true);
            setResizeLimits(1200, 800, 10000, 10000);

            centreWithSize(getWidth(), getHeight());
            setVisible(true);

            addKeyListener(RC505Application::getCommandManager().getKeyMappings());
        }

        ~MainWindow()
        {
            clearContentComponent();
        }

        MainComponent &mainComponent() { return _mainComponent; }

        virtual void closeButtonPressed() override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        MainComponent _mainComponent;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };


private:
    void initCommandManager()
    {
        _commandManager = new ApplicationCommandManager();
        _commandManager->registerAllCommandsForTarget(this);
    }

    CustomLookAndFeel _customLookAndFeel;
    ScopedPointer<MainMenuModel> _mainMenuModel;
    ScopedPointer<MainWindow> _mainWindow;
    ScopedPointer<ApplicationCommandManager> _commandManager;
};

START_JUCE_APPLICATION(RC505Application)
