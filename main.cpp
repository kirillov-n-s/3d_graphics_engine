#include "core3d/shapes.h"
#include "demos/basicdemo.h"
#include "io/obj.h"

int main()
{
	// const Core3d::TriangleMesh sphere = Core3d::Shapes::makeUvSphere(10.0f, 100, 100);
	// const Core3d::PolygonMesh spherePoly(sphere);
	// std::string error;
	// IO::writeObj(
	// 	spherePoly,
	// 	R"(C:\Users\kirillov_n_s\Desktop\Projects\engine\data\meshes\uv_sphere.obj)",
	// 	error,
	// 	IO::ObjWriterSettings{
	// 		.writeTexcoords = true,
	// 		.writeNormals = false
	// 	});
	Demos::BasicDemo demo;
	demo.run();
	return 0;
}
