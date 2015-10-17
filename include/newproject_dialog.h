#ifndef NEWPROJECTDIALOG_H
#define NEWPROJECTDIALOG_H

#include "wx/dialog.h"
#include "wx/grid.h"
#include "wx/bitmap.h"
#include "idz80_main_base.h"

class NewProjectDialog: public wxDialog, public LogBase
{
    public:
        NewProjectDialog(IDZ80MainBase *parent);

        virtual ~NewProjectDialog();
    protected:
        enum
        {
            idADD_FILE_BUTTON = 100,
            idREMOVE_FILE_BUTTON
        };
    private:
        IDZ80MainBase *main_;
        wxBoxSizer *main_sizer_;
        wxBitmap *add_icon_,
                 *remove_icon_;

        wxGrid  *filegrid_;
        int actualrow_;


        void BuildDialog();
        wxString DialogLoadProgramFile();

        void OnResize(wxSizeEvent &event);
        void OnAddButton(wxCommandEvent &event);
        void OnRemoveButton(wxCommandEvent &event);
};

#endif // NEWPROJECTDIALOG_H
