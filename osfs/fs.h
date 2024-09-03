#ifndef FS_H
#define FS_H

#include "../libsa/stdio.h"

typedef struct 
{
    uint8_t Name[11];			// 8.3 format
    uint8_t Attributes;			// Check VD_Attributes
    uint8_t _Reserved;			// Reserved (Zero)
    uint16_t CreatedTime;		// Time created
    uint16_t CreatedDate;		// Date created
    uint16_t AccessedDate;		// Accessed date
    uint16_t FirstSector;		// First sector, which sector data is stored in
    uint16_t ModifiedTime;		// Modified time
    uint16_t ModifiedDate;		// Modified date
} VD_Entry;

typedef struct 
{
    int Handle;			// ID
    bool IsDirectory;	// Is it a directory?
} VD_File;

enum VD_Attributes
{
    VD_ATTRIBUTE_READ_ONLY         = 0x01,
    VD_ATTRIBUTE_HIDDEN            = 0x02,
    VD_ATTRIBUTE_SYSTEM            = 0x04,
    VD_ATTRIBUTE_DIRECTORY         = 0x10,
    VD_ATTRIBUTE_ARCHIVE           = 0x20,
    VD_ATTRIBUTE_NONE              = 0x00
};

VD_File VD_Open(const char* path);
//uint32_t VD_Read(VD_File file);
bool VD_Write(VD_File file);

#endif