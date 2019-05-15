// P4G - Semester 2 - Group Project 2019
// CS4G : Group F
// Charlie Batten - 27012619, Nico Caruana - 27022205, Alin Medianu - 27005327

#ifndef GEOMETRYBUILDER_H
#define GEOMETRYBUILDER_H

// Some handy functions to build common primitives

class MeshManager;
class GameModel;
class Mesh;

Mesh& BuildQuad(MeshManager& mgr);

Mesh& BuildPyramid(MeshManager& mgr);

Mesh& BuildQuadPyramid(MeshManager& mgr);

Mesh& BuildCube(MeshManager& mgr);

Mesh& BuildSphere(MeshManager& mgr, int LatLines, int LongLines);

#endif // !GEOMETRYBUILDER_H
