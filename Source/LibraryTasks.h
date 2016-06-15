#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class LoadLibraryTask : public ThreadWithProgressWindow {
public:
    LoadLibraryTask(RC505::Library &library, const File &path) :
        ThreadWithProgressWindow("Loading Library ...", true, false),
        _library(library),
        _path(path),
        _success(false)
    {
    }

    bool success() const { return _success; }

    void run()
    {
        _success = _library.load(_path, [this] (double progress) {
            setProgress(progress);
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
        ThreadWithProgressWindow("Saving Library ...", true, false),
        _library(library),
        _path(path),
        _success(false)
    {
    }

    bool success() const { return _success; }

    void run()
    {
        _success = _library.save(_path, [this] (double progress) {
            setProgress(progress);
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
