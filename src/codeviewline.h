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
#include "d_asm_element.h"
#include "dasmdata.h"

struct CommentItem //stComment
{
    wxString CommentStr;
};
//typedef struct stComment CommentItem;

struct CodeViewItem//stCodeViewLine
{
    CommentItem *Comment;
    short int LabelProgAddr;
    short int LabelVarAddr;
    int Dasmitem;
    int Org;
    wxRect *RectArg1, *RectArg2;
};
//typedef struct stCodeViewLine CodeViewItem;

class CodeViewLine
{
    public:
        CodeViewLine(DAsmData *dasm);
        virtual ~CodeViewLine();

        void Clear();
        int AddDasm(const int dasmitem, const wxString &comment);
        int AddProgLabel(const int labeladdr, const wxString &comment);
        int AddVarLabel(const int labeladdr, const wxString &comment);
        int AddOrg(const int org, const wxString &comment);
        int Add(const wxString &comment);
        void Del(const int idx);
        void DelItem(CodeViewItem *cvi);
        void DelComment(CodeViewItem *cvi);
        void EditDasm(const int asmitem, const wxString &comment,int pos);
        void EditProgLabel(const int labeladdr, const wxString &comment,int pos);
        void EditVarLabel(const int labeladdr, const wxString &comment,int pos);
        void EditOrg(const int org, const wxString &comment, int pos);
        void Edit(const wxString &comment, const int pos);
        int InsertDasm(const int dasmitem, const wxString &comment,int pos);
        int InsertProgLabel(const int labeladdr, const wxString &comment,int pos);
        int InsertVarLabel(const int labeladdr, const wxString &comment,int pos);
        int InsertOrg(const int org, const wxString &comment, int pos);
        int Insert(const wxString &comment, const int pos);       //create a new line with comment
        int AppendComment(const wxString &comment, const int pos); // append a comment to an existing line
        void linkData(int indexdasm, int indexline, int countdasm);
        void UpdateDasmIndex(const int index, const int delta);

        bool IsEmpty();

        uint GetCount();
        void setData(CodeViewItem *cvi);
        bool getDataLineAddress(uint addr, int &index);
        CodeViewItem *getData(uint index);


    protected:
    private:
        wxArrayPtrVoid  m_CodeLine;
        int             m_itemcount;
        DAsmData        *m_dasm;

        int AddNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr, const int org,const wxString &comment);
        int InsertNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
						  const int org, const wxString &comment, int pos);
        void EditItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
					  const int org, const wxString &comment, int pos);
};

#endif // CODEVIEWLINE_H
