using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Power
{
    unsafe struct RSDP
    {
        public fixed byte Signature[8];
        public byte Checksum;
        public fixed byte OEMID[6];
        public byte Revision;
        public uint RsdtAddress;
    }

    unsafe class Acpi
    {
        private static RSDP *rdsp = null;

        public unsafe static void find()
        {
            // Finding the RSDP
            // First attempt to bios data 
            byte* biosp = (byte*)0x000E0000;
            
            while((UInt32) biosp < 0x000FFFFF)
            {
                if (StringHelper.StartWith("RSD PTR ", biosp) && check_sum(biosp, 20))
                {
                    rdsp = (RSDP*)biosp;
                     
                    break;
                }

                biosp += 16;
            }

            if (rdsp == null)
            {
                byte* ebdap = (byte*)((*(UInt16*)0x040E) << 4);

                // Second attempt though edda
                while ((uint)ebdap < 0x000A0000)
                {
                    if (StringHelper.StartWith("RSD PTR ", ebdap) && check_sum(ebdap, 20))
                    {
                        rdsp = (RSDP*)ebdap;

                        break;
                    }

                    ebdap += 16;
                }
            }

            Console.WriteHex((uint)rdsp);
            Console.WriteLine("");
            Console.WriteHex(rdsp->Checksum);
        }

        private unsafe static bool check_sum(void *address, UInt32 length)
        {
            byte* bptr = (byte*)address;
            byte check = 0;

            for(int i = 0; i < length; i++)
            {
                check += *bptr;
                bptr++;
            }

            if (check == 0)
                return true;

            return false;
        }
    }
}
