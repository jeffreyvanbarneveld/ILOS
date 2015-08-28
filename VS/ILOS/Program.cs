using ILOS.Drivers.Char;
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

            byte[] buf = new byte[512];
            ATA.readSector(0, 1, buf);

            Console.WriteLine("Reading first sector of ATA drive");
            for(int i = 0; i < 512; i++)
            {
                Console.WriteHex(buf[i]);
                Console.PutChar(' ');
            }
        }
    }
}
