// Martin Masek - SCSS, Edith Cowan University, 2009
//
// Created: August 2009

#pragma once
#ifndef Geometry_OBJECT
#define Geometry_OBJECT

// include the headers for the OpenGL and OpenGL utilities libraries
#include <windows.h>
#include "GLEW/glew.h"
#include <GL/glu.h>
#include <GL/gl.h>

#include <vector> // include the 'vector' container from the standard template library
#include <algorithm>


#include "ASELoader.h"
#include "Vertex3.h"
#include "Vector3.h"
#include "TextureCreator.h"

using namespace std;



class Geometry
{
private:
	// geometry identifier
	string m_ID;
	int m_uniqueID;

	// vertex coordinates
	vector<Vertex3> m_vertexCoordinates; // a list of all the vertices in the model
	// triangle indices
	vector<int> m_triangleIndices; // list storing the order that vertices are drawn
	// texture coordinates
	vector<Vertex3> m_textureCoordinates;
	// texture coordinate indices
	vector<int> m_textureIndices;

	GLuint m_material;
	string m_materialFile;

	// modelToWorldTranform matrix; // To be done
	Vector3f m_relativeOrientation; // angle relative to parent (x,y,z angles)
	Vector3f m_relativePosition; // position relative to parent
	float m_boundingSphereRadius;

	Vector3f m_colour; // vertex colour

	// children
	vector<Geometry> m_children;

	GLuint m_bufferVertList;
	GLuint m_bufferTriList;


public:

	~Geometry()
	{
		// delete VBOs
		if (glIsBuffer(m_bufferVertList))
		{
		glDeleteBuffers(1, &m_bufferVertList);
		m_bufferVertList = NULL;
		}
		if (glIsBuffer(m_bufferTriList))
		{
		glDeleteBuffers(1, &m_bufferTriList);
		m_bufferTriList = NULL;		
		}

		if (m_material!=NULL)
		{
			//glDeleteTextures(1, &m_material);
			m_material = NULL;
		}
	}

	Geometry() // constructor
	{
		// clear the vectors
		m_textureCoordinates.clear();
		m_textureIndices.clear();
		m_vertexCoordinates.clear();
		m_triangleIndices.clear();
		m_textureCoordinates.clear();
		m_children.clear();
		m_relativeOrientation.set(0,0,0);
		m_relativePosition.set(0,0,0);
		m_colour.set(0,0,0);

		m_material = NULL;
		m_bufferVertList = NULL;
		m_bufferTriList = NULL;
		m_materialFile.clear();

		// create some example geometry
		Vertex3 test(-1,0,0);
		m_vertexCoordinates.push_back(test);
		test.set(0,1,0);
		m_vertexCoordinates.push_back(test);
		test.set(1,0,0);
		m_vertexCoordinates.push_back(test);

		m_triangleIndices.clear();
		m_triangleIndices.push_back(0);
		m_triangleIndices.push_back(1);
		m_triangleIndices.push_back(2);

		computeBoundingSphere(Vector3f(0,0,0));
	}

	bool hasVBO()
	{
		if (glIsBuffer(m_bufferVertList))
		{
			return true;
		}
		else return false;
	}

	void clearVBO()
	{
		glDeleteBuffers(1, &m_bufferVertList);
		m_bufferVertList = NULL;
		glDeleteBuffers(1, &m_bufferTriList);
		m_bufferTriList = NULL;
	}

	float getBoundingSphereRadius()
	{
		return m_boundingSphereRadius;
	}


	// bounding sphere computation - take radius as the furthest vertex from local origin.
	float computeBoundingSphere(Vector3f centre)
	{
		m_boundingSphereRadius = 0;
		float temp;
		if (!m_vertexCoordinates.empty())
		{
			// go through vertices and find maximum dist from origin
			for each (Vertex3 vert in m_vertexCoordinates)
			{
				//temp = pow(vert.x()-centre.x(),2) + pow(vert.y()-centre.y(),2) + pow(vert.z()-centre.z(),2);
				// take x and z only (bounding circle)
				temp = pow(vert.x()-centre.x(),2) + pow(vert.z()-centre.z(),2);
				temp = sqrt(temp);
				m_boundingSphereRadius = max(m_boundingSphereRadius,temp);
			}
			// check the kids vertices as well
			if (!m_children.empty())
			{
				for(vector<Geometry>::iterator child = m_children.begin(); child<m_children.end(); child++)
				{
					temp = child->computeBoundingSphere(centre-child->getPosition());
					m_boundingSphereRadius = max(m_boundingSphereRadius, temp);
				}
			}
		}
		return m_boundingSphereRadius;
	}


	bool isColiding(Vector3f position, float otherSphereRadius)
	{
		// bounding sphere collision detection
		Vector3f displacement = position - m_relativePosition;
		if (displacement.lengthSquared()<pow(m_boundingSphereRadius+otherSphereRadius,2))
		{
			return (1);
		}
		else
		{
			return (0);
		}
	}


	void addChild(Geometry newChild)
	{
		m_children.push_back(newChild);
	}

	void setMaterial(string name)
	{		
		m_materialFile = name;	
		m_material = TextureCreator::loadTexture(m_materialFile);
		if (!glIsTexture(m_material))
		{
			// error???
		}
	}

	GLuint getMaterial()
	{
		return m_material;
	}

	void setName(string &name)
	{
		m_ID = name.data();
	}

	string getName()
	{
		return m_ID;
	}

	void setID(int ID)
	{
		m_uniqueID = ID;
	}

	int getID()
	{
		return m_uniqueID;
	}

	void setColour(float r, float g, float b)
	{
		m_colour.set(r,g,b);
	}


	Vector3f getAngle()
	{
		return m_relativeOrientation;
	}

	bool setAngle(Vector3f angle, string geometryID)
	{
		if (!this->m_ID.compare(geometryID))
		{
			// do rotation of this
			m_relativeOrientation= angle;
			return true;
		}
		else // else check children
		{
			bool found = false;


			for(vector<Geometry>::iterator i = m_children.begin(); i<m_children.end(); i++)
			{
				// if ((*i)->setAngle(angle, geometryID))
				if (i->setAngle(angle, geometryID))
				{
					return true;
				}
			}
			return false;
		}
	}



	Vector3f getPosition()
	{
		return m_relativePosition;
	}

	bool setPosition(Vector3f position, string geometryID)
	{
		if (!this->m_ID.compare(geometryID))
		{
			// set position of this
			m_relativePosition = position;
			return true;
		}
		else // else check children
		{
			bool found = false;
			for(vector<Geometry>::iterator i = m_children.begin(); i<m_children.end(); i++)
			{
				// if ((*i)->setAngle(angle, geometryID))
				if (i->setPosition(position, geometryID))
				{
					return true;
				}
			}
			return false;
		}
	}


	void update() // update the Geometry
	{
		// first update itself

		// INSERT UPDATE CODE HERE

		// then update children

		for(vector<Geometry>::iterator child = m_children.begin(); child<m_children.end(); child++)
		{
			child->update();
		}


	}


	void loadGeometry(string fileName)
	{
		// loads model geometry from disk
		ASELoader::loadModel(m_vertexCoordinates, m_triangleIndices, m_textureCoordinates, m_textureIndices, m_materialFile, fileName);
		// load the texture file
		if (!m_materialFile.empty())
		{
			m_material = TextureCreator::loadTexture(m_materialFile);
		}
		computeBoundingSphere(Vector3f(0,0,0));
	}

	void setGeometry(vector<Vertex3>& vertices, vector<int>& triangles, vector<Vertex3> &textureCoordinates, vector<int> &textureIndices)
	{
		m_vertexCoordinates = vertices;
		m_triangleIndices = triangles;
		m_textureCoordinates = textureCoordinates;
		m_textureIndices = textureIndices;
		computeBoundingSphere(Vector3f(0,0,0));
	}

	void drawOpenGLImmediate()
	{
		glPushMatrix();

		// model to parent transform
		glTranslatef(m_relativePosition.x(), m_relativePosition.y(), m_relativePosition.z()); // translation

		glRotatef(m_relativeOrientation.x(), 1, 0, 0); // Euler rotation
		glRotatef(m_relativeOrientation.y(), 0, 1, 0);
		glRotatef(m_relativeOrientation.z(), 0, 0, 1);

		// end of model to parent transform

		// set the geometry colour
		glColor3f(m_colour.x(), m_colour.y(), m_colour.z());

		// activate the geometry texture

		if (!m_materialFile.empty())
		{
			if (!glIsTexture(m_material))
			{
				m_material = TextureCreator::loadTexture(m_materialFile); // error???
			}
			glEnable(GL_TEXTURE_2D);
			glBindTexture (GL_TEXTURE_2D, m_material);
		}


		// draw this geometry

		glBegin(GL_TRIANGLES);

		int count = 0;
		for each (int i in m_triangleIndices)
		{
			if (m_material!=NULL)
			{
				glTexCoord2f(m_textureCoordinates[m_textureIndices[count]].x(), m_textureCoordinates[m_textureIndices[count]].y());
			}
			glVertex3fv(m_vertexCoordinates[i]);
			count++;
		}

		glEnd();

		glDisable(GL_TEXTURE_2D);

		// now draw the kids
		for(vector<Geometry>::iterator child = m_children.begin(); child<m_children.end(); child++)
		{
			child->drawOpenGLImmediate();
		}

		glPopMatrix();
	}

	void createOpenGLVertexBufferObject()
	{
		//
		// Need to go from two arrays of indices (one for vertices, one for textures)
		// to a single index array with each index for a texture and vertex coordinate
		// this means expanding the array
		//

		vector<Vertex3> expandedVertices;
		vector<Vertex3> expandedTextures;
		vector<int> expandedIndices;
		expandedVertices.clear(); expandedTextures.clear(); expandedIndices.clear();
		int numberOfAddedVertices = 0;
		bool insertingNew = true;

		// loop through the existing indices
		for (unsigned int i = 0; i<m_triangleIndices.size(); i++)
		{
			// check if the vertex/texture pair is already inserted in the new list
			for (int j=0; j<numberOfAddedVertices; j++)
			{
				// check the vertex coordinate
				if (m_vertexCoordinates[m_triangleIndices[i]] == expandedVertices[j])
				{
					if (!m_textureIndices.empty()) // if the model has textures, check them
					{
						if (m_textureCoordinates[m_textureIndices[i]] == expandedTextures[j])
						{
							// yes, this combination already exists, merely point to it
							expandedIndices.push_back(j);
							insertingNew = false;
							break; // breaks out of the inner for loop
						}
					}
					else
					{
						// yes, this combination already exists, merely point to it
						expandedIndices.push_back(j);
						insertingNew = false;
						break; // breaks out of the inner for loop
					}
				}
			}
			if (insertingNew)
			{
				// if we are here it means there was no match, so insert the vertex and texture coordinates, and point to it
				expandedVertices.push_back(m_vertexCoordinates[m_triangleIndices[i]]);
				if (!m_textureCoordinates.empty())
				{
					expandedTextures.push_back(m_textureCoordinates[m_textureIndices[i]]);
				}
				expandedIndices.push_back(numberOfAddedVertices);
				numberOfAddedVertices++;
			}
			else
			{
				insertingNew = true;
			}

		}

		m_vertexCoordinates = expandedVertices;
		m_triangleIndices = expandedIndices;
		m_textureIndices = expandedIndices;
		if (!m_textureIndices.empty())
		{
			m_textureCoordinates = expandedTextures;
		}


		// *****************************
		// Create the vertex buffer
		// *****************************

		// get a buffer ID
		glGenBuffers(1, &m_bufferVertList);
		// make the buffer current
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferVertList);
		// make space for the data
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*3*m_vertexCoordinates.size() +sizeof(GLfloat)*2*m_textureCoordinates.size(), NULL, GL_STATIC_DRAW);

		// map the buffer to main memory
		GLfloat *vertBuff = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

		// copy data into buffer

		// vertices
		int currentIndex = 0;
		for each (Vertex3 currentVertex in m_vertexCoordinates)
		{
			vertBuff[currentIndex] = currentVertex.x();
			vertBuff[currentIndex+1] = currentVertex.y();
			vertBuff[currentIndex+2] = currentVertex.z();
			currentIndex += 3;
		}
		// texture coordinates
		if (!m_textureIndices.empty())
		{
		for each (Vertex3 currentTexture in m_textureCoordinates)
		{
			vertBuff[currentIndex] = currentTexture.x();
			vertBuff[currentIndex+1] = currentTexture.y();
			currentIndex += 2;
		}
		}

		// unmap the bufffer
		glUnmapBuffer(GL_ARRAY_BUFFER);


		// *****************************
		// Create the index buffer
		// *****************************

		// get a buffer ID
		glGenBuffers(1, &m_bufferTriList);
		// make the buffer current
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferTriList);
		// make space for the data
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * m_triangleIndices.size(), NULL, GL_STATIC_DRAW);

		// map the buffer to main memory
		GLuint *triBuff = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);

		// copy data into buffer
		currentIndex = 0;
		for each (int currentTriangleIndex in m_triangleIndices)
		{
			triBuff[currentIndex] = currentTriangleIndex;
			currentIndex += 1;
		}

		// unmap the bufffer
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	}

	void drawOpenGLVertexBufferObject()
	{
		// check that the buffer exists
		if (!glIsBuffer(m_bufferVertList))
		{
			createOpenGLVertexBufferObject();
		}

		glPushMatrix();

		// model to parent transform
		glTranslatef(m_relativePosition.x(), m_relativePosition.y(), m_relativePosition.z()); // translation

		glRotatef(m_relativeOrientation.x(), 1, 0, 0); // Euler rotation
		glRotatef(m_relativeOrientation.y(), 0, 1, 0);
		glRotatef(m_relativeOrientation.z(), 0, 0, 1);

		// end of model to parent transform



		// set the geometry colour
		glColor3f(m_colour.x(), m_colour.y(), m_colour.z());

		// draw this geometry

		// make the buffers current
		glBindBuffer(GL_ARRAY_BUFFER, m_bufferVertList);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferTriList);

		// enable the drawing of vertex positions
		glEnableClientState(GL_VERTEX_ARRAY);
		// point to the vertex data
		glVertexPointer(3,GL_FLOAT, 0, (float *)NULL);
		
		// same for textures
		if (!m_textureIndices.empty())
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2,GL_FLOAT,0, (float *)NULL + (m_vertexCoordinates.size()* 3));
		}


		// activate the geometry texture

		if (!m_materialFile.empty())
		{
			if (!glIsTexture(m_material))
			{
				m_material = TextureCreator::loadTexture(m_materialFile); // error???
			}
			glEnable(GL_TEXTURE_2D);
			glBindTexture (GL_TEXTURE_2D, m_material);
		}


		// draw using the buffer data
		glDrawElements(GL_TRIANGLES, m_triangleIndices.size(), GL_UNSIGNED_INT, 0);

		glDisable(GL_TEXTURE_2D);


		// now draw the kids
		for(vector<Geometry>::iterator child = m_children.begin(); child<m_children.end(); child++)
		{
			child->drawOpenGLVertexBufferObject();
		}

		glPopMatrix();
	}


};

#endif