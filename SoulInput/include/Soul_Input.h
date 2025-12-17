/********************************************************************
LC			10/13/2016
Created:	2016/10/10
Filename:	Soul_Input.h
Author:		Rubén Rodríguez

Purpose:	Cabecera que incluye las librerias linkeadas asi como los
archivos propios de la clase
*********************************************************************/
#pragma once

/************************************************************************* /
/* Inclucion de encabezados necesarios para la compilacion de la libreria*/
/*************************************************************************/
#include "Soul_Engine.h"

/************************************************************************/
/* Inclucion de cabeceras que incluye la libreria                       */
/************************************************************************/
#include "soOISManager.h"
#include "soOISEventHandler.h"
#include "soInputMap.h"
#include "soSuscription.h"

/************************************************************************/
/* Linkeo de librerias                                                  */
/************************************************************************/
#if defined(_DEBUG)
#pragma comment(lib, "OIS_static_d")
#else
#pragma comment(lib, "OIS_static")
#endif


#pragma comment(lib, "SoulEngine")
