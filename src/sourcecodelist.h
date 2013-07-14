/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   04-07-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * This module simplifies interface of std::list
 **************************************************************/


#include <vector>
#include <wx/string.h>
#include "IDZ80Base.h"
#include "labelitem.h"
#include <wx/gdicmn.h>


struct CodeViewItem
{
    wxString *Comment;
    LabelItem *LabelProgAddr;
    LabelItem *LabelVarAddr;
    int Dasmitem;
    int Org;
    wxRect *RectArg1, *RectArg2;
};

typedef std::vector<CodeViewItem *> SourceCodeList;




class SourceCodeLines
{
    public:
    SourceCodeLines();
    ~SourceCodeLines();
    void Add(CodeViewItem *cvi);
    void Clear();
    int Find(CodeViewItem *cvi);
    uint GetCount();
    CodeViewItem *GetLine(LineNumber line);
    void Insert(CodeViewItem *cvi, LineNumber line);
    void Remove(CodeViewItem *cvi);

    protected:


    private:
    SourceCodeList  code_list_;

};
