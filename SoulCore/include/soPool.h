/************************************************************************/
/**
* @LC     7/13/2016
* @file   soPool.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date    7/13/2016
* @brief  Declaration of class soPool.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "Soul_Math.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaracion de la clase soPool		                                */
	/************************************************************************/
	template <typename T>
	class soPool
	{		
		/************************************************************************/
		/* Declaracion de construcctores y destructor                           */
		/************************************************************************/
	public:		
		soPool()															/*!< Constructor Standard */
		{
			m_Begin = NULL;
			m_End = NULL;
			m_PoolObjectNumber = 20;
			m_Index = 0;
			m_ptrIndex = NULL;
			m_IsStartUp = false;
		}

		~soPool()															/*!< Destructor */
		{
			OnShutDown();			
		}

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:		
		T* m_End;															/*!< Direccion final de la pool */

		uint32 m_PoolObjectNumber;											/*!< Numero de elementos que conforman la pool */
		uint32 m_Index;														/*!< Ultimo lugar en ser ocupado en la pool */
		T* m_ptrIndex;														/*!< Puntero al objeto actual */

		bool m_IsStartUp;													/*!< Identifica si la pool esta inicializada */

	public:
		T* m_Begin;															/*!< Direccion de inicio de la pool */

		/************************************************************************/
		/* Funciones de ayuda de la clase                                       */
		/************************************************************************/
	private:
		T* JumpTo(const uint32 _Index)										/*!< Realisa saltos de memoria al lugar especificado */
		{			
			if (_Index > m_Index && m_IsStartUp)
			{
				SOE_ThrowException("soPool", "JumpTo()", "Invalid memory acces.");
				return NULL;
			}
				
			T* PtrData = m_Begin;
			PtrData += _Index;
			return PtrData;
		}
		
	public:
		RESULT StartUp(const uint32& _PoolObjectNumber = MIN_POOL_ELEMENTS)	/*!< Inicializa la Pool segun las especificaciones dadas */
		{
			m_Begin = static_cast<T*>(malloc(sizeof(T) * _PoolObjectNumber));
			if (m_Begin == NULL)
			{
				SOE_ThrowException("soPool", "OnStartUp()", "Memory alloc failure to initialize pool container.");
				OnShutDown();
				return EC_FALSE;
			}

			m_PoolObjectNumber = _PoolObjectNumber;
			m_ptrIndex = m_Begin;
			
			m_End = JumpTo(m_PoolObjectNumber - 1);
			m_IsStartUp = true;
			return EC_OK;
		}

		void OnShutDown()													/*!< Libera la memoria asignada a la pool */
		{
			if (!m_IsStartUp)
			{
				SOE_OutputLOG("soPool", "OnShutDown()", "Try to shut down a non initializated pool ");
			}
			SAFE_FREE(m_Begin);
		}
				
		void InsertElement(T _NewElement)									/*!< Ingresa un nuevo elemento en la pool */
		{
			if (!m_IsStartUp)
			{
				SOE_OutputLOG("soPool", "InsertElement()", "Unable to alloc object in pool. The container isn´t initializated");
				return;
			}
						
			if (m_Index < m_PoolObjectNumber)
			{
				memcpy(m_ptrIndex, &_NewElement, sizeof(T));
				
				m_Index++;
				m_ptrIndex = JumpTo(m_Index);
			}
		}

		T* GetElement(const uint32& _Index)									/*!< Otorga un puntero al elemento segun el indice especificado */
		{
			if (!m_IsStartUp)
			{
				SOE_OutputLOG("soPool", "GetElement()", "Unable to take object in pool. The container isn´t initializated");
				return NULL;
			}

			return JumpTo(_Index);
		}

		uint32 GetPoolSizeOff()												/*!< Retorna el tamaño de la pool en Bytes */
		{
			return  (sizeof(T) * m_PoolObjectNumber);
		}

		void Reset()														/*!< ignora cualquier dato anterior situando al indice al inicio */
		{
			m_Index = 0;
			m_ptrIndex = m_Begin;
		}

		uint32 GetPoolObjectsNumber()										/*!< Retorna el numero total de elementos de la pool */
		{			
			uint32 Total = m_Index;
			return Total;
		}
	};
}

