//
//  hfs_extent_ops.h
//  hfsinspect
//
//  Created by Adam Knight on 6/4/13.
//  Copyright (c) 2013 Adam Knight. All rights reserved.
//

#ifndef hfsinspect_hfs_extents_h
#define hfsinspect_hfs_extents_h

#include "hfs/types.h"
#include "hfs/hfs_extentlist.h"

int  hfsplus_get_extents_btree (BTreePtr* tree, const HFSPlus* hfs) __attribute__((nonnull));
int  hfsplus_extents_find_record (HFSPlusExtentRecord* record, hfs_block_t* record_start_block, const HFSPlusFork* fork, size_t startBlock) __attribute__((nonnull));
int  hfsplus_extents_compare_keys (const HFSPlusExtentKey* key1, const HFSPlusExtentKey* key2) __attribute__((nonnull));
bool hfsplus_extents_get_extentlist_for_fork (ExtentList* list, const HFSPlusFork* fork) __attribute__((nonnull));
int  hfsplus_extents_get_node (BTreeNodePtr* node, const BTreePtr bTree, bt_nodeid_t nodeNum) __attribute__((nonnull));

void swap_HFSPlusExtentKey          (HFSPlusExtentKey* record) __attribute__((nonnull));
void swap_HFSPlusExtentRecord       (HFSPlusExtentDescriptor record[]) __attribute__((nonnull));
void swap_HFSPlusExtentDescriptor   (HFSPlusExtentDescriptor* extent) __attribute__((nonnull));

#endif
