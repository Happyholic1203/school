#ifndef _BUF_H
#define _BUF_H

#include <cstdio>
#include "db.h"
#include "page.h"


enum ReplacementPolicy {
        LRU,
        MRU
};

class BufMgr
{

        public:

                BufMgr( int bufsize, ReplacementPolicy policy = LRU);
                ~BufMgr();
                Status PinPage( PageID pid, Page*& page, Bool emptyPage=FALSE );
                Status UnpinPage( PageID pid, Bool dirty=FALSE );
                Status NewPage( PageID& pid, Page*& firstpage,int howmany=1 );
                Status FreePage( PageID pid );
                Status FlushPage( PageID pid );
                Status FlushAllPages();

                unsigned int getNumOfBuffers();
                unsigned int getNumOfUnpinnedBuffers();

                unsigned int hitRatioPercentage();

};


#endif // _BUF_H
