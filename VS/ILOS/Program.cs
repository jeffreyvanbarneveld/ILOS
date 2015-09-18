using ILOS.Drivers.Char;
using ILOS.Drivers.Storage;

namespace ILOS
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Starting ILOS...");
            SerialPort.Init();
            Console.Write("Time is: " + RTC.Hours);
            Console.WriteLine(":" + RTC.Minutes);
            Console.WriteLine("Looking for disks.");
            ATA.Init();

            foreach(IDE_Device device in ATA.Devices)
            {
                if (device.Exists)
                {
                    Console.WriteLine("Found drive with name: " + device.Name);
                }
            }

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
