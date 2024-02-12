#ifndef BEG_MODEL_H
#define BEG_MODEL_H

#include <bmath.h>
#include <color.h>

#include <glad/glad.h>

#include <algorithm>
#include <vector>

namespace BEG {

class Model {
private:
    /* packed array of vertices and vertex colors */
    std::vector<float> mVertices{};

    unsigned int mVAO{}, mVBO{};

public:
    enum class ModelError {
        VerticesIndicesMismatchError,
        TooFewColorsError
    };

    Model();
    Model(
        const std::vector<Vector<3>>& vertices,
        const std::vector<Vector<3>>& normals,
        const std::vector<int>& colorIndices,
        const std::vector<Color>& colors
    );
    ~Model();

    Model& operator=(Model&& model);

    static Model cube(const std::vector<int>& colorIndices, const std::vector<Color>& colors);
    static Model cube(const Color& color);

    void render();
};

}

#endif