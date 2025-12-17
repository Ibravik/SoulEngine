#include "soProfiler.h"
#include <assert.h>
#include "soStringFormat.h"

namespace SoulSDK
{
	soProfiler::soProfiler()
	{
		m_startProfile = 0;
		m_endProfile = 0;
	}


	soProfiler::~soProfiler()
	{
	}
	
	void soProfiler::Init(size_t _numOfProfileSamples)
	{
		m_samples.reserve(_numOfProfileSamples);
		m_history.reserve(_numOfProfileSamples);

		m_timer.Reset();
		m_startProfile = static_cast<float>(m_timer.getMilliseconds());
	}
	
	void soProfiler::Begin(soString& _name)
	{
		size_t i = 0;

		while (i < m_samples.size() && m_samples[i].bValid == true)
		{
			if (m_samples[i].strName.compare(_name) == 0)
			{
				m_samples[i].iOpenProfiles++;
				m_samples[i].iProfileInstances++;
				m_samples[i].fStartTime = static_cast<float>(m_timer.getMilliseconds());
				assert(m_samples[i].iOpenProfiles == 1); //Max 1 abierto a la vez
				return;
			}
			i++;
		}

		//Si ya existe reseteamos
		if (!m_samples.empty())
		{
			for (size_t j = 0; j < m_samples.size(); ++j)
			{
				if (m_samples[j].strName.compare(_name) == 0)
				{
					m_samples[j].bValid = true;
					m_samples[j].iOpenProfiles = 1;
					m_samples[j].iProfileInstances = 1;
					m_samples[j].fAccumulator = 0;
					m_samples[j].fStartTime = static_cast<float>(m_timer.getMilliseconds());
					m_samples[j].fChildrenSampleTime = 0;
					return;
				}
			}
		}

		//Si no existe el Sample se crea
		ProfileSample tempSample;
		tempSample.strName = _name;
		tempSample.bValid = true;
		tempSample.iOpenProfiles = 1;
		tempSample.iProfileInstances = 1;
		tempSample.fAccumulator = 0;
		tempSample.fStartTime = static_cast<float>(m_timer.getMilliseconds());
		tempSample.fChildrenSampleTime = 0;

		//Se agrega a la lista de samples
		m_samples.push_back(tempSample);
	}

	void soProfiler::End(soString& _name)
	{
		//Si no hay samples retorna
		if (m_samples.empty())
			return;

		uint32 numParents = 0;
		size_t i = 0;

		while (i < m_samples.size() && m_samples[i].bValid == true)
		{
			//Si es un sample que ya existe
			if (m_samples[i].strName.compare(_name) == 0)
			{
				uint32 inner = 0;
				int32 parent = -1;
				float fEndTime = static_cast<float>(m_timer.getMilliseconds());

				m_samples[i].iOpenProfiles--;

				//count all parents and find the inmediate parent
				while (m_samples.size() > inner &&	m_samples[inner].bValid == true)
				{
					if (m_samples[inner].iOpenProfiles > 0)
					{ //Found parent (any open profiles are parents)
						numParents++;
						if (parent < 0)
						{ //Replace invalid parent (index)
							parent = inner;
						}
						else if (m_samples[inner].fStartTime >= m_samples[parent].fStartTime)
						{  //Replace with more inmediate parent
							parent = inner;
						}
					}
					inner++;
				}

				//Remember the current number of parents of the sample
				m_samples[i].iNumParents = numParents;

				if (parent > -1)
				{ //Record this time in fChildrenSampleTime (add it in)
					m_samples[parent].fChildrenSampleTime += fEndTime - m_samples[i].fStartTime;
				}

				//Save sample time in accumulator
				m_samples[i].fAccumulator += fEndTime - m_samples[i].fStartTime; //Este es el verdadero tiempo en ms que se tarda el sample
				return;
			}
			i++;
		}
		
	}

	void soProfiler::DumpOutputToBuffer()
	{
		m_endProfile = static_cast<float>(m_timer.getMilliseconds());
		
		//Clear Text Buffer
		m_bufferText.str("");

		//4 digits after the point
		m_bufferText.precision(5);

		//Put Text in Buffer
		m_bufferText << "MilliSec: Ave:  Min:  Max:  #: Profile name: \n" << "-----------------------------------------\n";

			if (!m_samples.empty())
			{
			for (size_t i = 0; i < m_samples.size(); ++i)
			{
				if (m_samples[i].bValid)
				{
					float sampleTime, percentTime, aveTime, minTime, maxTime;
					soString indentedName;
					uint32 numSpaces;

					if (m_samples[i].iOpenProfiles < 0)
					{
						assert(!"ProfileEnd() called without a ProfileBegin()");
					}
					else if (m_samples[i].iOpenProfiles > 0)
					{
						assert(!"ProfileBegin() called without a ProfileEnd()");
					}
					
					sampleTime = m_samples[i].fAccumulator - m_samples[i].fChildrenSampleTime; //Se le resta el acumulado de los hijos ya que ellos son los que se tardan
					percentTime = (sampleTime / (m_endProfile - m_startProfile)) * 100.0f; //Se saca el porcentaje de lo que gasta cada sample
					aveTime = minTime = maxTime = percentTime;

					//Add ne mesurement into the history and get ave, min and max
					StoreProfileInHistory(m_samples[i].strName, percentTime);
					GetProfileFromHistory(m_samples[i].strName, &aveTime, &minTime, &maxTime);

					//Format data
					m_bufferText << " " << m_samples[i].fAccumulator << "ms" << " " << aveTime << "%" << "    " << minTime << "%" << "    " << maxTime << "%" << "     " << m_samples[i].iProfileInstances << ' ';
					
					//Agragar tabulaciones dependiendo el numero de padres que tenga
					indentedName = m_samples[i].strName;
					numSpaces = m_samples[i].iNumParents * 4;
					indentedName.insert(indentedName.begin(), numSpaces, ' ');

					m_bufferText << ws2s(indentedName.c_str()) << "\n";
				}
			}
		}

		//Reset samples for next frame
		for (size_t i = 0; i < m_samples.size(); ++i)
		{
			m_samples[i].bValid = false;
		}

		m_startProfile = static_cast<float>(m_timer.getMilliseconds());
	}

	void soProfiler::StoreProfileInHistory(soString & _name, float _percent)
	{
		
		float oldRatio;
		float newRatio = 0.8f * (m_deltaTime / 1000); //Para que esten en decimal
		if (newRatio > 1.0f)
		{
			newRatio = 1.0f;
		}
		oldRatio = 1.0f - newRatio;

		size_t i = 0;
		while (i < m_history.size() && m_history[i].bValid == true)
		{
			if (m_history[i].strName.compare(_name) == 0)
			{
				m_history[i].fAverange = (m_history[i].fAverange * oldRatio) + (_percent * newRatio);
				if (_percent < m_history[i].fMin)
				{
					m_history[i].fMin = _percent;
				}
				else
				{
					m_history[i].fMin = (m_history[i].fMin * oldRatio) + (_percent * newRatio);
				}

				if (_percent > m_history[i].fMax)
				{
					m_history[i].fMax = _percent;
				}
				else
				{
					m_history[i].fMax = (m_history[i].fMax * oldRatio) + (_percent * newRatio);
				}
				return;
			}
			i++;
		}
	
		//Add to history
		ProfileSampleHistory tempHistory;
		tempHistory.strName = _name;
		tempHistory.bValid = true;
		tempHistory.fAverange = tempHistory.fMax = tempHistory.fMin = _percent;

		m_history.push_back(tempHistory);
	}

	void soProfiler::GetProfileFromHistory(soString & _name, float* _ave, float* _min, float* _max)
	{
		size_t i = 0;

		while (i < m_history.size() && m_history[i].bValid == true)
		{
			if (m_history[i].strName.compare(_name) == 0)
			{
				*_ave = m_history[i].fAverange;
				*_min = m_history[i].fMin;
				*_max = m_history[i].fMax;
				return;
			}
			i++;
		}
		
		*_ave = *_min = *_max = 0.0f;
	}

	std::stringstream * soProfiler::GetOutputBuffer()
	{
		return &m_bufferText;
	}
	void SoulSDK::soProfiler::SetDeltaTime(float _dt)
	{
		m_deltaTime = _dt;
	}
}
