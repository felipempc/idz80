/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   05-08-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Open and fill the mnemonics database
 **************************************************************/

#ifndef MNEMONICXMLFILE_H
#define MNEMONICXMLFILE_H

// ****** DEBUG FLAG ******
#ifndef _IDZ80_DEBUG_
//#define _IDZ80_DEBUG_
#endif
// ************************

#include <wx/textctrl.h>
#include <wx/xml/xml.h>

#ifdef _IDZ80_DEBUG_
#include <wx/textfile.h>
#endif

#include "mnemonic_container.hpp"


enum XMLF_Exceptions
{
    XMLFE_ATTR_OPCODE_NOT_FOUND,
    XMLFE_ATTR_OPCODE_SIZE_ERROR_CONV,
    XMLFE_ATTR_OPCODE_SIZE_VALIDATE_ERROR,
    XMLFE_ATTR_OPCODE_CONVERSION_ERROR,
    XMLFE_ITEM_OPCODE_NOT_FOUND,
    XMLFE_ATTR_SIZE_NOT_FOUND,
    XMLFE_ATTR_MNMONIC_STR_NOT_FOUND,
    XMLFE_ARG_BASIC_SIZE_NOT_FOUND,
    XMLFE_ARG_BASIC_COUNT_NOT_FOUND,
    XMLFE_ARG_BASIC_POSITION_NOT_FOUND,
    XMLFE_ARG_BASIC_LOAD_FAILED
};

typedef std::vector<wxXmlNode *> NodeGroupList;


class MnemonicXMLFile
{
    public:
        MnemonicXMLFile(MnemonicContainer *t_mnemonics, wxTextCtrl *t_log);

        bool Open(const wxString &mnemonicfile);

    protected:
        static const wxString MNEMONIC_FILE_VERSION_STR;
        static const wxString MNEMONIC_FILE_STR;
        static const wxString MNEMONIC_PROCESSOR_STR;
        static const wxString MNEMONIC_GROUP_STR;
        static const wxString MNEMONIC_GROUP_JUMP_STR;
        static const wxString MNEMONIC_GROUP_LOAD8BIT_STR;
        static const wxString MNEMONIC_GROUP_LOAD16BIT_STR;
        static const wxString MNEMONIC_GROUP_CALL_STR;
        static const wxString MNEMONIC_GROUP_RETURN_STR;
        static const wxString MNEMONIC_GROUP_RST_STR;
        static const wxString MNEMONIC_GROUP_ARITHMETIC8BIT_STR;
        static const wxString MNEMONIC_GROUP_ARITHMETIC16BIT_STR;
        static const wxString MNEMONIC_GROUP_GPARITHMETRIC_STR;
        static const wxString MNEMONIC_GROUP_EXCHANGE_STR;
        static const wxString MNEMONIC_GROUP_BLOCKTRANSFER_STR;
        static const wxString MNEMONIC_GROUP_BLOCKSEARCH_STR;
        static const wxString MNEMONIC_GROUP_LOGICAL_STR;
        static const wxString MNEMONIC_GROUP_CPUCONTROL_STR;
        static const wxString MNEMONIC_GROUP_ROTATESHIFT_STR;
        static const wxString MNEMONIC_GROUP_BITSET_STR;
        static const wxString MNEMONIC_GROUP_BITRESET_STR;
        static const wxString MNEMONIC_GROUP_BITTEST_STR;
        static const wxString MNEMONIC_GROUP_INPUT_STR;
        static const wxString MNEMONIC_GROUP_OUTPUT_STR;
        static const wxString MNEMONIC_GROUP_STACK_STR;

        static const wxString MNEMONIC_ITEM_STR;
        static const wxString MNEMONIC_OPCODE_STR;
        static const wxString MNEMONIC_ARGUMENT_STR;
        static const wxString MNEMONIC_CONDICIONAL;
        static const wxString MNEMONIC_EXPLICIT_ARGUMENT;

        static const wxString ATTRIBUTE_VERSION_STR;
        static const wxString ATTRIBUTE_PROCESSOR_STR;
        static const wxString ATTRIBUTE_SIZE_STR;
        static const wxString ATTRIBUTE_MNMONIC_STR;
        static const wxString ATTRIBUTE_OPCODE_STR;
        static const wxString ATTRIBUTE_COUNT_STR;
        static const wxString ATTRIBUTE_ARGUMENT_POSITION;
        static const wxString ATTRIBUTE_ARGUMENT_TYPE;
        static const wxString ATTRIBUTE_RST_VALUE;
        static const wxString ATTRIBUTE_ARGUMENT_SOURCE_TYPE;
        static const wxString ATTRIBUTE_ARGUMENT_SOURCE_OPERAND;
        static const wxString ATTRIBUTE_ARGUMENT_DESTINATION_TYPE;
        static const wxString ATTRIBUTE_ARGUMENT_DESTINATION_OPERAND;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_REGISTER;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_ADDRESS;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_REGISTER_OFFSET;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_DATA;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_OFFSET;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_VARIABLE;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_ABSOLUTE;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_RELATIVE;
        static const wxString ATTRIBUTE_ARGUMENT_OPERAND_IO;
        static const wxString ATTRIBUTE_REGISTER_A;
        static const wxString ATTRIBUTE_REGISTER_B;
        static const wxString ATTRIBUTE_REGISTER_C;
        static const wxString ATTRIBUTE_REGISTER_D;
        static const wxString ATTRIBUTE_REGISTER_E;
        static const wxString ATTRIBUTE_REGISTER_F;
        static const wxString ATTRIBUTE_REGISTER_H;
        static const wxString ATTRIBUTE_REGISTER_L;
        static const wxString ATTRIBUTE_REGISTER_I;
        static const wxString ATTRIBUTE_REGISTER_R;
        static const wxString ATTRIBUTE_REGISTER_BC;
        static const wxString ATTRIBUTE_REGISTER_DE;
        static const wxString ATTRIBUTE_REGISTER_HL;
        static const wxString ATTRIBUTE_REGISTER_IX;
        static const wxString ATTRIBUTE_REGISTER_IY;
        static const wxString ATTRIBUTE_REGISTER_SP;
        static const wxString ATTRIBUTE_VALUE_LITERAL;
        static const wxString ATTRIBUTE_RST_00;
        static const wxString ATTRIBUTE_RST_08;
        static const wxString ATTRIBUTE_RST_10;
        static const wxString ATTRIBUTE_RST_18;
        static const wxString ATTRIBUTE_RST_20;
        static const wxString ATTRIBUTE_RST_28;
        static const wxString ATTRIBUTE_RST_30;
        static const wxString ATTRIBUTE_RST_38;

        static const wxUniChar ARGUMENT_MARK;

    private:
        wxXmlDocument *m_xml_mnemonic;
        MnemonicContainer *m_mnemonics;
        wxTextCtrl *m_log_text;
        Statistics m_statistics;

        void ProcessFile();
        void ProcessGroup(const wxXmlNode *t_groupitem);
        bool FindGroups(NodeGroupList &grouplist);
        void ProcessInstruction(wxXmlNode *instruction_node, MnemonicItem * const instruction);
        void ProcessArguments(wxXmlNode *instruction_node, MnemonicItem *instruction);
        void ProcessCharacteristcs(wxXmlNode *instruction_node, MnemonicItem *instruction);
        Groups GetGroupFromStr(const wxString &groupstr);
        OperandType GetOperandTypeFromStr(const wxString &ot_str);
        Operands GetOperandFromStr(const wxString &op_str);
        void ParseMnemonicString(const wxString &rawstr, StringVector &liststr);
        void ParseOpcodeString(MnemonicItem *instruction, const wxString &bytecodestr, const wxString &sizestr);
        unsigned int CalculateSignature(const ByteCode &bytecode);
        wxXmlNode *GetInInstructionNode(wxXmlNode *instruction_node, const wxString &node_str);
        void PrintErrorMessages(XMLF_Exceptions t_exception, int t_line);

        void LogIt(const wxString textout);

};

#endif // MNEMONICXMLFILE_H
