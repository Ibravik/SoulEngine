/********************************************************************/
/**
* @LC	  10/8/2016
* @file   soViewPort.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/29/2015
* @brief  Implementacion de la clase soViewPort
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soViewPort.h"

/************************************************************************/
/* Definicion de la clase soViewPort									*/
/************************************************************************/
namespace SoulSDK
{
	soViewPort::soViewPort()
	{
		ZeroMemory(&m_ViewPort, sizeof(D3D11_VIEWPORT));
		Connect(std::bind(&soViewPort::OnStartUp, this, std::placeholders::_1));
	}
	
	soViewPort::~soViewPort()
	{
		ShutDown();
	}

	RESULT soViewPort::OnStartUp(const ViewPortData& _ViewPortInfo)
	{			
		m_ViewPort.Width = _ViewPortInfo.Width;
		m_ViewPort.Height = _ViewPortInfo.Height;
		m_ViewPort.TopLeftX = _ViewPortInfo.TopLeftX;
		m_ViewPort.TopLeftY = _ViewPortInfo.TopLeftY;
		m_ViewPort.MinDepth = _ViewPortInfo.MinDepth;
		m_ViewPort.MaxDepth = _ViewPortInfo.MaxDepth;

		return EC_OK;
	}

	void soViewPort::OnShutDown()
	{

	}
}