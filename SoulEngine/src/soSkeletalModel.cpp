/********************************************************************/
/**
* @LC	  9/27/2016
* @file   soSkeletalModel.cpp
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/24/2016
* @brief  Implementacion de la clase soSkeletalModel
* @bug	   No known bugs.
*/
/********************************************************************/

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soMesh.h"
#include "soSkeletalModel.h"

/************************************************************************/
/* Definicion de la clase soSkinnedModel				                */
/************************************************************************/
namespace SoulSDK
{
	soSkeletalModel::soSkeletalModel()
	{
		m_IsStatic = (uint32)0;
	}

	soSkeletalModel::~soSkeletalModel()
	{
		ShutDown();
	}
	
	void soSkeletalModel::CreateNodeGraph()
	{
		//Queue de chequeo de nodos
		std::queue<aiNode*> NodeList;
		aiNode* Node = m_Scene->mRootNode;
		NodeList.push(m_Scene->mRootNode);

		std::queue<soJoint*> GraphList;
		soJoint* GraphNode;
		soJoint* Parent;

		//Nodo inicial del grafo
		SetNodeInfo(*Node, m_RootNode, NULL);
		m_RootNode.m_ParentTransform = soMath::IdentityMatrix4x4();
		GraphList.push(&m_RootNode);

		uint32 ID = 0;
		while (!NodeList.empty())
		{
			//Tomamos el primer nodo de la lista
			Node = NodeList.front();
			GraphNode = GraphList.front();

			//Sacamos al nodo visitado
			NodeList.pop();
			GraphList.pop();
			
			//Mapeamos el nodo
			m_JointMap[GraphNode->m_name] = GraphNode;			

			//Asignamos un ID
			GraphNode->m_ID = ID;
			ID++;

			//Mapeamos los nodos que nos afectan
			Parent = GraphNode->m_Parent;
			uint32 NodeIndex = 0;
			while (Parent != NULL)
			{
				m_SkeletalCB.GraphJoint[GraphNode->m_ID].BoneIndexHierarchy[NodeIndex] = Parent->m_ID;
				NodeIndex++;
				Parent = Parent->m_Parent;
			}
			m_SkeletalCB.GraphJoint[GraphNode->m_ID].BoneIndexHierarchyNum = NodeIndex;
						
			//Agregamos los nodos hijos a la lista
			for (uint32 i = 0; i < Node->mNumChildren; i++)
			{
				NodeList.push(*&Node->mChildren[i]);

				soJoint* NewChild = SOE_NEW soJoint;
				GraphList.push(NewChild);
				GraphNode->AddConexions(NewChild);
				SetNodeInfo(*Node->mChildren[i], *NewChild, GraphNode);
			}
		}

		//Ahora relacionamos los huesos al grafo general y asignamos indice a los vertices del hueso relacionado
		for (uint32 a = 0; a < m_Scene->mNumMeshes; a++)
		{
			//Ahora relacionamos toda la informacion en una sola lista 
			for (uint32 i = 0; i < m_Scene->mMeshes[a]->mNumBones; i++)
			{
				soString BoneName(s2ws(std::string(m_Scene->mMeshes[a]->mBones[i]->mName.C_Str())));

				if (m_JointMap.find(BoneName) != m_JointMap.end())
				{
					if (!m_JointMap[BoneName]->IsStarted())
					{
						//Encontramos el nodo correspondiente al huesito					
						m_JointMap[BoneName]->StartUp();	
						m_JointMap[BoneName]->SetOffsetMatrix(m_Scene->mMeshes[a]->mBones[i]->mOffsetMatrix);						
					}
				}
			}
		}

		//Por ultimo seteamos el indice segun el hueso a los meshes creados
		for (uint32 i = 0; i < static_cast<uint32>(m_MeshList.size()); i++)
		{
			for (uint32 j = 0; j < m_MeshList.at(i)->m_VertexBuffer.GetsVertexArraySize(); j++)
			{				
				//Asignamos el indice q se asigno al nodo segun el nombre que este en el vertice
				for (uint32 k = 0; k < 4; k++)
				{
					soString* Name = m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).BoneName[k];
					if (Name != NULL)
					{
						if (m_JointMap.at(*m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).BoneName[k])->IsStarted())
						{
							m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).BoneIndex[k] = m_JointMap.at(*m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).BoneName[k])->m_ID;
						}
						SAFE_DELETE(m_MeshList.at(i)->m_VertexBuffer.GetsVertexArrayElement(j).BoneName[k]);
					}
				}				
			}
			SAFE_DELETE(m_MeshList.at(i)->m_VertexBuffer.m_ptrVertexBuffer);
			m_MeshList.at(i)->m_VertexBuffer.CreateVertexBuffer();
		}

	}

	void soSkeletalModel::SetNodeInfo(const aiNode& _aiNode, soJoint& _Joint, soJoint* _ParentJoint)
	{
		_Joint.m_name = soString(s2ws(std::string(_aiNode.mName.C_Str())));
		_Joint.m_Parent = _ParentJoint;

		_Joint.m_NodeTransformation.m00 = _aiNode.mTransformation.a1; _Joint.m_NodeTransformation.m10 = _aiNode.mTransformation.b1; _Joint.m_NodeTransformation.m20 = _aiNode.mTransformation.c1; _Joint.m_NodeTransformation.m30 = _aiNode.mTransformation.d1;
		_Joint.m_NodeTransformation.m01 = _aiNode.mTransformation.a2; _Joint.m_NodeTransformation.m11 = _aiNode.mTransformation.b2; _Joint.m_NodeTransformation.m21 = _aiNode.mTransformation.c2; _Joint.m_NodeTransformation.m31 = _aiNode.mTransformation.d2;
		_Joint.m_NodeTransformation.m02 = _aiNode.mTransformation.a3; _Joint.m_NodeTransformation.m12 = _aiNode.mTransformation.b3; _Joint.m_NodeTransformation.m22 = _aiNode.mTransformation.c3; _Joint.m_NodeTransformation.m32 = _aiNode.mTransformation.d3;
		_Joint.m_NodeTransformation.m03 = _aiNode.mTransformation.a4; _Joint.m_NodeTransformation.m13 = _aiNode.mTransformation.b4; _Joint.m_NodeTransformation.m23 = _aiNode.mTransformation.c4; _Joint.m_NodeTransformation.m33 = _aiNode.mTransformation.d4;

		if (_ParentJoint != NULL)
		{
			_Joint.m_ParentTransform = _ParentJoint->m_NodeTransformation * _Joint.m_NodeTransformation;
		}
		else
		{
			_Joint.m_ParentTransform = _Joint.m_NodeTransformation;
		}
	}

	uint32 soSkeletalModel::FindRotation(soKeyFrame& _KeyFrame, float _deltaAnimationTime)
	{
		uint32 _Size = static_cast<uint32>(_KeyFrame.m_RotationKey.size());
		_Size--;
		for (uint32 i = 0; i < _Size; i++)
		{
			if (_deltaAnimationTime < _KeyFrame.m_RotationKey.at(i + 1).KeyTime)
			{
				return i;
			}			
		}		
		return 0;
	}

	uint32 soSkeletalModel::FindScaling(soKeyFrame& _KeyFrame, float _deltaAnimationTime)
	{
		uint32 _Size = static_cast<uint32>(_KeyFrame.m_ScaleKey.size());
		_Size--;
		for (uint32 i = 0; i < _Size; i++)
		{
			if (_deltaAnimationTime < _KeyFrame.m_ScaleKey.at(i + 1).KeyTime)
			{
				return i;
			}
		}
		
		return 0;
	}

	uint32 soSkeletalModel::FindPosition(soKeyFrame& _KeyFrame, float _deltaAnimationTime)
	{
		uint32 _Size = static_cast<uint32>(_KeyFrame.m_TranslationKey.size());
		_Size--;
		for (uint32 i = 0; i < _Size; i++)
		{
			if (_deltaAnimationTime < _KeyFrame.m_TranslationKey.at(i + 1).KeyTime)
			{
				return i;
			}

		}
		return 0;
	}

	void soSkeletalModel::CalcInterpolatedRotation(soKeyFrame& _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation)
	{
		_Animation.RotationIndex[0] = 0;
		_Animation.RotationIndex[1] = 0;
		_Animation.InterpolationFactor[1] = 0.0f;

		// we need at least two values to interpolate...
		if (_KF.m_RotationKey.size() == 1)
		{
#ifdef SoftwareAnimation
			soMath::QuaternionToMatrixRotation4x4(_KF.m_RotationKey.at(0).Rotation, _OutMat);
#endif
			return;
		}

		uint32 RotationIndex = FindRotation(_KF, _Delta);
		uint32 NextRotationIndex = (RotationIndex + 1);

		/*if (static_cast<uint32>(_Animation._Animation->m_Duration) == static_cast<uint32>(_KF.m_RotationKey.size() - 1))
		{
			Math::QuaternionToMatrixRotation4x4(Math::QuaternionConjugate(_KF.m_RotationKey.at(RotationIndex).Rotation), _OutMat);
			return;
		}*/

		float DeltaTime = _KF.m_RotationKey.at(NextRotationIndex).KeyTime - _KF.m_RotationKey.at(RotationIndex).KeyTime;
		float Factor = (_Delta - _KF.m_RotationKey.at(RotationIndex).KeyTime) / DeltaTime;
		
#ifdef SoftwareAnimation
		soQuaternion StartRotationQ = soMath::QuaternionConjugate(_KF.m_RotationKey.at(RotationIndex).Rotation);
		soQuaternion EndRotationQ = soMath::QuaternionConjugate(_KF.m_RotationKey.at(NextRotationIndex).Rotation);		
		soMath::QuaternionToMatrixRotation4x4(soMath::QuaternionSLerp(StartRotationQ, EndRotationQ, Factor), _OutMat);
#endif
		_Animation.RotationIndex[0] = RotationIndex;
		_Animation.RotationIndex[1] = NextRotationIndex;
		_Animation.InterpolationFactor[1] = Factor;
	}

	void soSkeletalModel::CalcInterpolatedScaling(soKeyFrame& _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation)
	{
		_Animation.ScaleIndex[0] = 0;
		_Animation.ScaleIndex[1] = 0;
		_Animation.InterpolationFactor[2] = 0.0f;

		if (_KF.m_ScaleKey.size() == 1)
		{
#ifdef SoftwareAnimation
			_OutMat = soMath::ScalarMatrix4x4(_KF.m_ScaleKey.at(0).Scalation);			
#endif	
			return;
		}

		uint32 ScalingIndex = FindScaling(_KF, _Delta);
		uint32 NextScalingIndex = (ScalingIndex + 1);

		/*if (static_cast<uint32>(_Animation._Animation->m_Duration) == static_cast<uint32>(_KF.m_ScaleKey.size() - 1))
		{
			_OutMat = Math::ScalarMatrix4x4(_KF.m_ScaleKey.at(ScalingIndex).Scalation);
			return;
		}*/

		float DeltaTime = _KF.m_ScaleKey.at(NextScalingIndex).KeyTime - _KF.m_ScaleKey.at(ScalingIndex).KeyTime;
		float Factor = (_Delta - _KF.m_ScaleKey.at(ScalingIndex).KeyTime) / DeltaTime;

#ifdef SoftwareAnimation
		soVector3D Start = _KF.m_ScaleKey.at(ScalingIndex).Scalation;
		soVector3D End = _KF.m_ScaleKey.at(NextScalingIndex).Scalation;
		_OutMat = soMath::ScalarMatrix4x4(soMath::Lerp(Start, End, Factor));
#endif
		_Animation.ScaleIndex[0] = ScalingIndex;
		_Animation.ScaleIndex[1] = NextScalingIndex;
		_Animation.InterpolationFactor[2] = Factor;
	}

	void soSkeletalModel::CalcInterpolatedPosition(soKeyFrame& _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation)
	{
		_Animation.PositionIndex[0] = 0;
		_Animation.PositionIndex[1] = 0;
		_Animation.InterpolationFactor[0] = 0.0f;

		if (_KF.m_TranslationKey.size() == 1)
		{
#ifdef SoftwareAnimation
			_OutMat = soMath::TranslationMatrix4x4(_KF.m_TranslationKey.at(0).Translation);
#endif
			return;
		}

		uint32 PositionIndex = FindPosition(_KF, _Delta);
		uint32 NextPositionIndex = (PositionIndex + 1);

		/*if (static_cast<uint32>(_Animation._Animation->m_Duration) == static_cast<uint32>(_KF.m_TranslationKey.size() - 1))
		{
			_OutMat = Math::TranslationMatrix4x4(_KF.m_TranslationKey.at(PositionIndex).Translation);
			return;
		}*/

		float DeltaTime = _KF.m_TranslationKey.at(NextPositionIndex).KeyTime - _KF.m_TranslationKey.at(PositionIndex).KeyTime;
		float Factor = (_Delta - _KF.m_TranslationKey.at(PositionIndex).KeyTime) / DeltaTime;

#ifdef SoftwareAnimation
		soVector3D Start = _KF.m_TranslationKey.at(PositionIndex).Translation;
		soVector3D End = _KF.m_TranslationKey.at(NextPositionIndex).Translation;
		_OutMat = soMath::TranslationMatrix4x4(soMath::Lerp(Start, End, Factor));
#endif
		_Animation.PositionIndex[0] = PositionIndex;
		_Animation.PositionIndex[1] = NextPositionIndex;
		_Animation.InterpolationFactor[0] = Factor;
	}

	soKeyFrame* soSkeletalModel::FindAnimationKeyFrame(soAnimation* _ActiveAnimation, uint32& _NodeID)
	{
		soKeyFrame* pNodeAnim = NULL;
		if (_ActiveAnimation->m_KeyFrameMap.find(_NodeID) != _ActiveAnimation->m_KeyFrameMap.end())
		{
			pNodeAnim = &_ActiveAnimation->m_KeyFrameMap[_NodeID];
		}

		return pNodeAnim;
	}

	void soSkeletalModel::GetBoneTransform(soKeyFrame* _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation)
	{
		if (_KF)
		{//Si un canal nos afecta
		 // Interpolate scaling and generate scaling transformation matrix
			soMatrix4x4 ScalingM;
			CalcInterpolatedScaling(*_KF, _Delta, ScalingM, _Animation);

			// Interpolate rotation and generate rotation transformation matrix
			soMatrix4x4 RotationM;
			CalcInterpolatedRotation(*_KF, _Delta, RotationM, _Animation);

			// Interpolate translation and generate translation transformation matrix
			soMatrix4x4 TranslationM;
			CalcInterpolatedPosition(*_KF, _Delta, TranslationM, _Animation);

			// Combine the above transformations
			_OutMat = TranslationM * RotationM * ScalingM;
		}
	}

	void soSkeletalModel::GetFinalBoneTransform(AnimationData& _AnimationData)
	{
		for (uint32 i = 0; i < 120; i++)
		{
#ifdef SoftwareAnimation			
			if (_AnimationData.BlendFactor != 1.0f)
			{
				_AnimationData.OutAnimation.BoneMatrix[i] *= (1.0f - _AnimationData.BlendFactor);
				_AnimationData.InAnimation.BoneMatrix[i] *= (_AnimationData.BlendFactor);
				_AnimationData.BoneMatrix[i] = soMath::Lerp(_AnimationData.OutAnimation.BoneMatrix[i], _AnimationData.InAnimation.BoneMatrix[i], _AnimationData.BlendFactor);
			}
			else
			{
				_AnimationData.BoneMatrix[i] = _AnimationData.InAnimation.BoneMatrix[i];
			}
#else
				_AnimationData.BoneMatrix[i] = _AnimationData.InAnimation.BoneMatrix[i];
#endif
		}
	}

	void soSkeletalModel::AnimNodeHeirarchy(Animation& _Animation)
	{
		//Queue de chequeo de nodos
		std::queue<soJoint*> GraphList;
		soJoint* GraphNode;

		//Nodo inicial del grafo
		GraphList.push(&m_RootNode);
		m_RootNode.m_ParentTransform = soMath::IdentityMatrix4x4();

		while (!GraphList.empty())
		{
			//Tomamos el primer nodo de la lista
			GraphNode = GraphList.front();

			//Sacamos al nodo visitado
			GraphList.pop();

			//Transformamos el nodo visitado, Comenzamos la transformacion desde el nodo padre
			uint32 NodeID(GraphNode->m_ID);

			//Tomamos nuestra transformacion original
			soMatrix4x4 GlobalTransformation = GraphNode->m_NodeTransformation;

			//Buscamos el canal de animacion que nos afecta
			soKeyFrame* pKeyFrame = NULL;
			if (_Animation._Animation != NULL)
			{
				pKeyFrame = FindAnimationKeyFrame(_Animation._Animation, NodeID);
				GetBoneTransform(pKeyFrame, _Animation.DeltaTime, GlobalTransformation, _Animation);
			}

			//Formamos la matriz de transformacion del join
#ifdef SoftwareAnimation
			GlobalTransformation = GraphNode->m_ParentTransform * GlobalTransformation;
			GraphNode->m_ParentTransform = GlobalTransformation;
			if (GraphNode->IsStarted())
			{				
				_Animation.BoneMatrix[GraphNode->m_ID] = GlobalTransformation * GraphNode->m_JoinOffsetMatrix;
			}
#endif
			//Agregamos los nodos hijos a la lista
			for (uint32 i = 0; i < static_cast<uint32>(GraphNode->m_conexions.size()); i++)
			{                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     
				((soJoint*)GraphNode->m_conexions.at(i))->m_ParentTransform = GlobalTransformation;
				GraphList.push(((soJoint*)GraphNode->m_conexions.at(i)));
			}
		}
	}

	void soSkeletalModel::BoneTransform(AnimationData& _AnimationData)
	{
		float TicksPerSecond = 0.0f;
		float TimeInTicks = 0.0f;

		if (_AnimationData.InAnimation._Animation != NULL)
		{
			TicksPerSecond = _AnimationData.InAnimation._Animation->m_TicksPerSecond != 0 ? _AnimationData.InAnimation._Animation->m_TicksPerSecond : 30.0f;
			TimeInTicks = _AnimationData.InAnimation.AnimationTime * TicksPerSecond;
			_AnimationData.InAnimation.DeltaTime = fmod(TimeInTicks, _AnimationData.InAnimation._Animation->m_Duration);
		}

		if (_AnimationData.OutAnimation._Animation != NULL)
		{
			TicksPerSecond = _AnimationData.OutAnimation._Animation->m_TicksPerSecond != 0 ? _AnimationData.OutAnimation._Animation->m_TicksPerSecond : 30.0f;
			TimeInTicks = _AnimationData.OutAnimation.AnimationTime * TicksPerSecond;
			_AnimationData.OutAnimation.DeltaTime = fmod(TimeInTicks, _AnimationData.OutAnimation._Animation->m_Duration);
		}

#ifdef SoftwareAnimation
		ZeroMemory(_AnimationData.BoneMatrix, sizeof(soMatrix4x4) * 120);
		ZeroMemory(_AnimationData.InAnimation.BoneMatrix, sizeof(soMatrix4x4) * 120);
		ZeroMemory(_AnimationData.OutAnimation.BoneMatrix, sizeof(soMatrix4x4) * 120);
#endif
		AnimNodeHeirarchy(_AnimationData.InAnimation);

		if (_AnimationData.BlendFactor != 1.0f)
		{
			AnimNodeHeirarchy(_AnimationData.OutAnimation);
		}

		//Blend Factor entre 2 animaciones
		m_SkeletalCB.BlendFactor = _AnimationData.BlendFactor;

		//Informacion de keyframe de posicion
		m_SkeletalCB.KeyFrameTransformIn[0].KeyFrameIndex = _AnimationData.InAnimation.PositionIndex[0];
		m_SkeletalCB.KeyFrameTransformIn[0].NextKeyFrameIndex = _AnimationData.InAnimation.PositionIndex[1];
		m_SkeletalCB.KeyFrameTransformIn[0].InterpolationFactor = _AnimationData.InAnimation.InterpolationFactor[0];

		m_SkeletalCB.KeyFrameTransformOut[0].KeyFrameIndex = _AnimationData.OutAnimation.PositionIndex[0];
		m_SkeletalCB.KeyFrameTransformOut[0].NextKeyFrameIndex = _AnimationData.OutAnimation.PositionIndex[1];
		m_SkeletalCB.KeyFrameTransformOut[0].InterpolationFactor = _AnimationData.OutAnimation.InterpolationFactor[0];

		//Informacion de keyframe de Rotacion
		m_SkeletalCB.KeyFrameTransformIn[1].KeyFrameIndex = _AnimationData.InAnimation.RotationIndex[0];
		m_SkeletalCB.KeyFrameTransformIn[1].NextKeyFrameIndex = _AnimationData.InAnimation.RotationIndex[1];
		m_SkeletalCB.KeyFrameTransformIn[1].InterpolationFactor = _AnimationData.InAnimation.InterpolationFactor[1];

		m_SkeletalCB.KeyFrameTransformOut[1].KeyFrameIndex = _AnimationData.OutAnimation.RotationIndex[0];
		m_SkeletalCB.KeyFrameTransformOut[1].NextKeyFrameIndex = _AnimationData.OutAnimation.RotationIndex[1];
		m_SkeletalCB.KeyFrameTransformOut[1].InterpolationFactor = _AnimationData.OutAnimation.InterpolationFactor[1];

		//Informacion de keyframe de escalacion
		m_SkeletalCB.KeyFrameTransformIn[2].KeyFrameIndex = _AnimationData.InAnimation.ScaleIndex[0];
		m_SkeletalCB.KeyFrameTransformIn[2].NextKeyFrameIndex = _AnimationData.InAnimation.ScaleIndex[1];
		m_SkeletalCB.KeyFrameTransformIn[2].InterpolationFactor = _AnimationData.InAnimation.InterpolationFactor[2];

		m_SkeletalCB.KeyFrameTransformOut[2].KeyFrameIndex = _AnimationData.OutAnimation.ScaleIndex[0];
		m_SkeletalCB.KeyFrameTransformOut[2].NextKeyFrameIndex = _AnimationData.OutAnimation.ScaleIndex[1];
		m_SkeletalCB.KeyFrameTransformOut[2].InterpolationFactor = _AnimationData.OutAnimation.InterpolationFactor[2];

#ifdef SoftwareAnimation
		GetFinalBoneTransform(_AnimationData);
#endif
	}

	void soSkeletalModel::SyncUpKeyFrame(soAnimation& _NewAnimation)
	{
		//Queue de chequeo de nodos
		std::queue<soJoint*> GraphList;
		soJoint* GraphNode;

		//Nodo inicial del grafo
		GraphList.push(&m_RootNode);

		while (!GraphList.empty())
		{
			//Tomamos el primer nodo de la lista
			GraphNode = GraphList.front();

			//Sacamos al nodo visitado
			GraphList.pop();

			//Buscamos el keyframe con el nombre del nodo y asignamos el ID
			if (_NewAnimation.m_KeyFrameInfo.find(GraphNode->m_name) != _NewAnimation.m_KeyFrameInfo.end())
			{
				_NewAnimation.m_KeyFrameMap[GraphNode->m_ID] = _NewAnimation.m_KeyFrameInfo[GraphNode->m_name];
			}

			//Agregamos los nodos hijos a la lista
			for (uint32 i = 0; i < static_cast<uint32>(GraphNode->m_conexions.size()); i++)
			{
				GraphList.push(((soJoint*)GraphNode->m_conexions.at(i)));
			}
		}

		_NewAnimation.m_KeyFrameInfo.clear();
	}

	RESULT soSkeletalModel::Load(const ResourceParameters& _ResourceParameters)
	{
		if (FAIL(SUPER::Load(_ResourceParameters)))
		{
			return EC_FALSE;
		}
		
		//Cargamos la informacion de los huesos
		CreateNodeGraph();

		m_Importer.FreeScene();

		AddAnimation(_ResourceParameters.FilePath);

		return EC_OK;
	}

	void soSkeletalModel::Update(float _DeltaTime )
	{		
		(void*)&_DeltaTime;
	}

	void soSkeletalModel::Update(AnimationData& _AnimationData)
	{	
		//Actualizamos las transformaciones de los huesos 
		BoneTransform(_AnimationData);
	}

	soAnimation* soSkeletalModel::PlayAnimation(soString& _AnimationName)
	{
		if (m_AnimationMap.find(_AnimationName) != m_AnimationMap.end())
		{
			return &m_AnimationMap[_AnimationName];
		}
		return NULL;
	}

	void soSkeletalModel::AddAnimation(soString _FilePath)
	{
		//Primero leemos la informacion del archivo fuente
		if (FAIL(ReadFileInfo(_FilePath)))
		{
			return;
		}

		//Ahora seteamos las animaciones del modelo
		for (unsigned int i = 0; i < m_Scene->mNumAnimations; i++)
		{
			soAnimation NewAnimation;
			NewAnimation.StartUp(*m_Scene->mAnimations[i]);
			if (NewAnimation.m_Name.empty())
			{
				uint32 ID = static_cast<uint32>(m_AnimationMap.size());
				NewAnimation.m_Name = uint2ws(ID);
				NewAnimation.m_ID = ID;
			}
			SyncUpKeyFrame(NewAnimation);
						
			m_AnimationMap[NewAnimation.m_Name] = NewAnimation;	
		}
		
		m_Importer.FreeScene();
	}
}
