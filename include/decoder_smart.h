/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-05-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program in smart mode.
 **************************************************************/

  // MUST BE COMPLETELY REWRITED !!!!


#ifndef _DECODER_SMART_H_
#define _DECODER_SMART_H_

#include "decoder.h"


class SmartDecoder: public Decoder
{
public:
    SmartDecoder(ProcessBase *parent, LogWindow *logparent);
    ~SmartDecoder();
    bool FullDisassemble(LabelManager *parent);
    void Clear();



protected:

private:
    void ProcessCallSubrotine();
    void ProcessReturnSubrotine();
    bool GetNextNearJump(SortedIntArray *jmplist, AbsoluteAddress _start, AbsoluteAddress _end, AbsoluteAddress &nextaddr);
    bool GetNextFarJump(SortedIntArray *jmplist, AbsoluteAddress &nextaddr);
    bool TestIfOutBoundaries(AbsoluteAddress addr);
    void UpdateBoundaries();
    bool CallSubroutine(DisassembledItem *de);
    bool ReturnSubroutine(DisassembledItem *de, AbsoluteAddress &dest_address);
    bool ProcessBranch(DisassembledItem *de, bool &processing_status);
    void FillData();

    void DebugShowList(const wxString &listname, SortedIntArray *_list);
    void DebugShowJmpList(const wxString &listname, SortedIntArray *_list);

    SortedIntArray      *address_list_,
                        *address_list_processed_;

    SubRoutineCtrl      *sub_routine_;

    uint    start_address_,
            end_address_,
            exec_address_,
            prg_counter_,
            last_prg_counter_,
            next_address_,
            actual_address_;
};


#endif // _DECODER_SMART_H_
