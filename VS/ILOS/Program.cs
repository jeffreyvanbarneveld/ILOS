using ILOS.Drivers.Char;
using ILOS.Drivers.Power;
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

            Keyboard.LedEnable(KbLeds.NumberLock);
            Keyboard.LedEnable(KbLeds.CapsLock);
            Keyboard.LedDisable(KbLeds.CapsLock);
        }
    }
}
