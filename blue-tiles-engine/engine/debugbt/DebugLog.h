#pragma once

#include <stdio.h>
#include <string>
#include <sdl2/SDL_log.h>

using namespace std;

namespace DebugLog
{
// Taken from SDL_test_log.h
static char* TimestampToString(const time_t timestamp);

static string TimestampedString(string msg);

void Info(string infoMsg);

void Info(int category, string infoMsg);

void Warn(string warnMsg);

void Warn(int category, string warnMsg);

void Error(string errorMsg);

void Error(int category, string errorMsg);

};