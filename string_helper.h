#ifndef string_helper_h
#define string_helper_h

/* Include standard cpp headers */

#include <string>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

/* Split given string with given delimiter */
extern vector<std::string> split(const string & s, char delimiter);

#endif
