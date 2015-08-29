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
        
        /// <summary>
        /// Initialize COM port
        /// </summary>
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

        /// <summary>
        /// Is transmit not empty?
        /// </summary>
        /// <returns>Transmit empty?</returns>
        private static bool transmit_not_empty()
        {
            return ((Portio.In8(COM1 + 0x05) & 0x20) == 0);
        }

        /// <summary>
        /// Do we have incoming data?
        /// </summary>
        /// <returns>Do we?</returns>
        private static int transmit_received()
        {
            int port = Portio.In8(COM1 + 0x05);
            return (port & 1);
        }

        /// <summary>
        /// Write char to COM port when no data is coming in
        /// </summary>
        /// <param name="c">Char to write</param>
        public static void WriteChar(char c)
        {
            // Wait till transmit is empty
            while (transmit_not_empty()) ;

            // Write byte
            Portio.Out8(COM1, (byte)c);
        }

        /// <summary>
        /// Write string to COM port when no data is coming in
        /// </summary>
        /// <param name="str">String to write</param>
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

        /// <summary>
        /// Read char from COM port
        /// </summary>
        /// <returns>Read char</returns>
        public static char Read()
        {
            // Wait till transmit is not empty
            while (transmit_received() == 0) ;

            // Read byte
            byte dat = Portio.In8(COM1);

            return (char)dat;
        }

        /// <summary>
        /// Read line from COM port
        /// </summary>
        /// <returns>Read string</returns>
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
