using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Char
{
    /// <summary>
    /// Keyboard led state flags
    /// </summary>
    enum KbLeds
    {
        ScrollLock = 0x01,
        NumberLock = 0x02,
        CapsLock = 0x04
    }

    /// <summary>
    /// Keyboard class
    /// </summary>
    class Keyboard
    {
        private static int ledByte = 0x00;

        /// <summary>
        /// Enable keyboard led
        /// </summary>
        /// <param name="led">The led</param>
        public static void LedEnable(KbLeds led)
        {
            ledByte |= (int)led;

            updateLeds();
        }

        /// <summary>
        /// Disable keyboard led
        /// </summary>
        /// <param name="led">the led</param>
        public static void LedDisable(KbLeds led)
        {
            ledByte = ledByte & ~(int)led;

            updateLeds();
        }
        
        /// <summary>
        /// Update leds to hardware
        /// </summary>
        private static void updateLeds()
        {
            Portio.Out8(0x60, 0xED);
            while (!(Portio.In8(0x60) == 0xfa)) ;
            Portio.Out8(0x60, (uint)ledByte);
        }
    }
}
