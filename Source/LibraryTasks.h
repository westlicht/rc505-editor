#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class LoadLibraryTask : public ThreadWithProgressWindow {
public:
    LoadLibraryTask(RC505::Library &library, const File &path) :
        ThreadWithProgressWindow("Loading Library ...", false, false),
        _library(library),
        _path(path),
        _success(false)
    {
    }

    bool success() const { return _success; }

    void run()
    {
        setStatusMessage("Preparing ...");
        _version = RC505::Library::libraryVersion(_path);
        if (_version != RC505_VERSION) {
            return;
        }
        _success = _library.load(_path, [this] (String status) {
            setStatusMessage(status);
        });
    }

    static bool loadLibrary(RC505::Library &library, const File &path) {
        LoadLibraryTask task(library, path);
        task.runThread();
        if (!task.success()) {
            if (task._version != RC505_VERSION) {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error",
                    "Failed to load library from '" + path.getFullPathName() + "'!\n"
                    "Library uses OS revision " + String(task._version) + ", but this software only supports revision " + String(RC505_VERSION) + ".\n"
                    "Please update your RC505 Loop Station to a compatible OS revision!"
                );
            } else {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Failed to load library from '" + path.getFullPathName() + "'!");
            }
        }
        return task.success();
    }

private:
    RC505::Library &_library;
    File _path;
    int _version;
    bool _success;
};

class SaveLibraryTask : public ThreadWithProgressWindow {
public:
    SaveLibraryTask(RC505::Library &library, const File &path) :
        ThreadWithProgressWindow("Saving Library ...", false, false),
        _library(library),
        _path(path),
        _success(false)
    {
    }

    bool success() const { return _success; }

    void run()
    {
        setStatusMessage("Preparing ...");
        _success = _library.save(_path, [this] (String status) {
            setStatusMessage(status);
        });
    }

    static bool saveLibrary(RC505::Library &library, const File &path) {
        SaveLibraryTask task(library, path);
        task.runThread();
        if (!task.success()) {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Failed to save library to '" + path.getFullPathName() + "'!");
        }
        return task.success();
    }

private:
    RC505::Library &_library;
    File _path;
    bool _success;
};
