/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-05-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program in smart mode.
 **************************************************************/

  // REWRITE: Work in progress


#ifndef _DECODER_SMART_H_
#define _DECODER_SMART_H_

#include "decoder.h"

// DEBUG DEFINITION
#define IDZ80_DEBUG_DECODER

class SmartDecoder: public Decoder
{
public:
    SmartDecoder(ProjectBase *t_parent);
    ~SmartDecoder();
    bool FullDisassemble();
    void Clear();

protected:

private:
    void ProcessCallSubrotine();
    void ProcessReturnSubrotine();
    bool GetNextNearJump(IntArray &t_jmplist, AbsoluteAddress t_start, AbsoluteAddress t_end, AbsoluteAddress &t_nextaddr);
    bool GetNextFarJump(IntArray &t_jmplist, AbsoluteAddress &t_nextaddr);
    bool TestIfOutBoundaries(AbsoluteAddress t_addr);
    void UpdateBoundaries();
    bool CallSubroutine(DisassembledItem *t_de);
    bool ReturnSubroutine(DisassembledItem *t_de, AbsoluteAddress &t_dest_address);
    bool ProcessBranch(DisassembledItem *t_de, bool &t_processing_status);
    void FillData();
    bool FindAddressIn(const int &t_address, const IntArray &t_address_list);
    bool RemoveAddressFrom(const int &t_address, IntArray &t_address_list);

    void DebugShowList(const wxString &t_listname, const IntArray &t_list);
    //void DebugShowJmpList(const wxString &t_listname, const IntArray &t_list);  IT BECAME REDUNDANT

    IntArray    m_address_list,
                m_address_list_processed;

    SubRoutineCtrl      *m_sub_routine;

    uint    m_start_address,
            m_end_address,
            m_exec_address,
            m_prg_counter,
            m_last_prg_counter,
            m_next_address,
            m_actual_address;
};


#endif // _DECODER_SMART_H_
