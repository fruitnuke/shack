#include "sha1.h"
#include "binary_utils.h"
#include "hexstream.h"
#include <iostream>
#include <fstream>
#include <tracedog/tracing.hpp>
#include <boost/program_options.hpp>

using namespace std;

namespace po = boost::program_options;

po::variables_map parse_options(int argc, char** argv)
{
    po::options_description desc("Allowed options");
    desc.add_options()
        ("hex", "interpret input as hex strings instead of binary.")
        ("file,f", po::value<string>(), "file to hash");

    po::positional_options_description positional_args;
    positional_args.add("file", 1);

    po::command_line_parser parser(argc, argv);
    parser.options(desc);
    parser.positional(positional_args);

    po::variables_map vm;
    po::store(parser.run(), vm);
    po::notify(vm); 
    return vm;
}

int main(int argc, char** argv)
{
    ios::sync_with_stdio(false);

    // What is this?
    // std::set_terminate (__gnu_cxx::__verbose_terminate_handler);
    
    TRDG_INITIALIZE_LIB();
    TRDG_START_TRACING();

    po::variables_map options = parse_options(argc, argv);

    vector<unsigned char> bits;

    try
    {
        if (options.count("hex"))
        {
            hexstream in(cin.rdbuf());
            in.exceptions(ios_base::badbit);
            while (!in.eof())
            {
                in >> bits;
                     
                if (in.good())
                {
                    cout << sha1::hash(bits) << endl;
                }
                else if (in.fail() && !in.eof())
                {
                    cerr << "Invalid hex string." << endl;
                    in.clear();
                }
            }
        }
        else 
        {
            // TODO: handle file failures (inc. file doesn't exist)
            // TODO: Profile. Pretty slow on a 20 MB file.
            if (!options.count("file"))
            {
                cerr << "Need to specify an input file." << endl;
                return 1;;
            }
    
            ifstream in;
            in.open(options["file"].as<string>(), ios::in | ios::binary);
            if (in.is_open())
            {
                in.seekg(0, ios::end);
                ifstream::pos_type size = in.tellg();
                bits.resize(size);
                in.seekg(0, ios::beg);
                if (in.read((char*)&bits[0], size));
                    cout << sha1::hash(bits) << endl;
                in.close();
            }             
        }
    }
    catch(ios_base::failure& exc)
    {   
        cerr << exc.what() << endl; 
        throw;
    }

    return 0;
}
