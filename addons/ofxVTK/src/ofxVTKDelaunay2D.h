//
// ofxVTKDelaunay2D.h
// ofxVTKDelaunay2D
//
// Created by Roger on 22/06/10.
//
//
// Constrained Delaunay 2D
//
// Example from: http://www.vtk.org/Wiki/VTK/Examples/ConstrainedDelaunay2D
//
// Resources:
//	http://www.vtk.org/Wiki/Main_Page
//	http://www.vtk.org/doc/nightly/html/annotated.html
//	http://www.vtk.org/doc/nightly/html/classvtkDelaunay2D.html
//	http://www.vtk.org/doc/nightly/html/classvtkCellArray.html
//	http://www.vtk.org/doc/nightly/html/classvtkPolyData.html
// 
// Perform a 2D Delaunay triangulation respecting a specified boundary.
// This examples constructs a 10x10 grid of points.
// It then defines a polygon that uses the points in the grid.
// We want to triangulate all of the points except the region inside the boundary of the polygon.
// rectangular hole of We expect a square hole of size 4x3 in the resulting triangulated plane. 
//


#ifndef OFX_VTK_DELAUNAY_2D
#define OFX_VTK_DELAUNAY_2D

#define VTK_EXCLUDE_STRSTREAM_HEADERS

#include "ofMain.h"
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkPolyData.h>
#include <vtkPolygon.h>
#include <vtkSmartPointer.h>
#include <vtkDelaunay2D.h>
#include <vtkMath.h>
#include <vtkXMLPolyDataWriter.h>


class ofxVTKDelaunay2D {
	
public:
	
	ofxVTKDelaunay2D();
	
	void reset();
	
	int addPoint(ofPoint p);
	int addPoint(float x, float y, float z = 0.f);
	
	void setBoundaryPoint(int i);
	
	void update();
	
	void draw(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	
	void writeVptFile(const char *fileName);
	
	// This is the final mesh (triangle vertexes)
	vector<ofPoint>		triangles;
	
private:
	
	vtkSmartPointer<vtkPoints> points;			// points to triangulate
	vtkSmartPointer<vtkPolygon> boundary;		// the boundary
	vtkSmartPointer<vtkDelaunay2D> delaunay;	// the mesh

};


#endif


