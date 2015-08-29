using ILVM;

namespace ILOS
{
    class Portio
    {
        private static Registers regs = default(Registers);

        public static void Out8(ushort port, byte value)
        {
            //Registers regs = default(Registers);
            regs.eax = value;
            regs.edx = port;
            Asm.Execute("out dx, al", regs);
        }

        public static byte In8(ushort port)
        {
            //Registers regs = default(Registers);
            regs.edx = port;
            Asm.Execute("in al, dx", regs);
            return (byte)regs.eax;
        }

        public static void Out16(ushort port, ushort value)
        {
            //Registers regs = default(Registers);
            regs.eax = value;
            regs.edx = port;
            Asm.Execute("out dx, ax", regs);
        }

        public static ushort In16(ushort port)
        {
            //Registers regs = default(Registers);
            regs.edx = port;
            Asm.Execute("in ax, dx", regs);
            return (ushort)regs.eax;
        }

        public static void Out32(ushort port, uint value)
        {
            regs.eax = value;
            regs.edx = port;
            Asm.Execute("out dx, eax", regs);
        }

        public static uint In32(ushort port)
        {
            regs.edx = port;
            Asm.Execute("in ax, dx", regs);
            return regs.eax;
        }
    }
}
