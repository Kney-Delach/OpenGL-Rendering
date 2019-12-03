/***************************************************************************
 * Filename		: CameraTrack.h
 * Name			: Ori Lazar
 * Date			: 20/10/2019
 * Description	: Contains the declaration for the camera track points and class
 *                this is used to move the camera automatically through the scene.
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
#include "Core/Core/Timestep.h"
#include "glm/vec3.hpp"

namespace Exalted
{
	struct CameraTrackPoint
	{
		glm::vec3 Position;
		float Yaw;
		float Pitch;
		float Time;
		int PostProcessingFlag; 
		CameraTrackPoint(const glm::vec3& position, float yaw, float pitch, float time, int postProcessingFlag = 0)
			: Position(position), Yaw(yaw), Pitch(pitch), Time(time), PostProcessingFlag(postProcessingFlag)
			{}
	};
	
	class CameraTrack
	{
	public:
		CameraTrack(int id = 0) : m_StartTime(0), m_TrackID(id) {}

		void AddTrackPoint(const CameraTrackPoint& trackPoint) { m_TrackPoints.emplace_back(trackPoint); }
		void PrepareTrack();
		int Update(Timestep deltaTime, glm::vec3& position, float& yaw, float& pitch);
	private:
		std::vector<CameraTrackPoint> m_TrackPoints; 
		float m_StartTime;
		int m_TrackID; // not currently using this 
	};
}