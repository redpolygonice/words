#ifndef CMDPARSER_H
#define CMDPARSER_H

#include "types.h"

/** Command line parser */
class CmdParser
{
private:
	string _fileName;

public:
	CmdParser();
	void parse(int argc, char **argv);
	inline string getInputFile() const { return _fileName; }
};

#endif // CMDPARSER_H
