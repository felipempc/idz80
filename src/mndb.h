/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe MPC ()
 * License:   GPL
 * This module is a database of mnemonic items
 **************************************************************/



#ifndef _IDZ80_MNDB_H
#define _IDZ80_MNDB_H

#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>

#include "IDZ80Base.h"
#include "MnemonicItem.h"
#include "logbase.h"


WX_DECLARE_OBJARRAY(MnemonicItem *, MnemonicArray);

class MnemonicDataBase: public LogBase
{
    public:
        MnemonicDataBase(LogWindow *logparent);
        ~MnemonicDataBase();
        bool Open(wxString& filename);
        void Clear();
        void Find(wxArrayInt& mnemonics_found_list, byte opcode, uint scanoffset);
        MnemonicItem *FindByOpCode(const ByteCode& code);
        MnemonicItem *GetData(uint index);
        uint GetCount();
        uint GetAllocated();
        bool IsLoaded();

    private:
        static const int MIN_ARRAY_ITEMS = 3;
        uint            memory_allocated_;
        MnemonicArray   *mnemonic_list_;

        bool ReadFileAndStore(wxTextFile& tf);
        bool ProcessAndStore(wxArrayString& mnemonic_line_items, int currentSection, int line);
        bool SetupArgument(MnemonicItem *mnemonic, wxString &strline);
        void SetupBranch(MnemonicItem *mnemonic);

};

#endif
