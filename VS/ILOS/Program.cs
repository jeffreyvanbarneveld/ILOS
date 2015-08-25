using ILVM;

namespace ILOS
{
    class Program
    {
        static void Out8(ushort port, byte value)
        {
            Registers regs = default(Registers);
            regs.eax = value;
            regs.edx = port;
            Asm.Execute("outb dx, al", regs);
        }

        static void Main(string[] args)
        {
            int x = 5;
            int y = 5;
            int index = y * 80 + x;

            // Move vga cursor as a test
            Out8(0x3D4, 14);
            Out8(0x3D5, (byte)(index >> 8));
            Out8(0x3D4, 15);
            Out8(0x3D5, (byte)(index & 0xFF));
        }
    }
}
