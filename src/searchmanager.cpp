/****************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   28-11-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Manages the searching sequences.
 **************************************************************/


#include "searchmanager.h"


/*
    Exception class
 */
SearchException::SearchException(bool underflow)
{
    if (underflow)
        error_str_ = "Start reached !";
    else
        error_str_ = "End reached !";
}

wxString SearchException::getErrorStr()
{
    return error_str_;
}
/* ****************************************************** */


/*
    Search control
 */
SearchManager::SearchManager()
{
    first = 0;
    last = 0;
    current = 0;
    target_ = 0;
    circular_search = false;
    first_time_search = true;
    literal = true;
    variable = true;
    jumps_calls = true;
}

DisassembledIndex SearchManager::Next()
{
    if (first_time_search)
        first_time_search = false;
    else
        current++;

    if (circular_search)
    {
        if (current > last)
            current = first;
    }
    else
    {
        if (current > last)
            throw SearchException();
    }

    return current;
}


DisassembledIndex SearchManager::Prev()
{
    if (first_time_search)
        first_time_search = false;
    else
    {
        if (circular_search)
        {
            if (current <= first)
                current = last;
            else
                current--;
        }
        else
        {
            if (current <= first)
                throw SearchException(true);
            else
                current--;
        }
    }

    return current;
}


DisassembledIndex SearchManager::Current()
{
    return current;
}


bool SearchManager::SearchLiteral()
{
    return literal;
}

bool SearchManager::SearchVariable()
{
    return variable;
}

bool SearchManager::SearchJumpsCalls()
{
    return jumps_calls;
}


word SearchManager::Target()
{
    return target_;
}

void SearchManager::Setup(const DisassembledIndex &first_item, const DisassembledIndex &last_item, uint search_config,
                          word target)
{
    first = first_item;
    last = last_item;
    current = first;
    circular_search = ((search_config & SEARCH_WRAPARAOUND) != 0);
    literal = ((search_config & SEARCH_LITERALS) != 0);
    variable = ((search_config & SEARCH_VARIABLES) != 0);
    jumps_calls = ((search_config & SEARCH_JUMPS_CALLS) != 0);
    first_time_search = true;
    target_ = target;
}





