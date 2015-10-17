/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   28-11-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manages the searching sequences.
 **************************************************************/



#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H

#include <wx/string.h>
#include "idz80_base.h"
#include "search_defs.h"


class SearchException
{
    public:
        SearchException(bool underflow = false);
        wxString getErrorStr();
    private:
        wxString    error_str_;

};


class SearchManager
{
    public:
        SearchManager();
        DisassembledIndex Next();
        DisassembledIndex Prev();
        DisassembledIndex Current();
        bool SearchLiteral();
        bool SearchVariable();
        bool SearchJumpsCalls();
        word Target();
        void Setup(const DisassembledIndex &first_item, const DisassembledIndex &last_item, uint search_config, word target);
    protected:
    private:
        DisassembledIndex   first,
                            last,
                            current;
        word    target_;
        bool    circular_search,
                first_time_search,
                literal,
                variable,
                jumps_calls;
};

#endif // SEARCHMANAGER_H
