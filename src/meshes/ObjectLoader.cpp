#include "ObjectLoader.h"
#include "Mesh.h"
#include "Material.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>
#include <fstream>
#include <unordered_map>

void createMeshesFromAiScene(const aiScene *scene, RigidObject *object);
void createMaterialsFromAiScene(const aiScene *scene, RigidObject *object);
void printAiSceneInfo(const aiScene *scene);

/**
 * Constructor
 */
ObjectLoader::ObjectLoader() { }

/**
 * TODO
 * @param filepath TODO
 * @return TODO
 */
void ObjectLoader::loadFromFile(const char *filepath, RigidObject *object)
{
	// The Assimp importer loads the 3D file
	Assimp::Importer importer;

	// The aiScene pointer will contain all of the
	// information from the loaded file
	const aiScene *scene;

	// Test to see if the file can be opened.
	// TODO There are better ways to do this
	std::ifstream f(filepath);
	if (! f.good())
	{
		std::cout << "ERROR: Unable to open the file: " << filepath << std::endl;
		return;
	}
	f.close();

	// Load the meshes from the file into the aiScene
//	scene = importer.ReadFile(filepath, aiProcessPreset_TargetRealtime_Quality);
	scene = importer.ReadFile(filepath,
		aiProcess_Triangulate |
//		aiProcessPreset_TargetRealtime_Quality |
//		aiProcess_FlipUVs |
		aiProcess_FixInfacingNormals |
		aiProcess_GenUVCoords
	);
	if (! scene)
	{
		std::cout << importer.GetErrorString() << std::endl;
		return;
	}

	// Print everything about this file to the console
	printAiSceneInfo(scene);

	createMeshesFromAiScene(scene, object);
	createMaterialsFromAiScene(scene, object);

	return;
}

/**
 * TODO
 * @param scene TODO
 * @return TODO
 */
void createMeshesFromAiScene(const aiScene *scene, RigidObject *object)
{
	if (! scene || ! object)
		return;

	// Loop over the meshes and extract relevant information
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		Mesh m;
		const aiMesh *mesh = scene->mMeshes[i];

		if (mesh->mNumVertices / mesh->mNumFaces != 3)
			continue;

		std::vector<unsigned int> vertices;
		if (mesh->HasPositions())
		{
			for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
			{
				aiVector3D vec = mesh->mVertices[j];
				Position position(vec.x, vec.y, vec.z);
				unsigned int index = object->addPosition(position);
				vertices.push_back(index);
			}
		}

		std::vector<glm::uvec3> positionIndices;
		if (mesh->HasFaces())
		{
			for (unsigned int j = 0; j < mesh->mNumFaces; ++j)
			{
				int x = mesh->mFaces[j].mIndices[0];
				int y = mesh->mFaces[j].mIndices[1];
				int z = mesh->mFaces[j].mIndices[2];
				positionIndices.push_back(glm::uvec3(
					vertices[x],
					vertices[y],
					vertices[z]
				));
			}
		}

		std::vector<unsigned int> normalIndices;
		if (mesh->HasNormals())
		{
			for (unsigned int j = 0; j < mesh->mNumVertices; ++j)
			{
				aiVector3D vec = mesh->mNormals[j];
				Normal normal(vec.x, vec.y, vec.z);
				unsigned int index = object->addNormal(normal);
				normalIndices.push_back(index);
			}
		}

		for (unsigned int j = 0; j < normalIndices.size()/3; j++)
		{
			Face f(
				positionIndices[j].x,
				positionIndices[j].y,
				positionIndices[j].z,
				0,
				0,
				0,
				normalIndices[j*3+0],
				normalIndices[j*3+1],
				normalIndices[j*3+2]);
			m.addFace(f);
		}


		if (scene->HasMaterials())
		{
			m.setMaterialIndex(mesh->mMaterialIndex);
		}

		for (unsigned int j = 0; j < mesh->GetNumColorChannels(); ++j)
		{
			if (mesh->HasVertexColors(j))
			{
				// TODO
			}
		}

		for (unsigned int j = 0; j < mesh->GetNumUVChannels(); ++j)
		{
			if (mesh->HasTextureCoords(j))
			{
				// TODO
			}
		}

		object->addMesh(m);
	}
}

/**
 * TODO
 * @param scene TODO
 * @return TODO
 */
void createMaterialsFromAiScene(const aiScene *scene, RigidObject *object)
{
	if (! scene)
		return;

	if (! scene->HasMaterials())
		return;

	std::unordered_map<unsigned int, Material> materials;

	for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		Material m;
		aiMaterial *material = scene->mMaterials[i];

		aiString matName;
		material->Get(AI_MATKEY_NAME, matName);
		m.setName(matName.C_Str());

		aiColor3D color(0.0f, 0.0f, 0.0f);
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		m.setAmbient(glm::vec3(color.r, color.g, color.b));

		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		m.setDiffuse(glm::vec3(color.r, color.g, color.b));

		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		m.setSpecular(glm::vec3(color.r, color.g, color.b));

		material->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		m.setEmission(glm::vec3(color.r, color.g, color.b));

		float shininess = 0.0f;
		material->Get(AI_MATKEY_SHININESS, shininess);
		m.setSpecularExponent(shininess);

		// TODO Add support for textures
//		unsigned int diffuseTextureCount = material->GetTextureCount(aiTextureType_DIFFUSE);
//		cout << "Diffuse texture count " << diffuseTextureCount << endl;
//		for (unsigned int k = 0; k < diffuseTextureCount; k++)
//		{
//			aiString textureFilePath;
//			if (AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, k, &textureFilePath))
//			{
//				cout << "Diffuse Texture file: " << textureFilePath.data << endl;
//			}
//		} // end of for diffuseTextureCount
//
//		unsigned int specularTextureCount = material->GetTextureCount(aiTextureType_SPECULAR);
//		cout << "Specular texture count " << specularTextureCount << endl;
//		for (unsigned int k = 0; k < specularTextureCount; k++)
//		{
//			aiString textureFilePath;
//			if (AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, k, &textureFilePath))
//			{
//				cout << "Specular texture file: " << textureFilePath.data << endl;
//			}
//		} // end of for specularTextureCount
//
//		unsigned int normalMapCount = material->GetTextureCount(aiTextureType_NORMALS);
//		cout << "Normal map count " << normalMapCount << endl;
//		for (unsigned int k = 0; k < normalMapCount; k++)
//		{
//			aiString textureFilePath;
//			if (AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, k, &textureFilePath))
//			{
//				cout << "Specular texture file: " << textureFilePath.data << endl;
//			}
//		} // end of for specularTextureCount
//
//		cout << endl;

		materials.insert({ i, m });
	} // end of for mNumMaterials

	return;
}


using namespace std;

/**
 * TODO
 * @param name TODO
 * @param vector TODO
 */
void printVector3D(string name, aiVector3D vector)
{
	cout << name << " " << vector.x << " "
		<< vector.y << " "
		<< vector.z << endl;
}

/**
 * TODO
 * @param name TODO
 * @param color TODO
 */
void printColor3D(string name, aiColor3D color)
{
	cout << name << " " << color.r << " "
		<< color.g << " "
		<< color.b << endl;
}

/**
 * TODO
 * @param layer TODO
 */
void indent(unsigned int layer)
{
	for (unsigned int m = 0; m < layer; m++)
	{
		cout << "    ";
	}
}

/**
 * TODO
 * @param matrix TODO
 * @param layer TODO
 */
void printMatrix4x4(aiMatrix4x4 matrix, unsigned int layer = 0)
{
	indent(layer);
	cout << matrix.a1 << ", " << matrix.a2 << ", " << matrix.a3 << ", " << matrix.a4 << endl;
	indent(layer);
	cout << matrix.b1 << ", " << matrix.b2 << ", " << matrix.b3 << ", " << matrix.b4 << endl;
	indent(layer);
	cout << matrix.c1 << ", " << matrix.c2 << ", " << matrix.c3 << ", " << matrix.c4 << endl;
	indent(layer);
	cout << matrix.d1 << ", " << matrix.d2 << ", " << matrix.d3 << ", " << matrix.d4 << endl;
}

/**
 * TODO
 * @param node TODO
 * @param layer TODO
 */
void printNodeTree(const aiNode* node, unsigned int layer)
{
	if (!node)
	{
		cout << "printNodeTree(): null pointer" << endl;
		return;
	}

	indent(layer);

	cout << "node: " << node->mName.C_Str();
	if (node->mNumMeshes > 0)
	{
		cout << "(Linked with mesh ";

		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			cout << "#" << node->mMeshes[i] << " ";
		}
		cout << ')' << endl;
	} else
	{
		cout << endl;
	}

	indent(layer);
	cout << "Transformation matrix" << endl;
	printMatrix4x4(node->mTransformation, layer);

	// indent(layer);
	// cout << "Number of children " << node->mNumChildren << endl;

	cout << endl;

	for (unsigned int j = 0; j < node->mNumChildren; j++)
	{
		printNodeTree(node->mChildren[j], layer + 1);
	}
}

/**
 * TODO
 * @param scene TODO
 */
void printAiSceneInfo(const aiScene* scene)
{
	if (!scene)
	{
		cout << "printAiSceneInfo(): null pointer" << endl;
		return;
	}

	cout << endl << "---------- Node Tree ----------" << endl;
	printNodeTree(scene->mRootNode, 0);

	if (scene->HasMeshes())
	{
		cout << endl << "---------- Meshes ----------" << endl;
		cout << "Total number of meshes: " << scene->mNumMeshes << endl << endl;

		for (unsigned int i = 0; i < scene->mNumMeshes; i++)
		{
			const aiMesh *currentMesh = scene->mMeshes[i];
			cout << "Mesh #" << i << endl;
			cout << "Name " << currentMesh->mName.C_Str() << endl;

			cout << "This mesh has " << currentMesh->GetNumUVChannels() << " UV(Texture) channels." << endl;
			cout << "This mesh is linked with material #" << currentMesh->mMaterialIndex << endl;

			string primitiveType = "";
			switch (currentMesh->mPrimitiveTypes)
			{
				case aiPrimitiveType_POINT:
					primitiveType = "point";
					break;
				case aiPrimitiveType_LINE:
					primitiveType = "line";
					break;
				case aiPrimitiveType_TRIANGLE:
					primitiveType = "triangle";
					break;
				case aiPrimitiveType_POLYGON:
					primitiveType = "polygon";
					break;
				default:
					primitiveType = "unknown";
					break;
			}
			cout << "Primitive type " << primitiveType << endl;

			if (currentMesh->HasPositions())
			{
				cout << "Number of vertex positions: " << currentMesh->mNumVertices << endl;

				for (unsigned int j = 0; j < currentMesh->mNumVertices; j++)
				{

					cout << "\tvertex (" << currentMesh->mVertices[j].x << ", "
						<< currentMesh->mVertices[j].y << ", "
						<< currentMesh->mVertices[j].z << ")" << endl;
				}
			} else
			{
				cout << "There is no vertex position in mesh # " << i << endl;
			}

			if (currentMesh->HasFaces())
			{
				cout << "Number of faces:" << currentMesh->mNumFaces << endl;

				for (unsigned int j = 0; j < currentMesh->mNumFaces; j++)
				{
					cout << "\tface #" << j << ": ";

					for (unsigned int k = 0; k < currentMesh->mFaces[j].mNumIndices; k++)
					{
						cout << currentMesh->mFaces[j].mIndices[k] << ", ";
					}
					cout << endl;
				}
			} else
			{
				cout << "There is no face (element) in mesh # " << i << endl;
			}

			if (currentMesh->HasNormals())
			{
				cout << "Number of normals:" << currentMesh->mNumVertices << endl;

				for (unsigned int j = 0; j < currentMesh->mNumVertices; j++)
				{
					cout << "\tnormal (" << currentMesh->mNormals[j].x << ", "
						<< currentMesh->mNormals[j].y << ", " << currentMesh->mNormals[j].z << ")" << endl;
				}
			} else
			{
				cout << "There is no normal vectors in mesh # " << i << endl;
			}

			// Each mesh may have multiple UV(texture) channels (multi-texture). Here we only print out
			// the first channel. Call currentMesh->GetNumUVChannels() to get the number of UV channels
			// for this mesh.
			if (currentMesh->HasTextureCoords(0))
			{
				cout << "Number of texture coordinates for UV(texture) channel 0:" << currentMesh->mNumVertices << endl;

				// mTextureCoords is different from mVertices or mNormals in that it is a 2D array, not a 1D array.
				// The first dimension of this array is the texture channel for this mesh.
				// The second dimension is the vertex index number.
				for (unsigned int j = 0; j < currentMesh->mNumVertices; j++)
				{
					cout << "\ttexture coordinates (" << currentMesh->mTextureCoords[0][j].x << ", "
						<< currentMesh->mTextureCoords[0][j].y << ")" << endl;
				}
			} else
			{
				cout << "There is no texture coordinate in mesh # " << i << endl;
			}

			cout << endl;
		}  // end of for loop
	} // end of if HasMeshes

	if (scene->HasMaterials())
	{
		cout << endl << "---------- Materials ----------" << endl;
		cout << "Total number of materials: " << scene->mNumMaterials << endl << endl;

		for (unsigned int i = 0; i < scene->mNumMaterials; i++)
		{

			cout << "Material #" << i << endl;

			aiMaterial *currentMaterial = scene->mMaterials[i];

			aiString matName;
			currentMaterial->Get(AI_MATKEY_NAME, matName);
			cout << "Name " << matName.C_Str() << endl;

			aiColor3D color(0.0f, 0.0f, 0.0f);
			currentMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
			cout << "Ambient color {" << color.r << ", " << color.g << ", " << color.b << "}" << endl;

			currentMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			cout << "Diffuse color {" << color.r << ", " << color.g << ", " << color.b << "}" << endl;

			currentMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			cout << "Specular color {" << color.r << ", " << color.g << ", " << color.b << "}" << endl;

			float shininess = 0.0f;
			currentMaterial->Get(AI_MATKEY_SHININESS, shininess);
			cout << "Shininess " << shininess << endl;

			currentMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
			cout << "Emissive color {" << color.r << ", " << color.g << ", " << color.b << "}" << endl;

			unsigned int diffuseTextureCount = currentMaterial->GetTextureCount(aiTextureType_DIFFUSE);
			cout << "Diffuse texture count " << diffuseTextureCount << endl;
			for (unsigned int k = 0; k < diffuseTextureCount; k++)
			{
				aiString textureFilePath;
				if (AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, k, &textureFilePath))
				{
					cout << "Diffuse Texture file: " << textureFilePath.data << endl;
				}
			} // end of for diffuseTextureCount

			unsigned int specularTextureCount = currentMaterial->GetTextureCount(aiTextureType_SPECULAR);
			cout << "Specular texture count " << specularTextureCount << endl;
			for (unsigned int k = 0; k < specularTextureCount; k++)
			{
				aiString textureFilePath;
				if (AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_SPECULAR, k, &textureFilePath))
				{
					cout << "Specular texture file: " << textureFilePath.data << endl;
				}
			} // end of for specularTextureCount

			unsigned int normalMapCount = currentMaterial->GetTextureCount(aiTextureType_NORMALS);
			cout << "Normal map count " << normalMapCount << endl;
			for (unsigned int k = 0; k < normalMapCount; k++)
			{
				aiString textureFilePath;
				if (AI_SUCCESS == scene->mMaterials[i]->GetTexture(aiTextureType_NORMALS, k, &textureFilePath))
				{
					cout << "Specular texture file: " << textureFilePath.data << endl;
				}
			} // end of for specularTextureCount

			cout << endl;
		} // end of for mNumMaterials
	} // end if has material

	if (scene->HasLights())
	{
		cout << endl << "---------- lights ----------" << endl;
		cout << "Total number of lights: " << scene->mNumLights << endl << endl;

		for (unsigned int i = 0; i < scene->mNumLights; i++)
		{
			aiLight *currentLight = scene->mLights[i];
			cout << "Light index: " << i << endl;
			cout << "Name: " << currentLight->mName.C_Str() << endl;
			cout << "Type: ";
			switch (currentLight->mType)
			{
				case aiLightSource_POINT:
					cout << "point light" << endl;
					break;
				case aiLightSource_DIRECTIONAL:
					cout << "directional light" << endl;
					break;
				case aiLightSource_SPOT:
					cout << "spotlight" << endl;
					break;
				default:
					cout << "unknown" << endl;
					break;
			}

			if ((currentLight->mType == aiLightSource_POINT) || (currentLight->mType == aiLightSource_SPOT))
			{
				printVector3D("Position", currentLight->mPosition);
			}

			if ((currentLight->mType == aiLightSource_DIRECTIONAL) || (currentLight->mType == aiLightSource_SPOT))
			{
				printVector3D("Direction", currentLight->mDirection);
			}

			printColor3D("Ambient color", currentLight->mColorAmbient);
			printColor3D("Diffuse color", currentLight->mColorDiffuse);
			printColor3D("Specular color", currentLight->mColorSpecular);

			if ((currentLight->mType == aiLightSource_POINT) || (currentLight->mType == aiLightSource_SPOT))
			{
				cout << "Constant attenuation " << currentLight->mAttenuationConstant << endl;
				cout << "Linear attenuation " << currentLight->mAttenuationLinear << endl;
				cout << "Quadratic attenuation " << currentLight->mAttenuationQuadratic << endl;
			}

			if (currentLight->mType == aiLightSource_SPOT)
			{
				cout << "Inner cone angle " << currentLight->mAngleInnerCone << endl;
				cout << "Outer cone angle " << currentLight->mAngleOuterCone << endl;
			}

			cout << endl;
		} // end for
	} // end if has lights

	if (scene->HasCameras())
	{
		cout << endl << "---------- Cameras ----------" << endl;
		cout << "Total number of cameras: " << scene->mNumCameras << endl << endl;

		for (unsigned int i = 0; i < scene->mNumCameras; i++)
		{
			aiCamera *currentCamera = scene->mCameras[i];

			cout << "Camera index: " << i << endl;
			cout << "Name: " << currentCamera->mName.C_Str() << endl;

			printVector3D("Position", currentCamera->mPosition);
			printVector3D("Look-at vector", currentCamera->mLookAt);
			printVector3D("Up vector", currentCamera->mUp);
			cout << "Aspect ratio " << currentCamera->mAspect << endl;
			cout << "Horizontal field of view " << currentCamera->mHorizontalFOV << endl;
			cout << "Near clip plane " << currentCamera->mClipPlaneNear << endl;
			cout << "Far clip plane " << currentCamera->mClipPlaneFar << endl;

			cout << "Camera matrix" << endl;
			aiMatrix4x4 currentCameraMatrix;
			currentCamera->GetCameraMatrix(currentCameraMatrix);
			printMatrix4x4(currentCameraMatrix);

			cout << endl;
		} // end fore
	} // end if has cameras

	if (scene->HasTextures())
	{
		cout << endl << "---------- Embedded textures ----------" << endl;
		cout << "Total number of embedded textures: " << scene->mNumTextures << endl << endl;

		for (unsigned int i = 0; i < scene->mNumTextures; i++)
		{
			aiTexture *currentTexture = scene->mTextures[i];

			cout << "Texture #" << i << endl;
			cout << "Height " << currentTexture->mHeight << endl;
			cout << "Width " << currentTexture->mWidth << endl;
			// cout << "Data " << currentTexture->pcData << endl;
		}
	} // end of if HasTextures

	if (scene->HasAnimations())
	{
		cout << endl << "Has animation" << endl;
	}
}

