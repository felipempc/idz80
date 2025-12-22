/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program
 **************************************************************/

#include "decoder.hpp"
#include <wx/string.h>

 // WORK IN PROGRESS


const uint Decoder::OPCODE_NOT_FOUND;

Decoder::Decoder(ProjectBase *parent)
{
    m_project_base = parent;
    m_disassembled_list = 0;
    m_program = 0;
    m_labels = parent->m_labels;
    m_mnemonic = parent->m_mnemonics;

    SetTextLog(parent->GetTextLog());
    ModuleName = "DECODER";

}



void Decoder::SetProgramIndex(const unsigned int program_index)
{
    m_disassembled_list = m_project_base->m_disassembled_mgr->Index(program_index);
    m_program = m_project_base->m_programs_mgr->Index(program_index);
}



/// @brief Search for an opcode pattern (from Program)
/// @param prg_index Offset in file program
/// @param maxitems  Max items of opcode to fetch
/// @return the index of mnemonic item. Should return MnemonicItem* instead
uint Decoder::Fetch(const FileOffset prg_index, uint maxitems)
{
    uint 		offset,
                nitems,
                ret = 0;
    wxArrayInt	foundItems;
    byte		scan;
    MnemonicItem    *mnemonic = 0;

    foundItems.Clear();
    offset = 0;
    if (maxitems > MAX_OPCODE_SIZE)
        maxitems = MAX_OPCODE_SIZE;

    while (offset < maxitems)
    {
        scan = m_program->GetData(prg_index + offset);
        m_mnemonic->Find(foundItems, scan, offset);
        ++offset;
        if (foundItems.GetCount() < 2)
			break;
    }
    nitems = foundItems.GetCount();
    if (nitems == 1)
    {
        ret = foundItems.Last();
        mnemonic = m_mnemonic->Item(ret);
        if (mnemonic->GetByteCodeSize() > maxitems)
            ret = OPCODE_NOT_FOUND;
    }
    else
        ret = OPCODE_NOT_FOUND;

    return ret;
}



/*
 * Fill the dasmelement with intruction or data information
 * and insert in the list of dasmelements
 * Arguments: de - an already initiated dasmelement
 *            prg_index - of program file to disassemble
 *            position - of item to be inserted in the list
 * Return = index of the item inserted in the list
  */
uint Decoder::Decode(DisassembledItem *de, FileOffset prg_index, DisassembledIndex dasm_position)
{
    uint     mnc_item,
            ret = -1;
    uint    scan_limit;
    DisassembledItem     *next_de;


    // ensure it won't overwrite existing dasmitems
    next_de = m_disassembled_list->GetData(dasm_position);
    if (next_de == 0)
        scan_limit = m_program->GetSize();
    else
        scan_limit = next_de->GetOffsetInFile() - prg_index;

    mnc_item = Fetch(prg_index, scan_limit);

    if (mnc_item == OPCODE_NOT_FOUND)
    {
        de->SetupDataItem(prg_index);
        ret = m_disassembled_list->Insert(de, dasm_position);
    }
    else
    {
        de->SetupInstructionItem(m_mnemonic->Item(mnc_item), prg_index);
    	ret = m_disassembled_list->Insert(de, dasm_position);
    }

    return ret;
}




/// @brief Create and fill Labels to the arguments
/// @param de Pointer to DisassembledItem
/// @param index of a DisassembledItem in the list
void Decoder::SetupArgumentLabels(DisassembledItem *de, DisassembledIndex index)
{
    Arguments args;

    uint            argument_value;
    wxString		str;

    args = de->GetMnemonic()->GetSourceArgument();

    for (unsigned int i; i < 2; ++i) {
        args = de->GetMnemonic()->GetArgument(i);
        switch (args.type)
        {
            case OT_VARIABLE:
                                argument_value = de->GetArgumentValue(i, 0);
                                str = m_labels->sys_vars->Find(argument_value);
                                m_labels->var_labels->AddLabel(argument_value, str, index);
                                de->SetArgumentStyle(i, STYLE_LABELED);
                                break;
            case OT_ABSOLUTE_ADDRESS:
            case OT_RELATIVE_ADDRESS:
                                argument_value = de->GetArgumentValue(i, m_disassembled_list->GetBaseAddress(index));
                                str = m_labels->sys_calls->Find(argument_value);
                                m_labels->prog_labels->AddLabel(argument_value, str, index);
                                de->SetArgumentStyle(i, STYLE_LABELED);
                                break;
            case OT_IO_ADDRESS:
                                argument_value = de->GetArgumentValue(i, 0);
                                str = m_labels->sys_io->Find(argument_value);
                                m_labels->io_labels->AddLabel(argument_value, str, index);
                                de->SetArgumentStyle(i, STYLE_LABELED);
                                break;
            case OT_NONE:
            case OT_DATA:
            case OT_OFFSET:
                                break;
        } // switch
    } // for
}



void Decoder::MSXCheckFunctionRegisters(DisassembledItem *de)
{
    switch(de->GetArgumentValue(0, 0))
    {
    case 5:
        if (m_registers.C->isLoaded())
        {
            wxString str = m_labels->sys_const->Find(m_registers.C->GetValue());
            if (!str.IsEmpty())
                m_labels->constant_labels->AddLabel(m_registers.C->GetValue(), str);
        }

        break;
    }
}





uint Decoder::MSXWeirdRST(DisassembledItem *de, DisassembledIndex dasm_position)
{
    uint offset, new_program_counter;
    Arguments args;

    offset = de->GetOffsetInFile();
    new_program_counter = 0;

    args = de->GetMnemonic()->GetArgument(0);
    switch (args.operand)
    {
        case OP_08:
                    //LogIt(wxString::Format("[0x%.4X] Rst 08h detected !", m_actualaddress));
                    break;

        case OP_30: // Inter slot call subroutine
                    // ID of Slot: DB slot_number
                    de = new DisassembledItem(de->GetProgram());
                    de->SetupDataItem(++offset);
                    m_disassembled_list->Insert(de, dasm_position++);

                    // address to be called: DW address
                    de = new DisassembledItem(de->GetProgram());
                    de->SetupDataItem(++offset);
                    de->SetLength(2);
                    de->SetArgumentStyle(0, STYLE_WORD_HEX);
                    m_disassembled_list->Insert(de, dasm_position);
                    new_program_counter = 3;    // 1 byte ID + 2 bytes Address
                    break;
    }
    return new_program_counter;
}



void Decoder::SetCartridgeLabels()
{
	uint address;

	address = m_program->GetCartridgeHeader()->init;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "INIT");

	address = m_program->GetCartridgeHeader()->statement;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "STATEMENT");

	address = m_program->GetCartridgeHeader()->device;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "DEVICE");

	address = m_program->GetCartridgeHeader()->text;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "TEXT");
}



void Decoder::FullDisassemble()
{
    uint i, f, item;
    DisassembledItem *de;

    if ((!m_program) || (!m_disassembled_list))
        return;

    m_labels->clearUserLabels();

    if (m_program->isCartridge())
        SetCartridgeLabels();

    f = m_program->GetSize();
    for (i = 0; i < f; i++)
    {
        de = new DisassembledItem(m_program);
        item = Decode(de, i);
        SetupArgumentLabels(de, item);
        m_registers.LoadRegister(de);
        i += MSXWeirdRST(de, f);
        i += (de->GetLength() - 1);
    }
}



void Decoder::DisassembleItems(RangeItems &dasm_range)
{
    uint 		i, item,
				dasm_last,
				disassembled_count,
				program_first,
				program_last;
    DisassembledItem	*de;

    if ((!m_program) || (!m_disassembled_list))
        return;

    dasm_last = dasm_range.Index + dasm_range.Count - 1;
    disassembled_count = m_disassembled_list->GetCount();

    if ((dasm_range.Index < disassembled_count) || (dasm_last < disassembled_count))
    {
        de = m_disassembled_list->GetData(dasm_range.Index);
        program_first = de->GetOffsetInFile();
        de = m_disassembled_list->GetData(dasm_range.Index + dasm_range.Count - 1);
        program_last = de->GetOffsetInFile() + de->GetLength();

        LogIt(wxString::Format("Deleting index = %d, count = %d.", dasm_range.Index, dasm_range.Count));
        m_disassembled_list->Del(dasm_range.Index, dasm_range.Count);
        disassembled_count = dasm_range.Index;
        dasm_range.Count = 0;
        for (i = program_first; i < program_last; i++)
        {
            de = new DisassembledItem(m_program);
            item = Decode(de, i, disassembled_count++);
            SetupArgumentLabels(de, item);
            //i += MSXWeirdRST(de, (dasm_last + 1));
            i += (de->GetLength() - 1);
            dasm_range.Count++;
        }
    }
}

