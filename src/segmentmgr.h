/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 MSX
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2012-10-01
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 ***
 * This module manages segment lists
 **************************************************************/


#ifndef _SEGMENTMGR_H_
#define _SEGMENTMGR_H_

#include <wx/dynarray.h>
#include "IDZ80Base.h"

struct SegmentItem {
        uint    segstart,
                segend;

};


class SegmentMgr {
    public:
        SegmentMgr();
        ~SegmentMgr();

        int Add(uint _start, uint _end);
        void Del(uint index);
        void Clear();
        SegmentItem *GetItem(uint index);
        SegmentItem *First();
        SegmentItem *Last();
        uint GetCount();
        bool inSegment(uint address);
        void Sort();
        void Optimize();
        void SetSegmentIndex(int segidx);
        int GetSegmentIndex();
        SegmentItem *GetSegmentPtr();

    protected:
    private:
        wxArrayPtrVoid *m_segmentlist;
        int             m_SegmentIndex;
        SegmentItem     *m_SegmentPtr;



};

int CompareSegments(void **item1, void **item2);

#endif

