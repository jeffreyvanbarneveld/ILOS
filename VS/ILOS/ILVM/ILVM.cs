namespace ILVM
{
    struct Registers
    {
        public uint eax;
        public uint ebx;
        public uint ecx;
        public uint edx;
        public uint esi;
        public uint edi;
    }

    class Asm
    {
        // Changed by compiler
        public static void Execute(string asm, Registers regs)
        {
        }
    }
}