/********************************************************************/
/**
* @file   soProfiler.h
* @author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   2017/06/01
* @brief  
*
*
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

#include "soPlatformTypes.h"
#include "soTimer.h"
#include <sstream>

namespace SoulSDK
{
	typedef struct 
	{
		bool bValid;										/*!< Wheter this data is valid */
		uint32 iProfileInstances;							/*!< # of times Profile Begin called */
		int32 iOpenProfiles;								/*!< # of times Profile Begin w/o */

		soString strName;									/*!< Name of sample */
		float fStartTime;									/*!< Current open profile start time */
		float fAccumulator;								/*!< All samples this frame added */

		float fChildrenSampleTime;						/*!< Time taken by all children */
		uint32 iNumParents;									/*!< Number of profile parents */
			
	}ProfileSample;

	typedef struct
	{
		bool bValid;										/*!< Wheter this data is valid */
		soString strName;									/*!< Name of sample */
		float fAverange;									/*!< Average time per frame (percentage) */
		float fMin;											/*!< Minimum time per frame (percentage) */
		float fMax;											/*!< Maximum time per frame (percentage) */

	} ProfileSampleHistory;

	class soProfiler
	{
	public:
		soProfiler();											/*!< Standard Constructor */
		virtual ~soProfiler();									/*!< Standard Desstructor */

	private:
		std::vector<ProfileSample> m_samples;					/*!< Samples List */
		std::vector<ProfileSampleHistory> m_history;			/*!< History List */
		soTimer m_timer;										/*!< Timer of the Profiler */
		std::stringstream m_bufferText;							/*!< Text Buffer */

		float m_startProfile;
		float m_endProfile;
		float m_deltaTime;

	public:
		void Init(size_t _numOfProfileSamples);
		void Begin(soString& _name);
		void End(soString& _name);
		void DumpOutputToBuffer();
		std::stringstream * GetOutputBuffer();
		void SetDeltaTime(float _dt);

	private:
		void StoreProfileInHistory(soString& _name, float _percent);
		void GetProfileFromHistory(soString& _name, float* _ave, float* _min, float* _max);
	};
}

