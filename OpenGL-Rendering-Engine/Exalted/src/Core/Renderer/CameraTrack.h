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
		CameraTrackPoint(const glm::vec3& position, float yaw, float pitch, float time)
			: Position(position), Yaw(yaw), Pitch(pitch), Time(time)
			{}
	};
	
	class CameraTrack
	{
	public:
		CameraTrack(int id) : m_TrackID(id), m_StartTime(0) {}

		void AddTrackPoint(const CameraTrackPoint& trackPoint) { m_TrackPoints.emplace_back(trackPoint); }
		void PrepareTrack();
		void Update(Timestep deltaTime, glm::vec3& position, float& yaw, float& pitch);
	private:
		
		std::vector<CameraTrackPoint> m_TrackPoints; 
		int m_TrackID;
		float m_StartTime;
	};
}
