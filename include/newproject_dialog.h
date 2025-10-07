#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "wx/dialog.h"
#include "wx/grid.h"
#include "wx/bitmap.h"
#include "wx/arrstr.h"

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
        IDZ80MainBase *main_;
        wxBoxSizer *main_sizer_;

        wxGrid  *filegrid_;
        wxButton *Cancel_button_, *OK_button_, *Add_button_, *Remove_button_;
        int actualrow_;

        void BuildDialog();
        bool DialogLoadProgramFile(wxArrayString &file_list_str);

        bool AddFileToGrid(wxString& filestr);

        void OnResize(wxSizeEvent &event);
        void OnAddButton(wxCommandEvent &event);
        void OnRemoveButton(wxCommandEvent &event);
        void OnOkButtonPressed(wxCommandEvent &event);
        void OnCancelButtonPressed(wxCommandEvent &event);
        void OnGridLeftDoubleClick(wxGridEvent& event);
};

#endif // NEWPROJECTDIALOG_H
