using ILOS.Drivers.Char;
using ILOS.Drivers.Sound;
using ILOS.Drivers.Storage;
using System;

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
            Console.WriteLine("Looking for ata disk ON ATA_PRIMARY");
            ATA.Init();
            if (ATA.FirstDevice.Exists)
                Console.WriteLine("Found drive on ATA_PRIMARY");
            else
                Console.WriteLine("No disk found :(");

            Console.WriteLine("Name of ATA disk: " + ATA.FirstDevice.Name);
            Console.WriteLine("Duplicating sector 1 on sector2 and read sector 2");
            byte[] buf = new byte[512];
            ATA.ReadSector(0, 1, buf);
            ATA.WriteSector(1, 1, buf);
            
            Console.WriteLine("Reading second sector");
            ATA.ReadSector(1, 1, buf);
            for(int i = 0; i < 512; i++)
            {
                Console.WriteHex(buf[i]);
                Console.PutChar(' ');
            }
        }
    }
}
