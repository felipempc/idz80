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

#include "main_dialog_base.h"

class NewProjectDialog: public wxDialog, public LogBase
{
    public:
        NewProjectDialog(IDZ80MainBase *parent);

        virtual ~NewProjectDialog();
    protected:
        enum
        {
            idADD_FILE_BUTTON = 100,
            idREMOVE_FILE_BUTTON,
            idGRID_LEFT_DCLICK
        };
    private:
        IDZ80MainBase *main_dialog_;
        wxBoxSizer *main_sizer_;

        wxGrid  *filegrid_;
        wxButton *Cancel_button_, *OK_button_, *Add_button_, *Edit_button_, *Remove_button_;
        int actualrow_;
        bool program_loaded;

        void BuildDialog();
        bool DialogLoadProgramFile(wxArrayString &file_list_str);

        bool AddFileToGrid(wxString& filestr);
        void FillRow(RawData *program);
        void UpdateAddressCells(int line, RawData *program);
        void DialogEditRow(int line);

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
