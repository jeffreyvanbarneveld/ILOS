﻿namespace ILOS.Drivers.Storage
{
    struct IDE_Device
    {
        public bool Exists;
        public byte Channel;

        public ushort Base;
        public byte Drive;
        public ulong Size;

        public uint CmdSet;
        public ushort Type;
        public ushort Capabilities;

        public ushort Cylinders;
        public ushort Heads;
        public ushort Sectorspt;

        public string Name;
    }
}
