using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS
{
    class RTC
    {
        private const byte SECONDS = 0x00;
        private const byte MINUTES = 0x02;
        private const byte HOURS   = 0x04;
        private const byte WEEKDAY = 0x06;
        private const byte DAYOFMONTH = 0x07;
        private const byte MONTH = 0x08;
        private const byte YEAR = 0x09;
        private const byte CENTURY = 0x32;

        private const int outPort = 0x70;
        private const int inPort = 0x71;

        /// <summary>
        /// Get number of seconds from RTC
        /// </summary>
        public static int Seconds
        {
            get { return ReadReg(SECONDS); }
        }

        /// <summary>
        /// Get number of minutes from RTC
        /// </summary>
        public static int Minutes
        {
            get { return ReadReg(MINUTES); }
        }

        /// <summary>
        /// Get number of hours from RTC
        /// </summary>
        public static int Hours
        {
            get { return ReadReg(HOURS); }
        }

        /// <summary>
        /// Get weekday from RTC
        /// </summary>
        public static int Weekday
        {
            get { return ReadReg(WEEKDAY); }
        }

        /// <summary>
        /// Get year from RTC
        /// </summary>
        public static int Year
        {
            get { return ReadReg(YEAR); }
        }


        private static byte getUpdateFlag()
        {
            Portio.Out8(outPort, 0x0A);
            return Portio.In8(inPort);
        }

        /// <summary>
        /// Read RTC chip using register number
        /// </summary>
        /// <param name="register">Register number</param>
        /// <returns></returns>
        public static int ReadReg(byte register)
        {
            Portio.Out8(outPort, register);
            byte regValue = Portio.In8(inPort);

            return (regValue & 0x0F) + ((regValue / 16) * 10);
        }
    }
}
