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


#include "mnemonic_container.hpp"
#include "rawdatamanager.hpp"
#include "disassembled_manager.hpp"
#include "source_code_manager.hpp"
#include "labelmanager.hpp"
#include "idz80_directories.hpp"


class ProjectBase: public IDZ80Directories, public DebugLogBase
{
    public:
    SourceCodeManager   *m_sourcecode_mgr;
    DisassembledManager *m_disassembled_mgr;
    RawDataManager      *m_programs_mgr;
    MnemonicContainer   *m_mnemonics;
    LabelManager        *m_labels;
};

#endif // PROJECTBASE_H
