/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   29-10-2012 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Controls subroutines
 **************************************************************/

#include "subroutine.hpp"


SubRoutineCtrl::SubRoutineCtrl(DebugLogWindow *logparent)
{
    m_stack_pointer = 0;
    m_actual_subroutine = 0;
    m_return_conditional = false;
    m_called_subroutines.clear();
    SetTextLog(logparent);
    ModuleName = "SubRoutine";
}

SubRoutineCtrl::~SubRoutineCtrl()
{
    clear();
}



/// @brief Search in CalledSubRoutines for t_address
/// @param t_address The address to be found
/// @return True if found
bool SubRoutineCtrl::searchAddressInCalledSubRoutines(unsigned int t_address)
{
    for (auto address : m_called_subroutines) {
        if (address == t_address)
            return true;
    }
    return false;
}



/// @brief Registers data from the subroutine, simulating a call instruction
/// @param entryaddress The subroutine address
/// @param nextaddress The address of the next instruction
/// @return True if it's the first time this subroutine at entryaddress is called.
bool SubRoutineCtrl::call_subroutine(uint t_entryaddress, uint t_nextaddress)
{
    SubRoutineData *sr_data;
    uint ret_cond;
    bool ret = false;

    if (!searchAddressInCalledSubRoutines(t_entryaddress)) {
        ret = true;
        m_stack.push(t_nextaddress);        // stores the address of the next instruction in the stack
        if (m_return_conditional)
            ret_cond = 0;
        else
            ret_cond = 1;
        m_stack.push(ret_cond);             // stores "return conditional" in the stack
        m_return_conditional = false;       // reset "return conditional"

        sr_data = new SubRoutineData;       // Creates an object to store the subroutine data
        sr_data->Start = t_entryaddress;
        sr_data->Size = 0;                  // Will be filled at the "return" instruction
        sr_data->End = t_entryaddress;      // Will be filled at the "return" instruction
        m_subroutines.push_back(sr_data);   // Stores the subroutine data in the list of subroutines

        m_actual_subroutine = sr_data;      // Points to actual subroutine data
        ++m_stack_pointer;
        m_called_subroutines.push_back(t_entryaddress);     // Registers this subroutine in the already called(registered) subroutines
    }
    return ret;
}


/// @brief Restores data after a "called" subroutine, simulating an "return" instruction
/// @param t_exitaddress Address of the next instruction after call.
/// @return The address of the next instruction
uint SubRoutineCtrl::return_subroutine(uint t_exitaddress)
{
    uint returnaddress = 0;
    uint ret_cond;

    if (m_stack_pointer > 0)
    {
        m_actual_subroutine->End = t_exitaddress;
        ret_cond = m_stack.top();                       // Restores the status of "return condition"
        m_stack.pop();                                  // Removes it from stack
        if (ret_cond == 0)                              // Updates the "return condition" status
            m_return_conditional = true;
        else
            m_return_conditional = false;

        returnaddress = m_stack.top();                  // Recovers the address of the next instruction from the stack
        m_stack.pop();                                  // Removes it from stack.
        --m_stack_pointer;
    }
    if (m_stack_pointer == 0)
        m_actual_subroutine = 0;
    else
        m_actual_subroutine = m_subroutines[m_stack_pointer - 1];

    return returnaddress;
}


/// @brief Updates the size of the subroutine in the subroutine data
void SubRoutineCtrl::updateSize()
{
    if (m_actual_subroutine != 0)
        m_actual_subroutine->Size++;
}


/// @brief Checks if it's inside a subroutine
/// @return True if it's inside
bool SubRoutineCtrl::isInside()
{
    return (m_stack_pointer > 0);
}



/// @brief Checks if return instruction has a condition to return.
/// @return 
bool SubRoutineCtrl::hasConditionalReturn()
{
    return m_return_conditional;
}



/// @brief Verifies if the address subroutine has been already "called" (registered)
/// @param address The address of a subroutine
/// @return True if it has been already called
bool SubRoutineCtrl::alreadyCalled(uint address)
{
    return (searchAddressInCalledSubRoutines(address));
}



void SubRoutineCtrl::clear()
{
    m_called_subroutines.clear();
    m_actual_subroutine = 0;
    clearSubRoutines();
    m_stack_pointer = 0;
}



/// @brief Clear the SubRoutineData struct in the list
void SubRoutineCtrl::clearSubRoutines()
{
    SubRoutineData *srd;
    int size;

    size = m_subroutines.size();

    for(int i = 0; i < size; i++)
    {
        srd = m_subroutines.back();
        if (srd != 0)
            delete srd;
        m_subroutines.pop_back();
    }
}



/// @brief Get the stack pointer
/// @return 
int SubRoutineCtrl::getCounter()
{
    return m_stack_pointer;
}



/// @brief Flags that the return instruction has a condition to return.
void SubRoutineCtrl::setConditionalReturn()
{
    m_return_conditional = true;
}

