/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 * This module shows information about the program file
 **************************************************************/

#ifndef SHOWFILEINFO_H
#define SHOWFILEINFO_H

//(*Headers(ShowFileInfo)
#include <wx/stattext.h>
#include <wx/statbox.h>
#include <wx/panel.h>
#include <wx/button.h>
#include <wx/dialog.h>
//*)


#include "ProcessData.h"

class ShowFileInfo: public wxDialog
{
	public:

		ShowFileInfo(wxWindow* parent,wxWindowID id=wxID_ANY,const wxPoint& pos=wxDefaultPosition,const wxSize& size=wxDefaultSize);
		virtual ~ShowFileInfo();
		void SendInfo(ProcessData *process);

		//(*Declarations(ShowFileInfo)
		wxButton* B_OK;
		wxStaticText* StaticText2;
		wxStaticText* StaticText6;
		wxPanel* Panel1;
		wxStaticText* StaticText1;
		wxStaticText* StaticText3;
		wxStaticBox* StaticBox1;
		wxStaticBox* StaticBox2;
		wxStaticText* StaticText5;
		wxStaticText* StaticText4;
		//*)

	protected:

		//(*Identifiers(ShowFileInfo)
		static const long ID_BUTTON1;
		static const long ID_STATICBOX1;
		static const long ID_STATICTEXT1;
		static const long ID_STATICTEXT2;
		static const long ID_STATICTEXT3;
		static const long ID_STATICBOX2;
		static const long ID_STATICTEXT4;
		static const long ID_STATICTEXT5;
		static const long ID_STATICTEXT6;
		static const long ID_PANEL1;
		//*)

	private:
	    ProcessData *m_process;

		//(*Handlers(ShowFileInfo)
		void OnB_OKClick(wxCommandEvent& event);
		void OnInit(wxInitDialogEvent& event);
		//*)

		DECLARE_EVENT_TABLE()
};

#endif
