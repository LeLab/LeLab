#ifndef BYTEBUFFER_H
#define BYTEBUFFER_H

#include <vector>
#include <ostream>
#include "Defines.h"

typedef uint8 Byte;
#define DEFAULT_BUFFER_SIZE 64 // bytes

class ByteBuffer
{
public:
    ByteBuffer();
    ByteBuffer(uint32 capacity);
    ByteBuffer(const ByteBuffer& other);

    const Byte* Data() const;

    void Clear();

    Byte operator[](uint32 pos) const;
    template <typename T> T Read(uint32 position) const;

    uint32 GetReadPos() const;
    void SetReadPos(uint32 readPos);

    uint32 GetWritePos() const;
    void SetWritePos(uint32 writePos);

    void FinishRead();

    template <typename T> void ReadSkip();
    void ReadSkip(uint32 size);

    template <typename T> T Read();
    void Read(Byte* dest, uint32 count);

    template <typename T> void Append(T val);
    template <typename T> void Put(uint32 pos, T val);
    template <typename T> void Append(const T* src, uint32 count);
    void Append(const ByteBuffer& other);
    void Append(const Byte* src, uint32 count);
    void Put(uint32 pos, const Byte* src, uint32 count);

    uint32 Size() const;
    bool IsEmpty() const;

    void Resize(uint32 newSize);
    void Reserve(uint32 size);

    void Print(std::ostream& stream) const;

protected:
    std::vector<Byte> _buffer;
    uint32 _readPos;
    uint32 _writePos;
};

template <typename T>
void ByteBuffer::ReadSkip()
{
    ReadSkip(sizeof(T));
}

template <typename T>
T ByteBuffer::Read(uint32 position) const
{
    assert(position + sizeof(T) <= Size());
    T value = *((T const*)&_buffer[position]);
    return value;
}

template <typename T>
T ByteBuffer::Read()
{
    T val = Read<T>(_readPos);
    _readPos += sizeof(T);
    return val;
}

template <typename T>
void ByteBuffer::Append(T val)
{
    Append((Byte*)&val, sizeof(T));
}

template <typename T>
void ByteBuffer::Put(uint32 pos, T val)
{
    Put(pos, (Byte*)&val, sizeof(T));
}

template <typename T>
void ByteBuffer::Append(const T* src, uint32 count)
{
    return Append((const Byte*)src, count * sizeof(T));
}

#endif // BYTEBUFFER_H