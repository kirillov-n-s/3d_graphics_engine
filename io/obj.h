#ifndef IO_OBJ_H
#define IO_OBJ_H

#include <string>
#include "../core3d/polygonmesh.h"

namespace IO {

    struct ObjReaderSettings
    {
        bool errorOnVertex4NotOne = true;
        bool errorOnTexcoord3NotZero = true;
        bool readTexcoords = true;
    };

    struct ObjWriterSettings
    {
        bool writeTexcoords = true;
        bool writeNormals = true;
    };

    Core3d::PolygonMesh readObj(
            const std::string &path,
            std::string &error,
            const ObjReaderSettings &settings = {});

    void writeObj(
            const Core3d::PolygonMesh &mesh,
            const std::string &path,
            std::string &error,
            const ObjWriterSettings &settings = {});
}

#endif
