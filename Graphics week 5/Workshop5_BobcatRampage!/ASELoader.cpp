
// Basic ASE File loader
// Martin Masek, SCSS - Edith Cowan University, 2009
//
// .ase (ASCII Scene Export) files

#include "ASELoader.h"

fstream ASELoader::m_modelFile;
int ASELoader::m_numberOfVertices = 0;
int ASELoader::m_numberOfFaces = 0;

void ASELoader::loadModel(vector<Vertex3> &vertices, vector<int> &triangles, vector<Vertex3> &textures, vector<int> &texturedTriangles, string &textureFile, string fileName)
{
	vertices.clear();
	triangles.clear();
	m_modelFile.clear(); // this is needed otherwise file report eof.
	
	
	m_modelFile.open(fileName.c_str());


	if (m_modelFile.is_open())
	{
		char line[255];
		while (!m_modelFile.eof())
		{
			m_modelFile.getline(line,255);
			string currentLine = line;
			// check the line to see if it starts a block
			if (currentLine.find("*MATERIAL_LIST")!= string::npos)
			{
				// read materials....
				readMaterial(textureFile);
			}
			else if (currentLine.find("*GEOMOBJECT")!= string::npos) 
			{
				// read geometry...
				readGeometry(vertices, triangles, textures, texturedTriangles);
			}
			
			else if (currentLine.find("*GROUP")!= string::npos)
			{
			}
			else if (currentLine.find("*LIGHTOBJECT")!= string::npos)
			{
				// read light objects...
			}
			else if (currentLine.find("*CAMERAOBJECT")!= string::npos)
			{
				// read camera objects...
			}
			else
			{
				// read an unknown object...
				// if the line of the unknown line contains an open curly
				// bracket, skip to after the next close curly bracket.
				if (currentLine.find("{")!= string::npos)
				{
					do
					{
						m_modelFile.getline(line,255);
						currentLine = line;
					} while (currentLine.find("}") == string::npos);	

				}
			}
		}

		m_modelFile.close();
	}
}

void ASELoader::readMaterial(string &textureFile)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line, 255);
	currentLine = line;
	do
	{

		if (currentLine.find("*MATERIAL ") != string::npos)
		{
			readMap(textureFile);
		}
		else
		{
			// possible to exdend to handle other blocks
			// read an unknown object...
			// if the line of the unknown line contains an open curly
			// bracket, skip to after the next close curly bracket.
			if (currentLine.find("{") != string::npos)
			{
				do
				{
					m_modelFile.getline(line, 255);
					currentLine = line;
				} while (currentLine.find("}") == string::npos);
			}
		}
		m_modelFile.getline(line, 255);
		currentLine = line;

	} while (currentLine.find("}") == string::npos);
}


void ASELoader::readMap(string &textureFile)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line, 255);
	currentLine = line;
	do
	{

		if (currentLine.find("*MAP_DIFFUSE ") != string::npos)
		{
			// read name of image file to use for texture
			readTextureFile(textureFile);
		}
		else
		{
			// possible to exdend to handle other blocks
			// read an unknown object...
			// if the line of the unknown line contains an open curly
			// bracket, skip to after the next close curly bracket.
			if (currentLine.find("{") != string::npos)
			{
				do
				{
					m_modelFile.getline(line, 255);
					currentLine = line;
				} while (currentLine.find("}") == string::npos);
			}
		}
		m_modelFile.getline(line, 255);
		currentLine = line;

	} while (currentLine.find("}") == string::npos);
}


void ASELoader::readTextureFile(string &textureFile)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line, 255);
	currentLine = line;
	do
	{

		if (currentLine.find("*BITMAP ") != string::npos)
		{
			// read name of image file to use for texture
			int displacement = (int)currentLine.find("*BITMAP ") + 8;
			textureFile = currentLine.substr(displacement);
			// strip " " from the file name
			int index;
			index = textureFile.find_first_of("\"");
			textureFile.erase(index, 1);
			index = textureFile.find_last_of("\"");
			textureFile.erase(index, 1);
		}
		else
		{
			// possible to exdend to handle other blocks
			// read an unknown object...
			// if the line of the unknown line contains an open curly
			// bracket, skip to after the next close curly bracket.
			if (currentLine.find("{") != string::npos)
			{
				do
				{
					m_modelFile.getline(line, 255);
					currentLine = line;
				} while (currentLine.find("}") == string::npos);
			}
		}
		m_modelFile.getline(line, 255);
		currentLine = line;

	} while (currentLine.find("}") == string::npos);
}



void ASELoader::readGeometry(vector<Vertex3> &vertices, vector<int> &triangles, vector<Vertex3> &textures, vector<int> &texturedTriangles)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line,255);
	currentLine = line;
	do
	{

		if (currentLine.find("*MESH ")!= string::npos)
		{
			// read mesh data...
			readMesh(vertices, triangles, textures, texturedTriangles);
		}
		else
		{
			// possible to exdend to handle other blocks
			// read an unknown object...
			// if the line of the unknown line contains an open curly
			// bracket, skip to after the next close curly bracket.
			if (currentLine.find("{")!= string::npos)
			{
				do
				{
					m_modelFile.getline(line,255);
					currentLine = line;
				} while (currentLine.find("}") == string::npos);
			}
		}
		m_modelFile.getline(line,255);
		currentLine = line;

	} while (currentLine.find("}") == string::npos);
}

void ASELoader::readMesh(vector<Vertex3> &vertices, vector<int> &triangles, vector<Vertex3> &textures, vector<int> &texturedTriangles)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line,255);
	currentLine = line;
	do
	{
		if (currentLine.find("*MESH_NUMVERTEX")!= string::npos)
		{
			// read number of vertices...
			int displacement = (int)currentLine.find("*MESH_NUMVERTEX") + 15;
			m_numberOfVertices = atoi(currentLine.substr(displacement).c_str());
		}
		else if (currentLine.find("*MESH_NUMFACES")!= string::npos)
		{
			// read number of faces...
			int displacement = (int)currentLine.find("*MESH_NUMFACES") + 14;
			m_numberOfFaces = atoi(currentLine.substr(displacement).c_str());
		}
		else if (currentLine.find("*MESH_VERTEX_LIST")!= string::npos)
		{
			// read the actual vertices...
			readVertexList(vertices);
		}
		else if (currentLine.find("*MESH_FACE_LIST")!= string::npos)
		{
			// read the actual faces...
			readMeshFaceList(triangles);
		}
		else if (currentLine.find("*MESH_TVERTLIST") != string::npos)
		{
			// read the actual faces...
			readTVertexList(textures);
		}
		else if (currentLine.find("*MESH_TFACELIST") != string::npos)
		{
			// read the actual faces...
			readTMeshFaceList(texturedTriangles);
		}

		else
		{
			// read some unknown data

			// read an unknown object...
			// if the line of the unknown line contains an open curly
			// bracket, skip to after the next close curly bracket.
			if (currentLine.find("{")!= string::npos)
			{
				do
				{
					m_modelFile.getline(line,255);
					currentLine = line;
				} while (currentLine.find("}") == string::npos);
			}
		}
		m_modelFile.getline(line,255);
		currentLine = line;
	} while (currentLine.find("}") == string::npos);
}

void ASELoader::readVertexList(vector<Vertex3> &vertices)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line,255);
	currentLine = line;
	int i;
	float x,y,z;
	Vertex3 newVertex;
	do
	{
		if (currentLine.find("*MESH_VERTEX ")!= string::npos)
		{
			// read in the x,y,z components of the vertex
			// this line in the .ASE file is of the form (eg.):
			// *MESH_VERTEX    0	-20.1053	-18.7514	0.0000

			int displacement = (int)currentLine.find("*MESH_VERTEX ") + 13;

			//sscanf(currentLine.substr(displacement).c_str(), "%d\t%f\t%f\t%f", &i, &x, &y, &z);
			sscanf_s(currentLine.substr(displacement).c_str(), "%d\t%f\t%f\t%f", &i, &x, &y, &z);

			newVertex.set(x,y,z);
			vertices.push_back(newVertex);
		}
		m_modelFile.getline(line,255);
		currentLine = line;

	} while (currentLine.find("}") == string::npos);

}



void ASELoader::readMeshFaceList(vector<int> &triangles)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line,255);
	currentLine = line;
	int i, a, b, c;
	do
	{
		if (currentLine.find("*MESH_FACE ")!= string::npos)
		{
			// read in the vertex indices for the face
			// this line is of the form (eg.):
			// *MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 

			int displacement = (int)currentLine.find("*MESH_FACE ") + 11;
			// just read the A,B,C indices of the triangle (ignore the other stuff for now)
			string test1 = currentLine.substr(displacement);
			const char* tempString = test1.c_str();
			//sscanf(tempString, "%d: A: %d B: %d C: %d", &i, &a, &b, &c);
			sscanf_s(tempString, "%d: A: %d B: %d C: %d", &i, &a, &b, &c);

			triangles.push_back(a);
			triangles.push_back(b);
			triangles.push_back(c);
		}
		m_modelFile.getline(line,255);
		currentLine = line;
	} while (currentLine.find("}") == string::npos);
}

void ASELoader::readTVertexList(vector<Vertex3> &textures)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line, 255);
	currentLine = line;
	int i;
	float x, y, z;
	Vertex3 newVertex;
	do
	{
		if (currentLine.find("*MESH_TVERT ") != string::npos)
		{
			// read in the x,y,z components of the vertex
			// this line in the .ASE file is of the form (eg.):
			// *MESH_VERTEX    0	-20.1053	-18.7514	0.0000

			int displacement = (int)currentLine.find("*MESH_TVERT ") + 12;

			//sscanf(currentLine.substr(displacement).c_str(), "%d\t%f\t%f\t%f", &i, &x, &y, &z);
			sscanf_s(currentLine.substr(displacement).c_str(), "%d\t%f\t%f\t%f", &i, &x, &y, &z);

			newVertex.set(x, 1-y, z);
			textures.push_back(newVertex);
		}
		m_modelFile.getline(line, 255);
		currentLine = line;

	} while (currentLine.find("}") == string::npos);

}

void ASELoader::readTMeshFaceList(vector<int> &texturedTriangles)
{
	char line[255];
	string currentLine = line;

	// read the file until we come to a close bracket
	m_modelFile.getline(line, 255);
	currentLine = line;
	int i, a, b, c;
	do
	{
		if (currentLine.find("*MESH_TFACE ") != string::npos)
		{
			// read in the vertex indices for the face
			// this line is of the form (eg.):
			// *MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 

			int displacement = (int)currentLine.find("*MESH_TFACE ") + 12;
			// just read the A,B,C indices of the triangle (ignore the other stuff for now)
			string test1 = currentLine.substr(displacement);
			const char* tempString = test1.c_str();
			//sscanf(tempString, "%d: A: %d B: %d C: %d", &i, &a, &b, &c);
			sscanf_s(tempString, "%d\t%d\t%d\t%d", &i, &a, &b, &c);
			texturedTriangles.push_back(a);
			texturedTriangles.push_back(b);
			texturedTriangles.push_back(c);
		}
		m_modelFile.getline(line, 255);
		currentLine = line;
	} while (currentLine.find("}") == string::npos);
}