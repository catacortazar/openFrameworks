/*
 *  ofxDXF.cpp
 *  Digitallique
 *
 *  Created by Roger on 22/05/10.
 *  Copyright 2010 Studio Avante. All rights reserved.
 *
 */

#include "ofxDXF.h"
#include "globals.h"


ofxDXF::ofxDXF()
{
}
ofxDXF::~ofxDXF()
{
}


/////////////////////////////////////////////////////////////////
//
// FEED WITH TRIANGLES
//

//
// Feed Joints
bool ofxDXF::feedJoints(vector<ofPoint> *origJoints, ofPoint *ref)
{
	// Clear current data
	joints.clear();
	// Save joints
	for (int n = 0 ; n < (*origJoints).size() ; n++)
	{
		// Get original tri
		ofPoint joint = (*origJoints)[n];
		// Send to reference point
		joint -= *ref;
		// Invert Y axis
		joint.y *= -1.0;
		// Save this joint
		joints.push_back(joint);
	}
}

//
// Feed Triangles
bool ofxDXF::feedTris(vector<Tri> *origTris, ofPoint *ref)
{
	// generate save time
	time_t now;
	time ( &now );
	strcpy(creationDate, &(ctime(&now)[4]));
	
	// Clear current data
	tris.clear();
	min = 0.0;
	max = 0.0;
	
	// Check all tris
	for (int n = 0 ; n < (*origTris).size() ; n++)
	{
		// Get original tri
		Tri tri = (*origTris)[n];
		// Process every vertex
		for (int v = 0 ; v < 3 ; v++)
		{
			// Send to reference point
			tri.vertex[v] -= *ref;
			// Invert Y axis
			tri.vertex[v].y *= -1.0;
			// Check bounds
			this->checkBounds(&(tri.vertex[v]));
		}
		// Save this tri
		tris.push_back(tri);
	}
	
	// make bounds
	bounds.x = min.x;
	bounds.y = min.y;
	bounds.z = min.z;
	bounds.width = (max.x - min.x);
	bounds.height = (max.y - min.y);
	bounds.depth = (max.z - min.z);
}

//
// Check if point is within min/max bounds
void ofxDXF::checkBounds(ofPoint *p)
{
	if (p->x < min.x)
		min.x = p->x;
	if (p->y < min.y)
		min.y = p->y;
	if (p->z < min.z)
		min.z = p->z;
	if (p->x > max.x)
		max.x = p->x;
	if (p->y > max.y)
		max.y = p->y;
	if (p->z > max.z)
		max.z = p->z;
}



/////////////////////////////////////////////////////////////////
//
// EXPORT DXF
//

//
// Create file
bool ofxDXF::exportDXF(const char *filename, const char *comm)
{
	// save comment
	if (comm != NULL)
		strcpy(comment, comm);
	else
		strcpy (comment, "");
	
	// Open file
	stream.open(filename,ios::trunc);
	if (stream.is_open() == false)
	{
		printf("ERROR creating DXF file [%s]\n",filename);
		return false;
	}
	
	// Write DXF file
	printf("Exporting DXF [%s]...\n",filename);
	this->makeDXF();
	
	// Ok!
	stream.close();
	printf("DXF export done!\n");
	return true;
}

//
// Make DXF File
//
// From:
//	http://local.wasp.uwa.edu.au/~pbourke/dataformats/dxf/min3d.html
//	http://usa.autodesk.com/adsk/servlet/item?siteID=123112&id=12272454&linkID=10809853
void ofxDXF::makeDXF()
{
	///////////////////////////////////
	//
	// HEADER SECTION
	//
	// Comments
	if (strlen(comment))
	{
		stream << "999\n";
		stream << comment << "\n";
	}
	stream << "999\n";
	stream << "ofxDXF by Roger Sodre\n";
	stream << "999\n";
	stream << "Export date: " << creationDate;
	//
	// File info
	stream << "0\n";
	stream << "SECTION\n";
	stream << "2\n";
	stream << "HEADER\n";
	stream << "9\n";
	stream << "$ACADVER\n";
	stream << "1\n";
	stream << "AC1024\n";
	//
	// Model extends
	stream << "9\n";
	stream << "$EXTMIN\n";
	stream << "10\n";
	stream << min.x << "\n";
	stream << "20\n";
	stream << min.y << "\n";
	stream << "30\n";
	stream << min.z << "\n";
	stream << "9\n";
	stream << "$EXTMAX\n";
	stream << "10\n";
	stream << max.x << "\n";
	stream << "20\n";
	stream << max.y << "\n";
	stream << "30\n";
	stream << max.z << "\n";
	//
	// End section
	stream << "0\n";
	stream << "ENDSEC\n";
	
	///////////////////////////////////
	//
	// TABLES SECTION
	//
	stream << "0\n";
	stream << "SECTION\n";
	stream << "2\n";
	stream << "TABLES\n";
	//
	// Line Type = Solid
	stream << "0\n";
	stream << "TABLE\n";
	stream << "2\n";
	stream << "LTYPE\n";
	stream << "70\n";
	stream << "1\n";
	stream << "0\n";
	stream << "LTYPE\n";
	stream << "2\n";
	stream << "CONTINUOUS\n";
	stream << "70\n";
	stream << "64\n";
	stream << "3\n";
	stream << "Solid Lines\n";
	stream << "72\n";
	stream << "65\n";
	stream << "73\n";
	stream << "0\n";
	stream << "40\n";
	stream << "0.000000\n";
	stream << "0\n";
	stream << "ENDTAB\n";
	//
	// Single Layer
	stream << "0\n";
	stream << "TABLE\n";
	stream << "2\n";
	stream << "LAYER\n";
	stream << "0\n";
	stream << "LAYER\n";
	stream << "2\n";		// layer name
	stream << "Layer1\n";	// layer name
	stream << "70\n";		// flags
	stream << "0\n";		// flags
	stream << "62\n";		// color number
	stream << "1\n";		// color number
	stream << "6\n";		// line style
	stream << "CONTINUOUS\n";
	stream << "0\n";
	stream << "ENDTAB\n";
	//
	// Style
	stream << "0\n";
	stream << "TABLE\n";
	stream << "2\n";
	stream << "STYLE\n";
	stream << "70\n";
	stream << "0\n";
	stream << "0\n";
	stream << "ENDTAB\n";
	//
	// End section
	stream << "0\n";
	stream << "ENDSEC\n";
	
	///////////////////////////////////
	//
	// BLOCKS SECTION
	//
	// Make empty
	stream << "0\n";
	stream << "SECTION\n";
	stream << "2\n";
	stream << "BLOCKS\n";
	stream << "0\n";
	stream << "ENDSEC\n";
	
	///////////////////////////////////
	//
	// ENTITIES SECTION (geometry)
	//
	stream << "0\n";
	stream << "SECTION\n";
	stream << "2\n";
	stream << "ENTITIES\n";
	//
	// TRIANGLES
	for (int n = 0 ; n < tris.size() ; n++)
	{
		Tri *tri = &tris[n];
		stream << "0\n";
		stream << "3DFACE\n";
		// Layer
		stream << "8\n";
		stream << "Layer1\n";
		// Color
		stream << "62\n";
		stream << "1\n";
		// Vertex 0
		stream << "10\n";
		stream << tri->vertex[0].x << "\n";
		stream << "20\n";
		stream << tri->vertex[0].y << "\n";
		stream << "30\n";
		stream << tri->vertex[0].z << "\n";
		// Vertex 1
		stream << "11\n";
		stream << tri->vertex[1].x << "\n";
		stream << "21\n";
		stream << tri->vertex[1].y << "\n";
		stream << "31\n";
		stream << tri->vertex[1].z << "\n";
		// Vertex 2
		stream << "12\n";
		stream << tri->vertex[2].x << "\n";
		stream << "22\n";
		stream << tri->vertex[2].y << "\n";
		stream << "32\n";
		stream << tri->vertex[2].z << "\n";
		// Vertex 3 = 2
		/*
		stream << "13\n";
		stream << tri->vertex[2].x << "\n";
		stream << "23\n";
		stream << tri->vertex[2].y << "\n";
		stream << "33\n";
		stream << tri->vertex[2].z << "\n";
		*/
	}
	//
	// End section
	stream << "0\n";
	stream << "ENDSEC\n";

	///////////////////////////////////
	//
	// FINITO!
	//
	stream << "0\n";
	stream << "EOF\n";
}



/////////////////////////////////////////////////////////////////
//
// EXPORT PYTHON
//

//
// Create file
bool ofxDXF::exportPython(const char *filename, const char *comm)
{
	// save comment
	if (comm != NULL)
		strcpy(comment, comm);
	else
		strcpy (comment, "");
	
	// Open file
	stream.open(filename,ios::trunc);
	if (stream.is_open() == false)
	{
		printf("ERROR creating Python file [%s]\n",filename);
		return false;
	}
	
	// Write Python file
	printf("Exporting Python [%s]...\n",filename);
	this->makePython();
	
	// Ok!
	stream.close();
	printf("Python export done!\n");
	return true;
}

//
// Make Python File
//
void ofxDXF::makePython()
{
	//
	// Comments
	if (strlen(comment))
		stream << "# " << comment << "\n";
	stream << "# Export date: " << creationDate;
	stream << "\n";
	
	//
	// Joints
	stream << "#\n";
	stream << "# Joints\n";
	stream << "#\n";
	stream << "# Cada joint contem 1 vertice\n";
	stream << "#\n";
	stream << "joints = [\n";
	for (int n = 0 ; n < joints.size() ; n++)
	{
		ofPoint *joint = &joints[n];
		stream << "\t[ \"" << jointNames[n] << "\", ["<<joint->x<<", "<<joint->y<<", "<<joint->z<<"] ]";
		if (n < (joints.size()-1))
			stream << ",";
		stream << "\n";
	}
	stream << "]\n";
	stream << "\n";
	
	//
	// Triangles
	stream << "#\n";
	stream << "# Triangulos\n";
	stream << "#\n";
	stream << "# Cada triangulo contem 3 vertices compostos de [ vertex, normal, texture ]\n";
	stream << "#\n";
	stream << "mesh = [\n";
	for (int n = 0 ; n < tris.size() ; n++)
	{
		Tri *tri = &tris[n];
		
		stream << "\t# Triangulo " << n << "\n";
		stream << "\t[\n";
		for (int v = 0 ; v < 3 ; v++)
		{
			stream << "\t\t[ ["<<tri->vertex[v].x<<", "<<tri->vertex[v].y<<", "<<tri->vertex[v].z<<"], ";
			stream << "["<<tri->normal.x<<", "<<tri->normal.y<<", "<<tri->normal.z<<"], ";
			stream << "["<<tri->tex[v].x<<", "<<tri->tex[v].y<<", "<<tri->tex[v].z<<"] ]";
			if (v < (3-1))
				stream << ",";
			stream << "\n";
		}
		stream << "\t]";
		if (n < (tris.size()-1))
			stream << ",";
		stream << "\n";
	}
	stream << "]\n";
	stream << "\n";
}











