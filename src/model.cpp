


#include <cstring>
#include <unordered_map>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"



namespace std {
    template<> struct hash<Vertex_> {
        size_t operator()(Vertex_ const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

void LoadModel(const std::string modelPath, std::vector<Vertex_> *vertices, std::vector<uint32_t> *indices, glm::vec3 color)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
    {
        throw std::runtime_error(warn + err);
    }
    std::unordered_map<Vertex_, uint32_t> uniqueVertices{};

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex_ vertex{};

            if (attrib.vertices.size() > 0)
            {
                vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]};
            }

            if (attrib.texcoords.size() > 0)
            {
                vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};
            }
            
            vertex.color = color;

            if (attrib.normals.size() > 0)
            {
                vertex.normal = {
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]};
            }

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices->size());
                vertices->push_back(vertex);
            }

            indices->push_back(uniqueVertices[vertex]);
        }
    }
}

void LoadModel(const std::string modelPath, std::vector<Vertex_> *vertices, std::vector<uint32_t> *indices)
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, modelPath.c_str()))
    {
        throw std::runtime_error(warn + err);
    }
    std::unordered_map<Vertex_, uint32_t> uniqueVertices{};

    for (const auto &shape : shapes)
    {
        for (const auto &index : shape.mesh.indices)
        {
            Vertex_ vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]};

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]};

            vertex.color = {0.3f, 1.0f, 0.6f};

            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices->size());
                vertices->push_back(vertex);
            }

            indices->push_back(uniqueVertices[vertex]);
        }
    }
}


void BindModel(VkCommandBuffer commandBuffer, Model* model)
{   
    VkBuffer buffers[] = {model->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
    if (model->hasIndexBuffer) 
    {
        vkCmdBindIndexBuffer(commandBuffer, model->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    }
}
