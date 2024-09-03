#include "fs.h"
#include "../libkern/ide.h"
#include "../libsa/string.h"

// Max 12 files in root?
VD_Entry entries[12] = {0};

VD_Entry VD_Read(uint32_t sector)
{
	const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
	const uint32_t LBA = 0;
	const uint8_t NO_OF_SECTORS = 1;
	char buf[ATA_SECTOR_SIZE] = {0};

	VD_Entry testE;

    memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + sector, (uint32_t)buf);
    memcpy(&testE, buf, sizeof(testE));
    
    return testE;
}

VD_Entry VD_Open(const char* path)
{
	for (int i = 0; i < 12; ++i) 
	{
        if (strcmp(entries[i].Name, path) == 0) 
        {
            return entries[i]; // Return a copy of the matching entry
        }
    }
    // Return a "null" entry or a default value if not found
    VD_Entry notFound;
    strcpy(notFound.Name, "NULL");
    notFound.Attributes = 0x00;
    notFound.IsDirectory = false;
    notFound.Position = -1;
    notFound.FirstSector = -1;
    return notFound;
}

void VD_ReadData(const char* path, char* out)
{
	VD_Entry e = VD_Open(path);

	const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
	const uint32_t LBA = 0;
	const uint8_t NO_OF_SECTORS = 1;
	char buf[ATA_SECTOR_SIZE] = {0};

	memset(buf, 0, sizeof(buf));
    ide_read_sectors(DRIVE, NO_OF_SECTORS, LBA + e.FirstSector + 1, (uint32_t)buf);

    strcpy(out, buf);
}

bool VD_Write(VD_Entry file)
{
	const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
	const uint32_t LBA = 0;
	const uint8_t NO_OF_SECTORS = 1;
	char buf[ATA_SECTOR_SIZE] = {0};

	VD_Entry e = file;

    memset(buf, 0, sizeof(buf));
    memcpy(buf, &e, sizeof(e));
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA + e.FirstSector, (uint32_t)buf);

    for (int i = 0; i < 12; i++) 
    {
        if (strcmp(entries[i].Name, "") == 0) 
        {
            entries[i] = e;
            break;
        }
    }

	return true;
}

bool VD_WriteData(char* data, VD_Entry file)
{
	const int DRIVE = ata_get_drive_by_model("QEMU HARDDISK");
	const uint32_t LBA = 0;
	const uint8_t NO_OF_SECTORS = 1;
	char buf[ATA_SECTOR_SIZE] = {0};

	VD_Entry e = file;

    strcpy(buf, data);
    ide_write_sectors(DRIVE, NO_OF_SECTORS, LBA + (e.FirstSector + 1), (uint32_t)buf);

    return true;
}