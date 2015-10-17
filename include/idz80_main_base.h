/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   29-07-2015 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for dialog.
 **************************************************************/


#ifndef IDZ80MAINBASE_H
#define IDZ80MAINBASE_H


#include <wx/frame.h>

#include "mnemonic_container.h"
#include "rawdatamanager.h"
#include "disassembled_container.h"
#include "SourceCodeLines.h"
#include "labelmanager.h"
#include "idz80_directories.h"


class IDZ80MainBase: public IDZ80Directories, public wxFrame
{
    public:
        SourceCodeLines     *CodeViewLines_;
        DisassembledContainer
                            *Disassembled_;
        RawDataManager      *Programs_;
        MnemonicContainer   *Mnemonics_;
        LabelManager        *Labels_;
};

#endif // IDZ80MAINBASE_H
