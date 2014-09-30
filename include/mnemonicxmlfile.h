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

#include <wx/textctrl.h>
#include "mnemonic_container.h"


enum XMLF_Exceptions
{
    XMLFE_GROUP_NOT_FOUND
};

typedef std::vector<wxXmlNode *> NodeGroupList;

class MnemonicXMLFile
{
    public:
        MnemonicXMLFile(MnemonicContainer *mnm, wxTextCtrl *_log);
        ~MnemonicXMLFile();

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

        static const wxString ATTRIBUTE_VERSION_STR;
        static const wxString ATTRIBUTE_ITEM_STR;
        static const wxString ATTRIBUTE_SIZE_STR;
        static const wxString ATTRIBUTE_STRING_STR;
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


        static const wxUniChar ARGUMENT_MARK;

    private:
        wxXmlDocument *xml_mnemonic_;
        MnemonicContainer *mnemonics_;
        wxTextCtrl *log_;
        Statistics statistics_;

        void ProcessFile();
        void ProcessGroup(const wxXmlNode *groupitem);
        bool FindGroups(NodeGroupList &grouplist);
        MnemonicItem *ProcessInstruction(wxXmlNode *instruction_node, const Groups currentgroup);
        Groups GetGroupFromStr(const wxString &groupstr);
        void ParseMnemonicString(const wxString &rawstr, wxArrayString &liststr);
        bool ParseOpcodeString(MnemonicItem *instruction, const wxString &bytecodestr, const wxString &sizestr);
        wxXmlNode *GetOpcodeNode(wxXmlNode *intruction_node);

};

//TODO: Evoluir MnemonicXMLFile para preencher todo Mnemonics

#endif // MNEMONICXMLFILE_H
