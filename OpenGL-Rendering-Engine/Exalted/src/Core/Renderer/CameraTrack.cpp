/***************************************************************************
 * Filename		: CameraTrack.h
 * Name			: Ori Lazar
 * Date			: 20/10/2019
 * Description	: Contains the implementation for the camera track points and class
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
#include "expch.h"
#include "CameraTrack.h"
#include "Core/Application.h"
#include "CameraTrack/CameraTrackFlags.h"

#define TIME static_cast<float>(Exalted::Application::Get().GetWindow().GetTime())

namespace Exalted
{
	void CameraTrack::PrepareTrack()
	{
		m_StartTime = TIME;
	}

	int CameraTrack::Update(Timestep deltaTime, glm::vec3& position, float& yaw, float& pitch)
	{
		glm::vec3 tempPosA;
		glm::vec3 tempPosB;
		float tempYawA;
		float tempYawB;
		float tempPitchA;
		float tempPitchB;

		const float elapsedTime = TIME - m_StartTime;
		float totalTime = 0.0f;
		int trackPointIndex = -1;

		for (int i = 0; i < m_TrackPoints.size(); i++) 
		{
			if (elapsedTime <= totalTime)
			{
				trackPointIndex = i - 1;
				break;
			}
			totalTime += m_TrackPoints[i].Time;
		}
		if (trackPointIndex == -1) 
			return -1;
		CameraTrackFlags::PostProcessingChoice = m_TrackPoints[trackPointIndex].PostProcessingFlag;
		tempPosA = m_TrackPoints[trackPointIndex - 1].Position;
		tempPosB = m_TrackPoints[trackPointIndex].Position;
		tempYawA = m_TrackPoints[trackPointIndex - 1].Yaw;
		tempYawB = m_TrackPoints[trackPointIndex].Yaw;
		tempPitchA = m_TrackPoints[trackPointIndex - 1].Pitch;
		tempPitchB = m_TrackPoints[trackPointIndex].Pitch;
		const float normalizedTime = ((elapsedTime - totalTime) / m_TrackPoints[trackPointIndex].Time) * -1;
		//normalizedTime *= -1;

		position = tempPosA * normalizedTime + tempPosB * (1.0f - normalizedTime);
		pitch = tempPitchA * normalizedTime + tempPitchB * (1.0f - normalizedTime);
		yaw = tempYawA * normalizedTime + tempYawB * (1.0f - normalizedTime);
		return 0;
	}
}