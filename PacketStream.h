#pragma once
namespace MBUtility
{
    class PacketInputStream
    {
    public:
        virtual size_t ReadMaxPacketSize() = 0;
        virtual size_t ReadPacket(void* OutBuffer,size_t BufferSize,double Timeout = -1) = 0;
    };
    class PacketOutputStream
    {
    public:
        virtual size_t WriteMaxPacketSize() = 0;
        virtual void WritePacket(const void* InBuffer,size_t BufferSize,double Timout = -1) = 0;
    };

    //WritePacket and ReadPacket must be thread-safe
    class BidirectionalPacketStream : public PacketInputStream,public PacketOutputStream
    {
    };
}
