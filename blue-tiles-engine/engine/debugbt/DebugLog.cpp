#include "DebugLog.h"
#include <iostream>



void DebugLog::Info(string infoMsg)
{
	cout << infoMsg << endl;
}

void DebugLog::Error(string errorMsg)
{
	cerr << errorMsg << endl;
}
