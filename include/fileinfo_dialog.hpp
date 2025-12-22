/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Window that shows information about a loaded program
 **************************************************************/

#ifndef SHOWFILEINFO_H
#define SHOWFILEINFO_H


#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "project_base.hpp"
//#include "process_data.h"

class ShowFileInfo: public wxDialog
{
	public:

		ShowFileInfo(ProjectBase *parent, wxWindowID id = wxID_ANY, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

//		void SendInfo(ProcessData *process);

		wxButton* B_OK;

		wxPanel* Panel1;

		wxStaticText* StaticText1;
		wxStaticText* StaticText2;
		wxStaticText* StaticText3;
		wxStaticText* StaticText4;
		wxStaticText* StaticText5;
		wxStaticText* StaticText6;
		wxStaticText* StaticText7;
		wxStaticText* StaticText8;
        wxStaticText* StaticText9;

		wxStaticBox* StaticBox1;
		wxStaticBox* StaticBox2;



	protected:

		static const long ID_BUTTON1;
		static const long ID_STATICBOX1;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICBOX2;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_STATICTEXT7;
		static const long ID_STATICTEXT8;
		static const long ID_STATICTEXT9;
		static const long ID_PANEL1;

	private:
//	    ProcessData *m_process;

		void OnB_OKClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);

};

#endif
