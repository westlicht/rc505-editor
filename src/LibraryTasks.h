#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class LoadLibraryTask : public ThreadWithProgressWindow
{
public:
    LoadLibraryTask(RC505::Library& library, const File& path)
        : ThreadWithProgressWindow("Loading Library ...", false, false)
        , _library(library)
        , _path(path)
        , _success(false)
    {
    }

    bool success() const { return _success; }

    void run()
    {
        setStatusMessage("Preparing ...");
        _info = RC505::Library::libraryInfo(_path);
        if (!_info.valid || _info.revision != RC505::Revision)
        {
            return;
        }
        _success = _library.load(_path, [this](String status)
                                 { setStatusMessage(status); });
    }

    static bool loadLibrary(RC505::Library& library, const File& path)
    {
        LoadLibraryTask task(library, path);
        task.runThread();
        if (!task.success())
        {
            auto info = task._info;
            if (!info.valid)
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Failed to load library from '" + path.getFullPathName() + "'!\n"
                                                                                                                                          "This is not a valid library folder.\n"
                                                                                                                                          "Please select a valid RC505 Loop Station library folder.");
            }
            else if (info.revision != RC505::Revision)
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Failed to load library from '" + path.getFullPathName() + "'!\n"
                                                                                                                                          "Library uses OS revision "
                                                                                   + String(info.revision) + ", but this software only supports revision " + String(RC505::Revision) + ".\n"
                                                                                                                                                                                       "Please update your RC505 Loop Station to a compatible OS revision.");
            }
            else
            {
                AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Failed to load library from '" + path.getFullPathName() + "'!");
            }
        }
        return task.success();
    }

private:
    RC505::Library& _library;
    File _path;
    RC505::Library::Info _info;
    bool _success;
};

class SaveLibraryTask : public ThreadWithProgressWindow
{
public:
    SaveLibraryTask(RC505::Library& library, const File& path)
        : ThreadWithProgressWindow("Saving Library ...", false, false)
        , _library(library)
        , _path(path)
        , _success(false)
    {
    }

    bool success() const { return _success; }

    void run()
    {
        setStatusMessage("Preparing ...");
        _success = _library.save(_path, [this](String status)
                                 { setStatusMessage(status); });
    }

    static bool saveLibrary(RC505::Library& library, const File& path)
    {
        SaveLibraryTask task(library, path);
        task.runThread();
        if (!task.success())
        {
            AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Error", "Failed to save library to '" + path.getFullPathName() + "'!");
        }
        return task.success();
    }

private:
    RC505::Library& _library;
    File _path;
    bool _success;
};
