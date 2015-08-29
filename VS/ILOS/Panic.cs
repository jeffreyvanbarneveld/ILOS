using System.Runtime.CompilerServices;

namespace ILOS
{
    class Panic
    {
        public static void Do(string message, [CallerLineNumber] int lineNumber = 0, [CallerFilePath] string file = "", [CallerMemberName] string caller = null)
        {
            Console.AttributeByte = 0xCF;

            Console.ClearScreen();
            Console.CursorY = 1;
            Console.CursorX = 20;
            Console.WriteLine(@"  _____   _____         ____      _____");
            Console.CursorX = 20;
            Console.WriteLine(@" (_   _) (_   _)       / __ \    / ____\");
            Console.CursorX = 20;
            Console.WriteLine(@"   | |     | |        / /  \ \  ( (___");
            Console.CursorX = 20;
            Console.WriteLine(@"   | |     | |       ( ()  () )  \___ \");
            Console.CursorX = 20;
            Console.WriteLine(@"   | |     | |   __  ( ()  () )      ) )");
            Console.CursorX = 20;
            Console.WriteLine(@"  _| |__ __| |___) )  \ \__/ /   ___/ /");
            Console.CursorX = 20;
            Console.WriteLine(@" /_____( \________/    \____/   /____/");
            Console.CursorX = 20;
            Console.CursorY++;

            Console.WriteLine("Red dead from hell\n");
            Console.CursorX = 20;
            Console.WriteLine("Message: " + message);
            Console.CursorX = 20;
            Console.WriteLine("File: " + file);
            Console.CursorX = 20;
            Console.Write("Line: " + lineNumber);
            Console.Write(" (" + caller + ")");

            Console.AttributeByte = 0xCC;
            Console.DisableCursor();

            for (;;) ;
        }
    }
}
