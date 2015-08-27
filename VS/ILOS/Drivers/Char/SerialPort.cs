using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Char
{
    class SerialPort
    {
        private const ushort COM1 = 0x3F8;
        

        public static void Init()
        {
            Portio.Out16(COM1 + 1, 0x00); // We just want the data interrupt :)
            Portio.Out16(COM1 + 3, 0x80); // Enable DLAB
            Portio.Out16(COM1 + 0, 0x03); // Set baud rate to 115200 (lo)
            Portio.Out16(COM1 + 1, 0x00); // (hi)
            Portio.Out16(COM1 + 3, 0x03);
            Portio.Out16(COM1 + 2, 0xC7); // Enable FIFO and clear
            Portio.Out16(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
        }

        private static bool transmit_not_empty()
        {
            return ((Portio.In8(COM1 + 0x05) & 0x20) == 0);
        }

        private static int transmit_received()
        {
            int port = Portio.In8(COM1 + 0x05);
            return (port & 1);
        }

        public static void WriteChar(char c)
        {
            while (transmit_not_empty()) ;

            Portio.Out8(COM1, (byte)c);
        }

        public static void Write(string str)
        {
            int len = str.Length;
            int i = 0;

            while (i < len)
            {
                WriteChar(str[i]);
                i++;
            }
        }

        public static char Read()
        {
            while (transmit_received() == 0) ;

            byte dat = Portio.In8(COM1);

            return (char)dat;
        }
        public static string ReadLine()
        {
            string str = "";

            char b;
            while ((b = Read()) != (byte)'\n')
                str += b;

            return str;
        }

    }
}
