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
        void FindItems(wxArrayInt& arrayint, byte opcode, uint scanoffset);
        MnemonicItem *FindItem(const ByteCode& code);
        MnemonicItem *GetData(uint index);
        uint GetCount();
        uint GetAllocated();
        bool IsLoaded();

    private:
        static const int MIN_ARRAY_ITEMS = 3;
        uint            m_totalAllocated;
        MnemonicArray   *m_MnemonicList;

        bool doReadData(wxTextFile& tf);
        bool addData(wxArrayString& arraystr, int currentSection, int line);
        bool SetupArgument(MnemonicItem *mnemonic, wxString &strline);
        void SetupBranch(MnemonicItem *mnemonic);

};

#endif
