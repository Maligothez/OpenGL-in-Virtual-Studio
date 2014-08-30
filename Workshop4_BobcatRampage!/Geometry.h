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

	// modelToWorldTranform matrix; // To be done
	Vector3f m_relativeOrientation; // angle relative to parent (x,y,z angles)
	Vector3f m_relativePosition; // position relative to parent
	float m_boundingSphereRadius;

	Vector3f m_colour; // vertex colour

	// children
	vector<Geometry> m_children;

	GLuint mBufferVertList;
	int mNumberOfVertices;

public:
	
    Geometry() // constructor
	{
		// clear the vectors
		m_vertexCoordinates.clear();
		m_triangleIndices.clear();
		m_children.clear();
		m_relativeOrientation.set(0,0,0);
		m_relativePosition.set(0,0,0);
		m_colour.set(0,0,0);

		
		// create some example geometry
		/*Vertex3 test(-1,0,0);
		m_vertexCoordinates.push_back(test);
		test.set(0,1,0);
		m_vertexCoordinates.push_back(test);
		test.set(1,0,0);
		m_vertexCoordinates.push_back(test);*/

		m_triangleIndices.clear();
		m_triangleIndices.push_back(0);
		m_triangleIndices.push_back(1);
		m_triangleIndices.push_back(2);

		mNumberOfVertices = 144;
		//createOpenGLVertexBufferObject();

		computeBoundingSphere(Vector3f(0,0,0));
	}

	~Geometry() // destructor
	{
		// the object is being destroyed - so free its VBO
		glDeleteBuffers(1, &mBufferVertList);
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
				for each (Geometry child in m_children)
				{
					temp = child.computeBoundingSphere(centre-child.getPosition());
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

			//for each (Geometry child in m_children)
			//{
			//	if (child.setAngle(angle, geometryID))
			//	{
			//		return true;
			//	}
			//}
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
		for each (Geometry child in m_children)
		{
			child.update();
		}


	}

	
	void loadGeometry(string fileName)
	{
		// loads model geometry from disk
		ASELoader::loadModel(m_vertexCoordinates, m_triangleIndices, fileName);
		computeBoundingSphere(Vector3f(0,0,0));
	}

	void setGeometry(vector<Vertex3>& vertices, vector<int>& triangles)
	{
		m_vertexCoordinates = vertices;
		m_triangleIndices = triangles;
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

		// draw this geometry
		
		glBegin(GL_TRIANGLES);

		for each (int i in m_triangleIndices)
		{
			glVertex3fv(m_vertexCoordinates[i]);
		}

		glEnd();


		// now draw the kids
		for each (Geometry child in m_children)
		{
			child.drawOpenGLImmediate();
		}

		glPopMatrix();
	}

	void createOpenGLVertexBufferObject()
	{
		glGenBuffers(1, &mBufferVertList);

		// make the buffer object current as an array buffer (used to store vertex data)
		glBindBuffer(GL_ARRAY_BUFFER, mBufferVertList);
		// reserve some space in memory for the vertex data (uses the current buffer - i.e.. the one referred to in the previous glBindBuffer line)
		// 3*sizeof(GLfloat)*(3+3) ??? - this is the amount of memory we are reserving.
		// 3 vertices, each one has 3 Glfloats to represent position and 3 GL gloats to represent colour
		// the sizeof() function gives us the amount of memory taken by a particular type
		// ie. after this, we have room for 3*(3+3) == 18 GLfloats in the buffer.
		glBufferData(GL_ARRAY_BUFFER, mNumberOfVertices*sizeof(GLfloat)*(3+0+3), NULL, GL_STATIC_DRAW);

		// this maps our buffer (video memory) to main memory, through the variable
		//  vertBuff. Modifying vertBuff modifies video memory!
		GLfloat *vertBuff = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER,GL_WRITE_ONLY);

		int vertIndex = 0;

		for each (int i in m_triangleIndices)
		{
			vertBuff[vertIndex++] = m_vertexCoordinates[i].x();
			vertBuff[vertIndex++] = m_vertexCoordinates[i].y();
			vertBuff[vertIndex++] = m_vertexCoordinates[i].z();
			vertBuff[vertIndex++] = m_colour.x();
			vertBuff[vertIndex++] = m_colour.y();
			vertBuff[vertIndex++] = m_colour.z();
		}

		// unmap the buffer - this break the connection between vertBuff and video memory
		// you must do this before drawing the buffer
		glUnmapBuffer(GL_ARRAY_BUFFER);

		// note:  the above are just some numbers in memory - we haven't told OpenGL
		// yet what they mean (it doesn't know they are vertex coordinates and colours)
		// we will do that next.

		// Each vertex has 3 components, is a GLfloat, stride is 0 (dont have to jump over memory to get to the next vertex)
		// and the vertices begin at the beginning of the buffer - (float *)NULL
		glVertexPointer(3, GL_FLOAT, 24, (float *)NULL);

		// the colours begin after the vertices, so from the start of the buffer
		// jump forward by numberOfVertices*3 (because each vertex has 3 components)
		glColorPointer(3, GL_FLOAT, 24,(float *)NULL + 3);
	}
	
	void drawOpenGLVertexBufferObject()
	{
		mNumberOfVertices = m_triangleIndices.size();
		if (!glIsBuffer(mBufferVertList))
		{
			createOpenGLVertexBufferObject();
		}
		// make sure that the buffer hasn't become invalid for some reason, if it has - recreate it.
		glPushMatrix();
		// model to parent transform
		glTranslatef(m_relativePosition.x(), m_relativePosition.y(), m_relativePosition.z()); // translation

		glRotatef(m_relativeOrientation.x(), 1, 0, 0); // Euler rotation
		glRotatef(m_relativeOrientation.y(), 0, 1, 0);
		glRotatef(m_relativeOrientation.z(), 0, 0, 1);

		// make the buffer current
		glBindBuffer(GL_ARRAY_BUFFER, mBufferVertList);

		// enable vertex coordinates
		glEnableClientState(GL_VERTEX_ARRAY);
		// enable colours
		glEnableClientState(GL_COLOR_ARRAY);

		// Each vertex has 3 components, is a GLfloat, stride is 0 (dont have to jump over memory to get to the next vertex)
		// and the vertices begin at the beginning of the buffer - (float *)NULL
		glVertexPointer(3, GL_FLOAT, 24, (float *)NULL);

		// the colours begin after the vertices, so from the start of the buffer
		// jump forward by numberOfVertices*3 (because each vertex has 3 components)
		glColorPointer(3, GL_FLOAT, 24, (float *)NULL + 3);
		
		// draw all the vertices, connected up as triangles
		glDrawArrays(GL_TRIANGLES, 0, mNumberOfVertices);
		// now draw the kids
		for each (Geometry child in m_children)
		{
			child.drawOpenGLVertexBufferObject();
		}

		glPopMatrix();
	}
};

#endif