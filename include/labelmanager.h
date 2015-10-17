/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class to manage system and user labels
 **************************************************************/

#ifndef LABELMANAGER_H_
#define LABELMANAGER_H_

#include "systemlabels.h"
#include "labelslist.h"

class LabelManager
{
    public:
        // User Labels
        LabelListCtrl       *var_labels,
                            *prog_labels,
                            *io_labels,
                            *constant_labels;
        // System Labels
		SystemLabelList		*sys_calls,
							*sys_vars,
							*sys_io,
							*sys_const;
        LabelManager();
        bool LoadSystemLabels(wxString filename);
        void ClearUserLabels();

};




#endif // LABELMANAGER_H_
