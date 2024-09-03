#ifndef FS_H
#define FS_H

#include "../libsa/stdio.h"

typedef struct 
{
    char Name[11];			// 8.3 format
    uint8_t Attributes;
    bool IsDirectory;
    uint32_t Position;
    uint32_t FirstSector;	// First sector, which sector data is stored in
} VD_Entry;

// typedef struct 
// {
//     int Handle;			// ID
//     bool IsDirectory;	// Is it a directory?
//     VD_Entry entry;
// } VD_File;

enum VD_Attributes
{
    VD_ATTRIBUTE_READ_ONLY         = 0x01,
    VD_ATTRIBUTE_HIDDEN            = 0x02,
    VD_ATTRIBUTE_SYSTEM            = 0x04,
    VD_ATTRIBUTE_DIRECTORY         = 0x10,
    VD_ATTRIBUTE_ARCHIVE           = 0x20,
    VD_ATTRIBUTE_NONE              = 0x00
};

VD_Entry VD_Open(const char* path);
VD_Entry VD_Read(uint32_t sector);
bool VD_Write(VD_Entry file);
bool VD_WriteData(char* data, VD_Entry file);
void VD_ReadData(const char* path, char* out);

#endif