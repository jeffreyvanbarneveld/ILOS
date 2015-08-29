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
        /// <param name="frequence">Frequence</param>
        public static void Play(uint frequence)
        {
            // Calculate countdown value
            uint countdown = 1193180 / frequence;

            // Set PIT to the frequency
            Portio.Out8(0x43, 0xB6);
            Portio.Out8(0x42, (byte)(countdown));
            Portio.Out8(0x42, (byte)(countdown >> 8));

            // Play the sound using the PC speaker
            byte temp = Portio.In8(0x61);

            // Is the speaker on?
            if (temp != (temp | 3))
            {
                // Set first two bits to 1 to turn on the speaker
                Portio.Out8(0x61, (byte)(temp | 3));
            }
        }

        /// <summary>
        /// Stop sound from the PC speaker
        /// </summary>
        public static void Stop()
        {
            // Set first 2 bits to zero to stop the PC speaker
            byte temp = (byte)(Portio.In8(0x61) & 0xFC);

            Portio.Out8(0x61, temp);
        }
    }
}
