/************************************************************************/
/**
* @LC	  10/8/2016
* @file   soConstantBuffer.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   9/14/2016
* @brief  Declaration of soConstantBuffer. Este buffer se caracteriza por
		  conformarce de cualquier tipo de informacion. Esta info sin embargo
		  tiene la caracteristica de poder mapearse en ram, para haci actua
		  lizarla en el hardware de video en cualquier momento.
* @bug	  No know Bugs
*/
/************************************************************************/
#pragma once

/************************************************************************/
/* Inclusión de cabeceras requeridas                                    */
/************************************************************************/
#include "SoulD3D11APIGraphicDataStruct.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Forward class declaration                                            */
	/************************************************************************/
	class soBuffer;

	/************************************************************************/
	/* Declaracion de la clase soConstantBuffer                             */
	/************************************************************************/
	class soConstantBuffer : public soStandardClass<const ConstantBufferData&>
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soConstantBuffer();												/*!< Constructor Standard */
		~soConstantBuffer();											/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables miembro de la clase                         */
		/************************************************************************/
	private:
		const void* m_ptrData;											/*!< Puntero a la direccion inicial de la informacion de alimentacion del buffer */
		uint32 m_ByteWidth;												/*!< Tamaño de la informacion de alimentacion del buffer */

	public:
		soBuffer* m_ptrConstantBuffer;									/*!< Constant buffer */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		RESULT OnStartUp(const ConstantBufferData& _ConstantBufferData);/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;								/*!< Libera los recursos solicitados por la clase para su iniciacion */

	public:
		void UpdateBuffer(const void* _ptrData = NULL);					/*!< Actualiza la informacion del buffer. Si no se manda referencia, se toma la que se utilizo para la creacion del buffer */
		void* MapDiscard();												/*!< Retorna un puntero al inicio de la memoria del buffer */
		void Unmap();													/*!< Deslokea el recurso mapeado para la copia directa de datos */		
	};
}