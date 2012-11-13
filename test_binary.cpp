// test_primitive_types.cpp
// For testing purposes. If all goes well, there should be no output.
#include "serialize.hpp"

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main()
{
    char char_data = 'C';
    int int_data = 225;
    double double_data = 0x0007fc00;
    string string_data = "this is a string.";
    float float_data = 0.0/0.0;

    ofstream os("out.txt", ios::out|ios::binary|ios::trunc);
    os.seekp(ios::beg);
    BinaryStreamWriter writer(os);
    writer<<char_data<<int_data<<double_data<<string_data;
    os.close();

    char char_read;
    int int_read;
    double double_read;
    string string_read;
    float float_read;

    ifstream is("out.txt", ios::in|ios::binary);
    if (is.is_open())
    {
        is.seekg(ios::end);
        cout<< "File size: "<< is.tellg()<<endl;
        is.seekg(ios::beg);
        BinaryStreamReader reader(is);
        reader>>char_read>>int_read>>double_read>>string_read>>float_read;
        is.close();
    }
    if (char_read != char_data)
        cout<<"Read char: "<<char_read<<" | Expected char: "<<char_data<<endl;
    if (int_read != int_data)
        cout<<"Read int: "<<int_read<<" | Expected int: "<<int_data<<endl;
    if (double_read != double_data)
        cout<<"Read double: "<<double_read<<" | Expected double: "<<double_data<<endl;
    if (string_read != string_data)
        cout<<"Read string: "<<string_read<<" | Expected string: "<<string_data<<endl;
    if(float_read != float_data)
        cout<<"Read flost: "<<float_read<<"| Expected float: "<<float_data<<endl;
    return 0;
}
