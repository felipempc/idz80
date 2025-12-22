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

#include "mnemonic_item_base.hpp"


class MnemonicItem : public MnemonicItemBase
{
    public:
        enum MnemonicException
        {
            MAX_BYTECODE_REACHED
        };

        const Arguments ARGUMENT_NONE = {OP_NONE, OT_NONE};

        MnemonicItem();
        ~MnemonicItem();

        void Reset();

        Groups GetGroup();

        unsigned int GetMnemonicSignature();

        Arguments GetSourceArgument();
        Arguments GetDestinationArgument();
        Arguments GetArgument(const unsigned int index);

        ByteCode &GetByteCode();
        byte GetByteCode(byte index);
        unsigned int GetByteCodeSize();

        unsigned int GetArgumentCount();
        unsigned int GetArgumentSize();
        unsigned int GetArgumentPosition();

        bool GetConditionalBranch();

        bool HasExplicitArguments();

        unsigned int GetMnemonicStrCount();
        wxString GetMnemonicStr(unsigned int index);

        void SetGroup(Groups group);

        void SetSourceArgument(OperandType type, Operands operand);
        void SetDestinationArgument(OperandType type, Operands operand);

        void SetByteCode(ByteCode &bytecode, byte opcodesize);

        void ConfigArguments(byte argcount, byte argsize, byte argposition);

        void SetConditionalBranch(bool isconditional = false);

        void SetExplicitArguments(bool isexplicit = true);

        void SetMnemonicString(const wxArrayString &mnemonicstring);

        void SetMnemonicSignature(unsigned int signature);

    protected:
    private:
};

#endif // MNEMONICITEM_H
