using System.Linq;

namespace ILOS.Drivers.Storage
{
    unsafe partial class ATA
    {

        private static IDE_DEVICE[] devices = new IDE_DEVICE[4];
        public static IDE_DEVICE[] Devices
        {
            get { return devices; }
        }
        
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
            // Select correct drive
            select_drive(channel, drive);

            // Select base port for ATA drive
            uint @base;
            if (channel == ATA_PRIMARY)
                @base = ATA_PRIMARY_IO;
            else
                @base = ATA_SECONDARY_IO;

            // Set to first LBA
            Portio.Out8(@base + ATA_REG_SECCNT, 0x00);
            Portio.Out8(@base + ATA_REG_LBALO, 0x00);
            Portio.Out8(@base + ATA_REG_LBAMID, 0x00);
            Portio.Out8(@base + ATA_REG_LBAHI, 0x00);

            Portio.Out8(@base + ATA_REG_CMD, ATA_CMD_IDENTIFY);

            // Check if a drive is found
            byte status = Portio.In8(@base + ATA_REG_STATUS);
            if (status == 0)
            {
                return null;
            }
                

            // Wait until drive is not busy anymore
            while ((status & ATA_STATUS_BSY) != 0)
                status = Portio.In8(@base + ATA_REG_STATUS);

            while(true)
            {
                status = Portio.In8(@base + ATA_REG_STATUS);

                if ((status & ATA_STATUS_ERR) != 0)
                    return null;

                if ((status & ATA_STATUS_DRQ) != 0)
                    break;
            }

            // Read data from ATA drive
            byte[] ide_buf = new byte[256];
            for(int i = 0; i < 128; i++)
            {
                ushort shrt = Portio.In16(@base + ATA_REG_DATA);
                int offset = i * 2;
                ide_buf[offset + 0] = (byte)((shrt >> 8) & 0xFF);
                ide_buf[offset + 1] = (byte)((shrt) & 0xFF);
            }

            return ide_buf;
        }

        /// <summary>
        /// Wait for drive to be finished
        /// </summary>
        /// <param name="@base">Base IO base</param>
        private static void ata_poll(uint @base)
        {
            for (int i = 0; i < 4; i++)
                Portio.In8(@base + ATA_REG_ALTSTATUS);

            byte status = Portio.In8(@base + ATA_REG_STATUS);
            while ((status & ATA_STATUS_BSY) > 0)
                status = Portio.In8(@base + ATA_REG_STATUS);
            
            while((status & ATA_STATUS_DRQ) == 0)
            {
                status = Portio.In8(@base + ATA_REG_STATUS);

                if ((status & ATA_STATUS_DF) > 0)
                    Panic.Do("Device fault!");
                
                if ((status & ATA_STATUS_ERR) > 0)
                    Panic.Do("ERR IN ATA!!");
            }
        }

        /// <summary>
        /// Read sectors into the output buffer and return size in bytes
        /// </summary>
        /// <param name="lba">Input LBA</param>
        /// <param name="size">Size in sectors</param>
        /// <param name="buffer">Output buffer</param>
        /// <returns></returns>
        public static int ReadSector(uint device_num, uint lba, byte size, byte[] buffer)
        {
            // The driver only supports up to 4 drivers
            if (device_num > 3)
                return 0;

            // Get IDE device from array
            IDE_DEVICE device = devices[device_num];

            // Does the drive exist?
            if (!device.Exists)
                return 0;

            uint @base = device.Base;
            int drive = device.Drive;

            int cmd = (drive == ATA_MASTER) ? 0xE0 : 0xF0;

            // Set Drive
            Portio.Out8(@base + ATA_REG_DRIVE, (byte)(cmd | (byte)((lba >> 24) & 0x0F)));

            // Set PIO MODE
            Portio.Out8(@base + ATA_REG_FEATURE, ATA_FEATURE_PIO);

            // Set size
            Portio.Out8(@base + ATA_REG_SECCNT, size);

            // Set LBA
            Portio.Out8(@base + ATA_REG_LBALO, (byte)lba);
            Portio.Out8(@base + ATA_REG_LBAMID, (byte)(lba >> 8));
            Portio.Out8(@base + ATA_REG_LBAHI, (byte)(lba >> 16));

            // Issue command
            Portio.Out8(@base + ATA_REG_CMD, ATA_CMD_PIO_READ);

            // Wait till done
            ata_poll(@base);

            // Read data
            for (int i = 0; i < size * 256; i++)
            {
                ushort @in = Portio.In16(@base + ATA_REG_DATA);
                int pos = i * 2;
                buffer[pos] = (byte)@in;
                buffer[pos + 1] = (byte)(@in >> 8);
            }

            return size * 512;
        }

        /// <summary>
        /// Write sector to drive and return size in bytes
        /// </summary>
        /// <param name="lba">Input LBA</param>
        /// <param name="size">Output size in sectors</param>
        /// <param name="buffer">Input buffer</param>
        /// <returns></returns>
        public static int WriteSector(uint device_num, uint lba, byte size, byte[] buffer)
        {
            // The driver only supports up to 4 drivers
            if (device_num > 3)
                return 0;

            // Get IDE device from array
            IDE_DEVICE device = devices[device_num];

            // Does the drive exist?
            if (!device.Exists)
                return 0;
            
            uint @base = device.Base;
            int drive = device.Drive;

            int cmd = (drive == ATA_MASTER) ? 0xE0 : 0xF0;

            // Set Drive
            Portio.Out8(@base + ATA_REG_DRIVE, (byte)(cmd | (byte)((lba >> 24) & 0x0F)));

            // Set PIO MODE
            Portio.Out8(@base + ATA_REG_FEATURE, ATA_FEATURE_PIO);

            // Set size
            Portio.Out8(@base + ATA_REG_SECCNT, size);

            // Set LBA
            Portio.Out8(@base + ATA_REG_LBALO, (byte)lba);
            Portio.Out8(@base + ATA_REG_LBAMID, (byte)(lba >> 8));
            Portio.Out8(@base + ATA_REG_LBAHI, (byte)(lba >> 16));

            // Issue command
            Portio.Out8(@base + ATA_REG_CMD, ATA_CMD_PIO_WRITE);

            // Wait till done
            ata_poll(@base);

            // Wait for 400ns
            Portio.In8(@base + ATA_REG_STATUS);
            Portio.In8(@base + ATA_REG_STATUS);

            // Write data
            for (int i = 0; i < size * 256; i++)
            {
                int pos = i * 2;
                ushort shrt = (ushort)(((buffer[pos + 1] & 0xFF) << 8) | (buffer[pos] & 0xFF));

                Portio.Out16(@base + ATA_REG_DATA, shrt);
            }

            // Flush data
            Portio.Out8((ushort)(@base + ATA_REG_CMD), ATA_CMD_FLUSH);

            // Wait till done
            byte status = Portio.In8(@base + ATA_REG_STATUS);
            while((status & ATA_STATUS_BSY) > 0)
                status = Portio.In8(@base + ATA_REG_STATUS);

            return size * 512;
        }

        /// <summary>
        /// IDE Prope
        /// </summary>
        private static void Probe()
        {
            int num = 0;

            // Let's prope 4 devices!
            while (num < 4)
            {
                ushort @base;
                byte channel;
                byte drive;

                if (num <= 1)
                {
                    @base = ATA_PRIMARY_IO;
                    channel = ATA_PRIMARY;
                }
                else
                {
                    @base = ATA_SECONDARY_IO;
                    channel = ATA_SECONDARY;
                }

                if ((num % 2) != 0)
                    drive = ATA_SLAVE;
                else
                    drive = ATA_PRIMARY;

                devices[num] = new IDE_DEVICE();
                devices[num].Base = @base;
                devices[num].Channel = channel;
                devices[num].Drive = drive;

                byte[] result = identify(channel, drive);

                if (result == null)
                {
                    devices[num].Exists = false;
                    return;
                }

                devices[num].Exists = true;

                int pos = ATA_IDENT_COMMANDSETS;
                devices[num].CmdSet = (uint)(((0xFF & result[pos]) << 24) | ((0xFF & result[pos + 1]) << 16) | ((0xFF & result[pos + 2]) << 8) | (0xFF & result[pos + 3]));

                pos = ATA_IDENT_DEVICETYPE;
                devices[num].Type = (ushort)(((result[pos + 1] & 0xFF) << 8) | (result[pos] & 0xFF));

                pos = ATA_IDENT_CAPABILITIES;
                devices[num].Capabilities = (ushort)(((result[pos + 1] & 0xFF) << 8) | (result[pos] & 0xFF));

                pos = ATA_IDENT_CYLINDERS;
                devices[num].Cylinders = (ushort)(((result[pos + 1] & 0xFF) << 8) | (result[pos] & 0xFF));

                pos = ATA_IDENT_HEADS;
                devices[num].Heads = (ushort)(((result[pos + 1] & 0xFF) << 8) | (result[pos] & 0xFF));

                pos = ATA_IDENT_SECTORSPT;
                devices[num].Sectorspt = (ushort)(((result[pos + 1] & 0xFF) << 8) | (result[pos] & 0xFF));

                pos = ATA_IDENT_MAX_LBA;
                devices[num].Size = (uint)(((0xFF & result[pos]) << 24) | ((0xFF & result[pos + 1]) << 16) | ((0xFF & result[pos + 2]) << 8) | (0xFF & result[pos + 3]));

                // Model name
                pos = ATA_IDENT_MODEL;
                devices[num].Name = "";
                for (int i = 0; i < 40; i += 2)
                {
                    devices[num].Name += ((char)result[pos + i]).ToString();
                    devices[num].Name += ((char)result[pos + i + 1]).ToString();
                }

                num++;
            }
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
