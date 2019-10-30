/***************************************************************************
 * Filename		: Timestep.h
 * Name			: Ori Lazar
 * Date			: 29/10/2019
 * Description	: Contains the implementation for time taken calculation for 
 *                frame intervals. Stored in seconds.
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

//todo: Implement operator overrides (+,+=)

namespace Exalted
{
	class Timestep
	{
	public:
		Timestep(float time = 0.0f) : m_Time(time) {}
		_NODISCARD inline operator float() const { return m_Time; }
		_NODISCARD inline float GetSeconds() const { return m_Time; }
		_NODISCARD inline float GetMilliseconds() const { return m_Time * 1000.f; }
	private:
		float m_Time;
	};
}