using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS
{
    class StringHelper
    {
        public unsafe static bool StartWith(string tostart, byte *value)
        {
            int length = tostart.Length;

            byte* ptr = value;
            int i = 0;
            while(i < length)
            {
                if ((char)*ptr != tostart[i])
                {
                    return false;
                }

                ptr++;
                i++;
            }

            return true;
        }
    }
}
