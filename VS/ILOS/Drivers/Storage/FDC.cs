using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Storage
{
    partial class FDC
    {
        /// <summary>
        /// Initalize DMA
        /// </summary>
        private static void initDMA()
        {
            // Mask DMA channel 2
            Portio.Out8(0x0A, 0x06);

            // Reset master flip/flop
            Portio.Out8(0xD8, 0xFF);

            // Set address
            Portio.Out8(0x04, 0x00);
            Portio.Out8(0x04, 0x10);

            // Reset master
            Portio.Out8(0xD8, 0xFF);

            // Count to 0x23FF (number of bytes)
            Portio.Out8(0x05, 0xFF);
            Portio.Out8(0x05, 0x23);

            // Set external page register to zero
            Portio.Out8(0x80, 0x00);

            // Unmask DMA channel 2
            Portio.Out8(0x0A, 0x02);
        }

        /// <summary>
        /// Read DMA channel 2
        /// </summary>
        private static void readDMA()
        {
            // Mask DMA channel 2
            Portio.Out8(0x0A, 0x06);

            // Single transfer read channel 2
            Portio.Out8(0x0B, 0x56);

            // Unmask DMA channel 2
            Portio.Out8(0x0A, 0x02);
        }

        /// <summary>
        /// Write DMA channel 2
        /// </summary>
        private static void writeDMA()
        {
            // Mask DMA channel 2
            Portio.Out8(0x0A, 0x06);

            // Single transfer read channel 2
            Portio.Out8(0x0B, 0x5A);

            // Unmask DMA channel 2
            Portio.Out8(0x0A, 0x02);
        }

        /// <summary>
        /// Write DOR on FDC
        /// </summary>
        /// <param name="value">Value to be written</param>
        private static void writeDOR(byte value)
        {
            Portio.Out8(DOR, value);
        }

        private static void sendByte(byte b)
        {

        }
    }
}
