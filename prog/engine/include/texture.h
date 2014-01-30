#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
using namespace std;

class Texture
{
    public:
        Texture();
        virtual ~Texture();

        bool fromFile(string filepath_in);
    protected:
    private:
};

#endif // TEXTURE_H
