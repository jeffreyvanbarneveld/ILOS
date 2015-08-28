namespace ILOS.Drivers.Storage
{
    unsafe partial class ATA
    {
        const int ATA_PRIMARY_IO = 0x1F0;
        const int ATA_SECONDARY_IO = 0x170;

        private static IDE_DEVICE device = new IDE_DEVICE();
        public static IDE_DEVICE FirstDevice
        {
            get { return device; }
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
            Portio.Out8((ushort)(@base + ATA_REG_SECCNT), 0x00);
            Portio.Out8((ushort)(@base + ATA_REG_LBALO), 0x00);
            Portio.Out8((ushort)(@base + ATA_REG_LBAMID), 0x00);
            Portio.Out8((ushort)(@base + ATA_REG_LBAHI), 0x00);

            Portio.Out8((ushort)(@base + ATA_REG_CMD), ATA_CMD_IDENTIFY);

            // Check if a drive is found
            byte status = Portio.In8((ushort)(@base + ATA_REG_STATUS));
            if (status == 0)
                return null;

            // Wait until drive is not busy anymore
            while ((status & ATA_STATUS_BSY) != 0)
                status = Portio.In8((ushort)(@base + ATA_REG_STATUS));

            while(true)
            {
                status = Portio.In8((ushort)(@base + ATA_REG_STATUS));

                if ((status & ATA_STATUS_ERR) != 0)
                    return null;

                if ((status & ATA_STATUS_DRQ) != 0)
                    break;
            }

            // Read data from ATA drive
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
        /// Wait for drive to be finished
        /// </summary>
        /// <param name="@base">Base IO base</param>
        private static void ata_poll(uint @base)
        {
            for (int i = 0; i < 4; i++)
                Portio.In8((ushort)(@base + ATA_REG_ALTSTATUS));

            byte status = Portio.In8((ushort)(@base + ATA_REG_STATUS));
            while ((status & ATA_STATUS_BSY) > 0)
                status = Portio.In8((ushort)(@base + ATA_REG_STATUS));
            
            while((status & ATA_STATUS_DRQ) == 0)
            {
                status = Portio.In8((ushort)(@base + ATA_REG_STATUS));

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
        public static int readSector(uint lba, byte size, byte[] buffer)
        {
            if (!device.Exists)
                return 0;

            uint @base = device.Base;
            int drive = device.Drive;

            int cmd = (drive == ATA_MASTER) ? 0xE0 : 0xF0;

            // Set Drive
            Portio.Out8((ushort)(@base + ATA_REG_DRIVE), (byte)(cmd | (byte)((lba >> 24) & 0x0F)));

            // Set PIO MODE
            Portio.Out8((ushort)(@base + ATA_REG_FEATURE), ATA_FEATURE_PIO);

            // Set size
            Portio.Out8((ushort)(@base + ATA_REG_SECCNT), size);

            // Set LBA
            Portio.Out8((ushort)(@base + ATA_REG_LBALO), (byte)lba);
            Portio.Out8((ushort)(@base + ATA_REG_LBAMID), (byte)(lba >> 8));
            Portio.Out8((ushort)(@base + ATA_REG_LBAHI), (byte)(lba >> 16));

            // Issue command
            Portio.Out8((ushort)(@base + ATA_REG_CMD), ATA_CMD_PIO_READ);

            // Wait till done
            ata_poll(@base);

            // Read data
            for (int i = 0; i < size * 256; i++)
            {
                ushort @in = Portio.In16((ushort)(@base + ATA_REG_DATA));
                int pos = i * 2;
                buffer[pos] = (byte)@in;
                buffer[pos + 1] = (byte)(@in >> 8);
            }

            return size * 512;
        }

        /// <summary>
        /// IDE Prope
        /// </summary>
        private static void Probe()
        {
            ushort @base = ATA_PRIMARY_IO;
            byte channel = ATA_PRIMARY;
            byte drive = ATA_PRIMARY;

            device.Base = @base;
            device.Channel = channel;
            device.Drive = drive;

            byte[] result = identify(channel, drive);

            if (result == null)
            {
                device.Exists = false;
                return;
            }

            device.Exists = true;
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
