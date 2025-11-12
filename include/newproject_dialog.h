/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   20-12-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Load programs to disassemble
 **************************************************************/

#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "wx/dialog.h"
#include "wx/grid.h"
#include "wx/button.h"

#include "project_base.h"

class NewProjectDialog: public wxDialog, public LogBase
{
    public:
        NewProjectDialog(ProjectBase *parent);

        virtual ~NewProjectDialog();
    protected:
        enum
        {
            idADD_FILE_BUTTON = 100,
            idREMOVE_FILE_BUTTON,
            idGRID_LEFT_DCLICK
        };
    private:
        ProjectBase     *m_main_dialog;
        wxBoxSizer      *m_main_sizer;

        wxGrid  *m_filegrid;
        wxButton    *m_cancel_button, *m_OK_button, *m_add_button, *m_edit_button, *m_remove_button;
        int     m_actualrow;
        bool    m_program_loaded;

        void BuildDialog();
        bool DialogGetFileToList(wxArrayString &file_list_str);

        bool AddFileToGrid(wxString& filestr);
        void FillRow(RawData *program);
        void UpdateGridRow(unsigned int line, RawData *program);
        void DialogEditRow(unsigned int line);

        void OnResize(wxSizeEvent &event);
        void OnAddButton(wxCommandEvent &event);
        void OnEditButton(wxCommandEvent &event);
        void OnRemoveButton(wxCommandEvent &event);
        void OnOkButtonPressed(wxCommandEvent &event);
        void OnCancelButtonPressed(wxCommandEvent &event);
        void OnGridSelectedRow(wxGridEvent &event);
        void OnGridLeftDoubleClick(wxGridEvent &event);
};

#endif // NEWPROJECTDIALOG_H
