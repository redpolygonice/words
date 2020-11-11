#ifndef COMMON_H
#define COMMON_H

#include "types.h"

string i2s(int number);
int s2i(const string &text);
bool isFileExists(const string &fileName);
string currentTime();
StringList splitString(const string &text, const string &separator = " ");
string strReplace(const string &s, const string &search, const string &replace);

#endif // COMMON_H
