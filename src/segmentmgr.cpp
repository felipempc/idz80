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


 #include "segmentmgr.h"



SegmentMgr::SegmentMgr()
{
    m_segmentlist = new wxArrayPtrVoid();

}


SegmentMgr::~SegmentMgr()
{
    Clear();
    if (m_segmentlist != 0)
        delete m_segmentlist;
}



void SegmentMgr::Clear()
{
    int i;
    SegmentItem *si;

    for (i = 0; i < m_segmentlist->GetCount(); i++)
    {
        si = GetItem(i);
        if (si != 0)
        {
            delete si;
            si = 0;
        }
    }
    m_segmentlist->Clear();
}


int SegmentMgr::Add(uint _start, uint _end)
{
    SegmentItem *si;
    int ret = -1;

    if (_start <= _end)
    {
        si = new SegmentItem;
        si->segstart = _start;
        si->segend = _end;
        m_segmentlist->Add((void *)si);
        ret = m_segmentlist->GetCount() - 1;
    }
    return ret;
}




void SegmentMgr::Del(uint index)
{
    SegmentItem *si;

    if ((index >= 0) || (index < m_segmentlist->GetCount()))
    {
        si = (SegmentItem *)m_segmentlist->Item(index);
        if (si != 0)
        {
            delete si;
            si = 0;
        }
        m_segmentlist->RemoveAt(index);
    }
}





SegmentItem *SegmentMgr::GetItem(uint index)
{
    SegmentItem     *si = 0;

    if ((index >= 0) || (index < m_segmentlist->GetCount()))
    {
        si = (SegmentItem *)m_segmentlist->Item(index);
    }
    return si;
}




SegmentItem *SegmentMgr::First()
{
    SegmentItem     *si = 0;

    if (m_segmentlist->GetCount() > 0)
    {
        si = (SegmentItem *)m_segmentlist->Item(0);
    }
    return si;
}





SegmentItem *SegmentMgr::Last()
{
    SegmentItem     *si = 0;
    uint            sizelist;

    sizelist = m_segmentlist->GetCount();

    if (sizelist > 0)
    {
        si = (SegmentItem *)m_segmentlist->Item(sizelist - 1);
    }
    return si;
}


uint SegmentMgr::GetCount()
{
    return m_segmentlist->GetCount();
}



bool SegmentMgr::inSegment(uint address)
{
    int i;
    SegmentItem *si;
    bool ret = false;

    for (i = 0; i < m_segmentlist->GetCount(); i++)
    {
        si = GetItem(i);
        if ((address >= si->segstart) && (address <= si->segend))
        {
            ret = true;
            break;
        }
    }
    return ret;
}


void SegmentMgr::Sort()
{
    m_segmentlist->Sort(CompareSegments);
}



void SegmentMgr::Optimize()
{
    SegmentItem *si_1,
                *si_2;
    int         i,
                n;
    bool        processing = true;

    i = 0;
    while (processing)
    {
        n = GetCount();
        if ((n > 2) && (i < (n - 1)))
        {
            si_1 = GetItem(i);
            si_2 = GetItem(i + 1);

            if ((si_2->segstart >= si_1->segstart) && (si_2->segstart <= si_1->segend))
            {
                if ((si_2->segend >= si_1->segstart) && (si_2->segend <= si_1->segend))
                {
                    delete si_2;
                    si_2 = 0;
                    m_segmentlist->RemoveAt(i + 1);
                }
                else
                {
                    si_1->segend = si_2->segend;
                    delete si_2;
                    si_2 = 0;
                    m_segmentlist->RemoveAt(i + 1);
                }
            }
            else
                if (si_1->segend == (si_2->segstart - 1))
                {
                    si_1->segend = si_2->segend;
                    delete si_2;
                    si_2 = 0;
                    m_segmentlist->RemoveAt(i + 1);
                }
                else
                    i++;
        }
        else
            processing = false;

    }
}




int CompareSegments(void **item1, void **item2)
{
    SegmentItem *seg1, *seg2;

    seg1 = (SegmentItem *)*item1;
    seg2 = (SegmentItem *)*item2;

    if (seg1->segstart < seg2->segstart)
        return -1;

    if (seg1->segstart > seg2->segstart)
        return 1;
    return 0;
}




void SegmentMgr::SetSegmentIndex(int segidx)
{
    m_SegmentIndex = segidx;
    if (m_segmentlist->IsEmpty())
    {
        m_SegmentIndex = -1;
        m_SegmentPtr = 0;
    }
    else
    {
        if ((segidx >= 0) || (segidx < GetCount()))
            m_SegmentPtr = GetItem(segidx);
        else
            m_SegmentPtr = 0;
    }
}



int SegmentMgr::GetSegmentIndex()
{
    return m_SegmentIndex;
}



SegmentItem *SegmentMgr::GetSegmentPtr()
{
    return GetItem(m_SegmentIndex);
}



