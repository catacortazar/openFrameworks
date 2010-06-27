//
// ofxVTKDelaunay2D.cpp
// ofxVTKDelaunay2D
//
// Created by Roger on 22/06/10.
// Copyright 2010 Studio Avante. All rights reserved.
//
//

#include "ofxVTKDelaunay2D.h"

ofxVTKDelaunay2D::ofxVTKDelaunay2D() {
	points = vtkSmartPointer<vtkPoints>::New();
	boundary = vtkSmartPointer<vtkPolygon>::New();
	delaunay = vtkSmartPointer<vtkDelaunay2D>::New();
}

// Clean everything
//----------------------------------------
void ofxVTKDelaunay2D::reset(){
	points->Reset();
	boundary->GetPointIds()->Reset();
	triangles.clear();
}

// Add a point to triangulate
// Returns the point id / index in the points array
//----------------------------------------
int ofxVTKDelaunay2D::addPoint(ofPoint p){
	return (int) points->InsertNextPoint(p.v);
}
int ofxVTKDelaunay2D::addPoint(float x, float y, float z){
	return (int) points->InsertNextPoint((double)x, (double)y, (double)z);
}

// Set what points will make the boundary (optional)
// If added Clockwise: will define a boundary
// If added Counter-Clockwise: will define a hole
// This order is inverted in VTK because in openFrameworks we invert the Y-Axis
// 'i' is the index of the point, as returned by addPoint()
//----------------------------------------
void ofxVTKDelaunay2D::setBoundaryPoint(int i){
	boundary->GetPointIds()->InsertNextId((vtkIdType)i);
}

// Make mesh from points
//----------------------------------------
void ofxVTKDelaunay2D::update(){
	
	// Store points
	vtkSmartPointer<vtkPolyData> pointsPolyData = vtkSmartPointer<vtkPolyData>::New();
	pointsPolyData->SetPoints(points);
	
	// Create a cell array to store the boundary polygon in
	vtkSmartPointer<vtkCellArray> boundaryCellArray = vtkSmartPointer<vtkCellArray>::New();
	boundaryCellArray->InsertNextCell(boundary); 
	
	// Create a polydata to store the boundary.
	// The points must be the same as the points we will triangulate.
	vtkSmartPointer<vtkPolyData> boundaryPolyData = vtkSmartPointer<vtkPolyData>::New();
	boundaryPolyData->SetPoints(pointsPolyData->GetPoints());
	boundaryPolyData->SetPolys(boundaryCellArray);
	
	// Triangulate the grid points
	delaunay->SetInput ( pointsPolyData );
	delaunay->SetSource ( boundaryPolyData );
	delaunay->Update();
	unsigned long error =  delaunay->GetErrorCode();
	//printf("DELAUNAY ERROR [%d]\n",error);

	// mesh data pointers
	vtkPolyData *data = delaunay->GetOutput();		// vtk mesh data
	vtkCellArray *cells = data->GetPolys();			// each cell is a triangle
	
	// for every cell/triangle...
	for (vtkIdType c = 0 ; c < cells->GetNumberOfCells() ; c++)
	{
		vtkCell *cell = data->GetCell(c);
		// for each point in the triangle...
		for(vtkIdType p = 0; p < cell->GetNumberOfPoints(); p++)
		{
			// extract vertex
			double point[3];
			vtkIdType i = cell->GetPointId(p);		// index in the original data
			data->GetPoint(i,point);
			// push vertexes sequentially
			ofPoint ofpt = ofPoint((float)point[0],(float)point[1],(float)point[2]);
			triangles.push_back(ofpt);
			//printf("ofxVTKDelaunay2D:: CELL [%d] p[%d] xyz[%.2f/%.2f/%.2f]\n",c,i,ofpt.x,ofpt.y,ofpt.z);
		}
	}
}

// draw mesh as triangles
//----------------------------------------
void ofxVTKDelaunay2D::draw(float x, float y, float z){
	for (int n = 0 ; n < triangles.size() ; n+=3)
		ofTriangle(triangles[n], triangles[n+1], triangles[n+2]);
}

// Export mesh to a VTP file
//----------------------------------------
void ofxVTKDelaunay2D::writeVptFile(const char *fileName)
{
	vtkSmartPointer<vtkXMLPolyDataWriter> writer = vtkSmartPointer<vtkXMLPolyDataWriter>::New();
	writer->SetFileName ( fileName );
	writer->SetInput ( delaunay->GetOutput() );
	writer->Write();
}

