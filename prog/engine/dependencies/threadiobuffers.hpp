#ifndef THREADIOBUFFERS_H
#define THREADIOBUFFERS_H

#include <cmath>

template <class Type, unsigned int b_size_in>
class ThreadIObuffers
{
    public:
        Type buffer_data[2][b_size_in];
        const unsigned int b_size = b_size_in;

        template <unsigned int io_port>
        class IOBuffer
        {
            public:
                IOBuffer(ThreadIObuffers &iobuffers) : buffer_in(&iobuffers.buffer_data[io_port][0]),
                buffer_out(&iobuffers.buffer_data[1-io_port][0]) {};

                Type const * getReadPtr() {return buffer_in;}
                Type * getWritePtr() {return buffer_out;}

            private:
                Type const * buffer_in; // read data
                Type* buffer_out; // write data
        };
};
//
//template <class Type, unsigned int b_size_in, unsigned int io_port>
//class ThreadIObuffers<Type getInterface(unsigned int io_port_in)






#endif //THREADIOBUFFERS_H

