/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== COPYING ==================
static const unsigned char temp_binary_data_0[] =
"Copyright (c) 2003-2004 Mark Borgerding\n"
"\n"
"All rights reserved.\n"
"\n"
"Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:\n"
"\n"
"    * Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.\n"
"    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.\n"
"    * Neither the author nor the names of any contributors may be used to endorse or promote products derived from this software without specific prior written permission.\n"
"\n"
"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO E"
"VENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR"
" BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH D"
"AMAGE.\n";

const char* COPYING = (const char*) temp_binary_data_0;

//================== COPYING ==================
static const unsigned char temp_binary_data_1[] =
"Copyright 2002-2007 \tXiph.org Foundation\n"
"Copyright 2002-2007 \tJean-Marc Valin\n"
"Copyright 2005-2007\tAnalog Devices Inc.\n"
"Copyright 2005-2007\tCommonwealth Scientific and Industrial Research \n"
"                        Organisation (CSIRO)\n"
"Copyright 1993, 2002, 2006 David Rowe\n"
"Copyright 2003 \t\tEpicGames\n"
"Copyright 1992-1994\tJutta Degener, Carsten Bormann\n"
"\n"
"Redistribution and use in source and binary forms, with or without\n"
"modification, are permitted provided that the following conditions\n"
"are met:\n"
"\n"
"- Redistributions of source code must retain the above copyright\n"
"notice, this list of conditions and the following disclaimer.\n"
"\n"
"- Redistributions in binary form must reproduce the above copyright\n"
"notice, this list of conditions and the following disclaimer in the\n"
"documentation and/or other materials provided with the distribution.\n"
"\n"
"- Neither the name of the Xiph.org Foundation nor the names of its\n"
"contributors may be used to endorse or promote products derived from\n"
"this software without specific prior written permission.\n"
"\n"
"THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS\n"
"``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT\n"
"LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR\n"
"A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR\n"
"CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,\n"
"EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,\n"
"PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR\n"
"PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF\n"
"LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING\n"
"NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS\n"
"SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.\n";

const char* COPYING2 = (const char*) temp_binary_data_1;


const char* getNamedResource (const char*, int&) throw();
const char* getNamedResource (const char* resourceNameUTF8, int& numBytes) throw()
{
    unsigned int hash = 0;
    if (resourceNameUTF8 != 0)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0x63a1442d:  numBytes = 1475; return COPYING;
        case 0x108741a5:  numBytes = 1774; return COPYING2;
        default: break;
    }

    numBytes = 0;
    return 0;
}

const char* namedResourceList[] =
{
    "COPYING",
    "COPYING2"
};

}
