/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#ifndef _IDZ80_MNDB_H
#define _IDZ80_MNDB_H

#include <wx/string.h>
#include <wx/dynarray.h>
#include <wx/textctrl.h>
#include <wx/textfile.h>

#include "IDZ80Base.h"
#include "MnemonicItem.h"
#include "idz80debugbase.h"


WX_DECLARE_OBJARRAY(MnemonicItem *, MnemonicArray);

class MnemonicDataBase: public IDZ80LogBase
{
    public:
        MnemonicDataBase();
        ~MnemonicDataBase();
        bool Open(wxString& opcf);
        void Clear();
        void FindItems(wxArrayInt& arrayint,byte opcode,uint scanoffset);
        MnemonicItem *FindItem(const ByteCode& code);
        MnemonicItem *GetData(uint index);
        uint GetCount();
        uint GetAllocated();
        bool IsLoaded();

        // DEBUG:
        void DebugVodoo();

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
