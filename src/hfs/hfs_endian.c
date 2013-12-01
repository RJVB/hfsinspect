//
//  hfs_endian.c
//  hfsinspect
//
//  Created by Adam Knight on 5/5/13.
//  Copyright (c) 2013 Adam Knight. All rights reserved.
//

#include "hfs/hfs_endian.h"

#include "misc/output.h"
#include "misc/_endian.h"
#include "hfs/hfs_structs.h"
#include "hfs/btree/btree_endian.h"
#include "hfs/catalog.h"

void swap_HFSExtentDescriptor(HFSExtentDescriptor* record)
{
    Convert16(record->startBlock);
    Convert16(record->blockCount);
}

void swap_HFSExtentRecord(HFSExtentRecord* record)
{
    FOR_UNTIL(i, kHFSExtentDensity) swap_HFSExtentDescriptor(record[i]);
}

void swap_HFSMasterDirectoryBlock(HFSMasterDirectoryBlock* record)
{
    Convert16(record->drSigWord);	/* == kHFSSigWord */
	Convert32(record->drCrDate);	/* date and time of volume creation */
	Convert32(record->drLsMod);	/* date and time of last modification */
	Convert16(record->drAtrb);		/* volume attributes */
	Convert16(record->drNmFls);	/* number of files in root folder */
	Convert16(record->drVBMSt);	/* first block of volume bitmap */
	Convert16(record->drAllocPtr);	/* start of next allocation search */
	Convert16(record->drNmAlBlks);	/* number of allocation blocks in volume */
	Convert32(record->drAlBlkSiz);	/* size (in bytes) of allocation blocks */
	Convert32(record->drClpSiz);	/* default clump size */
	Convert16(record->drAlBlSt);	/* first allocation block in volume */
	Convert32(record->drNxtCNID);	/* next unused catalog node ID */
	Convert16(record->drFreeBks);	/* number of unused allocation blocks */
	Convert32(record->drVolBkUp);	/* date and time of last backup */
	Convert16(record->drVSeqNum);	/* volume backup sequence number */
	Convert32(record->drWrCnt);	/* volume write count */
	Convert32(record->drXTClpSiz);	/* clump size for extents overflow file */
	Convert32(record->drCTClpSiz);	/* clump size for catalog file */
	Convert16(record->drNmRtDirs);	/* number of directories in root folder */
	Convert32(record->drFilCnt);	/* number of files in volume */
	Convert32(record->drDirCnt);	/* number of directories in volume */
    // noswap: Convert32(record->drFndrInfo[8]);	/* information used by the Finder */
	Convert16(record->drEmbedSigWord);	/* embedded volume signature (formerly drVCSize) */
	swap_HFSExtentDescriptor(&record->drEmbedExtent);	/* embedded volume location and size (formerly drVBMCSize and drCtlCSize) */
	Convert32(record->drXTFlSize);	/* size of extents overflow file */
	swap_HFSExtentRecord(&record->drXTExtRec);	/* extent record for extents overflow file */
	Convert32(record->drCTFlSize);	/* size of catalog file */
	swap_HFSExtentRecord(&record->drCTExtRec);	/* extent record for catalog file */
}

void swap_HFSPlusVolumeHeader(HFSPlusVolumeHeader *record)
{
    Convert16(record->signature);
    Convert16(record->version);
    Convert32(record->attributes);
    Convert32(record->lastMountedVersion);
    Convert32(record->journalInfoBlock);
    
    Convert32(record->createDate);
    Convert32(record->modifyDate);
    Convert32(record->backupDate);
    Convert32(record->checkedDate);
    
    Convert32(record->fileCount);
    Convert32(record->folderCount);
    
    Convert32(record->blockSize);
    Convert32(record->totalBlocks);
    Convert32(record->freeBlocks);
    
    Convert32(record->nextAllocation);
    Convert32(record->rsrcClumpSize);
    Convert32(record->dataClumpSize);
    Convert32(record->nextCatalogID);
    
    Convert32(record->writeCount);
    Convert64(record->encodingsBitmap);
    
    HFSPlusVolumeFinderInfo* finderInfo = (HFSPlusVolumeFinderInfo*)&record->finderInfo;
    Convert32(finderInfo->bootDirID);
    Convert32(finderInfo->bootParentID);
    Convert32(finderInfo->openWindowDirID);
    Convert32(finderInfo->os9DirID);
    // noswap: reserved is reserved (uint32)
    Convert32(finderInfo->osXDirID);
    Convert64(finderInfo->volID);
    
    swap_HFSPlusForkData(&record->allocationFile);
    swap_HFSPlusForkData(&record->extentsFile);
    swap_HFSPlusForkData(&record->catalogFile);
    swap_HFSPlusForkData(&record->attributesFile);
    swap_HFSPlusForkData(&record->startupFile);
}

void swap_JournalInfoBlock(JournalInfoBlock* record)
{
    /*
     struct JournalInfoBlock {
     uint32_t	flags;
     uint32_t       device_signature[8];  // signature used to locate our device.
     uint64_t       offset;               // byte offset to the journal on the device
     uint64_t       size;                 // size in bytes of the journal
     uuid_string_t   ext_jnl_uuid;
     char            machine_serial_num[48];
     char    	reserved[JIB_RESERVED_SIZE];
     } __attribute__((aligned(2), packed));
     typedef struct JournalInfoBlock JournalInfoBlock;
     */
    Convert32(record->flags);
    FOR_UNTIL(i, 8) Convert32(record->device_signature[i]);
    Convert64(record->offset);
    Convert64(record->size);
    // noswap: uuid_string_t is a series of char
    // noswap: machine_serial_num is a series of char
    // noswap: reserved is reserved
}

void swap_HFSPlusForkData(HFSPlusForkData *record)
{
    Convert64(record->logicalSize);
    Convert32(record->totalBlocks);
    Convert32(record->clumpSize);
    swap_HFSPlusExtentRecord(record->extents);
}

void swap_HFSPlusExtentRecord(HFSPlusExtentDescriptor record[])
{
    FOR_UNTIL(i, kHFSPlusExtentDensity) swap_HFSPlusExtentDescriptor(&record[i]);
}

void swap_HFSPlusExtentDescriptor(HFSPlusExtentDescriptor *record)
{
    Convert32(record->startBlock);
    Convert32(record->blockCount);
}

void swap_HFSPlusExtentKey(HFSPlusExtentKey *record)
{
//    noswap: keyLength; swapped in swap_BTNode
    Convert32(record->fileID);
    Convert32(record->startBlock);
}

void swap_HFSUniStr255(HFSUniStr255 *unistr)
{
    Convert16(unistr->length);
    FOR_UNTIL(i, unistr->length) Convert16(unistr->unicode[i]);
}

void swap_HFSPlusAttrKey(HFSPlusAttrKey *record)
{
//    noswap: keyLength; swapped in swap_BTNode
    Convert16(record->pad);
    Convert32(record->fileID);
    Convert32(record->startBlock);
    Convert16(record->attrNameLen);
    FOR_UNTIL(i, record->attrNameLen) Convert16(record->attrName[i]);
}

void swap_HFSPlusAttrData(HFSPlusAttrData* record)
{
    // noswap: Convert32(record->recordType); (previously swapped)
    // noswap: Convert32(record->reserved[0]);
    // noswap: Convert32(record->reserved[1]);
    Convert32(record->attrSize);
}

void swap_HFSPlusAttrForkData(HFSPlusAttrForkData* record)
{
    // noswap: Convert32(record->recordType); (previously swapped)
    // noswap: Convert32(record->reserved);
    swap_HFSPlusForkData(&record->theFork);
}

void swap_HFSPlusAttrExtents(HFSPlusAttrExtents* record)
{
    // noswap: Convert32(record->recordType); (previously swapped)
    // noswap: Convert32(record->reserved);
    swap_HFSPlusExtentRecord(record->extents);
}

void swap_HFSPlusAttrRecord(HFSPlusAttrRecord* record)
{
    record->recordType = bswap32(record->recordType);
    switch (record->recordType) {
        case kHFSPlusAttrInlineData:
            // InlineData is no more; use AttrData.
            swap_HFSPlusAttrData(&record->attrData);
            break;
            
        case kHFSPlusAttrForkData:
            swap_HFSPlusAttrForkData(&record->forkData);
            break;
            
        case kHFSPlusAttrExtents:
            swap_HFSPlusAttrExtents(&record->overflowExtents);
            break;

        default:
            critical("Unknown attribute record type: %d", record->recordType);
            break;
    }
}

//int swap_BTreeNode(BTreeNodePtr node)
//{
//    BTreePtr bTree = node->bTree;
//    
//    // Check record offset 0 to see if we've done this before (last two bytes of the node). It's a constant 14.
//    uint16_t sentinel = *(uint16_t*)(node->data + bTree->headerRecord.nodeSize - sizeof(uint16_t));
//    if ( sentinel == 14 ) return 1;
//    
//    // Verify that this is a node in the first place (swap error protection).
//    sentinel = be16toh(sentinel);
//    if ( sentinel != 14 ) {
//        warning("Node %u is not a node (sentinel: %u != 14).", node->nodeNumber, sentinel);
//        errno = EINVAL;
//        return -1;
//    }
//    
//    /* First, swap things universal to all nodes. */
//    
//    // Swap node descriptor
//    swap_BTNodeDescriptor(node->nodeDescriptor);
//    
//    // Verify this is a valid node (additional protection against swap errors)
//    if (node->nodeDescriptor->kind < kBTLeafNode || node->nodeDescriptor->kind > kBTMapNode) {
//        warning("Invalid node kind: %d", node->nodeDescriptor->kind);
//        errno = EINVAL;
//        return -1;
//    }
//    
//    // Swap record offsets
//    uint16_t numRecords = node->nodeDescriptor->numRecords + 1;  // "+1" gets the free space record.
//    BTRecOffset *offsets = (BTRecOffset*)(node->data + bTree->headerRecord.nodeSize - (sizeof(BTRecOffset) * numRecords));
//    
//    FOR_UNTIL(i, numRecords) offsets[i] = be16toh(offsets[i]);
//    
//    // Validate offsets
//    off_t record_min = sizeof(BTNodeDescriptor);
//    off_t record_max = bTree->headerRecord.nodeSize;
//    
//    {
//        int prev = record_max;
//        FOR_UNTIL(i, numRecords) {
//            if (offsets[i] < record_min || offsets[i] > record_max) {
//                warning("record %u points outside this node: %u (%u, %u)", i, offsets[i], record_min, record_max);
//            }
//            if (i != 0 && offsets[i] > prev) {
//                warning("record %u is out of order (%u > %u)", i, offsets[i-1], offsets[i]);
//            }
//            prev = offsets[i];
//        }
//    }
//    
//    for (int recordNum = 0; recordNum < node->nodeDescriptor->numRecords; recordNum++) {
//        Bytes record = BTGetRecord(&bTree->headerRecord, node->data, recordNum);
//        BTRecOffset recordSize = BTGetRecordSize(&bTree->headerRecord, node->data, recordNum);
//
//        if (node->nodeDescriptor->kind == kBTIndexNode || node->nodeDescriptor->kind == kBTLeafNode) {
///*
// Immediately following the keyLength is the key itself. The length of the key is determined by the node type and the B-tree attributes. In leaf nodes, the length is always determined by keyLength. In index nodes, the length depends on the value of the kBTVariableIndexKeysMask bit in the B-tree attributes in the header record. If the bit is clear, the key occupies a constant number of bytes, determined by the maxKeyLength field of the B-tree header record. If the bit is set, the key length is determined by the keyLength field of the keyed record.
// 
// Following the key is the record's data. The format of this data depends on the node type, as explained in the next two sections. However, the data is always aligned on a two-byte boundary and occupies an even number of bytes. To meet the first alignment requirement, a pad byte must be inserted between the key and the data if the size of the keyLength field plus the size of the key is odd. To meet the second alignment requirement, a pad byte must be added after the data if the data size is odd.
//
// http://developer.apple.com/legacy/library/technotes/tn/tn1150.html#KeyedRecords
// */
//            
//            BTRecOffset keySize = bTree->headerRecord.maxKeyLength;
//            BTRecOffset dataSize = 0;
//            
//            if ( (node->nodeDescriptor->kind == kBTLeafNode) ||
//                (node->nodeDescriptor->kind == kBTIndexNode && bTree->headerRecord.attributes & kBTVariableIndexKeysMask) ) {
//                
//                BTreeKey *keyPtr = (BTreeKey*)record;
//                if (bTree->headerRecord.attributes & kBTBigKeysMask) {
//                    Convert16(keyPtr->length16);
//                }
//            }
//            
//            BTreeKey recordKey = {0};
//            Byte recordValue[recordSize]; ZERO_STRUCT(recordValue);
//            
//            if (btree_get_record(&recordKey, &recordValue, bTree, node, recordNum) < 0)
//                return -1;
//            
//            // Each tree has a different key type.
//            switch (bTree->treeID) {
//                case kHFSCatalogFileID:
//                {
//                    if (meta->keyLength < kHFSPlusCatalogKeyMinimumLength) {
//                        meta->keyLength = kHFSPlusCatalogKeyMinimumLength;
//                    } else if (meta->keyLength > kHFSPlusCatalogKeyMaximumLength) {
//                        critical("Invalid key length for catalog record: %d", meta->keyLength);
//                    }
//                    swap_HFSPlusCatalogKey((HFSPlusCatalogKey*)meta->key);
//                    break;
//                }
//                    
//                case kHFSExtentsFileID:
//                {
//                    if (meta->keyLength != kHFSPlusExtentKeyMaximumLength) {
//                        critical("Invalid key length for extent record: %d", meta->keyLength);
//                    }
//                    swap_HFSPlusExtentKey((HFSPlusExtentKey*)meta->key);
//                    break;
//                }
//                
//                case kHFSAttributesFileID:
//                {
//                    if (meta->keyLength < kHFSPlusAttrKeyMinimumLength) {
//                        meta->keyLength = kHFSPlusAttrKeyMinimumLength;
//                    } else if (meta->keyLength > kHFSPlusAttrKeyMaximumLength) {
//                        critical("Invalid key length for attribute record: %d", meta->keyLength);
//                    }
//                    
//                    HFSPlusAttrKey *key = (HFSPlusAttrKey*)meta->key;
//                    
//                    // Validate the size of the record
//                    if ( (char*) &key->attrName[1] > (meta->record + meta->length) ) {
//                        error("attribute key #%d offset too big (0x%04X)", meta->recordID, offsets[recordNum]);
//                        continue;
//                    }
//                    
//                    // Validate the size of the key
//                    if ( (char*)(meta->record + sizeof(key->keyLength) + meta->keyLength) > (meta->record + meta->length)) {
//                        error("attribute key #%d extends past the end of the record (%d > %d)",
//                              ( node->nodeDescriptor.numRecords - recordNum - 1 ),
//                              ( sizeof(key->keyLength) + meta->keyLength ),
//                              meta->length
//                              );
//                        continue;
//                    }
//                    
//                    swap_HFSPlusAttrKey((HFSPlusAttrKey*)meta->key);
//                    break;
//                }
//                   
//                default:
//                    critical("Unhandled tree type: %d", meta->treeCNID);
//                    break;
//            }
//            
//            if (meta->keyLength % 2) meta->keyLength++; // Round up to an even length.
//            
//            meta->value         = (char*)meta->key + sizeof(uint16_t) + meta->keyLength;
//            meta->valueLength   = meta->length - meta->keyLength;
//        }
//        
//        switch (node->nodeDescriptor.kind) {
//            case kBTIndexNode:
//            {
//                uint32_t *pointer = (uint32_t*)meta->value;
//                *pointer = bswap32(*pointer);
//                break;
//            }
//                
//            case kBTLeafNode:
//            {
//                switch (meta->treeID) {
//                    case kHFSCatalogFileID:
//                    {
//                        uint16_t recordKind = *(uint16_t*)meta->value;
//                        recordKind = bswap16(recordKind);
//                        HFSPlusCatalogRecord* catalogRecord = (HFSPlusCatalogRecord*)meta->value;
//                        
//                        switch (recordKind) {
//                            case kHFSPlusFolderRecord:
//                                swap_HFSPlusCatalogFolder(&catalogRecord->catalogFolder); break;
//                                
//                            case kHFSPlusFileRecord:
//                                swap_HFSPlusCatalogFile(&catalogRecord->catalogFile); break;
//                            
//                            case kHFSPlusFolderThreadRecord:
//                            case kHFSPlusFileThreadRecord:
//                                swap_HFSPlusCatalogThread(&catalogRecord->catalogThread); break;
//                                
//                            default:
//                                break;
//                        }
//                        break;
//                    }
//                        
//                    case kHFSExtentsFileID:
//                    {
//                        swap_HFSPlusExtentRecord((HFSPlusExtentDescriptor*)meta->value);
//                        break;
//                    }
//                    
//                    case kHFSAttributesFileID:
//                    {
//                        swap_HFSPlusAttrRecord((HFSPlusAttrRecord*)meta->value);
//                        break;
//                    }
//                        
//                    default:
//                    {
//                        critical("Unknown tree CNID: %d", meta->treeCNID);
//                        break;
//                    }
//                }
//                break;
//            }
//                
//            case kBTHeaderNode:
//            {
//                // Only swap the header.  Don't care about user and map.
//                if (recordNum == 0) {
//                    swap_BTHeaderRec((BTHeaderRec*)meta->record);
//                }
//                break;
//            }
//                
//            case kBTMapNode:
//            {
//                debug("Ignoring map node.");
//                break;
//            }
//                
//            default:
//            {
//                critical("Unknown node kind: %d", node->nodeDescriptor.kind);
//                break;
//            }
//        }
//    }
//    
//    return 1;
//}
