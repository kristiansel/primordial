#ifndef MEMSTREAMER_H
#define MEMSTREAMER_H

class MemStreamer // This could be useful for anything that reads from memory blob
{
public:
    MemStreamer(const char * memblock_in, int offset_in, int memsize_in)
             : memblock(memblock_in),
               offset(offset_in),
               memsize(memsize_in) {};
    ~MemStreamer() {};

    void chomp (void * dest, size_t size_in)
    {
        if (offset < memsize || size_in == size_t(0))
        {
            memcpy(dest, &memblock[offset], size_in);
            offset += size_in/sizeof(char);
        }
        else
        {
            std::cerr << "Error: tried to read outside memory block\n";
        }

    }
private:
    const char * memblock;
    int offset;
    int memsize;
};

#endif // MEMSTREAMER_H
