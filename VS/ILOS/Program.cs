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

            foreach(IDE_DEVICE device in ATA.Devices)
            {
                if (device.Exists)
                {
                    Console.WriteLine("Found drive with name: " + device.Name);
                }
            }
        }
    }
}
