/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   05-08-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Open and fill the mnemonics database
 **************************************************************/



#include "mnemonicxmlfile.h"




const wxString MnemonicXMLFile::MNEMONIC_FILE_STR = "MNEMONIC_LIST";
const wxString MnemonicXMLFile::MNEMONIC_FILE_VERSION_STR = "0.1";
const wxString MnemonicXMLFile::MNEMONIC_PROCESSOR_STR = "Z80A";

const wxString MnemonicXMLFile::MNEMONIC_GROUP_STR = "GROUPS";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_JUMP_STR = "JUMP";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_CALL_STR = "CALL";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_LOAD8BIT_STR = "LOAD8BIT";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_LOAD16BIT_STR = "LOAD16BIT";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_RETURN_STR = "RETURN";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_RST_STR = "RST";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_ARITHMETIC8BIT_STR = "ARITHMETIC8BIT";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_ARITHMETIC16BIT_STR = "ARITHMETIC16BIT";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_GPARITHMETRIC_STR = "GPARITHMETRIC";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_EXCHANGE_STR = "EXCHANGE";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_BLOCKTRANSFER_STR = "BLOCKTRANSFER";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_BLOCKSEARCH_STR = "BLOCKSEARCH";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_LOGICAL_STR = "LOGICAL";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_CPUCONTROL_STR = "CPUCONTROL";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_ROTATESHIFT_STR = "ROTATESHIFT";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_BITSET_STR = "BITSET";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_BITRESET_STR = "BITRESET";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_BITTEST_STR = "BITTEST";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_INPUT_STR = "INPUT";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_OUTPUT_STR = "OUTPUT";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_STACK_STR = "STACK";

const wxString MnemonicXMLFile::MNEMONIC_ITEM_STR = "ITEM";
const wxString MnemonicXMLFile::MNEMONIC_ARGUMENT_STR = "ARGUMENT";
const wxString MnemonicXMLFile::MNEMONIC_OPCODE_STR = "OPCODE";
const wxString MnemonicXMLFile::MNEMONIC_CONDICIONAL = "CONDICIONAL";

const wxString MnemonicXMLFile::ATTRIBUTE_VERSION_STR = "VERSION";
const wxString MnemonicXMLFile::ATTRIBUTE_ITEM_STR = "OPCODE";
const wxString MnemonicXMLFile::ATTRIBUTE_OPCODE_STR = "OPCODE";
const wxString MnemonicXMLFile::ATTRIBUTE_SIZE_STR = "SIZE";
const wxString MnemonicXMLFile::ATTRIBUTE_STRING_STR = "STRING";
const wxString MnemonicXMLFile::ATTRIBUTE_COUNT_STR = "COUNT";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_POSITION = "OPCODE_POSITION";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_TYPE = "TYPE";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_VALUE = "RST";

const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_DESTINATION_TYPE = "DESTINATION_TYPE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_DESTINATION_OPERAND = "DESTINATION_OPERAND";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_SOURCE_TYPE = "SOURCE_TYPE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_SOURCE_OPERAND = "SOURCE_OPERAND";

const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_ABSOLUTE = "ABSOLUTE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_RELATIVE = "RELATIVE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_IO = "IO";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_OFFSET = "OFFSET";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_VARIABLE = "VARIABLE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_DATA = "DATA";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_REGISTER = "REGISTER";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_ADDRESS = "REGISTER_ADDRESS";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_OFFSET = "REGISTER_OFFSET";

const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_A = "A";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_B = "B";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_C = "C";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_D = "D";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_E = "E";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_F = "F";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_H = "H";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_L = "L";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_I = "I";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_R = "R";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_BC = "BC";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_DE = "DE";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_HL = "HL";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_IX = "IX";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_IY = "IY";
const wxString MnemonicXMLFile::ATTRIBUTE_REGISTER_SP = "SP";
const wxString MnemonicXMLFile::ATTRIBUTE_VALUE_LITERAL = "LITERAL";


const wxUniChar MnemonicXMLFile::ARGUMENT_MARK = '$';



MnemonicXMLFile::MnemonicXMLFile(MnemonicContainer *mnm, wxTextCtrl *_log)
{
    xml_mnemonic_ = 0;
    mnemonics_ = mnm;
    log_ = _log;
}

MnemonicXMLFile::~MnemonicXMLFile()
{
    //dtor
}





bool MnemonicXMLFile::Open(const wxString &mnemonicfile)
{
    xml_mnemonic_ = new wxXmlDocument(mnemonicfile);
    bool ret;

    ret = xml_mnemonic_->IsOk();
    if(ret)
    {
        ProcessFile();
        mnemonics_->SetStatistics(statistics_);

        #ifdef _IDZ80_DEBUG_
        log_->AppendText("Open file completed !\n");
        #endif // _IDZ80_DEBUG_

    }
    else
    {
        log_->AppendText("XML file failed to load !\n\n");
    }


    delete xml_mnemonic_;
    xml_mnemonic_ = 0;
    return ret;
}




void MnemonicXMLFile::ProcessFile()
{
    NodeGroupList   group_list;
    wxXmlNode *group_item;
    uint index;

    if (!FindGroups(group_list))
        return;


    for(index = 0; index < group_list.size(); index++)
    {
        group_item = group_list[index];

        #ifdef _IDZ80_DEBUG_
        log_->AppendText(wxString::Format("%d: Processing group '%s'.\n", index, group_item->GetName()));
        #endif // _IDZ80_DEBUG_

        ProcessGroup(group_item);
    }
}




void MnemonicXMLFile::ProcessGroup(const wxXmlNode *groupitem)
{
    wxXmlNode *nextitem;
    Groups current_group;
    MnemonicItem *new_instruction;


    current_group = GetGroupFromStr(groupitem->GetName());
    if (current_group == GRP_NONE)
        throw XMLFE_GROUP_NOT_FOUND;
    nextitem = groupitem->GetChildren();

    while (nextitem)
    {
        new_instruction = ProcessInstruction(nextitem, current_group);
        if (new_instruction)
        {
            ProcessArguments(nextitem, new_instruction);
            mnemonics_->AddInstruction(new_instruction);
            statistics_.numinstructions++;
        }
        nextitem = nextitem->GetNext();
    }
}




MnemonicItem *MnemonicXMLFile::ProcessInstruction(wxXmlNode *instruction_node, const Groups currentgroup)
{
    #ifdef _IDZ80_DEBUG_
    wxString str_debug;
    #endif // _IDZ80_DEBUG_

    wxString opcode_str, temp_str;
    wxArrayString mnemonicslices;
    MnemonicItem *mnemonic_item;
    wxXmlNode *instruction_detail;
    bool success = false;
    int counter;

    mnemonic_item = static_cast<MnemonicItem *>(0);

    if (instruction_node->GetAttribute(ATTRIBUTE_STRING_STR, &opcode_str))
    {
        ParseMnemonicString(opcode_str, mnemonicslices);

        #ifdef _IDZ80_DEBUG_
        str_debug.Clear();
        for(counter = 0; counter < mnemonicslices.GetCount(); counter++)
        {
            str_debug << mnemonicslices[counter];
            if (counter != (mnemonicslices.GetCount() - 1))
                str_debug << "*";
        }

        log_->AppendText(wxString::Format("Item '%s' = [%s]\n", opcode_str, str_debug));
        #endif // _IDZ80_DEBUG_

        instruction_detail = GetInInstructionNode(instruction_node, MNEMONIC_OPCODE_STR);

        if (instruction_detail)
        {
            mnemonic_item = new MnemonicItem();
            mnemonic_item->SetGroup(currentgroup);
            mnemonic_item->SetMnemonicString(mnemonicslices);

            success = instruction_detail->GetAttribute(ATTRIBUTE_OPCODE_STR, &opcode_str);
            if (!success)
            {
                log_->AppendText("Failed OPCODE !\n");
                delete mnemonic_item;
                return static_cast<MnemonicItem *>(0);
            }

            success = instruction_detail->GetAttribute(ATTRIBUTE_SIZE_STR, &temp_str);
            if (!success)
            {
                log_->AppendText("Failed SIZE !\n");
                delete mnemonic_item;
                return static_cast<MnemonicItem *>(0);
            }

            success = ParseOpcodeString(mnemonic_item, opcode_str, temp_str);
            if (!success)
            {
                log_->AppendText("Failed to process opcode string !\n");
                delete mnemonic_item;
                return static_cast<MnemonicItem *>(0);
            }
            #ifdef _IDZ80_DEBUG_
            str_debug.Clear();
            for(counter = 0; counter < mnemonic_item->GetByteCodeSize(); counter++)
            {
                str_debug << wxString::Format("%.2X ", mnemonic_item->GetByteCode(counter));
            }
            str_debug = str_debug.Trim();

            log_->AppendText(wxString::Format("Bytecode<%d> = [%s]\n", mnemonic_item->GetByteCodeSize(), str_debug));
            #endif // _IDZ80_DEBUG_
        }
    }

    return mnemonic_item;
}




void MnemonicXMLFile::ProcessArguments(wxXmlNode *instruction_node, MnemonicItem *intruction)
{

}




bool MnemonicXMLFile::FindGroups(NodeGroupList &grouplist)
{
    wxXmlNode *group_item;

    statistics_.numgroups = 0;
    grouplist.clear();

    group_item = xml_mnemonic_->GetRoot()->GetChildren();
    group_item = group_item->GetChildren();

    while (group_item)
    {
        grouplist.push_back(group_item);
        statistics_.numgroups++;

        group_item = group_item->GetNext();
    }

    #ifdef _IDZ80_DEBUG_
    log_->AppendText(wxString::Format("Total groups = %d\n", statistics_.numgroups));
    #endif

    return (statistics_.numgroups > 0);
}




Groups MnemonicXMLFile::GetGroupFromStr(const wxString &groupstr)
{
    if (groupstr.IsSameAs(MNEMONIC_GROUP_JUMP_STR))
        return GRP_JUMP;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_LOAD8BIT_STR))
        return GRP_LOAD_8BIT;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_LOAD16BIT_STR))
        return GRP_LOAD_16BIT;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_CALL_STR))
        return GRP_CALL;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_RETURN_STR))
        return GRP_RETURN;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_RST_STR))
        return GRP_RST;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_ARITHMETIC8BIT_STR))
        return GRP_ARITHMETIC_8BIT;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_ARITHMETIC16BIT_STR))
        return GRP_ARITHMETIC_16BIT;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_GPARITHMETRIC_STR))
        return GRP_GP_ARITHMETIC;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_EXCHANGE_STR))
        return GRP_EXCHANGE;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_BLOCKTRANSFER_STR))
        return GRP_BLOCK_TRANSFER;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_BLOCKSEARCH_STR))
        return GRP_BLOCK_SEARCH;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_LOGICAL_STR))
        return GRP_LOGICAL;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_CPUCONTROL_STR))
        return GRP_CPU_CONTROL;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_ROTATESHIFT_STR))
        return GRP_ROTATE_SHIFT;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_BITSET_STR))
        return GRP_BIT_SET;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_BITRESET_STR))
        return GRP_BIT_RESET;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_BITTEST_STR))
        return GRP_BIT_TEST;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_INPUT_STR))
        return GRP_INPUT;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_OUTPUT_STR))
        return GRP_OUTPUT;

    if (groupstr.IsSameAs(MNEMONIC_GROUP_STACK_STR))
        return GRP_STACK;

    return GRP_NONE;
}





void MnemonicXMLFile::ParseMnemonicString(const wxString &rawstr, wxArrayString &liststr)
{
    int first_arg_pos, last_arg_pos;
    wxString strtemp;

    first_arg_pos = rawstr.Find(ARGUMENT_MARK, false);
    last_arg_pos = rawstr.Find(ARGUMENT_MARK, true);
    if ((first_arg_pos == last_arg_pos) && (first_arg_pos != wxNOT_FOUND))
    {
        liststr.Add(rawstr.Left(first_arg_pos));
        strtemp = rawstr.Right(rawstr.Len() - first_arg_pos - 1);
        liststr.Add(strtemp);
    }
    else
        if ((first_arg_pos != wxNOT_FOUND) || (last_arg_pos != wxNOT_FOUND))
        {
            liststr.Add(rawstr.Left(first_arg_pos));
            strtemp = rawstr.Mid(first_arg_pos + 1, (last_arg_pos - first_arg_pos - 1));
            if (!strtemp.IsEmpty())
                liststr.Add(strtemp);
        }
        else
            liststr.Add(rawstr);

}




bool MnemonicXMLFile::ParseOpcodeString(MnemonicItem *instruction, const wxString &bytecodestr, const wxString &sizestr)
{
    long converted;
    int num_spaces;
    byte informed_size;
    bool validated;
    ByteCode byte_code;
    wxString hex_str;

    if (!sizestr.ToLong(&converted))
        return false;

    informed_size = static_cast<byte>(converted);

    num_spaces = bytecodestr.Freq(' ');
    validated = (((bytecodestr.Len() - num_spaces) / 2) == informed_size);
    if (!validated)
    {
        #ifdef _IDZ80_DEBUG_
        log_->AppendText("Size FAILED validation.\n");
        #endif // _IDZ80_DEBUG_
        return false;
    }
    #ifdef _IDZ80_DEBUG_
    else
        log_->AppendText(wxString::Format("Size validated [%d].\n", informed_size));
    #endif // _IDZ80_DEBUG_

    for(uint counter = 0; counter < informed_size; counter++)
    {
        hex_str = bytecodestr.Mid(counter * 3, 2);
        if (!hex_str.ToLong(&converted, 16))
            return false;
        byte_code[counter] = static_cast<byte>(converted);
    }
    instruction->SetByteCode(byte_code, informed_size);
    log_->AppendText(wxString::Format("Bytecode size = %d [%d]\n", informed_size, instruction->GetByteCodeSize()));
    return true;
}




wxXmlNode *MnemonicXMLFile::GetInInstructionNode(wxXmlNode *intruction_node, const wxString &node_str)
{
    wxXmlNode *actual_param;
    actual_param = intruction_node->GetChildren();
    while (actual_param)
    {
        if (actual_param->GetName().IsSameAs(node_str))
            return actual_param;
        actual_param = actual_param->GetNext();
    }
    return 0;
}


