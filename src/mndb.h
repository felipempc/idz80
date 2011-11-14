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

#define MIN_ARRAY_ITENS 3


class MnemonicDataBase
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
    void DebugVodoo(wxTextCtrl& log);

    private:
    uint totalAllocated;
    wxArrayPtrVoid Data;
    bool doReadData(wxTextFile& tf);
    bool addData(wxArrayString& arraystr, int currentSection);

};

#endif
