//
// Created by Tim on 16/08/2018.
//

#include "TerrainComponent.h"
#include "../ecs/ecsManager.h"
#include <stb_image.h>
#include "../openGLFunctions.h"
#include "../loaders/assetManager.h"

COMPONENT_EXPORT(TerrainComponent, "terrainComponent")

TerrainComponent::TerrainComponent() {}
TerrainComponent::~TerrainComponent() {}

glm::vec3 constructNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3)
{
	glm::vec3 u = p2 - p1;
	glm::vec3 v = p3 - p1;

	glm::vec3 normal = glm::vec3(
		(u.y * v.z) - (u.z * v.y),
		(u.z * v.x) - (u.x * v.z),
		(u.x * v.y) - (u.y * v.x)
	);

	return normal;
}

void TerrainComponent::setValues(json inValues)
{
	width = inValues["width"];
	depth = inValues["depth"];
	lowerHeight = inValues["lowerHeight"];
	upperHeight = inValues["upperHeight"];

	filename = inValues["filename"];

	int comp, dataWidth, dataHeight;
	stbi_uc* heightData = stbi_load(filename.c_str(), &dataWidth, &dataHeight, &comp, STBI_rgb_alpha);
	if(!heightData)
		throw std::runtime_error("Heightmap could not be loaded");

	int tileWidth = dataWidth-1;
	int tileHeight = dataHeight-1;
	float heightRange = upperHeight - lowerHeight;

	vertices.reserve(static_cast<uint32_t>(dataWidth * dataHeight));
	for(int i = 0; i < dataHeight; i++)
	{
		for(int j = 0; j < dataWidth; j++)
		{
			TerrainVertex v{};
			v.position = glm::vec3(j*(width/tileWidth),0,i*(depth/tileHeight));
			v.position.y = heightData[(i*dataWidth + j)*4] / 256.0f * heightRange + lowerHeight;
			vertices.emplace_back(v);
		}
	}
	stbi_image_free(heightData);

	indices.reserve(static_cast<uint32_t>(tileWidth*tileHeight * 6));
	for(int i = 0; i < tileHeight; i++)
	{
		for(int j = 0; j < tileWidth; j++)
		{
			indices.emplace_back((i+1)*dataWidth + j+1);
			indices.emplace_back(i*dataWidth + j+1);
			indices.emplace_back(i*dataWidth + j);

			indices.emplace_back(i*dataWidth + j);
			indices.emplace_back((i+1)*dataWidth + j);
			indices.emplace_back((i+1)*dataWidth + j+1);
		}
	}
	for(int i = 0; i < indices.size(); i += 3)
	{
		TerrainVertex* v1 = &vertices[indices[i]];
		TerrainVertex* v2 = &vertices[indices[i+1]];
		TerrainVertex* v3 = &vertices[indices[i+2]];

		glm::vec3 normal = constructNormal(v1->position, v2->position, v3->position);
		v1->normal += normal;
		v2->normal += normal;
		v3->normal += normal;
	}

	for(auto &&vertex : vertices)
	{
		vertex.normal = glm::normalize(vertex.normal);
//		vertex.normal = (vertex.normal + glm::vec3(1))/2.0f;
	}

	std::vector<std::string> textureFilenames = inValues["textures"];
	ImageLoader* imgLoader = static_cast<ImageLoader*>(AssetManager::get().getLoader("image"));
	texture = imgLoader->loadTexture(textureFilenames);

	genBuffers();
}

void TerrainComponent::genBuffers()
{
	//Generate VUNM (vertexUvNormalMaterial) buffer using data
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(TerrainVertex), vertices.data(), GL_STATIC_DRAW);

	//Generate index buffer using data
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	createVAO();
}

void TerrainComponent::createVAO()
{
	glGenVertexArrays(1, &VAO);
	glSetBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		//Enable vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

		int inc = 0;
		GLuint vertLoc = 0;
		glEnableVertexAttribArray(vertLoc);
		glVertexAttribPointer(vertLoc, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*) (intptr_t) inc); //attribute, size, type, is normalised?, stride, offset
		inc += sizeof(glm::vec3);

		GLuint normLoc = 1;
		glEnableVertexAttribArray(normLoc);
		glVertexAttribPointer(normLoc, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex), (void*) (intptr_t) inc);
//		inc += sizeof(glm::vec3);
	glSetBindVertexArray(0);
}