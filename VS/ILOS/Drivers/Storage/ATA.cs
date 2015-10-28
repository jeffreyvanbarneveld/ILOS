namespace ILOS.Drivers.Storage
{
    unsafe partial class ATA
    {
        private static IDE_Device[] devices = new IDE_Device[4];
        public static IDE_Device[] Devices
        {
            get { return devices; }
        }

        /// <summary>
        /// Waits 400 ns on an ATA device
        /// </summary>
        /// <param name="@base">The base IO</param>
        private static void wait400ns(uint @base)
        {
            Portio.In8(@base + ATA_REG_ALTSTATUS);
            Portio.In8(@base + ATA_REG_ALTSTATUS);
            Portio.In8(@base + ATA_REG_ALTSTATUS);
            Portio.In8(@base + ATA_REG_ALTSTATUS);
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
                return null;

            // Wait until drive is not busy anymore
            status = Portio.In8(@base + ATA_REG_STATUS);
            while ((status & ATA_STATUS_BSY) != 0)
                status = Portio.In8(@base + ATA_REG_STATUS);
            
            while (true)
            {
                status = Portio.In8(@base + ATA_REG_STATUS);

                if ((status & ATA_STATUS_ERR) != 0)
                    return null;
                
                if ((status & ATA_STATUS_DRQ) != 0)
                    break;
            }
            
            // Read data from ATA drive
            byte[] ide_buf = new byte[256];
            int offset = 0;
            for(int i = 0; i < 128; i++)
            {
                ushort shrt = Portio.In16(@base + ATA_REG_DATA);
                ide_buf[offset + 0] = (byte)(shrt >> 8);
                ide_buf[offset + 1] = (byte)(shrt);
                offset += 2;
            }

            return ide_buf;
        }

        /// <summary>
        /// Wait for drive to be finished
        /// </summary>
        /// <param name="@base">Base IO base</param>
        private static void ata_poll(uint @base)
        {
            wait400ns(@base);

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
            // The driver only supports up to 4 drives
            if (device_num > 3)
                return 0;

            // Get IDE device from array
            IDE_Device device = devices[device_num];

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
            int offset = 0;
            for (int i = 0; i < size * 256; i++)
            {
                ushort @in = Portio.In16(@base + ATA_REG_DATA);
                buffer[offset + 0] = (byte)(@in);
                buffer[offset + 1] = (byte)(@in >> 8);
                offset += 2;
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
            IDE_Device device = devices[device_num];

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
            wait400ns(@base);

            // Write data
            for (int i = 0; i < size * 256; i++)
            {
                int pos = i * 2;
                ushort shrt = (ushort)((buffer[pos + 1] << 8) | buffer[pos]);

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

                devices[num] = new IDE_Device();

                devices[num].Base = @base;
                devices[num].Channel = channel;
                devices[num].Drive = drive;

                byte[] result = identify(channel, drive);
                
                if (result == null)
                {
                    devices[num].Exists = false;
                    num++;
                    continue;
                }

                devices[num].Exists = true;

                int pos = ATA_IDENT_COMMANDSETS;
                devices[num].CmdSet = (uint)((result[pos] << 24) | (result[pos + 1] << 16) | (result[pos + 2] << 8) | result[pos + 3]);

                pos = ATA_IDENT_DEVICETYPE;
                devices[num].Type = (ushort)((result[pos + 1] << 8) | result[pos]);

                pos = ATA_IDENT_CAPABILITIES;
                devices[num].Capabilities = (ushort)((result[pos + 1] << 8) | result[pos]);

                pos = ATA_IDENT_CYLINDERS;
                devices[num].Cylinders = (ushort)((result[pos + 1] << 8) | result[pos]);

                pos = ATA_IDENT_HEADS;
                devices[num].Heads = (ushort)((result[pos + 1] << 8) | result[pos]);

                pos = ATA_IDENT_SECTORSPT;
                devices[num].Sectorspt = (ushort)((result[pos + 1] << 8) | result[pos]);

                pos = ATA_IDENT_MAX_LBA;
                devices[num].Size = (uint)(((result[pos] << 24) | (result[pos + 1] << 16) | (result[pos + 2] << 8) | result[pos + 3]));

                // Model name
                pos = ATA_IDENT_MODEL;
                devices[num].Name = "";
                for (int i = 0; i < 40; i += 2)
                {
                    devices[num].Name += ((char)result[pos + i + 0]).ToString();
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

        /// <summary>
        /// Contains test code for ATA
        /// </summary>
        public static void Test()
        {
            Console.WriteLine("Reading 3 bytes of sector on IDE_PRIMARY_MASTER");

            byte[] buf = new byte[512];
            ATA.ReadSector(0, 0, 1, buf);

            Console.Write("Value: 0x");
            Console.WriteHex(buf[100]);
            Console.WriteHex(buf[510]);
            Console.WriteHex(buf[511]);
            Console.Write("\n");

            Console.WriteLine("Reading 3 bytes of sector on IDE_SECONDARY_MASTER");

            ATA.ReadSector(2, 0, 1, buf);

            Console.Write("Value: 0x");
            Console.WriteHex(buf[100]);
            Console.WriteHex(buf[510]);
            Console.WriteHex(buf[511]);
        }
    }
}
