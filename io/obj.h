#ifndef IO_OBJ_H
#define IO_OBJ_H

#include <string>
#include "../geometry/polygonmesh.h"

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

    Geometry::PolygonMesh readObj(
            const std::string &path,
            std::string &error,
            const ObjReaderSettings &settings = {});

    void writeObj(
            const Geometry::PolygonMesh &mesh,
            const std::string &path,
            std::string &error,
            const ObjWriterSettings &settings = {});
}

#endif
