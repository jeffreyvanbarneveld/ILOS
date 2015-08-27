namespace ILOS
{
    unsafe class Console
    {
        private static byte color = 0x0F;
        private static byte* start = (byte*) 0xB8000;
        public static int cur_x = 0;
        public static int cur_y = 0;
        
        /// <summary>
        /// Moves the VGA mode text cursor
        /// </summary>
        public static void MoveCursor()
        {
            int index = cur_y * 80 + cur_x;
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
            int offset = (cur_y * 80 + cur_x) * 2;
            
            if (c == '\n')
            {
                cur_x = 0;
                cur_y++;
            }
            else if (c == '\r')
            {
                cur_x = 0;
            }
            else if (c == '\t')
            {
                cur_x = (cur_x + 4) & ~(4 - 1);
            }
            else
            {
                start[offset + 0] = (byte)c;
                start[offset + 1] = color;

                cur_x++;
                if (cur_x > 80)
                {
                    cur_x = 0;
                    cur_y++;
                }
            }

            MoveCursor();
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
