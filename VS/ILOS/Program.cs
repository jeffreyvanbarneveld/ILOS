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
            Console.WriteLine("Looking for ata disk ON ATA_PRIMARY");
            ATA.Init();
            if (ATA.FirstDevice.Exists)
                Console.WriteLine("Found drive on ATA_PRIMARY");
            else
                Console.WriteLine("No disk found :(");

            
        }
    }
}
