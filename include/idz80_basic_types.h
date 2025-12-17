/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   26-06-2014 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Basic Types
 **************************************************************/

#ifndef IDZ80BASICTYPES_H
#define IDZ80BASICTYPES_H

#include <vector>

typedef unsigned char byte;
typedef unsigned short int word;
typedef unsigned int uint;

/// @brief Normal unsigned int vector, must be sorted manually. Include <algorithm> for sort.
typedef std::vector<uint> UintArray;
typedef std::vector<int> IntArray;
typedef std::vector<unsigned int> AddressVector;
typedef std::vector<int> IndexVector;

#endif // IDZ80BASICTYPES_H


