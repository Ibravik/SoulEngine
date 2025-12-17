/********************************************************************/
/**
* @file   soTimer.h
* @author Rubén Rodríguez Serrano (rubn2113@gmail.com)
* @date   2017/06/01
* @brief  
*
*
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once
namespace SoulSDK
{
	class soTimer
	{
	public:
		soTimer();											/*!< Standard Constructor */
		virtual ~soTimer();									/*!< Standard Destructor */

		void Reset();										/*!< Reset the timer to zero */

		unsigned long getMilliseconds();					/*!< Returns time in milliseconds since timer was initialized or last reset */

		unsigned long getMicroseconds();					/*!< Returns time in microseconds since timer was initialized or last reset */

		unsigned long getMillisecondsCPU();					/*!< Returns time in milliseconds since timer was initialized or last reset. Only CPU timer measured */

		unsigned long getMicrosecondsCPU();					/*!< Returns time in microseconds since timer was initialized or last reset. Only CPU timer measured */

		unsigned long getStartMs() const;					/*!< Returns the time at which the timer was initialized, in milliseconds. */

	private:
		struct Data;
		Data* m_pData;
	};
}
