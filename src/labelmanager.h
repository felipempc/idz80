/****************************************************************
 * Name:      IDZ80
 * Purpose:   Z80 Disessembler
 * Author:    Felipe Mainieri (felipe.mpc@gmail.com)
 * Created:   2009-11-09
 * Copyright: Felipe Mainieri ()
 * License:   GPL
 *
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
