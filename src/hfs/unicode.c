//
//  hfs/unicode.c
//  hfsinspect
//
//  Created by Adam Knight on 2015-02-01.
//  Copyright (c) 2015 Adam Knight. All rights reserved.
//

#include <string.h>

#include "hfs/unicode.h"
#include "hfs/catalog.h"        // HFSPlusMetadataFolder
#include "logging/logging.h"    // console printing routines

int hfsuctowcs(hfs_wc_str output, const HFSUniStr255* input)
{
    trace("output (%p), input (%p)", output, input);

    // Get the length of the input
    unsigned len = MIN(input->length, 255);

    // Copy the u16 to the wchar array
    for(unsigned i = 0; i < len; i++)
        output[i] = input->unicode[i];

    // Terminate the output at the length
    output[len] = L'\0';

    // Replace the catalog version with a printable version.
    if ( wcscmp(output, HFSPlusMetadataFolder) == 0 )
        mbstowcs(output, HFSPLUSMETADATAFOLDER, len);

    return len;
}

// FIXME: Eats higher-order Unicode chars. Could be fun.
HFSUniStr255 wcstohfsuc(const wchar_t* input)
{
    trace("input '%ls'", input);

    // Allocate the return value
    HFSUniStr255 output = {0};

    // Get the length of the input
    size_t       len    = MIN(wcslen(input), 255);

    // Iterate over the input
    for (unsigned i = 0; i < len; i++) {
        // Copy the input to the output
        wchar_t wchar   = input[i];
        UChar16 u16char = UChar32toUChar16(wchar);
        UTF16   uchar   = UChar16toUTF16( u16char );
        output.unicode[i] = uchar;
    }

    // Terminate the output at the length
    output.length = len;

    return output;
}

HFSUniStr255 strtohfsuc(const char* input)
{
    HFSUniStr255 output          = {0};
    wchar_t*     wide            = NULL;
    size_t       char_count      = 0;
    size_t       wide_char_count = 0;
    size_t       str_size        = 0;

    trace("input '%s'", input)

    str_size        = 256 * sizeof(wchar_t);
    wide            = ALLOC(str_size);
    assert(wide != NULL);

    wide_char_count = mbstowcs(wide, input, 255);
    if (wide_char_count > 0) {
        output = wcstohfsuc(wide);
    }
    SFREE(wide);

    char_count = strlen(input);
    if (char_count != wide_char_count) {
        error("Conversion error: mbstowcs returned a string of a different length than the input: %zd in; %zd out", char_count, wide_char_count);
    }

    return output;
}

//#if !defined(__GNUC__)

const UTF16 HI_SURROGATE_START = 0xD800;
const UTF16 LO_SURROGATE_START = 0xDC00;

const UTF32 LEAD_OFFSET        = HI_SURROGATE_START - (0x10000 >> 10);
const UTF32 SURROGATE_OFFSET   = 0x10000 - (HI_SURROGATE_START << 10) - LO_SURROGATE_START;

UChar32 UChar16toUChar32( UChar16 u16 )
{
    return ( (u16.hi << 10) + u16.lo + SURROGATE_OFFSET );
}

UChar16 UChar32toUChar16( UChar32 codepoint )
{
    UChar16 u16 = {0};

    if (codepoint < 0x10000) {
        u16.lo = codepoint;
    } else if (codepoint <= 0x10FFFF) {
        u16.hi = LEAD_OFFSET + (codepoint >> 10);
        u16.lo = 0xDC00 + (codepoint & 0x3FF);
    } else {
        error("invalid code_point");
    }

    return u16;
}

UTF16 UChar16toUTF16(UChar16 u16)
{
    return ((u16.hi << 8) + u16.lo);
}

//#endif
