#include "mesh.h"
#include "openGLFunctions.h"
#include "logger.h"

Mesh::Mesh(std::string inName)
{
	name = inName;
}
Mesh::Mesh(std::string inName, aiMesh *assimpMesh)
		: Mesh(inName)
{
	load(assimpMesh);
}

void Mesh::destruct()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}

void Mesh::createVAO()
{
	glGenVertexArrays(1, &VAO);
	glSetBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		//Enable vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		int inc = 0;
		GLuint vertLoc = 0;
		glEnableVertexAttribArray(vertLoc);
		glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (intptr_t) inc); //attribute, size, type, is normalised?, stride, offset
		inc += sizeof(glm::vec3);

		GLuint uvLoc = 1;
		glEnableVertexAttribArray(uvLoc);
		glVertexAttribPointer(uvLoc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (intptr_t) inc);
		inc += sizeof(glm::vec2);

		GLuint normLoc = 2;
		glEnableVertexAttribArray(normLoc);
		glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (intptr_t) inc);
		inc += sizeof(glm::vec3);

		GLuint matLoc = 3;
		glEnableVertexAttribArray(matLoc);
		glVertexAttribIPointer(matLoc, 1, GL_INT, sizeof(Vertex), (void*) (intptr_t) inc);
		//inc += sizeof(int);
	glSetBindVertexArray(0);
}

void Mesh::genBuffers()
{
	//Generate VUNM (vertexUvNormalMaterial) buffer using data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, collatedVertices.size() * sizeof(Vertex), collatedVertices.data(), GL_STATIC_DRAW);

	//Generate index buffer using data
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	createVAO();
}

void Mesh::load(aiMesh* assimpMesh)
{
	for(unsigned int j = 0; j < assimpMesh->mNumFaces; j++)
	{
		aiFace& assimpFace = assimpMesh->mFaces[j];

		for(unsigned int k = 0; k < assimpFace.mNumIndices; k++)
		{
			indices.push_back(assimpFace.mIndices[k]);
		}
	}
	for(unsigned int j = 0; j < assimpMesh->mNumVertices; j++)
	{
		aiVector3D vertex = assimpMesh->mVertices[j];
		glm::vec3 glmVert = glm::vec3(vertex.x,vertex.y,vertex.z);
		vertices.push_back(glmVert);

		glm::vec2 glmUv;
		if(assimpMesh->mTextureCoords[0])
		{
			aiVector3D uv = assimpMesh->mTextureCoords[0][j];
			glmUv = glm::vec2(uv.x, uv.y);
			uvs.push_back(glmUv);
		}

		glm::vec3 glmNormal;
		if(assimpMesh->mNormals)
		{
			aiVector3D normal = assimpMesh->mNormals[j];
			glmNormal = glm::vec3(normal.x, normal.y, normal.z);
			normals.push_back(glmNormal);
		}

		materialIndices.push_back(assimpMesh->mMaterialIndex);

		Vertex collatedVertex;
		collatedVertex.pos = glmVert;
		collatedVertex.uv = glmUv;
		collatedVertex.normal = glmNormal;
		collatedVertex.materialIndex = assimpMesh->mMaterialIndex;
		collatedVertices.push_back(collatedVertex);
	}

	genBuffers();
}

void Mesh::loadWithVectors(std::vector<glm::vec3> inVertices, std::vector<glm::vec2> inUvs, std::vector<glm::vec3> inNormals, std::vector<unsigned int> inIndices)
{
	vertices.swap(inVertices);
	uvs.swap(inUvs);
	normals.swap(inNormals);
	indices.swap(inIndices);
	for(int i = 0; inIndices.size(); i++)
		materialIndices.push_back(0);

	for(unsigned int i = 0; i < vertices.size(); i++)
	{
		Vertex collatedVertex;
		collatedVertex.pos = vertices[i];
		collatedVertex.uv = uvs[i];
		collatedVertex.normal = normals[i];
		collatedVertex.materialIndex = materialIndices[i];
		collatedVertices.push_back(collatedVertex);
	}

	genBuffers();
}
