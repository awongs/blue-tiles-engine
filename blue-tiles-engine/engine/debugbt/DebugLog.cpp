#include "DebugLog.h"
#include <iostream>


namespace DebugLog
{
	void Info(string infoMsg)
	{
		cout << infoMsg << endl;
	}

	void Error(string errorMsg)
	{
		cerr << errorMsg << endl;
	}
}
