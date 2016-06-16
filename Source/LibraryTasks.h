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
        _success = _library.load(_path, [this] (String status) {
            setStatusMessage(status);
        });
    }

    static bool loadLibrary(RC505::Library &library, const File &path) {
        LoadLibraryTask task(library, path);
        task.runThread();
        if (!task.success()) {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Failed to load library from '" + path.getFullPathName() + "'!");
        }
        return task.success();
    }

private:
    RC505::Library &_library;
    File _path;
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
