/* =========================================================================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

#ifndef BINARYDATA_H_70458113_INCLUDED
#define BINARYDATA_H_70458113_INCLUDED

namespace BinaryData
{
    extern const char*   iconlarge_png;
    const int            iconlarge_pngSize = 73464;

    extern const char*   iconsmall_png;
    const int            iconsmall_pngSize = 10347;

    extern const char*   MEMORY_RC0;
    const int            MEMORY_RC0Size = 1233520;

    extern const char*   SYSTEM_RC0;
    const int            SYSTEM_RC0Size = 13020;

    extern const char*   COPYING;
    const int            COPYINGSize = 1475;

    extern const char*   COPYING2;
    const int            COPYING2Size = 1774;

    // Points to the start of a list of resource names.
    extern const char* namedResourceList[];

    // Number of elements in the namedResourceList array.
    const int namedResourceListSize = 6;

    // If you provide the name of one of the binary resource variables above, this function will
    // return the corresponding data and its size (or a null pointer if the name isn't found).
    const char* getNamedResource (const char* resourceNameUTF8, int& dataSizeInBytes) throw();
}

#endif
