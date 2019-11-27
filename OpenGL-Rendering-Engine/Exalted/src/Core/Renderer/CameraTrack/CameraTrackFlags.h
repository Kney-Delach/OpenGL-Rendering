/***************************************************************************
 * Filename		: EnvironmentSceneResources.h
 * Name			: Ori Lazar
 * Date			: 25/11/2019
 * Description	: This header contains camera track flags, currently only used for post processing setup.
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

namespace Exalted
{
	struct CameraTrackFlags
	{
		static inline int PostProcessingChoice = 0; // 0 is equivalent to no post processing
	};
}