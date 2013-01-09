/**
 * Reverses the lines in a file. eg.
 *     dog
 *     cat
 *     mouse
 * .. becomes ..
 *     mouse
 *     cat
 *     dog
**/

#include <boost/iostreams/device/mapped_file.hpp>
#include <fstream>
#include <iostream>
#include <iterator>
#include <algorithm>

using boost::iostreams::mapped_file_source;

void reverse(mapped_file_source& in, std::ostream& out) {
    std::reverse_iterator<const char*> i(in.data() + in.size() - 1);
    std::reverse_iterator<const char*> end(in.data());
    for (;;) {
        auto enter = std::find(i, end, '\n');
        if (enter == end)
            break;
        size_t len(enter - i);
        if (len > 0)
            out.write(enter.base(), len);
        out << std::endl;
        i = enter+1;
    }
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " infile outfile" << std::endl
                  << "either file can be replaced with '-' "
                  << "to mean stdin or stdout respectively"
                  << std::endl;
        return -1;
    }
    
    std::string inFilename(argv[1]);
    std::string outFilename(argv[2]);

    mapped_file_source in;
    std::ostream* out(&std::cout);
    std::ofstream outAsFile;

    if (inFilename != "-") {
        in.open(inFilename);
    } else {
        // TODO: Read std::in into a temporary file before continuing
        std::cerr << "Using std::in as a file source is not yet supported" << std::endl;
        return -2;
    }
    if (outFilename != "-") {
        outAsFile.open(outFilename);
        out = &outAsFile;
    }

    reverse(in, *out);
}
