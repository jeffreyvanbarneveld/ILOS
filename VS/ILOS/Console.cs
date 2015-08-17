﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS
{
    unsafe class Console
    {
        private static byte color = 0xCE;
        private static byte* start = (byte*) 0xB8000;
        private static int cur_x = 0;
        private static int cur_y = 0;
        
        public static void putchar(char c)
        {
            int offset = (cur_y * 80 + cur_x) * 2;
            start[offset + 0] = (byte) c;
            start[offset + 1] = color;

            cur_x++;
            if (cur_x > 80)
            {
                cur_x = 0;
                cur_y++;
            }
        }

        public static void Write(string str)
        {
            int len = str.Length;
                     
            int i = 0;
            while (i++ < len)
                putchar(str[i]);
        }
    }
}
