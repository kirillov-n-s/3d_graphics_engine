#include <fstream>
#include <sstream>
#include <functional>
#include "../common/common.h"
#include "obj.h"

namespace IO {

    Geometry::PolygonMesh readObj(
            const std::string &path,
            std::string &error,
            const ObjReaderSettings &settings)
    {
        error = "";

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec2> texcoords;
        std::vector<glm::vec3> normals;

        std::vector<int> vertexIndices;
        std::vector<int> texcoordIndices;
        std::vector<int> normalIndices;

        std::vector<int> polygonStarts;
        polygonStarts.push_back(0);

        bool hasTexcoords = true;
        bool hasNormals = true;

        std::ifstream file { path };
        if (!file.is_open()) {
            error = "File " + path + " does not exist.";
            return {};
        }

        std::string header;
        for (;;) {

            file >> header;
            if (file.eof())
                break;

            if (header == "v") {
                glm::vec3 vertex;
                float w;
                file >> vertex.x >> vertex.y >> vertex.z;
                if (!file)
                    error = "Bad vertex.";
                file >> w;
                if (!file)
                    file.clear();
                else if (settings.errorOnVertex4NotOne && Common::fuzzyCompare(w, 1.0f))
                    error = "4th vertex coordinate not equal to 1.";
                if (!error.empty())
                    return {};
                vertices.push_back(vertex);
            }
            else if (header == "vt") {
                glm::vec2 texcoord;
                float z;
                file >> texcoord.x >> texcoord.y;
                if (!file)
                    error = "Bad texture coordinates.";
                file >> z;
                if (!file)
                    file.clear();
                else if (settings.readTexcoords && settings.errorOnTexcoord3NotZero && Common::fuzzyIsNull(z))
                    error = "3rd texture coordinate not equal to 0.";
                if (!error.empty())
                    return {};
                texcoords.push_back(texcoord);
            }
            else if (header == "vn") {
                glm::vec3 normal;
                file >> normal.x >> normal.y >> normal.z;
                if (!file) {
                    error = "Bad normal.";
                    return {};
                }
            }
            else if (header == "f") {

                std::string face;
                std::getline(file, face);
                face.erase(face.find_last_not_of(" \n\r\t") + 1);
                std::istringstream buffer { face };

                int vertexInd;
                int texcoordInd;
                int normalInd;

                int delim;

                for (;;) {

                    buffer >> vertexInd;
                    if (!buffer) {
                        error = "Bad face format.";
                        return {};
                    }
                    vertexIndices.push_back(vertexInd - 1);
                    if (buffer.eof()) {
                        if (hasTexcoords || hasNormals) {
                            error = "Inconsistent face format.";
                            return {};
                        }
                        break;
                    }

                    delim = buffer.get();
                    if (delim == ' ') {
                        if (hasTexcoords || hasNormals) {
                            if (!texcoordIndices.empty() || !normalIndices.empty()) {
                                error = "Inconsistent face format.";
                                return {};
                            }
                            hasTexcoords = hasNormals = false;
                        }
                        continue;
                    }
                    if (delim != '/') {
                        error = "Bad face format.";
                        return {};
                    }

                    buffer >> texcoordInd;
                    if (hasTexcoords && texcoordIndices.empty())
                        hasTexcoords = (bool)buffer;
                    else if (hasTexcoords != (bool)buffer) {
                        error = "Inconsistent face format.";
                        return {};
                    }
                    if (hasTexcoords)
                        texcoordIndices.push_back(texcoordInd - 1);
                    if (buffer.eof()) {
                        if (hasNormals) {
                            error = "Inconsistent face format.";
                            return {};
                        }
                        break;
                    }
                    if (!buffer)
                        buffer.clear();

                    delim = buffer.get();
                    if (delim == ' ') {
                        if (hasNormals) {
                            if (!normalIndices.empty()) {
                                error = "Inconsistent face format.";
                                return {};
                            }
                            hasNormals = false;
                        }
                        continue;
                    }
                    if (delim != '/') {
                        error = "Bad face format.";
                        return {};
                    }

                    buffer >> normalInd;
                    if (!buffer) {
                        error = "Bad face format.";
                        return {};
                    }
                    if (hasNormals)
                        normalIndices.push_back(normalInd - 1);
                    if (buffer.eof())
                        break;
                }

                polygonStarts.push_back(vertexIndices.size());

                const int nFaces = polygonStarts.size() - 1;
                if (polygonStarts[nFaces] - polygonStarts[nFaces - 1] < 3) {
                    error = "Not enough face elements.";
                    return {};
                }
            }
            else
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        file.close();

        return {
            vertices,
            vertexIndices,
            polygonStarts,
            {},
            settings.readTexcoords ? texcoords : std::vector<glm::vec2>{},
            settings.readTexcoords ? texcoordIndices : std::vector<int>{}
        };
    }

    void writeObj(
            const Geometry::PolygonMesh &mesh,
            const std::string &path,
            std::string &error,
            const ObjWriterSettings &settings)
    {
        error = "";

        std::ofstream file { path };
        if (!file.is_open()) {
            error = "Cannot open file " + path + ".";
            return;
        }

        const bool hasTexcoords = mesh.hasTexcoords();
        const bool hasNormals = mesh.hasNormals();

        for (const glm::vec3 &vertex : mesh.vertices)
            file << "v "
                 << vertex.x << ' '
                 << vertex.y << ' '
                 << vertex.z << "\n";
        file << '\n';

        if (hasTexcoords && settings.writeTexcoords) {

            for (const glm::vec2 &texcoord : mesh.texcoords)
                file << "vt "
                     << texcoord.x << ' '
                     << texcoord.y << '\n';
            file << '\n';
        }

        if (hasNormals && settings.writeNormals) {

            for (const glm::vec3 &normal : mesh.normals)
                file << "vn "
                     << normal.x << ' '
                     << normal.y << ' '
                     << normal.z << '\n';
            file << '\n';
        }

        const std::vector<int> &vertexIndices = mesh.vertexIndices;
        const std::vector<int> &texcoordIndices = mesh.texcoordIndices;
        const std::vector<int> &polygonStarts = mesh.polygonStarts;

        std::function<void(
                const std::vector<int> &,
                const std::vector<int> &,
                const int)> writeFace;
        if (hasTexcoords && hasNormals)
            writeFace = [&file](
                const std::vector<int> &vertexIndices,
                const std::vector<int> &texcoordIndices,
                const int faceElemInd) {

                file << (vertexIndices[faceElemInd] + 1) << '/'
                     << (texcoordIndices[faceElemInd] + 1) << '/'
                     << (vertexIndices[faceElemInd] + 1) << ' ';
            };
        else if (hasTexcoords)
            writeFace = [&file](
                    const std::vector<int> &vertexIndices,
                    const std::vector<int> &texcoordIndices,
                    const int faceElemInd) {

                file << (vertexIndices[faceElemInd] + 1) << '/'
                     << (texcoordIndices[faceElemInd] + 1) << ' ';
            };
        else if (hasNormals)
            writeFace = [&file](
                    const std::vector<int> &vertexIndices,
                    const std::vector<int> &,
                    const int faceElemInd) {

                file << (vertexIndices[faceElemInd] + 1) << "//"
                     << (vertexIndices[faceElemInd] + 1) << ' ';
            };
        else
            writeFace = [&file](
                    const std::vector<int> &vertexIndices,
                    const std::vector<int> &,
                    const int faceElemInd) {

                file << (vertexIndices[faceElemInd] + 1) << ' ';
            };

        const int nFaces = polygonStarts.size() - 1;
        int faceElemInd = 0;
        for (int i = 0; i < nFaces; i++) {
            file << "f ";
            for (int j = polygonStarts[i]; j < polygonStarts[i + 1]; j++, faceElemInd++)
                writeFace(
                        vertexIndices,
                        texcoordIndices,
                        faceElemInd);
            file << '\n';
        }
    }
}
