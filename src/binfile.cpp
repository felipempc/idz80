/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   26-12-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Binary file controls
 **************************************************************/

#include "binfile.hpp"


const byte BinFile::BIN_ID;
const uint BinFile::BIN_HEADER_SIZE;




BinFile::BinFile()
{
    bin_header_ = 0;
}

BinFile::~BinFile()
{
    ClearBin();
}


void BinFile::ClearBin()
{
    if (bin_header_)
        delete bin_header_;
    bin_header_ = 0;
}


const BinHeader *BinFile::GetBinHeader()
{
    return bin_header_;
}




bool BinFile::FillAndValidateBIN(void *source)
{
    bool	ret = false;
    byte *header;

    if (!bin_header_)
        bin_header_= new BinHeader;

    if (bin_header_)
    {
        header = static_cast<byte *>(source);
        bin_header_->id = static_cast<byte>(*header++ & 0xFF);
        bin_header_->start_address = static_cast<word>((*header++ + *header++ * 0x100) & 0xFFFF);
        bin_header_->end_address = static_cast<word>((*header++ + *header++ * 0x100) & 0xFFFF);
        bin_header_->execution_address = static_cast<word>((*header++ + *header * 0x100) & 0xFFFF);
        //memcpy(bin_header_, source, sizeof(BinHeader));

        if ((bin_header_->id == BIN_ID) &&
            (bin_header_->end_address > bin_header_->start_address) &&
            (bin_header_->end_address > bin_header_->execution_address) &&
            (bin_header_->execution_address >= bin_header_->start_address))
            ret = true;
        else
        {
            delete bin_header_;
            bin_header_ = 0;
        }

    }
    return ret;
}







