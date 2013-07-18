/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   18-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manages Z80 registers
 **************************************************************/

#include "z80registerlist.h"



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


void Z80RegisterList::LoadRegister(DisassembledItem *de)
{
    UpdateLife();

    switch (de->GetInstructionDetail())
    {
        case II_LD_A_N:
                        A->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_B_N:
                        B->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_C_N:
                        C->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_D_N:
                        D->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_E_N:
                        E->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_H_N:
                        H->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_L_N:
                        L->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_BC_N:
                        BC->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_DE_N:
                        DE->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_HL_N:
                        HL->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_SP_N:
                        SP->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_IX_N:
                        IX->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
        case II_LD_IY_N:
                        IY->Init(de->GetArgument(0, 0), de->GetOffset());
                        break;
    }
}
