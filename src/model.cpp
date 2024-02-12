#include <model.h>
#include <iostream>

using namespace BEG;

Model::Model() : mVertices{} {}

Model::Model(const std::vector<Vector<3>>& vertices,
             const std::vector<Vector<3>>& normals,
             const std::vector<int>& colorIndices,
             const std::vector<Color>& colors) : mVertices{} {
    if (vertices.size() != colorIndices.size())
        throw Model::ModelError::VerticesIndicesMismatchError;
    
    if (*std::max_element(std::begin(colorIndices), std::end(colorIndices)) >= static_cast<int>(colors.size()))
        throw Model::ModelError::TooFewColorsError;

    size_t index{ 0 };
    for (auto vertex : vertices) {
        mVertices.push_back(vertex.x());
        mVertices.push_back(vertex.y());
        mVertices.push_back(vertex.z());
        mVertices.push_back(normals[index].x());
        mVertices.push_back(normals[index].y());
        mVertices.push_back(normals[index].z());
        mVertices.push_back(colors[static_cast<size_t>(colorIndices[index])].r());
        mVertices.push_back(colors[static_cast<size_t>(colorIndices[index])].g());
        mVertices.push_back(colors[static_cast<size_t>(colorIndices[index])].b());

        ++index;
    }

    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);

    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(mVertices.size() * sizeof(float)), mVertices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Model::~Model() {
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

Model& Model::operator=(Model &&model) {
    this->mVertices = std::move(model.mVertices);

    this->mVAO = model.mVAO;
    this->mVBO = model.mVBO;

    model.mVAO = 0;
    model.mVAO = 0;

    return *this;
}

Model Model::cube(const std::vector<int>& colorIndices, const std::vector<Color>& colors) {
    return Model{
        {
            { -1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
            { -1.0f, -1.0f, -1.0f },

            { -1.0f, -1.0f,  1.0f },
            {  1.0f, -1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            { -1.0f,  1.0f,  1.0f },
            { -1.0f, -1.0f,  1.0f },

            { -1.0f,  1.0f,  1.0f },
            { -1.0f,  1.0f, -1.0f },
            { -1.0f, -1.0f, -1.0f },
            { -1.0f, -1.0f, -1.0f },
            { -1.0f, -1.0f,  1.0f },
            { -1.0f,  1.0f,  1.0f },

            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },

            { -1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f,  1.0f },
            {  1.0f, -1.0f,  1.0f },
            { -1.0f, -1.0f,  1.0f },
            { -1.0f, -1.0f, -1.0f },
     
            { -1.0f,  1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            { -1.0f,  1.0f,  1.0f },
            { -1.0f,  1.0f, -1.0f },
        },
        {
            {  0.0f,  0.0f, -1.0f },
            {  0.0f,  0.0f, -1.0f }, 
            {  0.0f,  0.0f, -1.0f }, 
            {  0.0f,  0.0f, -1.0f }, 
            {  0.0f,  0.0f, -1.0f }, 
            {  0.0f,  0.0f, -1.0f }, 

            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },
            {  0.0f,  0.0f,  1.0f },

            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },
            { -1.0f,  0.0f,  0.0f },

            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },
            {  1.0f,  0.0f,  0.0f },

            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },
            {  0.0f, -1.0f,  0.0f },

            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f },
            {  0.0f,  1.0f,  0.0f }
        },
        colorIndices,
        colors
    };
}

Model Model::cube(const Color& color) {
    return Model::cube(std::vector<int>(36, 0), { color });
}

void Model::render() {
    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<int>(mVertices.size() / 6));
}