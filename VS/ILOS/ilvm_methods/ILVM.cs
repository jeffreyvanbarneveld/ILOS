namespace ILVM
{
    public struct Registers
    {
        public uint eax;
        public uint ebx;
        public uint ecx;
        public uint edx;
        public uint esi;
        public uint edi;
    }

    public class Asm
    {
        // Changed by compiler
        public static void Execute(string asm, Registers regs)
        {
        }
    }
}