//
//  free_space.c
//  hfsinspect
//
//  Created by Adam Knight on 4/1/14.
//  Copyright (c) 2014 Adam Knight. All rights reserved.
//

#include <stdlib.h>
#include "operations.h"
#include "hfs/output_hfs.h"
#include "hfs/hfs.h"

void showFreeSpace(HIOptions *options)
{
    HFSFork *fork = NULL;
    if ( hfsfork_get_special(&fork, &options->hfs, kHFSAllocationFileID) < 0 )
        die(1, "Couldn't get a reference to the volume allocation file.");
    
    char *data = calloc(fork->logicalSize, 1);
    {
        size_t block_size = 128*1024, offset = 0;
        ssize_t bytes = 0;
        void *block = calloc(block_size, 1);
        while ( (bytes = hfs_read_fork_range(block, fork, block_size, offset)) > 0 ) {
            memcpy(data+offset, block, bytes);
            offset += bytes;
            if (offset >= fork->logicalSize) break;
        }
        free(block);
        
        if (offset == 0) {
            free(data);
            
            // We didn't read anything.
            if (bytes < 0) {
                // Error
                die(errno, "error reading allocation file");
            }
            
            return;
        }
    }
    
    size_t total_used = 0;
    size_t total_free = 0;
    size_t total_extents = 0;
    
    struct extent {
        uint8_t     used;
        size_t      start;
        size_t      length;
    };
    
    // The first allocation block is used by the VH.
    struct extent currentExtent = {1,0,1};
    
    for (size_t i = 0; i < options->hfs.vh.totalBlocks; i++) {
        bool used = BTIsBlockUsed(i, data, fork->logicalSize);
        if (used == currentExtent.used && (i != (options->hfs.vh.totalBlocks - 1))) {
            currentExtent.length++;
            continue;
        }
        
        total_extents += 1;
        
        if (currentExtent.used)
            total_used += currentExtent.length;
        else
            total_free += currentExtent.length;
        
        currentExtent.used = used;
        currentExtent.start = i;
        currentExtent.length = 1;
    }
    
    BeginSection("Allocation File Statistics");
    PrintAttribute("Extents", "%zu", total_extents);
    _PrintHFSBlocks("Used Blocks", total_used);
    _PrintHFSBlocks("Free Blocks", total_free);
    _PrintHFSBlocks("Total Blocks", total_used + total_free);
    EndSection();
    
    free(data);
    hfsfork_free(fork);
}

