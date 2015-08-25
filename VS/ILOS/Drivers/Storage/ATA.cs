using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Storage
{

    unsafe partial class ATA
    {
        const int ATA_PRIMARY_IO = 0x1F0;
        const int ATA_SECONDARY_IO = 0x170;

        private static IDE_DEVICE device = new IDE_DEVICE();
        
        /// <summary>
        /// Select IDE drive
        /// </summary>
        /// <param name="channel">Channel slave of master?</param>
        /// <param name="drive">Drive slave of master?</param>
        private static void select_drive(byte channel, byte drive)
        {
            if (channel == ATA_PRIMARY)
                if (drive == ATA_MASTER)
                    Portio.Out8(ATA_PRIMARY_IO + ATA_REG_DRIVE, 0xA0);
                else
                    Portio.Out8(ATA_PRIMARY_IO + ATA_REG_DRIVE, 0xB0);
            else
                if (drive == ATA_MASTER)
                    Portio.Out8(ATA_SECONDARY_IO + ATA_REG_DRIVE, 0xA0);
                else
                    Portio.Out8(ATA_SECONDARY_IO + ATA_REG_DRIVE, 0xB0);
        }

        /// <summary>
        /// IDE idenitify
        /// </summary>
        /// <param name="channel">Channel</param>
        /// <param name="drive">Slave or master?</param>
        /// <returns></returns>
        private static byte[] identify(byte channel, byte drive)
        {
            select_drive(channel, drive);

            uint @base;

            if (channel == ATA_PRIMARY)
                @base = ATA_PRIMARY_IO;
            else
                @base = ATA_SECONDARY_IO;

            Portio.Out8((ushort)(@base + ATA_REG_SECCNT), 0x00);
            Portio.Out8((ushort)(@base + ATA_REG_LBALO), 0x00);
            Portio.Out8((ushort)(@base + ATA_REG_LBAMID), 0x00);
            Portio.Out8((ushort)(@base + ATA_REG_LBAHI), 0x00);

            Portio.Out8((ushort)(@base + ATA_REG_STATUS), 0x00);

            byte status = Portio.In8((ushort)(@base + ATA_REG_STATUS));
            if (status == 0)
                return null;

            while ((Portio.In8((ushort)(@base + ATA_REG_STATUS)) & ATA_STATUS_BSY) != 0)
                status = Portio.In8((ushort)(@base + ATA_REG_STATUS));

            while(true)
            {
                status = Portio.In8((ushort)(@base + ATA_REG_STATUS));

                if ((status & ATA_STATUS_ERR) != 0)
                    return null;

                if ((status ^ ATA_STATUS_DRQ) != 0)
                    break;
            }

            byte[] ide_buf = new byte[256];
            
            for(int i = 0; i < 128; i++)
            {
                ushort shrt = Portio.In16((ushort)(@base + ATA_REG_DATA));
                int offset = i * 2;
                ide_buf[offset + 0] = (byte)((shrt >> 8) & 0xFF);
                ide_buf[offset + 1] = (byte)((shrt) & 0xFF);
            }

            return ide_buf;
        }

        /// <summary>
        /// IDE Prope
        /// </summary>
        private static void Probe()
        {
            uint @base = ATA_PRIMARY_IO;
            byte channel = ATA_PRIMARY;
            byte drive = ATA_PRIMARY;

            device.Base = @base;
            device.Channel = channel;
            device.Drive = drive;


        }

        /// <summary>
        /// Initialize first ATA Disk
        /// </summary>
        public static void Init()
        {
            Probe();
        }
    }
}
