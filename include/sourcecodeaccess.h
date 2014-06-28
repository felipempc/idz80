/***************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   15-01-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Base class for line code access
 **************************************************************/

#ifndef SOURCECODENAVIGATION_H
#define SOURCECODENAVIGATION_H

#include "source_code_container.h"



class SourceCodeAccess: public SourceCodeContainer
{
    public:
        SourceCodeAccess();

        CodeViewItem *First();
        CodeViewItem *Last();
        CodeViewItem *CurrentItem();
        CodeViewItem *Previous();
        CodeViewItem *Next();
        CodeViewItem *Line(LineNumber linenumber);

        LineNumber CurrentIndex();


    protected:
    private:
        CodeViewItem    *current_item_;
        LineNumber      current_index_;
};

#endif // SOURCECODENAVIGATION_H
