using ILOS.Drivers.Char;

namespace ILOS
{
    class Program
    {
        static void Main(string[] args)
        {
            Console.WriteLine("Starting ILOS...");

            SerialPort.Init();
            SerialPort.Write("Chuuk chuuk");
            Console.Write("Tijd is: " + RTC.Hours);
            Console.WriteLine(":" + RTC.Minutes);
            Console.WriteLine(" ");
            Console.WriteLine(" ");
            Console.WriteLine(" ");

            while (true)
            {
                byte b;
                while ((b = SerialPort.Read()) != (byte)'\n')
                    Console.PutChar((char)b);
                Console.PutChar('\n');
            }

        }
    }
}
