#include <iostream>
#include <stdlib.h>
#include <memory.h>

#include "hfpage.h"
#include "heapfile.h"
#include "buf.h"
#include "db.h"


// **********************************************************
// page class constructor

void HFPage::init(PageId pageNo)
{
  // fill in the body
	curPage = pageNo;
	slotCnt = 0;
	freePtr = 0;
	freeSpace = MAX_SPACE - DPFIXED;
	slot[0].length = -1;
	slot[0].offset = freePtr;
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
  // fill in the body
	return prevPage;
}

// **********************************************************
void HFPage::setPrevPage(PageId pageNo)
{
  // fill in the body
	prevPage = pageNo;
}

// **********************************************************
void HFPage::setNextPage(PageId pageNo)
{
  // fill in the body
	nextPage = pageNo;
}

// **********************************************************
PageId HFPage::getNextPage()
{
  // fill in the body
	return nextPage;
}

// **********************************************************
// Add a new record to the page. Returns OK if everything went OK
// otherwise, returns DONE if sufficient space does not exist
// RID of the new record is returned via rid parameter.
Status HFPage::insertRecord(char* recPtr, int recLen, RID& rid)
{
    // fill in the body
		// TODO: finding out how slots are arranged
		int emptySlotIdx = -1;
		for(int i = 0; i < slotCnt; ++i){
			if(slot[-i].length == -1){
				emptySlotIdx = i;
				break;
			}
		}

		if(emptySlotIdx == -1)
			emptySlotIdx = slotCnt++; // allocate a new slot
		emptySlotIdx = -emptySlotIdx;
		slot[emptySlotIdx].offset = freePtr;
		slot[emptySlotIdx].length = recLen;

		for(int i = 0; i < recLen; ++i)
			data[freePtr++] = recPtr[i];
}

// **********************************************************
// Delete a record from a page. Returns OK if everything went okay.
// Compacts remaining records but leaves a hole in the slot array.
// Use memmove() rather than memcpy() as space may overlap.
Status HFPage::deleteRecord(const RID& rid)
{
   // fill in the body
	 // rid.pageNo
	 // rid.slotNo
	 // TODO: last stopped here
}


// **********************************************************
// Exchange two records from a page. Returns OK if everything went okay.
// Use memmove() to move records if there is a hole while exchanging record.
Status HFPage::exchangeRecord(const RID& firstrid, const RID& secondrid)
{
	// fill in the body
}


// **********************************************************
// returns RID of first record on page
// returns DONE if page contains no records.  Otherwise, returns OK
Status HFPage::firstRecord(RID& firstRid)
{
    // fill in the body
}

// **********************************************************
// returns RID of next record on the page
// returns DONE if no more records exist on the page; otherwise OK
Status HFPage::nextRecord (RID curRid, RID& nextRid)
{
    // fill in the body
}

// **********************************************************
// returns length and copies out record with RID rid
Status HFPage::getRecord(RID rid, char* recPtr, int& recLen)
{
    // fill in the body
}

// **********************************************************
// returns length and pointer to record with RID rid.  The difference
// between this and getRecord is that getRecord copies out the record
// into recPtr, while this function returns a pointer to the record
// in recPtr.
Status HFPage::returnRecord(RID rid, char*& recPtr, int& recLen)
{
    // fill in the body
}

// **********************************************************
// Returns the amount of available space on the heap file page
int HFPage::available_space(void)
{
    // fill in the body
}

// **********************************************************
// Returns 1 if the HFPage is empty, and 0 otherwise.
// It scans the slot directory looking for a non-empty slot.
bool HFPage::empty(void)
{
    // fill in the body
}



