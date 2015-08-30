using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ILOS.Drivers.Storage
{
    partial class FDC
    {
        // IO PORTS
        private const ushort DOR  = 0x3F2;
        private const ushort MSR  = 0x3F4;
        private const ushort FIFO = 0x3F5;
        private const ushort CTRL = 0x3F7;

        // DOR masks
        private const byte DOR_MASK_DRIVE1          = 0x01;
        private const byte DOR_MASK_DRIVE2          = 0x02;
        private const byte DOR_MASK_DRIVE3          = 0x03;
        private const byte DOR_MASK_RESET           = 0x04;
        private const byte DOR_MASK_DMA             = 0x08;
        private const byte DOR_MASK_DRIVE1_MOTOR    = 0x20;
        private const byte DOR_MASK_DRIVE2_MOTOR    = 0x40;
        private const byte DOR_MASK_DRIVE3_MOTOR    = 0x80;


    }
}
