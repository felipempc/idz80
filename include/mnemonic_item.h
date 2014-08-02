/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   19-04-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Stores a mnemonic item
 **************************************************************/


#ifndef MNEMONICITEM_H
#define MNEMONICITEM_H

#include <mnemonic_item_base.h>


class MnemonicItem : public MnemonicItemBase
{
    public:
        enum MnemonicException
        {
            MAX_BYTECODE_REACHED
        };

/*
        MnemonicItem();
        ~MnemonicItem();
*/
        Groups GetGroup();

        Arguments GetSourceArgument();
        Arguments GetDestinationArgument();

        ByteCode &GetByteCode();
        byte GetByteCode(byte index);
        unsigned int GetByteCodeSize();

        unsigned int GetArgumentCount();
        unsigned int GetArgumentSize();
        unsigned int GetArgumentPosition();

        bool GetConditionalBranch();

        unsigned int GetMnemonicStrCount();
        wxString GetMnemonicStr(unsigned int index);

        void SetGroup(Groups group);

        void SetSourceArgument(OperandType type, Operands operand);
        void SetDestinationArgument(OperandType type, Operands operand);

        void SetByteCode(ByteCode &bytecode, byte opcodesize);

        void SetArgumentConfig(byte argcount, byte argsize, byte argposition);

        void SetConditionalBranch(bool isconditional = false);

        void SetMnemonicString(const wxArrayString &mnemonicstring);

    protected:
    private:
};

#endif // MNEMONICITEM_H
