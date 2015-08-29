using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Storage
{
    partial class ATA
    {
        public const byte ATA_PRIMARY = 0x00;
        public const byte ATA_SECONDARY = 0x01;

        public const byte ATA_MASTER = 0x00;
        public const byte ATA_SLAVE = 0x01;

        public const byte ATA_REG_DATA = 0x00;
        public const byte ATA_REG_ERROR = 0x01;
        public const byte ATA_REG_FEATURE = 0x01;
        public const byte ATA_REG_SECCNT = 0x02;
        public const byte ATA_REG_LBALO = 0x03;
        public const byte ATA_REG_LBAMID = 0x04;
        public const byte ATA_REG_LBAHI = 0x05;
        public const byte ATA_REG_DRIVE = 0x06;
        public const byte ATA_REG_CMD = 0x07;
        public const byte ATA_REG_STATUS = 0x07;
        public const byte ATA_REG_SECCOUNT1 = 0x08;
        public const byte ATA_REG_LBA3 = 0x09;
        public const byte ATA_REG_LBA4 = 0x0A;
        public const byte ATA_REG_LBA5 = 0x0B;
        public const byte ATA_REG_CONTROL = 0x0C;
        public const byte ATA_REG_ALTSTATUS = 0x0C;
        public const byte ATA_REG_DEVADDRESS = 0x0D;


        public const byte ATA_CMD_IDENTIFY = 0xEC;
        public const byte ATA_CMD_PIO_READ = 0x20;
        public const byte ATA_CMD_PIO_WRITE = 0x30;
        public const byte ATA_CMD_FLUSH = 0xE7;

        public const byte ATA_NO_DISK = 0x00;

        public const byte ATA_STATUS_BSY = 0x80;
        public const byte ATA_STATUS_ERR = 0x01;
        public const byte ATA_STATUS_RDY = (1 << 4);
        public const byte ATA_STATUS_SRV = 0x10;
        public const byte ATA_STATUS_DRQ = 0x08;
        public const byte ATA_STATUS_DF = 0x20;

        public const byte ATA_FEATURE_PIO = 0x00;
        public const byte ATA_FEATURE_DMA = 0x01;

        public const byte ATA_IDENT_DEVICETYPE = 0x00;
        public const byte ATA_IDENT_CYLINDERS = 0x6C;
        public const byte ATA_IDENT_HEADS = 0x6E;
        public const byte ATA_IDENT_SECTORSPT = 0x70;
        public const byte ATA_IDENT_SERIAL = 0x14;
        public const byte ATA_IDENT_MODEL = 0x36;
        public const byte ATA_IDENT_CAPABILITIES = 0x62;
        public const byte ATA_IDENT_FIELDVALID = 0x6A;
        public const byte ATA_IDENT_MAX_LBA = 0x78;
        public const byte ATA_IDENT_COMMANDSETS = 0xA4;
        public const byte ATA_IDENT_MAX_LBA_EXT = 0xC8;
    }

    struct IDE_DEVICE
    {
        public bool Exists;
        public byte Channel;

        public ushort Base;
        public byte Drive;
        public ulong Size;

        public uint cmdSet;
        public ushort type;
        public ushort capabilities;

        public byte support48;

        public ushort cylinders;
        public ushort heads;
        public ushort sectorspt;

        public string name;
    }
}
