using ILVM;

namespace ILOS
{
    class Portio
    {
        private static Registers regs = default(Registers);
        
        /// <summary>
        /// Write 8-bit value to the given port
        /// </summary>
        /// <param name="port">The port</param>
        /// <param name="value">The value</param>
        public static void Out8(uint port, uint value)
        {
            regs.eax = value;
            regs.edx = port;
            Asm.Execute("out dx, al", regs);
        }

        /// <summary>
        /// Read 8-bit value from the given port
        /// </summary>
        /// <param name="port">The port</param>
        /// <returns>Value from the port</returns>
        public static byte In8(uint port)
        {
            regs.edx = port;
            Asm.Execute("in al, dx", regs);
            return (byte)regs.eax;
        }

        /// <summary>
        /// Write 16-bit value to the given port
        /// </summary>
        /// <param name="port">The port</param>
        /// <param name="value">The value</param>
        public static void Out16(uint port, uint value)
        {
            regs.eax = value;
            regs.edx = port;
            Asm.Execute("out dx, ax", regs);
        }

        /// <summary>
        /// Read 16-bit value from the given port
        /// </summary>
        /// <param name="port">The port</param>
        /// <returns>Value from the port</returns>
        public static ushort In16(uint port)
        {
            regs.edx = port;
            Asm.Execute("in ax, dx", regs);
            return (ushort)regs.eax;
        }

        /// <summary>
        /// Write 32-bit value to the given port
        /// </summary>
        /// <param name="port">The port</param>
        /// <param name="value">The value</param>
        public static void Out32(uint port, uint value)
        {
            regs.eax = value;
            regs.edx = port;
            Asm.Execute("out dx, eax", regs);
        }

        /// <summary>
        /// Read 32-bit value from the given port
        /// </summary>
        /// <param name="port">The port</param>
        /// <returns>Value from the port</returns>
        public static uint In32(uint port)
        {
            regs.edx = port;
            Asm.Execute("in ax, dx", regs);
            return regs.eax;
        }
    }
}
