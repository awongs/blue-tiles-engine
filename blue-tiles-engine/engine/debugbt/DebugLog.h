#pragma once

#include <stdio.h>
#include <string>

using namespace std;

static class DebugLog
{

public:
	static void Info(string infoMsg);

	static void Error(string errorMsg);

};