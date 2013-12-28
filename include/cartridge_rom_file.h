/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Controls Cartridge ROM files
 **************************************************************/


#ifndef CARTRIDGEROMFILE_H
#define CARTRIDGEROMFILE_H

#include <wx/dynarray.h>

#include "idz80_base.h"

struct stCartHeader
{
	word	id;
	word	init;
	word	statement;
	word	device;
	word	text;
};
typedef struct stCartHeader CartHeader;




class CartridgeRomFile
{
    public:
        CartridgeRomFile();
        virtual ~CartridgeRomFile();

        const CartHeader *GetCartridgeHeader();
        bool GetEntries(SortedIntArray &entrylist);
        void ClearCartridgeInfo();
        bool HasBasic();
        bool PureBasic();
        bool isCartridge();

    protected:
        static const uint CARTRIDGE_HEADER_SIZE = 16;
		static const word CARTRIDGE_LAST_ADDRESS = 0xC000;
		static const word CARTRIDGE_HEADER_ADDRESS = 0x4000;

		bool ValidateCartridge(void *source);

    private:
		static const word ID_CARTRIDGE_ROM = 0x4142;
		static const word ID_CARTRIDGE_SUBROM = 0x4344;

        CartHeader		*cartridge_header_;

};

#endif // CARTRIDGEROMFILE_H
