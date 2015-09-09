
// Basic ASE File loader
// Martin Masek, SCSS - Edith Cowan University, 2009
//
// .ase (ASCII Scene Export) files


#pragma once

#include "fstream"
#include <vector>
#include "Vertex3.h"


using namespace std;



class ASELoader
{
public:
	static fstream m_modelFile;
	static int m_numberOfVertices;
	static int m_numberOfFaces;
	
	static void loadModel(vector<Vertex3> &vertices, vector<int> &triangles, vector<Vertex3> &textures, vector<int> &texturedTriangles, string &textureFile, string fileName);

private:

	static void ASELoader::readMaterial(string &textureFile);

	static void ASELoader::readMap(string &textureFile);

	static void ASELoader::readTextureFile(string &textureFile);

	static void readGeometry(vector<Vertex3> &vertices, vector<int> &triangles, vector<Vertex3> &textures, vector<int> &texturedTriangles);

	static void readMesh(vector<Vertex3> &vertices, vector<int> &triangles, vector<Vertex3> &textures, vector<int> &texturedTriangles);

	static void readVertexList(vector<Vertex3> &vertices);

	static void readMeshFaceList(vector<int> &triangles);

	static void readTVertexList(vector<Vertex3>&textures);

	static void readTMeshFaceList(vector<int>&texturedTriangles);

};
