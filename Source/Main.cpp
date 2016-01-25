#include "../JuceLibraryCode/JuceHeader.h"
#include "RC505.h"
#include "Theme.h"
#include "CommandIDs.h"

Component *createMainContentComponent(RC505::Library &library);

class RC505Application : public JUCEApplication {
public:
    RC505Application() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }

    void initialise (const String& commandLine) override
    {
        if (sendCommandLineToPreexistingInstance()) {
            DBG("Another instance is running - quitting...");
            quit();
            return;
        }

        initCommandManager();

        _mainMenuModel = new MainMenuModel();

#if JUCE_MAC
        MenuBarModel::setMacMainMenu(_mainMenuModel, nullptr, "Open Recent");
#endif

        _mainWindow = new MainWindow(getApplicationName());
    }

    void shutdown() override
    {
#if JUCE_MAC
        MenuBarModel::setMacMainMenu(nullptr);
#endif

        _mainWindow = nullptr;
        _mainMenuModel = nullptr;
    }

    void systemRequestedQuit() override
    {
        quit();
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
            getApp().createMenu (menu, menuName);
            return menu;
        }

        void menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/) override
        {
            getApp().handleMainMenuCommand (menuItemID);
        }
    };    

    enum {
        recentProjectsBaseID = 100,
        activeDocumentsBaseID = 300,
        colourSchemeBaseID = 1000
    };

    virtual StringArray getMenuNames()
    {
        return StringArray({"File", "Edit"});
    }

    virtual void createMenu(PopupMenu &menu, const String &menuName)
    {
        if (menuName == "File")             createFileMenu   (menu);
        else if (menuName == "Edit")        createEditMenu   (menu);
        else                                jassertfalse; // names have changed?
    }

    virtual void createFileMenu(PopupMenu &menu)
    {
        menu.addCommandItem(_commandManager, CommandIDs::openLibrary);
        menu.addSeparator();
        menu.addCommandItem(_commandManager, CommandIDs::saveLibrary);


        menu.addCommandItem(_commandManager, CommandIDs::newProject);
        menu.addSeparator();
        menu.addCommandItem(_commandManager, CommandIDs::open);

#if 0
        PopupMenu recentFiles;
        settings->recentFiles.createPopupMenuItems (recentFiles, recentProjectsBaseID, true, true);
        menu.addSubMenu ("Open Recent", recentFiles);
#endif

        menu.addSeparator();
        menu.addCommandItem(_commandManager, CommandIDs::closeDocument);
        menu.addCommandItem(_commandManager, CommandIDs::saveDocument);
        menu.addCommandItem(_commandManager, CommandIDs::saveDocumentAs);
        menu.addCommandItem(_commandManager, CommandIDs::saveAll);
        menu.addSeparator();
        menu.addCommandItem(_commandManager, CommandIDs::closeProject);
        menu.addCommandItem(_commandManager, CommandIDs::saveProject);
        menu.addSeparator();
        menu.addCommandItem(_commandManager, CommandIDs::openInIDE);
        menu.addCommandItem(_commandManager, CommandIDs::saveAndOpenInIDE);

#if !JUCE_MAC
        menu.addSeparator();
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::quit);
#endif
    }

    virtual void createEditMenu (PopupMenu& menu)
    {
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::undo);
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::redo);
        menu.addSeparator();
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::cut);
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::copy);
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::paste);
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::del);
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::selectAll);
        menu.addCommandItem(_commandManager, StandardApplicationCommandIDs::deselectAll);
        menu.addSeparator();
        menu.addCommandItem(_commandManager, CommandIDs::showFindPanel);
        menu.addCommandItem(_commandManager, CommandIDs::findSelection);
        menu.addCommandItem(_commandManager, CommandIDs::findNext);
        menu.addCommandItem(_commandManager, CommandIDs::findPrevious);
    }

    virtual void handleMainMenuCommand(int menuItemID)
    {
        if (menuItemID >= recentProjectsBaseID && menuItemID < recentProjectsBaseID + 100) {
            // open a file from the "recent files" menu
            //openFile (settings->recentFiles.getFile (menuItemID - recentProjectsBaseID));
        } else if (menuItemID >= activeDocumentsBaseID && menuItemID < activeDocumentsBaseID + 200) {
#if 0
            if (OpenDocumentManager::Document* doc = openDocumentManager.getOpenDocument(menuItemID - activeDocumentsBaseID))
                mainWindowList.openDocument(doc, true);
            else
                jassertfalse;
#endif
        } else if (menuItemID >= colourSchemeBaseID && menuItemID < colourSchemeBaseID + 200) {
            //settings->appearance.selectPresetScheme(menuItemID - colourSchemeBaseID);
        } else {
            //handleGUIEditorMenuCommand(menuItemID);
        }
    }

    void getAllCommands(Array<CommandID> &commands) override
    {
        JUCEApplication::getAllCommands(commands);
        commands.addArray({
            CommandIDs::openLibrary,
            CommandIDs::saveLibrary,
#if 0
            CommandIDs::newProject,
            CommandIDs::open,
            CommandIDs::closeAllDocuments,
            CommandIDs::saveAll,
            CommandIDs::showAppearanceSettings,
            CommandIDs::showUTF8Tool,
            CommandIDs::showSVGPathTool
#endif
        });
    }

    void getCommandInfo(CommandID commandID, ApplicationCommandInfo &result) override
    {
        switch (commandID) {
        case CommandIDs::openLibrary:
            result.setInfo("Open Library...", "Open RC505 library", CommandCategories::general, 0);
            break;
        case CommandIDs::saveLibrary:
            result.setInfo("Save Library...", "Save RC505 library", CommandCategories::general, 0);
            break;

        case CommandIDs::newProject:
            result.setInfo ("New Project...", "Creates a new Jucer project", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('n', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::open:
            result.setInfo ("Open...", "Opens a Jucer project", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('o', ModifierKeys::commandModifier, 0));
            break;

        case CommandIDs::showAppearanceSettings:
            result.setInfo ("Fonts and Colours...", "Shows the appearance settings window.", CommandCategories::general, 0);
            break;

        case CommandIDs::closeAllDocuments:
            result.setInfo ("Close All Documents", "Closes all open documents", CommandCategories::general, 0);
            //result.setActive (openDocumentManager.getNumOpenDocuments() > 0);
            break;

        case CommandIDs::saveAll:
            result.setInfo ("Save All", "Saves all open documents", CommandCategories::general, 0);
            result.defaultKeypresses.add (KeyPress ('s', ModifierKeys::commandModifier | ModifierKeys::altModifier, 0));
            break;

        case CommandIDs::showUTF8Tool:
            result.setInfo ("UTF-8 String-Literal Helper", "Shows the UTF-8 string literal utility", CommandCategories::general, 0);
            break;

        case CommandIDs::showSVGPathTool:
            result.setInfo ("SVG Path Helper", "Shows the SVG->Path data conversion utility", CommandCategories::general, 0);
            break;

        default:
            JUCEApplication::getCommandInfo (commandID, result);
            break;
        }
    }

    bool perform(const InvocationInfo &info) override
    {
        switch (info.commandID) {
        case CommandIDs::openLibrary: _mainWindow->openLibrary(); break;
        case CommandIDs::saveLibrary: _mainWindow->saveLibrary(); break;
#if 0        
            case CommandIDs::newProject:                createNewProject(); break;
            case CommandIDs::open:                      askUserToOpenFile(); break;
            case CommandIDs::saveAll:                   openDocumentManager.saveAll(); break;
            case CommandIDs::closeAllDocuments:         closeAllDocuments (true); break;
            case CommandIDs::showUTF8Tool:              showUTF8ToolWindow (utf8Window); break;
            case CommandIDs::showSVGPathTool:           showSVGPathDataToolWindow (svgPathWindow); break;

            case CommandIDs::showAppearanceSettings:    AppearanceSettings::showEditorWindow (appearanceEditorWindow); break;
#endif
            default:                                    return JUCEApplication::perform (info);
        }
        return true;
    }


    class MainWindow : public DocumentWindow, MountedVolumeListChangeDetector, public DragAndDropContainer {
    public:
        MainWindow (String name)  : DocumentWindow (name,
                                                    Colours::lightgrey,
                                                    DocumentWindow::allButtons)
        {
            //_library.save(File("/Users/freak/Desktop/ROLAND"));

            setUsingNativeTitleBar(true);
            setContentOwned (createMainContentComponent(_library), true);
            setResizable (true, true);

            centreWithSize (getWidth(), getHeight());
            setVisible (true);
            
            if (!_library.load(File("/Users/freak/Desktop/ROLAND"))) {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Invalid Library Folder");
            }

            //mountedVolumeListChanged();
            //FileChooser fc("Select ROLAND directory");
            //fc.browseForDirectory();
        }

        void closeButtonPressed() override
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
        
        virtual void mountedVolumeListChanged() override {
            Logger::outputDebugString("mounts changed");
            Array<File> roots;
            File::findFileSystemRoots(roots);
            for (const auto &root : roots) {
                Logger::outputDebugString(root.getFullPathName());
            }
        }

        void openLibrary()
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

        void saveLibrary()
        {
            _library.save(File("/Users/freak/Desktop/ROLAND"));
        }


    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)

        RC505::Library _library;
    };

private:
    void initCommandManager()
    {
        _commandManager = new ApplicationCommandManager();
        _commandManager->registerAllCommandsForTarget(this);

#if 0
        {
            CodeDocument doc;
            CppCodeEditorComponent ed (File::nonexistent, doc);
            _commandManager->registerAllCommandsForTarget (&ed);
        }
#endif
        //registerGUIEditorCommands();
    }

    ScopedPointer<MainMenuModel> _mainMenuModel;
    ScopedPointer<MainWindow> _mainWindow;
    ScopedPointer<ApplicationCommandManager> _commandManager;
};

START_JUCE_APPLICATION (RC505Application)
