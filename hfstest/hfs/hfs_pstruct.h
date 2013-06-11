//
//  hfs_pstruct.h
//  hfstest
//
//  Created by Adam Knight on 5/5/13.
//  Copyright (c) 2013 Adam Knight. All rights reserved.
//

#ifndef hfstest_hfs_pstruct_h
#define hfstest_hfs_pstruct_h

#include "hfs_structs.h"
#include "hfs_extentlist.h"

#define MAC_GMT_FACTOR 2082844800UL

void set_hfs_volume                 (HFSVolume *v);

void PrintVolumeHeader              (const HFSPlusVolumeHeader *vh);
void PrintExtentList                (const ExtentList* list, u_int32_t totalBlocks);
void PrintForkExtentsSummary        (const HFSFork* fork);
void PrintHFSPlusForkData           (const HFSPlusForkData *fork, u_int32_t cnid, u_int8_t forktype);
void PrintBTNodeDescriptor          (const BTNodeDescriptor *node);
void PrintBTHeaderRecord            (const BTHeaderRec *hr);
void PrintHFSPlusBSDInfo            (const HFSPlusBSDInfo *record);
void PrintFndrFileInfo              (const FndrFileInfo *record);
void PrintFndrDirInfo               (const FndrDirInfo *record);
void PrintFndrOpaqueInfo            (const FndrOpaqueInfo *record);
void PrintHFSPlusCatalogFolder      (const HFSPlusCatalogFolder *record);
void PrintHFSPlusCatalogFile        (const HFSPlusCatalogFile *record);
void PrintHFSPlusFolderThreadRecord (const HFSPlusCatalogThread *record);
void PrintHFSPlusFileThreadRecord   (const HFSPlusCatalogThread *record);
void PrintHFSPlusCatalogThread      (const HFSPlusCatalogThread *record);
void PrintJournalInfoBlock          (const JournalInfoBlock *record);

void PrintVolumeSummary             (const VolumeSummary *summary);
void PrintForkSummary               (const ForkSummary *summary);

void VisualizeHFSPlusExtentKey      (const HFSPlusExtentKey *record, const char* label, bool oneLine);
void VisualizeHFSPlusCatalogKey     (const HFSPlusCatalogKey *record, const char* label, bool oneLine);
void VisualizeHFSBTreeNodeRecord    (const HFSBTreeNodeRecord* record, const char* label);
void VisualizeData                  (const char* data, size_t length);

void PrintTreeNode                  (const HFSBTree *tree, u_int32_t nodeID);
void PrintNode                      (const HFSBTreeNode* node);
void PrintNodeRecord                (const HFSBTreeNode* node, int recordNumber);

#endif
