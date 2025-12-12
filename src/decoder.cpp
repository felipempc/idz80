/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module disassembles a program
 **************************************************************/

#include "decoder.h"
#include <wx/string.h>

 // MUST BE COMPLETELY REWRITED !!!!


const uint Decoder::OPCODE_NOT_FOUND;

Decoder::Decoder(ProjectBase *parent, const unsigned int program_index)
{
    m_disassembled_list = parent->m_disassembled_mgr->Index(program_index);
    m_program = parent->m_programs_mgr->Index(program_index);
    m_labels = parent->m_labels;
    m_mnemonic = parent->m_mnemonics;

    SetTextLog(parent->GetTextLog());
    ModuleName = "DECODER";

}





/*
 * Search for an opcode pattern (from Program)
 * in mndb.
 * TODO: WHY NOT RETURN DisassembledItem* ?
 */
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
        mnemonic = m_mnemonic->Item(ret); // TODO: remove -->>   GetData(ret); ???
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
        de->SetLength(1);
        de->SetFileOffset(prg_index);
        de->SetMnemonic(0);
        de->MarkAsData();
        ret = m_disassembled_list->Insert(de, dasm_position);
    }
    else
    {
        de->SetMnemonic(m_mnemonic->Item(mnc_item));
        de->SetLength(m_mnemonic->GetCount());
        de->MarkAsData(false);
        de->SetFileOffset(prg_index);
        if (de->GetMnemonic()->HasExplicitArguments()) {
            if (de->GetMnemonic()->GetArgumentCount() == 1) {

            }

        }
        if (de->CopyArguments(m_mnemonic))
			ret = process_->Disassembled->InsertDasm(de, dasm_position);
    }

    return ret;
}



/*
 *  Fill label list
 */
void Decoder::SetupArgumentLabels(DisassembledItem *de, DisassembledIndex index)
{
    enum ArgumentTypes	argtype;
    uint            argument;
    wxString		str;

	argtype = de->GetArgumentType(0);
	switch (argtype)
	{
		case ARG_VARIABLE:
                            argument = de->GetArgument(0, 0);
							str = m_labels->sys_vars->Find(argument);
							m_labels->var_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
		case ARG_ABS_ADDR:
		case ARG_REL_ADDR:
                            argument = de->GetArgument(0, process_->Disassembled->GetBaseAddress(index));
							str = m_labels->sys_calls->Find(argument);
							m_labels->prog_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
		case ARG_IO_ADDR:
                            argument = de->GetArgument(0, 0);
							str = m_labels->sys_io->Find(argument);
							m_labels->io_labels->AddLabel(argument, str, index);
							de->SetArgLabel();
							break;
        case ARG_NONE:
        case ARG_LITERAL:
        case ARG_OFFSET:
                            break;
	}
}









void Decoder::MSXCheckFunctionRegisters(DisassembledItem *de)
{
    switch(de->GetArgument(0, 0))
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

    offset = de->GetOffset();
    new_program_counter = 0;

    switch (de->GetInstructionDetail())
    {
        case II_RST_08:
                    //LogIt(wxString::Format("[0x%.4X] Rst 08h detected !", m_actualaddress));
                    break;

        case II_RST_30:
                    // ID of Slot
                    de = new DisassembledItem(process_->Program);
                    de->SetLength(1);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    process_->Disassembled->InsertDasm(de, dasm_position++);

                    // address to be called
                    de = new DisassembledItem(process_->Program);
                    de->SetLength(2);
                    de->SetOffset(++offset);
                    de->SetMnemonic(0);
                    de->SetType(et_Data);
                    de->SetStyleArgument(0, ast_wordhex);
                    process_->Disassembled->InsertDasm(de, dasm_position);
                    new_program_counter = 3;    // 1 byte ID + 2 bytes Address
                    break;
    }
    return new_program_counter;
}








void Decoder::SetCartridgeLabels()
{
	uint address;

	address = process_->Program->GetCartridgeHeader()->init;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "INIT");

	address = process_->Program->GetCartridgeHeader()->statement;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "STATEMENT");

	address = process_->Program->GetCartridgeHeader()->device;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "DEVICE");

	address = process_->Program->GetCartridgeHeader()->text;
	if (address != 0)
		m_labels->prog_labels->AddLabel(address, "TEXT");
}








void Decoder::FullDisassemble(LabelManager *parent)
{
    uint i, f, item;
    DisassembledItem *de;

    m_labels = parent;
    m_labels->ClearUserLabels();

    if (process_->Program->isCartridge())
        SetCartridgeLabels();

    f = process_->Program->GetSize();
    for (i = 0; i < f; i++)
    {
        de = new DisassembledItem(process_->Program);
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


    dasm_last = dasm_range.Index + dasm_range.Count - 1;
    disassembled_count = process_->Disassembled->GetCount();

    if ((dasm_range.Index < disassembled_count) || (dasm_last < disassembled_count))
    {
        de = process_->Disassembled->GetData(dasm_range.Index);
        program_first = de->GetOffset();
        de = process_->Disassembled->GetData(dasm_range.Index + dasm_range.Count - 1);
        program_last = de->GetOffset() + de->GetLength();

        LogIt(wxString::Format("Deleting index = %d, count = %d.", dasm_range.Index, dasm_range.Count));
        process_->Disassembled->DelDasm(dasm_range.Index, dasm_range.Count);
        disassembled_count = dasm_range.Index;
        dasm_range.Count = 0;
        for (i = program_first; i < program_last; i++)
        {
            de = new DisassembledItem(process_->Program);
            item = Decode(de, i, disassembled_count++);
            SetupArgumentLabels(de, item);
            //i += MSXWeirdRST(de, (dasm_last + 1));
            i += (de->GetLength() - 1);
            dasm_range.Count++;
        }
    }
}





