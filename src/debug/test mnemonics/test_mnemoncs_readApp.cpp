/***************************************************************
 * Name:      test_mnemoncs_readApp.cpp
 * Purpose:   Code for Application Class
 * Author:    felipempc (idz80a@gmail.com)
 * Created:   2014-07-04
 * Copyright: felipempc (#######)
 * License:
 **************************************************************/

#include "test_mnemoncs_readApp.h"
#include "test_mnemoncs_readMain.h"

IMPLEMENT_APP(test_mnemoncs_readApp);

bool test_mnemoncs_readApp::OnInit()
{
    test_mnemoncs_readFrame* frame = new test_mnemoncs_readFrame(0L, _("wxWidgets Application Template"));
    frame->SetIcon(wxICON(aaaa)); // To Set App Icon
    frame->Show();

    return true;
}
