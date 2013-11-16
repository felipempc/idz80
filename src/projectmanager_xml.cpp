/**************************************************************
 * Name:      IDZ80
 * Purpose:   Interactive Disassembler for Z80 processors
 * Author:    Felipe MPC (idz80a@gmail.com)
 * Created:   06-04-2013 (D-M-Y)
 * License:   GPLv3 (http://www.gnu.org/licenses/gpl-3.0.html)
 **************************************************************
 * Load / save a project in a XML file
 **************************************************************/

#include <wx/dynarray.h>
#include <wx/stopwatch.h>
#include <wx/filefn.h>

#include "projectmanager_xml.h"
#include "mnemonicitem.h"
#include "mndb_tools.h"


const wxString ProjectManagerXML::PROJECT_EXTENSION = ".dap";
const uint ProjectManagerXML::PROJECT_FILE_VERSION = 0;
const wxString ProjectManagerXML::ROOT_STR = "IDZ80";
const wxString ProjectManagerXML::SECTION_CODEVIEWLINE_STR = "CODEVIEWLINE";
const wxString ProjectManagerXML::SECTION_DISASSEMBLED_STR = "DISASSEMBLED";
const wxString ProjectManagerXML::SECTION_FILEPROPERTIES_STR = "FILEPROPERTIES";
const wxString ProjectManagerXML::SECTION_PROGRAMLABEL_STR = "PROGRAMLABEL";
const wxString ProjectManagerXML::SECTION_VARLABEL_STR = "VARLABEL";
const wxString ProjectManagerXML::SECTION_IOLABEL_STR = "IOLABEL";
const wxString ProjectManagerXML::SECTION_CONSTANTLABEL_STR = "CONSTANTLABEL";

const wxString ProjectManagerXML::SUBSECTION_FILENAME_STR = "NAME";
const wxString ProjectManagerXML::SUBSECTION_FILEPATH_STR = "PATH";
const wxString ProjectManagerXML::SUBSECTION_FILETYPE_STR = "TYPE";
const wxString ProjectManagerXML::SUBSECTION_CARTRIDGE_STR = "CARTRIDGE";
const wxString ProjectManagerXML::SUBSECTION_ADDRESS_STR = "ADDRESS";
const wxString ProjectManagerXML::SUBSECTION_LABEL_STR = "LABEL";
const wxString ProjectManagerXML::SUBSECTION_OPCODE_STR = "ITEM";
const wxString ProjectManagerXML::SUBSECTION_LINE_STR = "LINE";

const wxString ProjectManagerXML::ATTRIBUTE_VERSION_STR = "VERSION";
const wxString ProjectManagerXML::ATTRIBUTE_FILENAME_STR = "FILENAME";
const wxString ProjectManagerXML::ATTRIBUTE_ORIGINALPATH_STR = "ORIGINAL_PATH";
const wxString ProjectManagerXML::ATTRIBUTE_FILETYPE_STR = "FILETYPE";
const wxString ProjectManagerXML::ATTRIBUTE_STARTADDRESS_STR = "START";
const wxString ProjectManagerXML::ATTRIBUTE_EXECUTIONADDRESS_STR = "EXECUTION";
const wxString ProjectManagerXML::ATTRIBUTE_ENDADDRESS_STR = "END";
const wxString ProjectManagerXML::ATTRIBUTE_TOTALLINES_STR = "TOTAL_LINES";
const wxString ProjectManagerXML::ATTRIBUTE_LABELUSERS_STR = "LABEL_USERS";
const wxString ProjectManagerXML::ATTRIBUTE_OPCODETYPE_STR = "TYPE";
const wxString ProjectManagerXML::ATTRIBUTE_OPCODELENGTH_STR = "LENGTH";
const wxString ProjectManagerXML::ATTRIBUTE_OPCODEOFFSET_STR = "OFFSET";
const wxString ProjectManagerXML::ATTRIBUTE_OPCODE_STR = "OPCODE";
const wxString ProjectManagerXML::ATTRIBUTE_ARGUMENTNUM_STR = "NUM_ARGS";
const wxString ProjectManagerXML::ATTRIBUTE_ARGUMENTSIZE_STR = "SIZE_ARG";
const wxString ProjectManagerXML::ATTRIBUTE_ARGUMENTS_STR = "ARGUMENTS";
const wxString ProjectManagerXML::ATTRIBUTE_ORG_STR = "ORG";
const wxString ProjectManagerXML::ATTRIBUTE_DASMITEM_STR = "DASMITEM";
const wxString ProjectManagerXML::ATTRIBUTE_LINEPROGRAMLABEL_STR = "LINE_PROGRAMLABEL";
const wxString ProjectManagerXML::ATTRIBUTE_LINEVARLABEL_STR = "LINE_VARLABEL";
const wxString ProjectManagerXML::ATTRIBUTE_COMMENT_STR = "COMMENT";


ProjectManagerXML::ProjectManagerXML(ProcessData *process_data)
{
    saved = false;
    named = false;
    project_filename.Clear();
    process = process_data;
    CodeViewLines = process->CodeViewLines;
    log = 0;
}


bool ProjectManagerXML::IsSaved()
{
    return saved;
}

bool ProjectManagerXML::HasName()
{
    return named;
}

void ProjectManagerXML::New()
{
    saved = false;
    named = false;
    project_filename.Clear();
}


bool ProjectManagerXML::Save()
{
    bool ret = false;
    wxXmlDocument   xml_doc;

    if (named)
    {
        LogIt("Preparing to write...\n");
        wxStopWatch sw;
        writeHeader(xml_doc);
        writeDasmData(xml_doc);
        writeCodeLine(xml_doc);
        writeLabel(xml_doc, process->prog_labels, SECTION_PROGRAMLABEL_STR);
        writeLabel(xml_doc, process->var_labels, SECTION_VARLABEL_STR);
        writeLabel(xml_doc, process->io_labels, SECTION_IOLABEL_STR);
        writeLabel(xml_doc, process->constant_labels, SECTION_CONSTANTLABEL_STR);
        writeFileProperties(xml_doc);
        LogIt(wxString::Format("It took %dms...\n", sw.Time()));
        LogIt("Now saving...\n");
        sw.Start(0);
        ret = xml_doc.Save(project_filename, 4);
        LogIt(wxString::Format("It took %dms...\n", sw.Time()));

    }
    else
        ret = false;

    return ret;
}


bool ProjectManagerXML::Save(const wxString &filename)
{
    SetFilename(filename);
    return Save();
}



void ProjectManagerXML::writeHeader(wxXmlDocument &doc)
{
    wxXmlNode *root;

    doc.SetFileEncoding("UTF-8");
    doc.SetVersion("1.0");

    root = new wxXmlNode(wxXML_ELEMENT_NODE, ROOT_STR);
    root->AddAttribute(ATTRIBUTE_VERSION_STR, wxString::Format("%.4d", PROJECT_FILE_VERSION));
    doc.SetRoot(root);
}

void ProjectManagerXML::writeFileProperties(wxXmlDocument &doc)
{
    wxXmlNode *section, *items;
    wxString str;

    section = new wxXmlNode(doc.GetRoot(), wxXML_ELEMENT_NODE, SECTION_FILEPROPERTIES_STR);
    items = new wxXmlNode(section, wxXML_ELEMENT_NODE, SUBSECTION_FILENAME_STR);
    items->AddAttribute(ATTRIBUTE_FILENAME_STR, process->Program->GetFileName());
    items = new wxXmlNode(section, wxXML_ELEMENT_NODE, SUBSECTION_FILEPATH_STR);
    items->AddAttribute(ATTRIBUTE_ORIGINALPATH_STR, process->Program->GetFilePath());

    switch (process->Program->GetFileType())
    {
        case COM:
                    str = "COM";
                    break;
        case BIN:
                    str = "BIN";
                    break;
        case ROM:
                    str = "ROM";
                    break;
        default:
                    str = "UNKNOWN";
                    break;
    }
    items = new wxXmlNode(section, wxXML_ELEMENT_NODE, SUBSECTION_FILETYPE_STR);
    items->AddAttribute(ATTRIBUTE_FILETYPE_STR, str);

    if (process->Program->isCartridge())
    {
        items = new wxXmlNode(section, wxXML_ELEMENT_NODE, SUBSECTION_CARTRIDGE_STR);
    }

    items = new wxXmlNode(section, wxXML_ELEMENT_NODE, SUBSECTION_ADDRESS_STR);
    items->AddAttribute(ATTRIBUTE_STARTADDRESS_STR, wxString::Format("%X", process->Program->StartAddress));
    items->AddAttribute(ATTRIBUTE_EXECUTIONADDRESS_STR, wxString::Format("%X", process->Program->ExecAddress));
    items->AddAttribute(ATTRIBUTE_ENDADDRESS_STR, wxString::Format("%X", process->Program->EndAddress));
}


void ProjectManagerXML::writeLabel(wxXmlDocument &doc, LabelListCtrl *current_label, const wxString &section_name)
{
    wxXmlNode *section, *items;
    wxString str;
    LabelItem *label;
    int     i;
    uint    j, total_lines;

    if (!current_label->IsEmpty())
    {
        section = new wxXmlNode(doc.GetRoot(), wxXML_ELEMENT_NODE, section_name);
        total_lines = current_label->GetCount();
        section->AddAttribute(ATTRIBUTE_TOTALLINES_STR, wxString::Format("%d", total_lines));

        for (i = (total_lines - 1); i > -1; i--)
        {
            label = current_label->GetData(i);
            if (label)
            {
                items = new wxXmlNode(section, wxXML_ELEMENT_NODE, wxString::Format("%s_%d", SUBSECTION_LABEL_STR, i));
                items->AddAttribute(label->LabelStr, wxString::Format("%X", label->Address));

                if (label->LabelUsers)
                {
                    str.Clear();
                    for (j = 0; j < label->LabelUsers->GetCount(); j++)
                        str << wxString::Format(" %d",label->LabelUsers->Item(j));
                    str.Trim(false);
                    items->AddAttribute(ATTRIBUTE_LABELUSERS_STR, str);
                }
            }
        }
    }
}

/* Saves dasm structure
 *
 */
void ProjectManagerXML::writeDasmData(wxXmlDocument &doc)
{
    wxXmlNode *section, *items;
    int		i, c;
    uint	j, arg_aux, total_lines;
    wxString	aux_str;
    DisassembledItem *de;

    if (process->Disassembled->GetCount() != 0)
    {
        section = new wxXmlNode(doc.GetRoot(), wxXML_ELEMENT_NODE, SECTION_DISASSEMBLED_STR);
        total_lines = process->Disassembled->GetCount();
        section->AddAttribute(ATTRIBUTE_TOTALLINES_STR, wxString::Format("%d", total_lines));

        for (i = (total_lines - 1); i > -1; i--)
        {
            de = process->Disassembled->GetData(i);

            items = new wxXmlNode(section, wxXML_ELEMENT_NODE, wxString::Format("%s_%d", SUBSECTION_OPCODE_STR, i));
            items->AddAttribute(ATTRIBUTE_OPCODETYPE_STR, wxString::Format("%.2X", de->GetType()));
            items->AddAttribute(ATTRIBUTE_OPCODELENGTH_STR, wxString::Format("%.2X", de->GetLength()));
            items->AddAttribute(ATTRIBUTE_OPCODEOFFSET_STR, wxString::Format("%.4X", de->GetOffset()));

            aux_str.Clear();
            for(j = 0; j < de->GetLength(); j++)
                aux_str << wxString::Format("%.2X ", de->GetByteOpCode(j));

            aux_str.Trim();
            items->AddAttribute(ATTRIBUTE_OPCODE_STR, aux_str);

			if (de->GetArgumentCount() > 0)
			{
                items->AddAttribute(ATTRIBUTE_ARGUMENTNUM_STR, wxString::Format("%.2X", de->GetArgumentCount()));
                items->AddAttribute(ATTRIBUTE_ARGUMENTSIZE_STR, wxString::Format("%.2X", de->GetArgumentSize()));

				arg_aux = de->GetArgumentCount() * de->GetArgumentSize();
				aux_str.Clear();
				for(j = 0; j < arg_aux; j++)
				{
					c = de->GetByteArgument(j);
					c = c & 0xFF;
					aux_str << wxString::Format("%.2X ", c);
				}
				aux_str.Trim();
                items->AddAttribute(ATTRIBUTE_ARGUMENTS_STR, aux_str);
			}
        }
    }
}


void ProjectManagerXML::writeCodeLine(wxXmlDocument &doc)
{
    wxXmlNode *section, *item;
    int i;
    CodeViewItem *cvi;
    uint    total_lines;


    if (CodeViewLines->GetCount() > 0)
    {
        section = new wxXmlNode(doc.GetRoot(), wxXML_ELEMENT_NODE, SECTION_CODEVIEWLINE_STR);
        total_lines = CodeViewLines->GetCount();
        section->AddAttribute(ATTRIBUTE_TOTALLINES_STR, wxString::Format("%d", total_lines));

        for (i = (total_lines - 1); i > -1; i--)
        {
            cvi = CodeViewLines->getData(i);
            item = new wxXmlNode(section, wxXML_ELEMENT_NODE, wxString::Format("%s_%d", SUBSECTION_LINE_STR, i));
            if (cvi->Org >= 0)
                item->AddAttribute(ATTRIBUTE_ORG_STR, wxString::Format("%d", cvi->Org));

            if (cvi->Dasmitem >= 0)
                item->AddAttribute(ATTRIBUTE_DASMITEM_STR, wxString::Format("%d", cvi->Dasmitem));

            if (cvi->LabelProgAddr > 0)
                item->AddAttribute(ATTRIBUTE_LINEPROGRAMLABEL_STR, wxString::Format("%d", cvi->LabelProgAddr->Address));

            if (cvi->LabelVarAddr > 0)
                item->AddAttribute(ATTRIBUTE_LINEVARLABEL_STR, wxString::Format("%d", cvi->LabelVarAddr->Address));

            if (cvi->Comment)
                item->AddAttribute(ATTRIBUTE_COMMENT_STR, cvi->Comment->utf8_str());
        }
    }
}




bool ProjectManagerXML::Open(const wxString &filename, const wxString &syslabels_path)
{
    bool ret;
    wxXmlDocument   xml_doc;


    ret = xml_doc.Load(filename);
    if (ret)
    {
        readHeader(xml_doc);
        readFileProperties(xml_doc);
        if (process->SetupSystemLabels())
        {
            process->LoadSystemLabels(syslabels_path);
        }
        readDasmData(xml_doc);
        readCodeLine(xml_doc);
        readLabel(xml_doc, process->io_labels, SECTION_IOLABEL_STR);
        readLabel(xml_doc, process->var_labels, SECTION_VARLABEL_STR);
        readLabel(xml_doc, process->prog_labels, SECTION_PROGRAMLABEL_STR);
        readLabel(xml_doc, process->constant_labels, SECTION_CONSTANTLABEL_STR);
    }
    return ret;
}






bool ProjectManagerXML::readHeader(wxXmlDocument &doc)
{
    wxXmlNode   *node;
    wxString    str;

    node = doc.GetRoot();
    if (node->GetName() != ROOT_STR)
        return false;

    str = node->GetAttribute(ATTRIBUTE_VERSION_STR);
    if (str.IsEmpty())
        LogIt("Version not found !!\n");
    else
        LogIt("Version is " + str + ".\n");

    return true;
}



bool ProjectManagerXML::readFileProperties(wxXmlDocument &doc)
{
    wxXmlNode   *node, *section;
    wxString    str, fullfilename, typeStr;
    long        conv;
    uint        start_address, execution_address, end_address;


    section = findSection(doc.GetRoot()->GetChildren(), SECTION_FILEPROPERTIES_STR);
    if (section)
    {
        LogIt(wxString::Format("Found Configuration section in line %d !!\n", section->GetLineNumber()));
    }
    else
    {
        LogIt("Configuration section NOT found !!\n");
        return false;
    }

    node = findSection(section->GetChildren(), SUBSECTION_FILENAME_STR);
    if (!node)
        return false;
    fullfilename = node->GetAttribute(ATTRIBUTE_FILENAME_STR);
    if (fullfilename.IsEmpty())
    {
        LogIt("File name NOT found !!\n");
        return false;
    }

    node = findSection(section->GetChildren(), SUBSECTION_FILEPATH_STR);
    if (!node)
        return false;
    str = node->GetAttribute(ATTRIBUTE_ORIGINALPATH_STR);
    if (str.IsEmpty())
    {
        LogIt("Original path NOT found !!\n");
        return false;
    }
    fullfilename = str + "\\" + fullfilename;

    if (wxFileExists(fullfilename))
        LogIt("Original file:" + fullfilename + ", found !!\n");
    else
    {
        LogIt("Original file NOT found !\n");
        return false;
    }

    node = findSection(section->GetChildren(), SUBSECTION_FILETYPE_STR);
    if (!node)
        return false;
    typeStr = node->GetAttribute(ATTRIBUTE_FILETYPE_STR);
    LogIt("File type is " + typeStr + "\n");


    node = findSection(section->GetChildren(), SUBSECTION_ADDRESS_STR);
    if (!node)
        return false;
    str = node->GetAttribute(ATTRIBUTE_STARTADDRESS_STR);
    if (!str.IsEmpty())
    {
        if (!str.ToLong(&conv, 16))
        {
            LogIt("Invalid Start(" + str +") address!\n");
            return false;
        }
    }
    LogIt("Start =" + str + "\n");
    start_address = conv;

    str = node->GetAttribute(ATTRIBUTE_EXECUTIONADDRESS_STR);
    if (!str.IsEmpty())
    {
        if (!str.ToLong(&conv, 16))
        {
            LogIt("Invalid Execution(" + str +") address!\n");
            return false;
        }
    }
    LogIt("Execution =" + str + "\n");
    execution_address = conv;

    str = node->GetAttribute(ATTRIBUTE_ENDADDRESS_STR);
    if (!str.IsEmpty() && !str.ToLong(&conv, 16))
    {
        LogIt("Invalid End(" + str +") address!\n");
        return false;
    }
    LogIt("End =" + str + "\n");
    end_address = conv;

    process->Program->Open(fullfilename);
    process->Program->SetStrFileType(typeStr);
    process->Program->StartAddress = start_address;
    process->Program->ExecAddress = execution_address;
    process->Program->EndAddress = end_address;


    return true;
}



void ProjectManagerXML::readLabel(wxXmlDocument &doc, LabelListCtrl *current_label, const wxString &labelstr)
{
    wxXmlNode   *node, *section;
    wxXmlAttribute *attribute;
    wxString		str_name, str_addr, str_users;
    wxArrayString	arrstr;
    wxArrayInt		labelusers;
    uint			addr, i, linecount, total_lines;
    long			conv;


    section = findSection(doc.GetRoot()->GetChildren(), labelstr);

    if (section)
        LogIt("Found label [" + labelstr + "]\n");
    else
        return;

    str_addr = section->GetAttribute(ATTRIBUTE_TOTALLINES_STR);
    if (!str_addr.IsEmpty() && str_addr.ToLong(&conv))
    {
        total_lines = static_cast<uint>(conv);
    }
    else
    {
        LogIt("Fatal error ! TotalLines attribute is missing !\n");
        return;
    }

    node = section->GetChildren();

    linecount = 0;
    while (node)
    {
        attribute = node->GetAttributes();
        if (attribute)
        {
            str_name = attribute->GetName();
            str_addr = attribute->GetValue();
            str_users = node->GetAttribute(ATTRIBUTE_LABELUSERS_STR);
            ParseString(str_users, arrstr);
            for(i = 0; i < arrstr.Count(); i++)
                if (arrstr.Item(i).ToLong(&conv))
                    labelusers.Add(static_cast<int>(conv));
        }

        if (str_addr.ToLong(&conv, 16) && !labelusers.IsEmpty())
        {
            addr = static_cast<uint>(conv);
            current_label->AddLabel(addr, str_name, labelusers);
            linkLabels(&labelusers);
            linecount++;
        }

        labelusers.Clear();
        str_addr.Clear();

        node = node->GetNext();
    }

    if (linecount != total_lines)
        LogIt(wxString::Format("Failed Labels = %d\n", (total_lines - linecount)));
}


/* Loads dasm structure from string lines
 */
void ProjectManagerXML::readDasmData(wxXmlDocument &doc)
{
    wxXmlNode   *section, *node;
    wxString    str;
    uint    total_lines, linecount;
    long    conv;

    section = findSection(doc.GetRoot()->GetChildren(), SECTION_DISASSEMBLED_STR);
    if (!section)
    {
        LogIt("Failed to load DISASSEMBLED section\n");
        return;
    }

    str = section->GetAttribute(ATTRIBUTE_TOTALLINES_STR);
    if (!str.IsEmpty() && str.ToLong(&conv))
    {
        total_lines = static_cast<uint>(conv);
    }
    else
    {
        LogIt("Fatal error ! TotalLines attribute is missing !\n");
        return;
    }

    node = section->GetChildren();

    linecount = 0;
    while (node)
    {
        if (fillDasmData(node))
            linecount++;
        node = node->GetNext();
    }

    if (linecount != total_lines)
        LogIt(wxString::Format("Failed disassembled items = %d\n", (total_lines - linecount)));
}


bool ProjectManagerXML::fillDasmData(wxXmlNode *datanode)
{
    wxString	str;
    uint		i;
    long		conv;
    wxArrayString   arr_str;
    uint		len,
                arg_num = 0,
				arg_size = 0,
				arg_aux = 0;
    ByteCode		bc;
    OpCodeArguments	args;
    DisassembledItem		*de;
    MnemonicItem	*mi;
    bool    hasArguments = false;
    uint    health = 0;

    de = new DisassembledItem(process->Program);

    str = datanode->GetAttribute(ATTRIBUTE_OPCODETYPE_STR);
    if (!str.IsEmpty() && str.ToLong(&conv, 16))
    {
        de->SetType(static_cast<enum ElementType>(conv));
        health = 1;
    }

    str = datanode->GetAttribute(ATTRIBUTE_OPCODELENGTH_STR);
    if (!str.IsEmpty() && str.ToLong(&conv, 16))
    {
        len = static_cast<uint>(conv);
        de->SetLength(len);
        health += 2;
    }

    str = datanode->GetAttribute(ATTRIBUTE_OPCODEOFFSET_STR);
    if (!str.IsEmpty() && str.ToLong(&conv, 16))
    {
        de->SetOffset(static_cast<uint>(conv));
        health += 4;
    }


    str = datanode->GetAttribute(ATTRIBUTE_OPCODE_STR);
    if (!str.IsEmpty() && len)
    {
        ParseString(str, arr_str);
        memset(&bc, 0, MAX_OPCODE_SIZE);
        for (i = 0; i < len; i++)
        {
            str = arr_str[i];
            str.ToLong(&conv, 16);
            if (i < sizeof(ByteCode))
                bc[i] = static_cast<unsigned char>(conv);
        }
        de->CopyOpCode(bc);
        mi = process->Mnemonics->FindByOpCode(bc);
        if (mi != 0)
        {
            de->SetMnemonic(mi);
            health += 8;
            hasArguments = mi->HasArgument();
        }
    }

    if (health < 0xF)
    {
        LogIt(wxString::Format("[%d] Failed to load new instruction (health = %X)\n", datanode->GetLineNumber(), health));
        delete de;
        return false;
    }

    if (hasArguments)
    {
        str = datanode->GetAttribute(ATTRIBUTE_ARGUMENTNUM_STR);
        if (!str.IsEmpty() && str.ToLong(&conv, 16))
        {
            arg_num = static_cast<uint>(conv);
            health += 16;
        }

        str = datanode->GetAttribute(ATTRIBUTE_ARGUMENTSIZE_STR);
        if (!str.IsEmpty() && str.ToLong(&conv, 16))
        {
            arg_size = static_cast<uint>(conv);
            health += 32;
        }

        arg_aux = arg_size * arg_num;

        str = datanode->GetAttribute(ATTRIBUTE_ARGUMENTS_STR);
        if (!str.IsEmpty() && arg_aux)
        {
            memset(&args, 0, sizeof(OpCodeArguments));
            ParseString(str, arr_str);
            for (i = 0; i < arg_aux; i++)
            {
                str = arr_str[i];
                str.ToLong(&conv, 16);
                if (i < sizeof(OpCodeArguments))
                    args[i] = static_cast<unsigned char>(conv);
            }
            de->CopyArguments(args, arg_aux);
            health += 64;
        }

        if (health < 0x7F)
        {
            LogIt(wxString::Format("[%d] Failed to load new instruction arguments (health = %X)\n", datanode->GetLineNumber(), health));
            delete de;
            return false;
        }
    }

    process->Disassembled->AddDasm(de);
    return true;
}




void ProjectManagerXML::readCodeLine(wxXmlDocument &doc)
{
    wxXmlNode   *section, *node;
    wxString    str;
    uint    total_lines, linecount;
    long    conv;


    section = findSection(doc.GetRoot()->GetChildren(), SECTION_CODEVIEWLINE_STR);
    if (!section)
    {
        LogIt("Failed to load CodeViewLine section\n");
        return;
    }
    else
        LogIt("Found CodeviewLines \n");

    str = section->GetAttribute(ATTRIBUTE_TOTALLINES_STR);
    if (!str.IsEmpty() && str.ToLong(&conv))
    {
        total_lines = static_cast<uint>(conv);
    }
    else
    {
        LogIt("Fatal error ! TotalLines attribute is missing !\n");
        return;
    }


    node = section->GetChildren();

    linecount = 0;
    while (node)
    {
        if (fillCodeViewLine(node))
            linecount++;
        node = node->GetNext();
    }

    if (linecount != total_lines)
        LogIt(wxString::Format("Failed code view lines = %d\n", (total_lines - linecount)));
}



bool ProjectManagerXML::fillCodeViewLine(wxXmlNode *datanode)
{
    wxString	str, str_comment;
    long		conv;
    bool    commentary_line;

    int health = 0;


    str_comment = datanode->GetAttribute(ATTRIBUTE_COMMENT_STR);
    commentary_line = !str_comment.IsEmpty();


    str = datanode->GetAttribute(ATTRIBUTE_ORG_STR);

    if (!str.IsEmpty() && str.ToLong(&conv))
    {
        CodeViewLines->AddOrg(static_cast<int>(conv), str_comment);
        commentary_line = false;
        health++;
    }

    str = datanode->GetAttribute(ATTRIBUTE_DASMITEM_STR);
    if (!str.IsEmpty() && str.ToLong(&conv))
    {
        CodeViewLines->AddDasm(static_cast<int>(conv), str_comment);
        commentary_line = false;
        health++;
    }

    str = datanode->GetAttribute(ATTRIBUTE_LINEPROGRAMLABEL_STR);
    if (!str.IsEmpty() && str.ToLong(&conv))
    {
        CodeViewLines->AddProgLabel(static_cast<int>(conv), str_comment);
        commentary_line = false;
        health++;
    }

    str = datanode->GetAttribute(ATTRIBUTE_LINEVARLABEL_STR);
    if (!str.IsEmpty() && str.ToLong(&conv))
    {
        CodeViewLines->AddVarLabel(static_cast<int>(conv), str_comment);
        commentary_line = false;
        health++;
    }

    if (commentary_line)
    {
        CodeViewLines->Add(str_comment);
        health++;
    }

    // Empty line
    if (health == 0)
    {
        CodeViewLines->Add("");

        #ifdef IDZ80DEBUG
        LogIt(wxString::Format("Empty line: %d\n", datanode->GetLineNumber()));
        #endif // IDZ80DEBUG
    }

    return (health != 0);

}



void ProjectManagerXML::linkLabels(wxArrayInt *label_users)
{
    DisassembledItem *de;
    uint i, item;

    if (label_users > 0)
        for(i = 0; i < label_users->GetCount(); i++)
        {
            item = static_cast<uint>(label_users->Item(i));
            de = process->Disassembled->GetData(item);
            if (de != 0)
                de->SetArgLabel();
        }
}



wxXmlNode *ProjectManagerXML::findSection(wxXmlNode *firstchild, const wxString &section)
{
    bool foundsection = false;
    wxXmlNode *foundnode = 0;

    while (firstchild && !foundsection)
    {
        if (firstchild->GetName() == section)
        {
            foundsection = true;
            foundnode = firstchild;
        }
        firstchild = firstchild->GetNext();
    }
    return foundnode;
}


void ProjectManagerXML::SetFilename(const wxString &filename)
{
    int i;

    if (!filename.IsEmpty())
    {
        named = true;
        i = filename.Find('.');
        if (i > 0)
        {
            project_filename = filename.Left(i);
            project_filename << PROJECT_EXTENSION;
        }
        else
            project_filename = filename + PROJECT_EXTENSION;
    }
}


wxString ProjectManagerXML::GetFilename()
{
    int i;
    wxString str;

    i = project_filename.Find('.');
    if (i > 0)
        str = project_filename.Left(i);
    else
        str = project_filename;
    return str;
}


void ProjectManagerXML::SetLog(wxTextCtrl *set_log)
{
    log = set_log;
}


void ProjectManagerXML::LogIt(const wxString &str)
{
    if (log)
        log->AppendText(str);
}
