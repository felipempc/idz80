/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 ****************************************************************
 * This module stores the source code
 **************************************************************/

#ifndef _SOURCE_CODE_HPP_
#define _SOURCE_CODE_HPP_

#include "disassembled_item.hpp"
#include "disassembled_container.hpp"
#include "source_code_access.hpp"
#include "labelmanager.hpp"

// @brief The "text" source code
class SourceCode: public SourceCodeAccess
{
    public:
        SourceCode(DisassembledContainer *t_disassembled, LabelManager *t_labels);
        virtual ~SourceCode();

        void clear();
        int addDasmIndex(const DisassembledIndex t_dasmindex, const wxString &t_comment);
        int addProgramLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment);
        int addVarLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment);
        int addOrigin(const AbsoluteAddress t_org, const wxString &t_comment);
        int addComment(const wxString &t_comment);
        void delLine(const LineNumber t_line);
        void delSCItem(SourceCodeLine *t_scline);
        void delComment(SourceCodeLine *t_scline);
        void editDasmIndex(const DisassembledIndex t_dasmindex, const wxString &t_comment, LineNumber t_line);
        void editProgramLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment, LineNumber t_line);
        void editVarLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment, LineNumber t_line);
        void editOrigin(const AbsoluteAddress t_org, const wxString &t_comment, LineNumber t_line);
        void editComment(const wxString &t_comment, const LineNumber t_line);
        int insertDasmIndex(const DisassembledIndex t_dasmindex, const wxString &t_comment, LineNumber t_line);
        int insertProgramLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment, LineNumber t_line);
        int insertVarLabel(const AbsoluteAddress t_labeladdr, const wxString &t_comment, LineNumber t_line);
        int insertOrigin(const AbsoluteAddress t_org, const wxString &t_comment, LineNumber t_line);
        int insertComment(const wxString &t_comment, const LineNumber t_line);       //create a new line with t_comment
        int appendComment(const wxString &t_comment, const LineNumber t_line); // append a t_comment to an existing line
        void linkData(DisassembledIndex t_dasmindex, LineNumber indexline, uint countdasm);
        void updateDasmIndex(LineNumber t_line, const int delta);

        bool isEmpty();

        int getLineOfAddress(AbsoluteAddress t_addr);
        int getLineOfAddress(LineNumber line_index, uint line_count, AbsoluteAddress t_addr);
        int getFirstInstructionLine();

        void setFirstInstructionLine(int fstiline);


    protected:
    private:
        int m_itemcount;
        int m_first_instruction_line;
        LabelManager    *m_labels;
        DisassembledContainer   *m_disassembled;
        

        int addNewItem(const int t_dasmindex, const int t_labelprogaddr, const int t_labelvaraddr, const int t_org,const wxString &t_comment);
        int insertNewItem(const int t_dasmindex, const int t_labelprogaddr, const int t_labelvaraddr,
						  const int t_org, const wxString &t_comment, LineNumber t_line);
        void editItem(const int t_dasmindex, const int t_labelprogaddr, const int t_labelvaraddr,
					  const int t_org, const wxString &t_comment, LineNumber t_line);
};

#endif // CODEVIEWLINE_H
