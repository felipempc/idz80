/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   05-08-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Open and fill the mnemonics database
 **************************************************************/



#include "mnemonicxmlfile.hpp"



// File Main TAGS
const wxString MnemonicXMLFile::MNEMONIC_FILE_STR = "MNEMONIC_LIST";
const wxString MnemonicXMLFile::MNEMONIC_GROUP_STR = "GROUPS";
const wxString MnemonicXMLFile::MNEMONIC_ITEM_STR = "ITEM";
const wxString MnemonicXMLFile::MNEMONIC_ARGUMENT_STR = "ARGUMENT";
const wxString MnemonicXMLFile::MNEMONIC_OPCODE_STR = "OPCODE";

//File Attributes
const wxString MnemonicXMLFile::ATTRIBUTE_VERSION_STR = "VERSION";
const wxString MnemonicXMLFile::ATTRIBUTE_PROCESSOR_STR = "PROCESSOR";

//File Values
const wxString MnemonicXMLFile::MNEMONIC_FILE_VERSION_STR = "0.1";
const wxString MnemonicXMLFile::MNEMONIC_PROCESSOR_STR = "Z80A";

// Group values
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

// Instruction attributes
const wxString MnemonicXMLFile::ATTRIBUTE_OPCODE_STR = "OPCODE";
const wxString MnemonicXMLFile::ATTRIBUTE_SIZE_STR = "SIZE";
const wxString MnemonicXMLFile::ATTRIBUTE_MNMONIC_STR = "STRING";
const wxString MnemonicXMLFile::ATTRIBUTE_COUNT_STR = "COUNT";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_POSITION = "OPCODE_POSITION";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_TYPE = "TYPE";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_VALUE = "RST";

// Instruction Characteristcs
const wxString MnemonicXMLFile::MNEMONIC_CONDICIONAL = "CONDICIONAL";
const wxString MnemonicXMLFile::MNEMONIC_EXPLICIT_ARGUMENT = "EXPLICIT_ARGUMENT";

// Operand attributes
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_DESTINATION_TYPE = "DESTINATION_TYPE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_DESTINATION_OPERAND = "DESTINATION_OPERAND";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_SOURCE_TYPE = "SOURCE_TYPE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_SOURCE_OPERAND = "SOURCE_OPERAND";

// Operand type values
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_ABSOLUTE = "ABSOLUTE_ADDRESS";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_RELATIVE = "RELATIVE_ADDRESS";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_IO = "IO_ADDRESS";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_OFFSET = "OFFSET";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_VARIABLE = "VARIABLE";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_DATA = "DATA";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_REGISTER = "REGISTER";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_ADDRESS = "REGISTER_ADDRESS";
const wxString MnemonicXMLFile::ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_OFFSET = "REGISTER_OFFSET";

// Operand values
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
const wxString MnemonicXMLFile::ATTRIBUTE_RST_00 = "0";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_08 = "0x8";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_10 = "0x10";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_18 = "0x18";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_20 = "0x20";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_28 = "0x28";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_30 = "0x30";
const wxString MnemonicXMLFile::ATTRIBUTE_RST_38 = "0x38";

// Mnemonic argument mark
const wxUniChar MnemonicXMLFile::ARGUMENT_MARK = '$';



MnemonicXMLFile::MnemonicXMLFile(MnemonicContainer *t_mnemonics, wxTextCtrl *t_log)
{
    m_xml_mnemonic = 0;
    m_mnemonics = t_mnemonics;
    m_log_text = t_log;
    m_statistics.numgroups = 0;
    m_statistics.numinstructions = 0;
}



/// @brief Opens the XML file and start the processing.
/// @param mnemonicfile The name of the XML file.
/// @return True if the file was opened.
bool MnemonicXMLFile::Open(const wxString &mnemonicfile)
{
    m_xml_mnemonic = new wxXmlDocument(mnemonicfile);
    bool ret;

    ret = m_xml_mnemonic->IsOk();
    if(ret)
    {
        ProcessFile();
        m_mnemonics->SetStatistics(m_statistics);

        #ifdef _IDZ80_DEBUG_
        LogIt(wxString::Format("Number of instructions: %d\n", m_statistics.numinstructions));
        LogIt(wxString::Format("Number of groups: %d\n", m_statistics.numgroups));
        LogIt("Mnemonics loaded!\n");
        #endif // _IDZ80_DEBUG_

    }
    else
    {
        m_log_text->AppendText("XML file failed to load !\n\n");
    }

    delete m_xml_mnemonic;
    m_xml_mnemonic = 0;
    return ret;
}




/// @brief Processes the XML file.
void MnemonicXMLFile::ProcessFile()
{
    NodeGroupList   group_list;
    //uint index;

    if (!FindGroups(group_list))
        return;

    for(wxXmlNode *group_item : group_list)
    {
        ProcessGroup(group_item);
        //#ifdef _IDZ80_DEBUG_
        //LogToFile(wxString::Format("<<< %d >>>  Processing group '%s'.", index, group_item->GetName()));
        //#endif // _IDZ80_DEBUG_
    }
}




/// @brief Processes each group of instruction
/// @param t_groupitem 
void MnemonicXMLFile::ProcessGroup(const wxXmlNode *t_groupitem)
{
    wxXmlNode *nextitem;
    Groups current_group;
    MnemonicItem *new_instruction = 0;
    bool allocate_new = true;
    int line;
    unsigned int group_instructions = 0;


    current_group = GetGroupFromStr(t_groupitem->GetName());
    if (current_group == GRP_NONE)
        return;
    nextitem = t_groupitem->GetChildren();

    while (nextitem)
    {
        if(allocate_new)
        {
            new_instruction = new MnemonicItem();
            new_instruction->SetGroup(current_group);
        }

        try
        {
            ProcessInstruction(nextitem, new_instruction);
            ProcessCharacteristcs(nextitem, new_instruction);
            ProcessArguments(nextitem, new_instruction);
            m_mnemonics->AddInstruction(new_instruction);
            m_statistics.numinstructions++;
            group_instructions++;
            allocate_new = true;
        }
        catch(XMLF_Exceptions e)
        {
            line = nextitem->GetLineNumber();
            PrintErrorMessages(e, line);
            new_instruction->Reset();
            allocate_new = false;
        }
        //#ifdef _IDZ80_DEBUG_
        //LogToFile("\n");
        //#endif // _IDZ80_DEBUG_

        nextitem = nextitem->GetNext();
    }

    if (group_instructions == 0)
        delete new_instruction;
}




/// @brief Processes an instruction.
/// @param instruction_node node of instructions section in XML file.
/// @param instruction pointer of MnemonicItem of the instruction.
void MnemonicXMLFile::ProcessInstruction(wxXmlNode *instruction_node, MnemonicItem * const instruction)
{
    #ifdef _IDZ80_DEBUG_
    wxString str_debug;
    int counter_debug;
    #endif // _IDZ80_DEBUG_

    wxString        opcode_str, temp_str;
    StringVector    mnemonicslices;
    wxXmlNode       *instruction_detail;


    if (!instruction_node->GetAttribute(ATTRIBUTE_MNMONIC_STR, &opcode_str))
        throw XMLFE_ATTR_MNMONIC_STR_NOT_FOUND;

    ParseMnemonicString(opcode_str, mnemonicslices);
    instruction->SetMnemonicStrings(mnemonicslices);
/*
    #ifdef _IDZ80_DEBUG_
    str_debug.Clear();
    for(counter_debug = 0; counter_debug < mnemonicslices.GetCount(); counter_debug++)
    {
        str_debug << mnemonicslices[counter_debug];
        if (counter_debug != (mnemonicslices.GetCount() - 1))
            str_debug << "*";
    }
    LogIt(wxString::Format("[%.3d] Item '%s' = [%s].", m_statistics.numinstructions, opcode_str, str_debug));
    #endif // _IDZ80_DEBUG_
*/

    instruction_detail = GetInInstructionNode(instruction_node, MNEMONIC_OPCODE_STR);

    if (!instruction_detail)
        throw XMLFE_ITEM_OPCODE_NOT_FOUND;

    if(!instruction_detail->GetAttribute(ATTRIBUTE_OPCODE_STR, &opcode_str))
        throw XMLFE_ATTR_OPCODE_NOT_FOUND;

    if(!instruction_detail->GetAttribute(ATTRIBUTE_SIZE_STR, &temp_str))
        throw XMLFE_ATTR_SIZE_NOT_FOUND;

    ParseOpcodeString(instruction, opcode_str, temp_str);
    instruction->SetMnemonicSignature(CalculateSignature(instruction->GetByteCode()));
/*
    #ifdef _IDZ80_DEBUG_
    str_debug.Clear();
    for(counter_debug = 0; counter_debug < instruction->GetByteCodeSize(); counter_debug++)
    {
        str_debug << wxString::Format("%.2X ", instruction->GetByteCode(counter_debug));
    }
    str_debug = str_debug.Trim();

    LogIt(wxString::Format("[%.3d] Bytecode<%d> = [%s].", m_statistics.numinstructions, instruction->GetByteCodeSize(), str_debug));
    LogIt(wxString::Format("[%.3d] Signature = [%X].", m_statistics.numinstructions, instruction->GetMnemonicSignature()));
    #endif // _IDZ80_DEBUG_
*/

}




/// @brief Fills instructions' explicit arguments and its attributes, if it has.
/// @param instruction_node node of instructions section in XML file.
/// @param instruction pointer of MnemonicItem of the instruction.
void MnemonicXMLFile::ProcessArguments(wxXmlNode *instruction_node, MnemonicItem *instruction)
{
    wxString value_str;
    long value_num = 0;
    byte    arg_size = 0,
            arg_count = 0,
            arg_pos = 0;
    OperandType operand_type;

    instruction_node = GetInInstructionNode(instruction_node, MNEMONIC_ARGUMENT_STR);

    if (!instruction_node)
        return;

    if (instruction->HasExplicitArguments())
    {
        if (instruction_node->GetAttribute(ATTRIBUTE_SIZE_STR, &value_str))
        {
            if (value_str.ToLong(&value_num))
                arg_size = static_cast<byte>(value_num);
        }
        else
            throw XMLFE_ARG_BASIC_SIZE_NOT_FOUND;

        if (instruction_node->GetAttribute(ATTRIBUTE_COUNT_STR, &value_str))
        {
            if (value_str.ToLong(&value_num))
                arg_count = static_cast<byte>(value_num);
        }
        else
            throw XMLFE_ARG_BASIC_COUNT_NOT_FOUND;

        if (instruction_node->GetAttribute(ATTRIBUTE_ARGUMENT_POSITION, &value_str))
        {
            if (value_str.ToLong(&value_num))
                arg_pos = static_cast<byte>(value_num);
        }
        else
            throw XMLFE_ARG_BASIC_POSITION_NOT_FOUND;

        if ((arg_count * arg_pos * arg_size) != 0)
            instruction->ConfigArguments(arg_count, arg_size, arg_pos);
        else
            throw XMLFE_ARG_BASIC_LOAD_FAILED;
    }

    if (instruction_node->GetAttribute(ATTRIBUTE_ARGUMENT_DESTINATION_TYPE, &value_str))
    {
        operand_type = GetOperandTypeFromStr(value_str);
        if (instruction_node->GetAttribute(ATTRIBUTE_ARGUMENT_DESTINATION_OPERAND, &value_str))
            instruction->SetDestinationArgument(operand_type, GetOperandFromStr(value_str));
    }

    if (instruction_node->GetAttribute(ATTRIBUTE_ARGUMENT_SOURCE_TYPE, &value_str))
    {
        operand_type = GetOperandTypeFromStr(value_str);
        if (instruction_node->GetAttribute(ATTRIBUTE_ARGUMENT_SOURCE_OPERAND, &value_str))
            instruction->SetSourceArgument(operand_type, GetOperandFromStr(value_str));
    }

    if (instruction_node->GetAttribute(ATTRIBUTE_RST_VALUE, &value_str))
        instruction->SetSourceArgument(OT_ABSOLUTE_ADDRESS, GetOperandFromStr(value_str));

}




/// @brief Sets characteristics of the instruction, like if it has explicit arguments or it's a conditional branch.
/// @param instruction_node node of instructions section in XML file.
/// @param instruction pointer of MnemonicItem of the instruction.
void MnemonicXMLFile::ProcessCharacteristcs(wxXmlNode *instruction_node, MnemonicItem *instruction)
{
    wxXmlNode *instnode = 0;
    instnode = GetInInstructionNode(instruction_node, MNEMONIC_CONDICIONAL);
    if (instnode)
    {
        instruction->SetConditionalBranch(true);
    }

    instnode = GetInInstructionNode(instruction_node, MNEMONIC_EXPLICIT_ARGUMENT);
    if (instnode)
    {
        instruction->SetExplicitArguments(true);
    }

}




/// @brief Scans all the XML file to find the groups of instructions
/// @param grouplist stores the nodes of the found groups.
/// @return true if it founds the groups.
bool MnemonicXMLFile::FindGroups(NodeGroupList &grouplist)
{
    wxXmlNode *group_item;

    m_statistics.numgroups = 0;
    grouplist.clear();

    group_item = m_xml_mnemonic->GetRoot()->GetChildren();
    group_item = group_item->GetChildren();

    while (group_item)
    {
        grouplist.push_back(group_item);
        m_statistics.numgroups++;

        group_item = group_item->GetNext();
    }

    #ifdef _IDZ80_DEBUG_
    LogIt(wxString::Format("Total groups = %d.\n", m_statistics.numgroups));
    #endif

    return (m_statistics.numgroups > 0);
}



/// @brief Takes groupstr string and return the group code
/// @param groupstr the string name of the group.
/// @return the group code. GRP_NONE if not found.
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




/// @brief Takes ot_str string and return the operand type code
/// @param ot_str the string name of the operand type.
/// @return the operand type code. OT_NONE if not found.
OperandType MnemonicXMLFile::GetOperandTypeFromStr(const wxString &ot_str)
{
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_ABSOLUTE))
        return OT_ABSOLUTE_ADDRESS;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_RELATIVE))
        return OT_RELATIVE_ADDRESS;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_IO))
        return OT_IO_ADDRESS;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_OFFSET))
        return OT_OFFSET;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_VARIABLE))
        return OT_VARIABLE;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_DATA))
        return OT_DATA;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_REGISTER))
        return OT_REGISTER;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_ADDRESS))
        return OT_REGISTER_ADDRESS;
    if (ot_str.IsSameAs(ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_OFFSET))
        return OT_REGISTER_PLUS_OFFSET;

    return OT_NONE;
}




/// @brief Takes op_str string and return the operand code
/// @param op_str the string name of the operand.
/// @return the operand code. OP_NONE if not found.
Operands MnemonicXMLFile::GetOperandFromStr(const wxString &op_str)
{
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_A))
        return OP_A;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_B))
        return OP_B;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_C))
        return OP_C;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_D))
        return OP_D;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_E))
        return OP_E;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_F))
        return OP_F;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_H))
        return OP_H;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_L))
        return OP_L;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_I))
        return OP_I;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_R))
        return OP_R;

    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_BC))
        return OP_BC;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_DE))
        return OP_DE;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_HL))
        return OP_HL;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_IX))
        return OP_IX;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_IY))
        return OP_IY;
    if (op_str.IsSameAs(ATTRIBUTE_REGISTER_SP))
        return OP_SP;
    if (op_str.IsSameAs(ATTRIBUTE_VALUE_LITERAL))
        return OP_LITERAL;
    if (op_str.IsSameAs(ATTRIBUTE_RST_00))
        return OP_00;
    if (op_str.IsSameAs(ATTRIBUTE_RST_08))
        return OP_08;
    if (op_str.IsSameAs(ATTRIBUTE_RST_10))
        return OP_10;
    if (op_str.IsSameAs(ATTRIBUTE_RST_18))
        return OP_18;
    if (op_str.IsSameAs(ATTRIBUTE_RST_20))
        return OP_20;
    if (op_str.IsSameAs(ATTRIBUTE_RST_28))
        return OP_28;
    if (op_str.IsSameAs(ATTRIBUTE_RST_30))
        return OP_30;
    if (op_str.IsSameAs(ATTRIBUTE_RST_38))
        return OP_38;

    return OP_NONE;
}



/// @brief Separates the string of mnemonics into a string array.
/// @param rawstr The mnemonic string
/// @param liststr A vector of strings with the sliced mnemonic.
void MnemonicXMLFile::ParseMnemonicString(const wxString &rawstr, StringVector &liststr)
{
    int first_arg_pos, last_arg_pos;
    wxString strtemp;

    first_arg_pos = rawstr.Find(ARGUMENT_MARK, false);
    last_arg_pos = rawstr.Find(ARGUMENT_MARK, true);
    if ((first_arg_pos == last_arg_pos) && (first_arg_pos != wxNOT_FOUND))
    {
        liststr.push_back(rawstr.Left(first_arg_pos));
        strtemp = rawstr.Right(rawstr.Len() - first_arg_pos - 1);
        liststr.push_back(strtemp);
    }
    else
        if ((first_arg_pos != wxNOT_FOUND) || (last_arg_pos != wxNOT_FOUND))
        {
            liststr.push_back(rawstr.Left(first_arg_pos));
            strtemp = rawstr.Mid(first_arg_pos + 1, (last_arg_pos - first_arg_pos - 1));
            if (!strtemp.IsEmpty())
                liststr.push_back(strtemp);
        }
        else
            liststr.push_back(rawstr);

}




/// @brief Read the OPCODE field and convert to bytecode
/// @param instruction pointer to MnemonicItem.
/// @param bytecodestr the OPCODE field from XML file.
/// @param sizestr the size of the OPCODE.
void MnemonicXMLFile::ParseOpcodeString(MnemonicItem *instruction, const wxString &bytecodestr, const wxString &sizestr)
{
    long converted;
    int num_spaces;
    byte informed_size;
    bool validated;
    ByteCode byte_code;
    wxString hex_str;

    if (!sizestr.ToLong(&converted))
        throw XMLFE_ATTR_OPCODE_SIZE_ERROR_CONV;

    informed_size = static_cast<byte>(converted);

    num_spaces = bytecodestr.Freq(' ');
    validated = (((bytecodestr.Len() - num_spaces) / 2) == informed_size);
    if (!validated)
        throw XMLFE_ATTR_OPCODE_SIZE_VALIDATE_ERROR;

    //#ifdef _IDZ80_DEBUG_
    //LogToFile(wxString::Format("[%.3d] Size validated [%d].", m_statistics.numinstructions, informed_size));
    //#endif // _IDZ80_DEBUG_

    for(uint counter = 0; counter < informed_size; ++counter)
    {
        hex_str = bytecodestr.Mid(counter * 3, 2);
        if (!hex_str.ToLong(&converted, 16))
            throw XMLFE_ATTR_OPCODE_CONVERSION_ERROR;

        byte_code[counter] = static_cast<byte>(converted);
    }
    instruction->SetByteCode(byte_code, informed_size);


    //#ifdef _IDZ80_DEBUG_
    //LogToFile(wxString::Format("[%.3d] Bytecode size = %d [%d].", m_statistics.numinstructions, informed_size, instruction->GetByteCodeSize()));
    //#endif // _IDZ80_DEBUG_
}



/// @brief Converts a vector of bytes called ByteCode into a value, now called signature, to be faster and easy to compare.
/// @param bytecode The vector of bytes.
/// @return the signature
unsigned int MnemonicXMLFile::CalculateSignature(const ByteCode &bytecode)
{
    unsigned int signature = 0;
    unsigned int countdown = MAX_OPCODE_SIZE;
    unsigned int multiplier = 1;

    while(countdown-- > 0)
    {
        signature += bytecode[countdown] * multiplier;
        multiplier *= 0x100;
    }
    return signature;
}




/// @brief Searches for an attribute "node_str" in an instruction_node
/// @param instruction_node The instruction node
/// @param node_str The attribute
/// @return The node. Zero if not found.
wxXmlNode *MnemonicXMLFile::GetInInstructionNode(wxXmlNode *instruction_node, const wxString &node_str)
{
    wxXmlNode *actual_param;
    actual_param = instruction_node->GetChildren();
    while (actual_param)
    {
        if (actual_param->GetName().IsSameAs(node_str))
            return actual_param;
        actual_param = actual_param->GetNext();
    }
    return 0;
}




/// @brief Prints the error messages of exceptions.
/// @param t_exception The exception
/// @param t_line The line of the error.
void MnemonicXMLFile::PrintErrorMessages(XMLF_Exceptions t_exception, int t_line)
{
    switch(t_exception)
    {
        case XMLFE_ITEM_OPCODE_NOT_FOUND:
            LogIt(wxString::Format("[%d] Can't find item OPCODE !\n", t_line));
            break;

        case XMLFE_ATTR_OPCODE_NOT_FOUND:
            LogIt(wxString::Format("[%d] Can't find attribute OPCODE !\n", t_line));
            break;

        case XMLFE_ATTR_OPCODE_SIZE_ERROR_CONV:
            LogIt(wxString::Format("[%d] Can't convert opcode SIZE !\n", t_line));
            break;

        case XMLFE_ATTR_OPCODE_SIZE_VALIDATE_ERROR:
            LogIt(wxString::Format("[%d] Opcode size validation error !\n", t_line));
            break;

        case XMLFE_ATTR_OPCODE_CONVERSION_ERROR:
            LogIt(wxString::Format("[%d] Error while converting OPCODE sequence !\n", t_line));
            break;

        case XMLFE_ATTR_SIZE_NOT_FOUND:
            LogIt(wxString::Format("[%d] Can't find attribute SIZE !\n", t_line));
            break;

        case XMLFE_ATTR_MNMONIC_STR_NOT_FOUND:
            LogIt(wxString::Format("[%d] Can't find attribute mnemonic STRING !\n", t_line));
            break;

        case XMLFE_ARG_BASIC_SIZE_NOT_FOUND:
            LogIt(wxString::Format("[%d] Can't find attribute argument SIZE !\n", t_line));
            break;

        case XMLFE_ARG_BASIC_COUNT_NOT_FOUND:
            LogIt(wxString::Format("[%d] Can't find attribute argument COUNT !\n", t_line));
            break;

        case XMLFE_ARG_BASIC_POSITION_NOT_FOUND:
            LogIt(wxString::Format("[%d] Can't find attribute argument POSITION !\n", t_line));
            break;

        case XMLFE_ARG_BASIC_LOAD_FAILED:
            LogIt(wxString::Format("[%d] Argument basic attributes failed to load !\n", t_line));
            break;

        default:
            LogIt(wxString::Format("[%d] Unknow error !\n", t_line));
    }
}

/*
#ifdef _IDZ80_DEBUG_
void MnemonicXMLFile::LogToFile(const wxString textout)
{
    if (debug_file_open)
        debug_file.AddLine(textout);
}
#endif
*/

void MnemonicXMLFile::LogIt(const wxString textout)
{
    m_log_text->AppendText(textout);
}
