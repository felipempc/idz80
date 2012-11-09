/****************************************************************
 * Name:      IDZ80
 * Purpose:   Defines Application Frame
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 **************************************************************/



#ifndef PROJECTMANAGER_H
#define PROJECTMANAGER_H

#include <wx/textfile.h>

#include "ProcessData.h"
#include "codeview.h"

class ProjectManager
{
    public:
        ProjectManager(ProcessData *process, CodeView *codeview);
        virtual ~ProjectManager();
        bool IsSaved();
        bool HasName();
        void New();
        bool Save(bool overwrite = false);
        bool Save(const wxString &filename, bool overwrite = false);
        bool Open(const wxString &filename);
        void SetFilename(const wxString &filename);
        wxString GetFilename();

        void LogIt(const wxString &str);
        void SetLog(wxTextCtrl *log);

    protected:
        static const wxString projectExtension;

    private:
        bool        m_saved;
        bool        m_named;
        wxString    m_filename;
        ProcessData *Process;
        CodeView    *m_codeview;

        wxTextCtrl  *m_log;

        bool readHeader(wxTextFile &openfile);
        void readLabels(wxTextFile &openfile);
        void readDasmData(wxTextFile &openfile);
        void readCodeLine(wxTextFile &openfile);

        void linkLabels();

        void writeHeader(wxTextFile &savefile);
        void writeProglabel(wxTextFile &savefile);
        void writeVarlabel(wxTextFile &savefile);
        void writeIOlabel(wxTextFile &savefile);
        void writeDasmData(wxTextFile &savefile);
        void writeCodeLine(wxTextFile &savefile);
};

#endif // PROJECTMANAGER_H
