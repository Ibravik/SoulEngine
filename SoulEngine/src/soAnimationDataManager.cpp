/********************************************************************/
/**
* @LC     10/19/2016
* @file   soAnimationDataManager.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   10/19/2016
* @brief  Implementacion de la clase soAnimationDataManager
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soAnimationDataManager.h"

/************************************************************************/
/* Definicion de la clase soResourceManager                             */
/************************************************************************/
namespace SoulSDK
{
	soAnimationDataManager::soAnimationDataManager()
	{
		Connect(std::bind(&soAnimationDataManager::OnStartUp, this));

		m_Data = static_cast<uint8*>(malloc(sizeof(uint8) * BytesNumber));
		m_Index = 0;
		m_ptrDataIndex = m_Data;
		m_FreeSpace = BytesNumber;
		m_TotalSpace = BytesNumber;
	}
	
	soAnimationDataManager::~soAnimationDataManager()
	{
	}

	RESULT soAnimationDataManager::OnStartUp()
	{
		TextureData TD;

		TD.TextureType = API_Texture1D;
		TD.T1DData.Width = 16384;
		if (FAIL(m_GlobalAnimationBuffer.Create(TD)))
		{
			return EC_FALSE;
		}

		TD.TextureType = API_Texture1D;
		TD.T1DData.Width = 16384;
		if (FAIL(m_OutputBuffer.Create(TD)))
		{
			return EC_FALSE;
		}

		return EC_OK;
	}

	void soAnimationDataManager::OnShutDown()
	{
		SAFE_DELETE_ARRAY(m_Data);
	}

	void soAnimationDataManager::PushData(void* _ptrData, uint32 _SizeOf)
	{
		if (m_FreeSpace > _SizeOf)
		{
			memcpy(m_ptrDataIndex, _ptrData, _SizeOf);
			m_ptrDataIndex += _SizeOf;
			m_FreeSpace -= _SizeOf;
			m_Index++;
		}		
	}

	void soAnimationDataManager::InsertSkeletalMatrix(soJoint* _RootNode, SkeletalCB& _CB)
	{
		//Queue de chequeo de nodos
		std::queue<soJoint*> GraphList;
		soJoint* GraphNode;

		//Nodo inicial del grafo
		GraphList.push(_RootNode);

		//Indice inicial de la info del esqueleto
		_CB.SkeletalIndex = m_Index;

		uint32 BoneNumber = 0;
		
		//Matrices offset
		while (!GraphList.empty())
		{
			//Tomamos el primer nodo de la lista
			GraphNode = GraphList.front();

			//Sacamos al nodo visitado
			GraphList.pop();

			//Aumentamos el numero de huesos
			BoneNumber++;

			//Ingresamos la informacion del hueso al buffer
			PushData(&GraphNode->m_JoinOffsetMatrix, sizeof(soMatrix4x4));

			////Agregamos los nodos hijos a la lista
			//for (uint32 i = 0; i < GraphNode->GetChildrensArraySize(); i++)
			//{
			//	GraphList.push(&GraphNode->GetChildrensArrayElement(i));
			//}
		}

		//Numero de nodos del grafo 
		_CB.BoneNumber = BoneNumber;
		
		//Nodo inicial del grafo
		GraphList.push(_RootNode);

		//Matrices transform
		while (!GraphList.empty())
		{
			//Tomamos el primer nodo de la lista
			GraphNode = GraphList.front();

			//Sacamos al nodo visitado
			GraphList.pop();

			//Ingresamos la informacion del hueso al buffer
			PushData(&GraphNode->m_NodeTransformation, sizeof(soMatrix4x4));

			//Agregamos los nodos hijos a la lista
			/*for (uint32 i = 0; i < GraphNode->GetChildrensArraySize(); i++)
			{
				GraphList.push(&GraphNode->GetChildrensArrayElement(i));
			}*/
		}

		uint32 Depth;
		m_GlobalAnimationBuffer.Lock1D(Depth, (void**)&m_Data, m_TotalSpace);
		Depth = 4;
		m_GlobalAnimationBuffer.Unlock1D();
	}

	void soAnimationDataManager::InsertAnimation(soAnimation* _Animation, SkeletalCB& _CB)
	{
		std::map<uint32, soKeyFrame>::iterator it = _Animation->m_KeyFrameMap.begin();

		//Indice inicial de la info de la animacion
		_CB.Animations[_CB.AnimationNum].AnimationIndex = m_Index;

		//Numero de KeyFrames
		_CB.Animations[_CB.AnimationNum].AnimationKeyFrameNum = _Animation->m_KeyFrameMap.size();

		uint32 KFIndex = 0;
		while (it != _Animation->m_KeyFrameMap.end())
		{
			uint32 RotationMatrix = 0;
			uint32 ScaleMatrix = 0;
			uint32 TranslateMatrix = 0;

			_CB.Animations[_CB.AnimationNum].AnimationKeyFramePosMat[KFIndex] = static_cast<uint32>((*it).second.m_TranslationKey.size());
			for (size_t i = 0; i < (*it).second.m_TranslationKey.size(); i++)
			{
				soMatrix4x4 Position;
				Position = soMath::TranslationMatrix4x4((*it).second.m_TranslationKey.at(i).Translation);
				PushData(&Position, sizeof(soMatrix4x4));
				TranslateMatrix++;
			}

			_CB.Animations[_CB.AnimationNum].AnimationKeyFrameRotMat[KFIndex] = static_cast<uint32>((*it).second.m_RotationKey.size());
			for (size_t i = 0; i < (*it).second.m_RotationKey.size(); i++)
			{
				soMatrix4x4 Rotation;
				soMath::QuaternionToMatrixRotation4x4((*it).second.m_RotationKey.at(i).Rotation, Rotation);
				PushData(&Rotation, sizeof(soMatrix4x4));
				RotationMatrix++;
			}

			_CB.Animations[_CB.AnimationNum].AnimationKeyFrameScaMat[KFIndex] = static_cast<uint32>((*it).second.m_ScaleKey.size());
			for (size_t i = 0; i < (*it).second.m_ScaleKey.size(); i++)
			{
				soMatrix4x4 Scale;
				Scale = soMath::ScalarMatrix4x4((*it).second.m_ScaleKey.at(i).Scalation);
				PushData(&Scale, sizeof(soMatrix4x4));
				ScaleMatrix++;
			}	
			KFIndex++;
			it++;
		}
		
		_CB.AnimationNum++;
	}
}
