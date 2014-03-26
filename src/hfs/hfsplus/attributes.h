//
//  attributes.h
//  hfsinspect
//
//  Created by Adam Knight on 6/16/13.
//  Copyright (c) 2013 Adam Knight. All rights reserved.
//

#ifndef hfsinspect_hfs_attribute_ops_h
#define hfsinspect_hfs_attribute_ops_h

#include "hfs/hfs_types.h"

int hfs_get_attribute_btree     (BTreePtr *tree, const HFS *hfs) _NONNULL;
int hfs_attributes_compare_keys (const HFSPlusAttrKey *key1, const HFSPlusAttrKey *key2) _NONNULL;
int hfs_attributes_get_node     (BTreeNodePtr *node, const BTreePtr bTree, bt_nodeid_t nodeNum) _NONNULL;

void swap_HFSPlusAttrKey        (HFSPlusAttrKey *record) _NONNULL;
void swap_HFSPlusAttrData       (HFSPlusAttrData* record) _NONNULL;
void swap_HFSPlusAttrForkData   (HFSPlusAttrForkData* record) _NONNULL;
void swap_HFSPlusAttrExtents    (HFSPlusAttrExtents* record) _NONNULL;
void swap_HFSPlusAttrRecord     (HFSPlusAttrRecord* record) _NONNULL;

#endif
