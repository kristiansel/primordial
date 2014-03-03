#include <iostream>
#include <string>
#include <fstream>

#include "converter.hpp"

int main(int argc, char* argv[])
{
    std::string usage = std::string("usage: ")
                       +std::string("prconvert -Ifilepath -Ggeometryoutfile [-Bbonesoutfile] [-Aanimspecfile]\n")
                       +std::string("\tfilepath is the path of the input file\n")
                       +std::string("\tgeometryoutfile is the path of the .bgeo file to generate\n")
                       +std::string("\tbonesoutfile is the path of the .bbsn file to generate\n")
                       +std::string("\tanimspecfile is the path of the config file on how to split animations\n");

    std::string inputfile = "";
    std::string outputfile_geo = "";
    std::string outputfile_bns = "";
    std::string anispecfile = "";

//    std::cout << "numargs " << argc << "\n";
//
//    for (int i = 0; i<argc; i++)
//    {
//        std::cout << argv[i] << "\n";
//    }

    if (argc > 2 && argc < 6)
    {
        for (int i = 0; i<argc; i++)
        {
            std::string in = argv[i];
            std::string first = "";
            if (in.length() > 2)
            {
                if (in.substr(0, 2) == "-I") inputfile = in.substr(2, in.length()-2);
                if (in.substr(0, 2) == "-G") outputfile_geo = in.substr(2, in.length()-2);
                if (in.substr(0, 2) == "-B") outputfile_bns = in.substr(2, in.length()-2);
                if (in.substr(0, 2) == "-A") anispecfile = in.substr(2, in.length()-2);
            }
        }
    }
    else
    {
        std::cerr << "error: wrong number of input arguments\n";
        std::cout << usage;
        return -1;
    }

    if (!inputfile.empty() && !outputfile_geo.empty())
    {
        convert(inputfile,
                outputfile_geo,
                outputfile_bns,
                anispecfile);
    }
    else
    {
        std::cerr << "error: unable to parse input arguments\n";
        return -2;
    }

    return 0;
}
