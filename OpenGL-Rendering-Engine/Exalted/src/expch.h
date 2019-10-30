/***************************************************************************
 * Filename		: expch.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Precompiled header for this engine.
     .---.
   .'_:___".
   |__ --==|
   [  ]  :[|
   |__| I=[|
   / / ____|
  |-/.____.'
 /___\ /___\
***************************************************************************/
#pragma once

#include <iostream>
#include <memory>
#include <utility>
#include <algorithm>
#include <functional>

#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "Core/Log.h"

#ifdef EX_PLATFORM_WINDOWS
	#include <Windows.h>
#endif