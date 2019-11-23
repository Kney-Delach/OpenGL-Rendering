#include "expch.h"
#include "CameraTrack.h"
#include "Core/Application.h"

#define TIME static_cast<float>(Exalted::Application::Get().GetWindow().GetTime())

namespace Exalted
{
	void CameraTrack::PrepareTrack()
	{
		m_StartTime = TIME;
	}

	void CameraTrack::Update(Timestep deltaTime, glm::vec3& position, float& yaw, float& pitch)
	{
		glm::vec3 interpolatePos1, interpolatePos2;
		float interpolatePitch1, interpolatePitch2;
		float interpolateYaw1, interpolateYaw2;

		int indexPos = -1;

		float currentTime = TIME - m_StartTime;
		float combinedTime = 0.0f;

		for (int i = 0; i < m_TrackPoints.size(); ++i) 
		{
			if (currentTime <= combinedTime) 
			{
				indexPos = i - 1;
				break;
			}
			else {
				combinedTime += m_TrackPoints[i].Time;
			}
		}

		EX_CORE_TRACE("Index Position {0}", indexPos);
		if (indexPos == -1) 
		{
			return;
		}


		interpolatePos1 = m_TrackPoints[indexPos - 1].Position;
		interpolatePos2 = m_TrackPoints[indexPos].Position;

		interpolatePitch1 = m_TrackPoints[indexPos - 1].Pitch;
		interpolatePitch2 = m_TrackPoints[indexPos].Pitch;

		interpolateYaw1 = m_TrackPoints[indexPos - 1].Yaw;
		interpolateYaw2 = m_TrackPoints[indexPos].Yaw;

		float t = (currentTime - combinedTime) / m_TrackPoints[indexPos].Time;
		t *= -1;

		position = ((interpolatePos1 * t) + (interpolatePos2 * (1.0f - t)));  //todo: Implement a method to access the camera from here
		yaw = ((interpolateYaw1 * t) + (interpolateYaw2 * (1.0f - t)));		  //todo: Implement a method to access the camera from here
		pitch = ((interpolatePitch1 * t) + (interpolatePitch2 * (1.0f - t))); //todo: Implement a method to access the camera from here
	}
}
