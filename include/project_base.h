/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   29-07-2015 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base project class.     nov/2025
 **************************************************************/


#ifndef PROJECTBASE_H
#define PROJECTBASE_H


#include "mnemonic_container.h"
#include "rawdatamanager.h"
#include "disassembled_manager.h"
#include "SourceCodeLines.h"
#include "labelmanager.h"
#include "idz80_directories.h"


class ProjectBase: public IDZ80Directories, public DebugLogBase
{
    public:
        SourceCodeLines     *CodeViewLines_;
        DisassembledManager *m_disassembled_mgr;
        RawDataManager      *m_programs_mgr;
        MnemonicContainer   *m_mnemonics;
        LabelManager        *m_labels;
};

#endif // PROJECTBASE_H
