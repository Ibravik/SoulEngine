/********************************************************************/
/**
* @LC	  10/12/2016
* @file   soModel.h
* @Author Aldo Muñoz (lehi.munoz@gmail.com)
* @date   2/24/2016
* @brief  Implementacion de la clase soModel. Clase base para todo tipo
		  de Modelo. Este se conforma de triangulos y materiales.
* @bug	   No known bugs.
*/
/********************************************************************/
#pragma once

/************************************************************************/
/* Inclucion de cabeceras necesarias para la compilacion                */
/************************************************************************/
#include <assimp/Importer.hpp>				/*!< C++ importer interface */
#include <assimp/scene.h>					/*!< Output data structure */
#include <assimp/postprocess.h>			/*!< Post processing flags */
#include "Soul_Graphics.h"
#include "soResource.h"
#include "soMesh.h"

namespace SoulSDK
{
	/************************************************************************/
	/* Estructura de triangulos                                             */
	/************************************************************************/
	struct Triangle
	{
	public:
		Triangle()
		{
			TriangeVertex[0] = NULL;
			TriangeVertex[1] = NULL;
			TriangeVertex[2] = NULL;
		};

	public:
		Vertex* TriangeVertex[3];
		soVector3D TriangleCenter;

	public:
		void SetTriangleCenter()
		{
			float X = 0, Y = 0, Z = 0;
			for (uint8 i = 0; i < 3; i++)
			{
				X += TriangeVertex[i]->Position.X;
				Y += TriangeVertex[i]->Position.Y;
				Z += TriangeVertex[i]->Position.Z;
			}
			TriangleCenter = soVector3D(X / 3.0f, Y / 3.0f, Z / 3.0f);
		}

		bool Triangle_intersection(soRay& _Ray)
		{
			soVector3D e1, e2;  //Edge1, Edge2
			soVector3D P, Q, T;
			float det, inv_det, u, v;
			float t;

			//Find vectors for two edges sharing V1
			e1 = TriangeVertex[1]->Position - TriangeVertex[0]->Position;
			e2 = TriangeVertex[2]->Position - TriangeVertex[0]->Position;

			//Begin calculating determinant - also used to calculate u parameter
			P = soMath::CrossProduct(_Ray.m_VecDirection, e2);

			//if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
			det = soMath::DotProduct(e1, P);

			//NOT CULLING
			if (det > -soMath::EPSILON && det < soMath::EPSILON)
			{
				return false;
			}

			inv_det = 1.f / det;

			//calculate distance from V1 to ray origin
			T = _Ray.m_VecOrigin - TriangeVertex[0]->Position;

			//Calculate u parameter and test bound
			u = soMath::DotProduct(T, P) * inv_det;

			//The intersection lies outside of the triangle
			if (u < 0.f || u > 1.f)
			{
				return false;
			}

			//Prepare to test v parameter
			Q = soMath::CrossProduct(T, e1);

			//Calculate V parameter and test bound
			v = soMath::DotProduct(_Ray.m_VecDirection, Q) * inv_det;

			//The intersection lies outside of the triangle
			if (v < 0.f || u + v  > 1.f)
			{
				return false;
			}

			t = soMath::DotProduct(e2, Q) * inv_det;

			if (t > soMath::EPSILON) { //ray intersection
				return true;
			}

			// No hit, no win
			return false;
		}
	};

	/************************************************************************/
	/* Declaracion de la clase soModel, clase base de todos los modelos	    */
	/************************************************************************/
	class soModel : public soResource
	{
		/************************************************************************/
		/* Declaracion de contructores y destructor                             */
		/************************************************************************/
	public:
		soModel();																	/*!< Constructor Standard */
		virtual ~soModel();															/*!< Destructor */

		/************************************************************************/
		/* Declaracion de variables de la clase                                 */
		/************************************************************************/
	protected:
		Assimp::Importer m_Importer;												/*!< Objeto de Assimp para importar la informacion del archivo de carga */
		const aiScene *m_Scene;														/*!< Objeto que almacena la informacion cargada del archivo */

	public:
		GET_SAFE(soSphere, BoundingSphere)											/*!< Caja de colision del modelo para seleccion en modo editor */
		uint32 m_IsStatic;															/*!< Indica si el modelo es estatico o no */
		uint32 m_IsRougthnes;														/*!< Chequeo de tipo de rugosidad. En ocaciones se da una textura de glossines en lugar de un rougthness */
		std::vector<soMesh*> m_MeshList;											/*!< Arreglo que contiene los meshes del modelo */
		std::vector<Triangle> m_ModelTriangleList;									/*!< Array de triangulos del modelo */
		std::vector<uint32> m_DefaultMaterialID;									/*!< Array de ID de materiales default del modelo */
		
		/************************************************************************/
		/* Declaracion de funciones de ayuda de la clase                        */
		/************************************************************************/
	protected:	
		RESULT ReadFileInfo(soString _FilePath);									/*!< Ejecuta cualquier requisito previo para cargar el recurso */		
		void CreateTriangleList();													/*!< Crea el arreglo de triangulos para el modelo */
		void CreateBoundingSphere();												/*!< Crea la caja de colicion al rededor del modelo. esto no garantiza que sea optima ni que este diseniada para verificar triguers, es solo para frustum culling */

	public:
		virtual void ShutDown()override;											/*!< Destruccion del recurso */
		virtual RESULT Load(const ResourceParameters& _ResourceParameters)override;	/*!< Cargar desde Archivo, la ruta esta especificada en la estructura Resource identification */
		virtual void Update(float _DeltaTime = 0.0f) { (void)_DeltaTime; };			/*!< Actualiza cualquier informacion que necesite el modelo, como aspectos de la animacion */
		virtual void Render(uint32 __MatProp, std::vector<uint32> _MaterialIDs);	/*!< Renderea el Modelo con los materiales pedidos segun el id de material. Estos deben ir en orden segun el materia*/
	};
}

#pragma warning(default : 4005)