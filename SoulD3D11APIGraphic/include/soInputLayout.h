/********************************************************************
* @LC:	   	 10/8/2016
* @Created:	 2015/11/16
* @Filename: soImputLayout.h
* @Author:	 Lehi Muñoz (lehi.munoz@gmail.com)

* @Purpose:	Declaracion de la clase InputLayout. El proposito de esta
			clase es automatizar la creacion del layout para cualquier
			tipo de Vertex shader.
*********************************************************************/
#pragma once

/************************************************************************/
/* Inclusión de cabeceras requeridas                                    */
/************************************************************************/
#include "Soul_Core.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Declaración de la clase soImputLayout	                            */
	/************************************************************************/
	class soInputLayout : public soStandardClass<ID3DBlob*>
	{
		/************************************************************************/
		/* Constructores y destructor                                           */
		/************************************************************************/
	public:
		soInputLayout();																/*!< Constructor Standard */
		~soInputLayout();																/*!< Destructor */

		/************************************************************************************************************************/
		/* Declaración de variables miembro de la clase                                 										*/
		/************************************************************************************************************************/
	private:
		D3D11_INPUT_ELEMENT_DESC* m_ptrInputLayoutDescArray;							/*!< Lista de elementos del descriptor */
		unsigned int m_InputLayoutDescNumber;											/*!< Numero de elementos del descriptor */

	public:
		ID3D11InputLayout* m_ptrInputLayOut;											/*!< Recurso InputLayout de directX */

		/************************************************************************************************************************/
		/* Funciones de ayuda de la clase                                      													*/
		/************************************************************************************************************************/
	private:
		RESULT CreateInputLayoutDescFrsoVertexShaderSignature(ID3DBlob* _ptrShaderBlob);/*!< crea el desc para el layout a partir de un blob de shader */
		RESULT OnStartUp(ID3DBlob* _ptrShaderBlob);										/*!< Inicializa la clase segun las especificaciones recibidas */
		virtual void OnShutDown()override;												/*!< Libera los recursos solicitados por la clase para su iniciacion */
	};
}
