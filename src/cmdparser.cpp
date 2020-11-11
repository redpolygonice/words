#include "cmdparser.h"

CmdParser::CmdParser()
{
}

void CmdParser::parse(int argc, char **argv)
{
	if (argc >= 2)
		_fileName = argv[1];
}
