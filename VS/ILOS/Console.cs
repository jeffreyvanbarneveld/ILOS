namespace ILOS
{
    unsafe class Console
    {
        private static byte attribute = 0x0F;
        private static byte* textmem  = (byte*) 0xB8000;
        private static int cursor_x   = 0;
        private static int cursor_y   = 0;
        
        public int CursorX
        {
            get { return cursor_x; }
            set
            {
                cursor_x = value;
                MoveVGACursor();
            }
        }

        public int CursorY
        {
            get { return cursor_y; }
            set
            {
                cursor_y = value;
                MoveVGACursor();
            }
        }

        /// <summary>
        /// Moves the VGA mode text cursor
        /// </summary>
        public static void MoveVGACursor()
        {
            int index = cursor_y * 80 + cursor_x;
            Portio.Out8(0x3D4, 14);
            Portio.Out8(0x3D5, (byte)(index >> 8));
            Portio.Out8(0x3D4, 15);
            Portio.Out8(0x3D5, (byte)(index & 0xFF));
        }
        
        /// <summary>
        /// Plot char to screen
        /// </summary>
        /// <param name="c">Char to plot</param>
        public static void PutChar(char c)
        {
            int offset = (cursor_y * 80 + cursor_x) * 2;
            
            if (c == '\n')
            {
                cursor_x = 0;
                cursor_y++;
            }
            else if (c == '\r')
            {
                cursor_x = 0;
            }
            else if (c == '\t')
            {
                cursor_x = (cursor_x + 4) & ~(4 - 1);
            }
            else
            {
                textmem[offset + 0] = (byte)c;
                textmem[offset + 1] = attribute;

                cursor_x++;
                if (cursor_x > 80)
                {
                    cursor_x = 0;
                    cursor_y++;
                }
            }

            MoveVGACursor();
        }

        /// <summary>
        /// Write string to screen
        /// </summary>
        /// <param name="str">String to write</param>
        public static void Write(string str)
        {
            int len = str.Length;
            int i = 0;

            while(i < len)
            {
                PutChar(str[i]);
                i++;
            }
        }

        /// <summary>
        /// Write line to screen
        /// </summary>
        /// <param name="str">Line to write</param>
        public static void WriteLine(string str)
        {
            Write(str);
            PutChar('\n');
        }

    }
}
