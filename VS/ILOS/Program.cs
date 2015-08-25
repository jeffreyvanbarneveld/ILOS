using ILVM;

namespace ILOS
{
    class Program
    {
        static void Main(string[] args)
        {
            Registers regs = new Registers();
            Asm.Execute("mov eax, 1337", regs);
            Console.Write("eax=" + regs.eax);
        }
    }
}
