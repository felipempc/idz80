/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-05-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program in smart mode.
 **************************************************************/

  // TODO: REWRITE: Work in progress


#ifndef _DECODER_SMART_H_
#define _DECODER_SMART_H_

#include "decoder.hpp"

// DEBUG DEFINITION
#define IDZ80_DEBUG_DECODER

class SmartDecoder: public Decoder
{
public:
    SmartDecoder(ProjectBase *t_parent);
    ~SmartDecoder();
    void fullDisassemble();
    void clear();

protected:

private:
    //void ProcessCallSubrotine();
    //void ProcessReturnSubrotine();
    bool getNextNearJump(AddressVector &t_jmplist, AbsoluteAddress t_start, AbsoluteAddress t_end, AbsoluteAddress &t_nextaddr);
    //bool GetNextFarJump(AddressVector &t_jmplist, AbsoluteAddress &t_nextaddr);
    bool testIfOutBoundaries(AbsoluteAddress t_addr);
    void updateBoundaries();
    bool callSubroutine(DisassembledItem *t_dasmed_item);
    bool returnSubroutine(DisassembledItem *t_dasmed_item, AbsoluteAddress &t_dest_address);
    bool processBranch(DisassembledItem *t_dasmed_item, bool &t_processing_status);
    void fillData();
    bool findAddressIn(const int &t_address, const AddressVector &t_address_list);
    bool removeAddressFrom(const int &t_address, AddressVector &t_address_list);

    void debugShowList(const wxString &t_listname, const AddressVector &t_list);

    AddressVector   m_address_list,             // List of addresses to be processed
                    m_address_list_processed;   // List of already processed addresses.

    SubRoutineCtrl      *m_sub_routine;

    uint    m_start_address,
            m_end_address,
            m_exec_address,
            m_prg_counter,
            m_last_prg_counter,
            m_next_address,         // The address of the next instruction
            m_actual_address;
};


#endif // _DECODER_SMART_H_