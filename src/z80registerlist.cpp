/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   18-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manages Z80 registers
 **************************************************************/

#include "z80registerlist.hpp"



const int Z80RegisterList::TimeToLive;

Z80RegisterList::Z80RegisterList()
{
    A = new Z80Register(TimeToLive);
    B = new Z80Register(TimeToLive);
    C = new Z80Register(TimeToLive);
    D = new Z80Register(TimeToLive);
    E = new Z80Register(TimeToLive);
    H = new Z80Register(TimeToLive);
    L = new Z80Register(TimeToLive);
    BC = new Z80Register(TimeToLive);
    DE = new Z80Register(TimeToLive);
    HL = new Z80Register(TimeToLive);
    SP = new Z80Register(TimeToLive);
    IX = new Z80Register(TimeToLive);
    IY = new Z80Register(TimeToLive);
}



Z80RegisterList::~Z80RegisterList()
{
    delete A;
    delete B;
    delete C;
    delete D;
    delete E;
    delete H;
    delete L;
    delete BC;
    delete DE;
    delete HL;
    delete SP;
    delete IX;
    delete IY;
}



/// @brief Decrements life counter from the registers
void Z80RegisterList::UpdateLife()
{
    A->DecLife();
    B->DecLife();
    C->DecLife();
    D->DecLife();
    E->DecLife();
    H->DecLife();
    L->DecLife();
    BC->DecLife();
    DE->DecLife();
    HL->DecLife();
    SP->DecLife();
    IX->DecLife();
    IY->DecLife();
}



/// @brief Loads from the instruction (t_dasm_item) the register's data
/// @param t_dasm_item Disassembled instruction
void Z80RegisterList::LoadRegister(DisassembledItem *t_dasm_item)
{
    uint data = 0, offset = 0;

    UpdateLife();

    if ((t_dasm_item->GetMnemonic()->GetSourceArgument().operand == OP_LITERAL) &&
        (t_dasm_item->GetMnemonic()->GetDestinationArgument().type == OT_REGISTER)) {       // check if it's an "LD A, dd" kind of instruction
        
        data = t_dasm_item->GetArgumentValue(0);        // get the source(0) argument value
        offset = t_dasm_item->GetOffsetInFile();

        switch (t_dasm_item->GetMnemonic()->GetDestinationArgument().operand)
        {
            case OP_A:
                            A->Init(data, offset);
                            break;
            case OP_B:
                            B->Init(data, offset);
                            break;
            case OP_C:
                            C->Init(data, offset);
                            break;
            case OP_D:
                            D->Init(data, offset);
                            break;
            case OP_E:
                            E->Init(data, offset);
                            break;
            case OP_H:
                            H->Init(data, offset);
                            break;
            case OP_L:
                            L->Init(data, offset);
                            break;
            case OP_BC:
                            BC->Init(data, offset);
                            break;
            case OP_DE:
                            DE->Init(data, offset);
                            break;
            case OP_HL:
                            HL->Init(data, offset);
                            break;
            case OP_SP:
                            SP->Init(data, offset);
                            break;
            case OP_IX:
                            IX->Init(data, offset);
                            break;
            case OP_IY:
                            IY->Init(data, offset);
                            break;
        } //switch-case
    } // if
}
