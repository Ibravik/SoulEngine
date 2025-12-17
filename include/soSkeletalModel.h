/********************************************************************/
/**
* @LC	  10/12/2016
* @file   soSkeletalModel.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/24/2016
* @brief  Declaracion de la clase soSkeletalModel. Esta clase encapsula
la funcionalidad de animacion de un skined mesh.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include "soModel.h"
#include "soMesh.h"
#include "soAnimation.h"
#include "soKeyFrame.h"
#include "soJoint.h"

#define SoftwareAnimation

namespace SoulSDK
{
	/************************************************************************/
	/* Estructura contenedora de datos para reproducir una animacion        */
	/************************************************************************/
	struct Animation
	{
		soMatrix4x4 BoneMatrix[120];																		/*!< Matrices finales de la animacion */
		soAnimation* _Animation;																			/*!< Animacion para reproduccion */
		float AnimationTime;																				/*!< Tiempo actual de la animacion en segundos */
		float DeltaTime;																					/*!< Tiempo normalizado de la animacion */

		uint32 PositionIndex[2];																			/*!< Indices de interpolacion dentro de la animacion */
		uint32 RotationIndex[2];																			/*!< Indices de interpolacion dentro de la animacion */
		uint32 ScaleIndex[2];																				/*!< Indices de interpolacion dentro de la animacion */
		
		float InterpolationFactor[3];																		/*!< Factor de interpolacion entre keyframes*/

		Animation()
		{
			_Animation = NULL;
			AnimationTime = 0.0f;
		}

		void Update(float _DeltaTime)
		{
			//Actualizamos la animacion entrante
			if (_Animation != NULL)
			{
				//Actualizamos el tiempo de la animacion
				AnimationTime += _DeltaTime;

				if (_Animation->m_Duration <= AnimationTime)
				{
					AnimationTime = 0.0f;
				}
			}
		}
	};

	/***************************************************************************/
	/* Declaracion de estructura de informacion para la animacion. Debido a	   */
	/* que la animacion es un recurso, es necesario que el componente gestione */
	/* la informacion que necesita para llevarla de manera independiente       */
	/***************************************************************************/
	struct AnimationData
	{
		soMatrix4x4 BoneMatrix[120];																		/*!< CB de matrices finales */
		Animation InAnimation;																				/*!< Animacion de entrada */
		Animation OutAnimation;																				/*!< Animacion de salida */
		float BlendTime;																					/*!< Tiempo actual de blend */
		float FadeTime;																						/*!< Tiempo total para el fade */
		float BlendFactor;																					/*!< Peso de blend de la animacion */
		bool Blend;																							/*!< Identifica si esta activo un blend */

		AnimationData()
		{
			Blend = false;
			BlendFactor = 1.0f;
			BlendTime = 0.0f;
			FadeTime = 0.0f;
		};

		void Update(float _DeltaTime)
		{
			BlendTime += _DeltaTime;

			if (BlendFactor < 1.0f)
			{
				BlendFactor = BlendTime / FadeTime;
			}
			else
			{
				BlendFactor = 1.0f;
			}
			
			InAnimation.Update(_DeltaTime);
			OutAnimation.Update(_DeltaTime);
		}
	};

	/************************************************************************/
	/* Estructura que contiene el Constant Buffer del modelo, incluyendo de */
	/* sus animaciones														*/
	/************************************************************************/
	struct CBGraphNodeData
	{
		//Informacion del grafo		
		uint32 BoneIndexHierarchy[60];																				/*!< [Indice del nodo][Indice de nodos que le afectan] */
		uint32 BoneIndexHierarchyNum;																				/*!< [Indice del nodo][Numero de huesos que le afectan] */

		CBGraphNodeData()
		{
			BoneIndexHierarchyNum = 0;
		}
	};

	struct CBAnimationData
	{
		uint32 AnimationIndex;																						/*!< [AnimacionIndex] Indice del inicio de las matrices de animacion */
		uint32 AnimationKeyFrameNum;																				/*!< [AnimacionIndex] Numero de keyFrames por animacion */
		uint32 AnimationKeyFrameRotMat[300];																		/*!< [Indice de KeyFrame][Numero de matrices de Rotacion]*/
		uint32 AnimationKeyFrameScaMat[300];																		/*!< [Indice de KeyFrame][Numero de matrices de Escala]*/
		uint32 AnimationKeyFramePosMat[300];																		/*!< [Indice de KeyFrame][Numero de matrices de Posicion]*/

		CBAnimationData()
		{
			AnimationKeyFrameNum = 0;
		}
	};

	struct CBKeyFrameInfo
	{
		uint32 KeyFrameIndex;																						/*!< Indice de animacion de keyframe */
		uint32 NextKeyFrameIndex;																					/*!< Indice de animacion de keyframe siguiente */
		float InterpolationFactor;																					/*!< Factor de interpolacion de la animacion */

		CBKeyFrameInfo()
		{

		}
	};

	struct SkeletalCB
	{
		//Esqueleto
		uint32 SkeletalIndex;																						/*!< Indice de la posicion de las matrices del esqueleto en el buffer global */
		uint32 BoneNumber;																							/*!< Numero total de huesos en el esqueleto */
		CBGraphNodeData GraphJoint[120];																			/*!< Informacion por hueso */
		
		//Animaciones
		uint32 AnimationNum;																						/*!< Numero de animaciones del modelo */
		CBAnimationData Animations[30];																				/*!< Informacion por animacion */

		//Animacion Actual
		uint32 IndexAnimationIn;																					/*!< Indice de la animacion de salida */
		uint32 IndexAnimationOut;																					/*!< Indice de la animacion de entrada */
		CBKeyFrameInfo KeyFrameTransformIn[3];																		/*!< Informacion de cada keyframe para la interpolacion [Position][rotation][scale]*/
		CBKeyFrameInfo KeyFrameTransformOut[3];																		/*!< Informacion de cada keyframe para la interpolacion [Position][rotation][scale]*/
		float BlendFactor;																							/*!< Factor de blend para 2 animaciones */

		SkeletalCB()
		{
			IndexAnimationIn = 0;
			SkeletalIndex = 0;
			AnimationNum = 0;
			BoneNumber = 0;
		}
	};

	/************************************************************************/
	/* Declaracion de la clase soSkeletalModel							    */
	/************************************************************************/
	class soSkeletalModel : public soModel 
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soSkeletalModel();																							/*!< Constructor Standard */
		virtual ~soSkeletalModel();																					/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	private:
		std::map<soString, soJoint*> m_JointMap;																	/*!< Mapa con todos los nodos del grafo padre */
		soJoint m_RootNode;																							/*!< Nodo Inicial del Grafo del esqueleto */
		soAnimation* m_NextAnimation;																				/*!< Siguiente animacion para reproducir */

	public:
		std::map<soString, soAnimation> m_AnimationMap;																/*!< Mapa de animaciones disponibles */
		SkeletalCB m_SkeletalCB;																					/*!< CB que describe el grafo para el buffer general */

		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	private:
		void CreateNodeGraph();																						/*!< Crea el grafo de nodos perteneciente al modelo */
		void SetNodeInfo(const aiNode& _aiNode, soJoint& _Joint, soJoint* _ParentJoint);							/*!< Asigna la informacion del nodo */
		uint32 FindRotation(soKeyFrame& _KF, float _Delta);															/*!< Encuentra una rotacion en la animacion */
		uint32 FindScaling(soKeyFrame& _KF, float _Delta);															/*!< Encuentra una escalacion en la animacion */
		uint32 FindPosition(soKeyFrame& _KF, float _Delta);															/*!< Encuentra una traslacion en la animacion */
		void CalcInterpolatedRotation(soKeyFrame& _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation);	/*!< Calculo de interpolacion de una rotacion */
		void CalcInterpolatedScaling(soKeyFrame& _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation);	/*!< Calculo de interpolacion de una escalacion */
		void CalcInterpolatedPosition(soKeyFrame& _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation);	/*!< Calculo de interpolacion de una traslacion */
		soKeyFrame* FindAnimationKeyFrame(soAnimation* _ActiveAnimation, uint32& _NodeID);							/*!< Encuentra el canal de animacion asociado con el joint */
		void GetBoneTransform(soKeyFrame* _KF, float _Delta, soMatrix4x4& _OutMat, Animation& _Animation);			/*!< Calcula la matriz transformacion del hueso segun el keyframe y tiempo */
		void GetFinalBoneTransform(AnimationData& _AnimationData);													/*!< Calcula las matrices de transformacion finales */
		void AnimNodeHeirarchy(Animation& _Animation);																/*!< Lee el grafo y realiza las transformaciones pertinentes segun la animacion */
		void BoneTransform(AnimationData& _AnimationData);															/*!< Comienza con la transformacion de las matrices de los huesos */
		void SyncUpKeyFrame(soAnimation& _NewAnimation);															/*!< Sincornisa el id de los keyfrage con el de los nodos correspondientes */

	public:
		virtual RESULT Load(const ResourceParameters& _ResourceParameters)override;									/*!< Cargar desde Archivo, la ruta esta especificada en la estructura Resource identification */
		virtual void Update(float _DeltaTime = 0.0f);																/*!< Actualiza cualquier informacion que necesite el modelo, como aspectos de la animacion */
		void Update(AnimationData& _AnimationData);																	/*!< Actualizacion especifica de este recurso. */
		soAnimation* PlayAnimation(soString& _AnimationName);														/*!< Seteas la animacion activa */
		void AddAnimation(soString _FilePath);																		/*!< Carga animaciones para el modelo de el mismo o algun otro archivo */
		
	};
}
