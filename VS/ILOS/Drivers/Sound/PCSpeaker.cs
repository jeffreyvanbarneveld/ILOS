using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Sound
{
    class PCSpeaker
    {
        /// <summary>
        /// Play sound through the PC speaker
        /// </summary>
        /// <param name="frequence">PC speaker</param>
        public static void Play(uint frequence)
        {
            uint div;
            byte temp;

            div = 1193180 / frequence;

            Portio.Out8(0x43, 0xB6);
            Portio.Out8(0x42, (byte)(div));
            Portio.Out8(0x42, (byte)(div >> 8));

            temp = Portio.In8(0x61);
            if (temp != (temp | 3))
                Portio.Out8(0x61, (byte)(temp | 3));
        }

        /// <summary>
        /// Stop sound from the pc speaker
        /// </summary>
        public static void Stop()
        {
            byte temp = (byte)(Portio.In8(0x61) & 0xFC);

            Portio.Out8(0x61, temp);
        }
    }
}
