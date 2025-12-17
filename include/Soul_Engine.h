/********************************************************************
LC			8/16/2016
Created:	8/16/2016
Filename:	Soul_Engine.h
Author:		Lehi Muñoz

Purpose:	Cabecera que incluye las librerias linkeadas asi como los
			archivos propios de la clase
*********************************************************************/
#pragma once

/*************************************************************************/
/* Inclucion de encabezados necesarios para la compilacion de la libreria*/
/*************************************************************************/
#include "Soul_Graphics.h"

/************************************************************************/
/* Inclucion de cabeceras que incluye la libreria                       */
/************************************************************************/
#include "soResourceManager.h"
#include "soCameraManager.h"
#include "soGraphicDebugManager.h"
#include "soWorldManager.h"
#include "soAssetManager.h"
#include "soAnimationDataManager.h"
#include "soVRManager.h"
#include "soImGuiManager.h"
#include "soMaterialManager.h"

#include "soMaterial.h"
#include "soModel.h"
#include "soStaticModel.h"
#include "soSkeletalModel.h"
#include "soMesh.h"
#include "soMaterial.h"
#include "soResourceFactory.h"
#include "soTexture.h"
#include "soResource.h"
#include "soCamera.h"
#include "soComponent.h"
#include "soStaticModelComponent.h"
#include "soSkeletalModelComponent.h"
#include "soActor.h"
#include "soComponentFactory.h"
#include "soSceneNode.h"
#include "soSceneGraph.h"
#include "soEditorControl.h"
#include "SoulEngineDataStruct.h"
#include "soLight.h"
#include "soPhysicsManager.h"
#include "soTerrain.h"
#include "soFiniteStateMachine.h"
#include "soState.h"
#include "soEditorCamera.h"
#include "soPhysicsManager.h"
#include "soPhysicComponent.h"
#include "soFont.h"

/************************************************************************/
/* Inclucion de librerias necesarias para el funcionamiento de el modulo*/
/************************************************************************/
//Librerias de otros modulos
#pragma comment(lib, "SoulGraphics")