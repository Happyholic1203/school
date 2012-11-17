#include <iostream>
#include <stdlib.h>
#include <memory.h>
#include <cassert>
#include <string>

#include "hfpage.h"
#include "heapfile.h"
#include "buf.h"
#include "db.h"


// **********************************************************
// RID stores the positive slotNo, be careful when accessing
// the slots (i.e. access by slot[-i])

// **********************************************************
// page class constructor

void HFPage::init(PageId pageNo)
{
	curPage = pageNo;
	slotCnt = 0;
	freePtr = 0;
	freeSpace = MAX_SPACE - DPFIXED + sizeof(slot_t);
	slot[0].length = -1;
	slot[0].offset = freePtr;
	nextPage = -1;
	prevPage = -1;
}

// **********************************************************
// dump page utlity
void HFPage::dumpPage()
{
	int i;
	
	cout << "dumpPage, this: " << this << endl;
	cout << "curPage= " << curPage << ", nextPage=" << nextPage << endl;
	cout << "freePtr="  << freePtr << ",  freeSpace=" << freeSpace
	     << ", slotCnt=" << slotCnt << endl;

	for (i=0; i < slotCnt; i++) {
		cout << "slot["<< i <<"].offset=" << slot[-i].offset
		<< ", slot["<< i << "].length=" << slot[-i].length << endl;
	}
}

// **********************************************************
PageId HFPage::getPrevPage()
{
	return prevPage;
}

// **********************************************************
void HFPage::setPrevPage(PageId pageNo)
{
	prevPage = pageNo;
}

// **********************************************************
void HFPage::setNextPage(PageId pageNo)
{
	nextPage = pageNo;
}

// **********************************************************
PageId HFPage::getNextPage()
{
	return nextPage;
}

// **********************************************************
// Add a new record to the page. Returns OK if everything went OK
// otherwise, returns DONE if sufficient space does not exist
// RID of the new record is returned via rid parameter.
Status HFPage::insertRecord(char* recPtr, int recLen, RID& rid)
{
	// not enough space
	if(freeSpace < recLen + sizeof(slot_t))
		return DONE;

	int emptySlotIdx = -1;
	for(int i = 0; i < slotCnt; ++i){
		if(slot[-i].length == -1){
			emptySlotIdx = i;
			break;
		}
	}

	if(emptySlotIdx == -1)
		emptySlotIdx = slotCnt++; // allocate a new slot
	slot[-emptySlotIdx].offset = freePtr;
	slot[-emptySlotIdx].length = recLen;

	memcpy(data + freePtr, recPtr, recLen);
	freePtr += recLen;
	freeSpace -= sizeof(slot_t) + recLen;

	rid.pageNo = curPage;
	rid.slotNo = emptySlotIdx;
	return OK;
}

// **********************************************************
// Delete a record from a page. Returns OK if everything went okay.
// Compacts remaining records but leaves a hole in the slot array.
// Use memmove() rather than memcpy() as space may overlap.
Status HFPage::deleteRecord(const RID& rid)
{
	if(rid.pageNo != curPage || rid.slotNo >= slotCnt || rid.slotNo < 0)
		return DONE;

	slot_t& rmSlot = slot[-rid.slotNo];
	if(rmSlot.length == -1)
		return DONE;
	int rmStart = rmSlot.offset;
	int rmEnd = rmStart + rmSlot.length;

	// does compaction, and handles freePtr, freeSpace
	memmove(data + rmStart, data + rmEnd, freePtr - rmEnd);
	freePtr -= rmSlot.length;
	freeSpace += rmSlot.length;

	if(rid.slotNo == slotCnt - 1){ // last slot: delete the slot
		--slotCnt;
	}
	else{
		// update all subsequent slots to point forward
		for(int i = rid.slotNo+1; i < slotCnt; ++i)
			slot[-i].offset -= rmSlot.length;
	}

	rmSlot.length = -1; // effectively delete the slot
	freeSpace += sizeof(slot_t);

	return OK;
}


// **********************************************************
// Exchange two records from a page. Returns OK if everything went okay.
// Use memmove() to move records if there is a hole while exchanging record.
Status HFPage::exchangeRecord(const RID& firstrid, const RID& secondrid)
{
	if(firstrid.pageNo != curPage || secondrid.pageNo != curPage)
		return DONE;
	
	int offset1 = slot[-firstrid.slotNo].offset;
	int offset2 = slot[-secondrid.slotNo].offset;
	slot_t& slot1 = ((offset1 <= offset2) ? slot[-firstrid.slotNo] : slot[-secondrid.slotNo]);
	slot_t& slot2 = ((offset1 <= offset2) ? slot[-secondrid.slotNo] : slot[-firstrid.slotNo]);

	// assertion: slot1.offset <= slot2.offset
	// Case: s1.length > s2.length (the other case is automatically handled)
	// 1. initially: |---s1---|--b1--|-s2-|
	// 2. buffer s1, s2 (using strings)
	// 3. shift left b1 by s1.lengh - s2.length:
	//               |----|--b1--|--------|
	// 4. write out the buffers (s1, s2)
	//               |-s2-|--b1--|---s1---|
	// 5. update the slots


	// 2. buffer s1, s2 (using strings)
	string s1(data + slot1.offset, slot1.length);
	string s2(data + slot2.offset, slot2.length);

	// 3. shift left b1 by s1.lengh - s2.length:
	// initially: |---s1---|--b1--|-s2-|
	//                     | <- oldB1Start
	// shift to : |----|--b1--|--------|
	//                 | <- newB1Start
	char* newB1Start = data + slot1.offset + slot2.length; // from step3
	char* oldB1Start = data + slot1.offset + slot1.length; // from step1
	int b1Len = slot2.offset - (slot1.offset + slot1.length);
	memmove(newB1Start, oldB1Start, b1Len);

	// 4. write out the buffers (s1, s2)
	// initially: |---s1---|--b1--|-s2-|
	//            | <- newS2Start
	//            |-s2-|--b1--|---s1---|
	//                        | <- newS1Start
	int diff = s1.size() - s2.size(); // may be positive or negative
	char* newS2Start = data + slot1.offset;
	char* newS1Start = data + slot2.offset - diff;
	// swap s1, s2 (buffered)
	memmove(newS2Start, s2.c_str(), s2.size());
	memmove(newS1Start, s1.c_str(), s1.size());

	// 5. update the slots
	slot1.length = s2.size();
	slot2.length = s1.size();
	for(int i = 0; i < slotCnt; ++i){
		if(slot[-i].offset > slot1.offset && slot[-i].offset < slot2.offset)
			slot[-i].offset -= diff;
	}
	slot2.offset = slot2.offset - diff;
	return OK;
}


// **********************************************************
// returns RID of first record on page
// returns DONE if page contains no records.  Otherwise, returns OK
Status HFPage::firstRecord(RID& firstRid)
{
	for(int i = 0; i < slotCnt; ++i){
		if(slot[-i].length != -1){
			firstRid.slotNo = i;
			firstRid.pageNo = curPage;
			return OK;
		}
	}
	return DONE;
}

// **********************************************************
// returns RID of next record on the page
// returns DONE if no more records exist on the page; otherwise OK
Status HFPage::nextRecord (RID curRid, RID& nextRid)
{
	if(curRid.pageNo != curPage)
		return DONE;

	// boundary condition
	if(curRid.slotNo == slotCnt - 1)
		return DONE;
	if(curRid.slotNo < 0)
		return FAIL;

	for(int i = curRid.slotNo + 1; i < slotCnt; ++i){
		if(slot[-i].length != -1){
			nextRid.pageNo = curPage;
			nextRid.slotNo = i;
			return OK;
		}
	}
	return FAIL;
}

// **********************************************************
// returns length and copies out record with RID rid
Status HFPage::getRecord(RID rid, char* recPtr, int& recLen)
{
	if(rid.pageNo != curPage)
		return DONE;

	assert(rid.slotNo < slotCnt);
	assert(slot[-rid.slotNo].length != -1);

	recLen = slot[-rid.slotNo].length;
	memcpy(recPtr, data + slot[-rid.slotNo].offset, recLen);
	return OK;
}

// **********************************************************
// returns length and pointer to record with RID rid.  The difference
// between this and getRecord is that getRecord copies out the record
// into recPtr, while this function returns a pointer to the record
// in recPtr.
Status HFPage::returnRecord(RID rid, char*& recPtr, int& recLen)
{
	if(rid.pageNo != curPage)
		return DONE;

	assert(rid.slotNo < slotCnt);

	recLen = slot[-rid.slotNo].length;
	recPtr = data + slot[-rid.slotNo].offset;
	return OK;
}

// **********************************************************
// Returns the amount of available space on the heap file page
int HFPage::available_space(void)
{
	// one slot space is reserved for the next insertion
	return freeSpace - (slotCnt+1) * sizeof(slot_t);
}

// **********************************************************
// Returns 1 if the HFPage is empty, and 0 otherwise.
// It scans the slot directory looking for a non-empty slot.
bool HFPage::empty(void)
{
	for(int i = 0; i < slotCnt; ++i)
		if(slot[-i].length != -1)
			return false;
	return true;
}



