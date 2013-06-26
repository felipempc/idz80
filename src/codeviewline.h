/****************************************************************
 * Name:      IDZ80
 * Purpose:   Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module manages lines in CodeView
 **************************************************************/

#ifndef CODEVIEWLINE_H
#define CODEVIEWLINE_H

#include <wx/dynarray.h>

#include "IDZ80Base.h"
#include "disassembled_item.h"
#include "dasmdata.h"

struct CommentItem
{
    wxString CommentStr;
};


struct CodeViewItem
{
    CommentItem *Comment;
    short int LabelProgAddr;
    short int LabelVarAddr;
    int Dasmitem;
    int Org;
    wxRect *RectArg1, *RectArg2;
};


class CodeViewLine
{
    public:
        CodeViewLine(DAsmData *dasm);
        virtual ~CodeViewLine();

        void Clear();
        int AddDasm(const DisassembledIndex dasmitem, const wxString &comment);
        int AddProgLabel(const ProgramAddress labeladdr, const wxString &comment);
        int AddVarLabel(const ProgramAddress labeladdr, const wxString &comment);
        int AddOrg(const ProgramAddress org, const wxString &comment);
        int Add(const wxString &comment);
        void Del(const LineNumber idx);
        void DelItem(CodeViewItem *cvi);
        void DelComment(CodeViewItem *cvi);
        void EditDasm(const DisassembledIndex asmitem, const wxString &comment, LineNumber pos);
        void EditProgLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos);
        void EditVarLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos);
        void EditOrg(const ProgramAddress org, const wxString &comment, LineNumber pos);
        void Edit(const wxString &comment, const LineNumber pos);
        int InsertDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos);
        int InsertProgLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos);
        int InsertVarLabel(const ProgramAddress labeladdr, const wxString &comment, LineNumber pos);
        int InsertOrg(const ProgramAddress org, const wxString &comment, LineNumber pos);
        int Insert(const wxString &comment, const LineNumber pos);       //create a new line with comment
        int AppendComment(const wxString &comment, const LineNumber pos); // append a comment to an existing line
        void linkData(DisassembledIndex indexdasm, LineNumber indexline, uint countdasm);
        void UpdateDasmIndex(LineNumber index, const int delta);

        bool IsEmpty();

        uint GetCount();
        void setData(CodeViewItem *cvi);
        bool getLineOfAddress(ProgramAddress addr, int &index);
        bool getLineOfAddress(LineNumber line_index, uint line_count, ProgramAddress addr, int &index);
        CodeViewItem *getData(LineNumber index);


    protected:
    private:
        wxArrayPtrVoid  m_CodeLine;
        int             m_itemcount;
        DAsmData        *m_dasm;

        int AddNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr, const int org,const wxString &comment);
        int InsertNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
						  const int org, const wxString &comment, LineNumber pos);
        void EditItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
					  const int org, const wxString &comment, LineNumber pos);
};

#endif // CODEVIEWLINE_H
