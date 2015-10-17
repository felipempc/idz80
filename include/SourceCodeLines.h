/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 ****************************************************************
 * This module stores source code lines
 **************************************************************/

#ifndef SOURCECODELINES_H
#define SOURCECODELINES_H

#include "idz80_base.h"
#include "disassembled_item.h"
#include "disassembled_container.h"
#include "sourcecodeaccess.h"
#include "labelmanager.h"

class SourceCodeLines: public SourceCodeAccess
{
    public:
        SourceCodeLines(DisassembledContainer *dasm, LabelManager *labels);
        virtual ~SourceCodeLines();

        void Clear();
        int AddDasm(const DisassembledIndex dasmitem, const wxString &comment);
        int AddProgLabel(const AbsoluteAddress labeladdr, const wxString &comment);
        int AddVarLabel(const AbsoluteAddress labeladdr, const wxString &comment);
        int AddOrg(const AbsoluteAddress org, const wxString &comment);
        int AddComment(const wxString &comment);
        void Del(const LineNumber idx);
        void DelItem(CodeViewItem *cvi);
        void DelComment(CodeViewItem *cvi);
        void EditDasm(const DisassembledIndex asmitem, const wxString &comment, LineNumber pos);
        void EditProgLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos);
        void EditVarLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos);
        void EditOrg(const AbsoluteAddress org, const wxString &comment, LineNumber pos);
        void EditComment(const wxString &comment, const LineNumber pos);
        int InsertDasm(const DisassembledIndex dasmitem, const wxString &comment, LineNumber pos);
        int InsertProgLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos);
        int InsertVarLabel(const AbsoluteAddress labeladdr, const wxString &comment, LineNumber pos);
        int InsertOrg(const AbsoluteAddress org, const wxString &comment, LineNumber pos);
        int InsertComment(const wxString &comment, const LineNumber pos);       //create a new line with comment
        int AppendComment(const wxString &comment, const LineNumber pos); // append a comment to an existing line
        void linkData(DisassembledIndex indexdasm, LineNumber indexline, uint countdasm);
        void UpdateDasmIndex(LineNumber index, const int delta);

        bool IsEmpty();

        int getLineOfAddress(AbsoluteAddress addr);
        int getLineOfAddress(LineNumber line_index, uint line_count, AbsoluteAddress addr);
        int getFirstInstructionLine();

        void SetFirstInstructionLine(int fstiline);


    protected:
    private:
        LabelManager    *labels_;
        int             itemcount_;
        DisassembledContainer        *disassembled_;
        int             first_intruction_line;

        int AddNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr, const int org,const wxString &comment);
        int InsertNewItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
						  const int org, const wxString &comment, LineNumber pos);
        void EditItem(const int dasmitem, const int labelprogaddr, const int labelvaraddr,
					  const int org, const wxString &comment, LineNumber pos);
};

#endif // CODEVIEWLINE_H
