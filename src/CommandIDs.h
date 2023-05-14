#pragma once

namespace CommandIDs
{
enum
{
    newLibrary = 0x202000,
    openLibrary = 0x202001,
    saveLibrary = 0x202002,
    saveLibraryAs = 0x202003,
    closeLibrary = 0x202004,
};
}

namespace CommandCategories
{
static const char* const library = "Library";
}
