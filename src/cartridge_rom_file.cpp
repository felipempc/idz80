/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   09-11-2009 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Controls Cartridge ROM files
 **************************************************************/



#include "cartridge_rom_file.h"



const uint CartridgeRomFile::CARTRIDGE_HEADER_SIZE;
const word CartridgeRomFile::ID_CARTRIDGE_ROM;
const word CartridgeRomFile::ID_CARTRIDGE_SUBROM;
const word CartridgeRomFile::CARTRIDGE_HEADER_ADDRESS;
const word CartridgeRomFile::CARTRIDGE_LAST_ADDRESS;




CartridgeRomFile::CartridgeRomFile()
{
    cartridge_header_ = 0;
}




CartridgeRomFile::~CartridgeRomFile()
{
    ClearCartridgeInfo();
}




void CartridgeRomFile::ClearCartridgeInfo()
{
    if (cartridge_header_)
        delete cartridge_header_;

    cartridge_header_ = 0;
}




bool CartridgeRomFile::ValidateCartridge(void *source)
{
	bool	is_cartridge = false,
            cart_init = false,
            cart_text = false,
            cart_statement = false,
            cart_device = false;
    byte    *header;

    if (!cartridge_header_)
        cartridge_header_ = new CartHeader;

    header = static_cast<byte *>(source);
    memcpy(cartridge_header_, source, sizeof(CartHeader));
    cartridge_header_->id = static_cast<word>((*header++ * 0x100 + *header) & 0xFFFF);


	if ((cartridge_header_->id == ID_CARTRIDGE_ROM) || (cartridge_header_->id == ID_CARTRIDGE_SUBROM))
	{
		cart_init = ((cartridge_header_->init) && (cartridge_header_->init > CARTRIDGE_HEADER_ADDRESS) &&
                    (cartridge_header_->init < CARTRIDGE_LAST_ADDRESS));
        cart_statement = ((cartridge_header_->statement) && (cartridge_header_->statement > CARTRIDGE_HEADER_ADDRESS) &&
                        (cartridge_header_->statement < CARTRIDGE_LAST_ADDRESS));
        cart_device = ((cartridge_header_->device) && (cartridge_header_->device > CARTRIDGE_HEADER_ADDRESS) &&
                    (cartridge_header_->device < CARTRIDGE_LAST_ADDRESS));
        cart_text = ((cartridge_header_->text) && (cartridge_header_->text > CARTRIDGE_HEADER_ADDRESS) &&
                    (cartridge_header_->text < CARTRIDGE_LAST_ADDRESS));

		is_cartridge = cart_init || cart_statement || cart_device || cart_text;
	}

    if (!is_cartridge)
    {
        delete cartridge_header_;
        cartridge_header_ = 0;
    }


	return is_cartridge;
}




bool CartridgeRomFile::GetEntries(SortedIntArray &entrylist)
{
    bool success = false;

    entrylist.clear();

    if (cartridge_header_)
    {
        if (cartridge_header_->init)
            entrylist.push_back(cartridge_header_->init);
        if (cartridge_header_->statement)
            entrylist.push_back(cartridge_header_->statement);
        if (cartridge_header_->device)
            entrylist.push_back(cartridge_header_->device);
    }

    if (entrylist.size() > 0)
        success = true;

    return success;
}




const CartHeader *CartridgeRomFile::GetCartridgeHeader()
{
    return cartridge_header_;
}




bool CartridgeRomFile::HasBasic()
{
    return (cartridge_header_ && cartridge_header_->text);
}




bool CartridgeRomFile::PureBasic()
{
    return (cartridge_header_ && !cartridge_header_->device && !cartridge_header_->init &&
            !cartridge_header_->statement && cartridge_header_->text);
}




bool CartridgeRomFile::isCartridge()
{
    return (cartridge_header_);
}




