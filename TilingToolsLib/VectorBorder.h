#pragma once
#ifndef		gmxVECTORBORDER_H
#define		gmxVECTORBORDER_H 
#include "stdafx.h"
#include "FileSystemFuncs.h"
#include "StringFuncs.h"

namespace gmx
{


class VectorOperations
{
public:
  static OGRGeometry*		    ReadAndTransformGeometry				(string vector_file, OGRSpatialReference  *p_tiling_srs);

 	static string				      GetVectorFileNameByRasterFileName	(string raster_file);
	static OGRPolygon*			  CreateOGRPolygonByOGREnvelope		(OGREnvelope &envelope);
	static OGREnvelope			  CombineOGREnvelopes					(OGREnvelope	&envp1, OGREnvelope	&envp2);
	static OGREnvelope			  InetersectOGREnvelopes				(OGREnvelope	&envp1, OGREnvelope	&envp2);
	static OGRLinearRing**		GetLinearRingsRef					(OGRGeometry	*p_ogr_geom, int &num_rings);
  static bool               CalcIntersectionBetweenLineAndPixelLineGeometry (int y_line, OGRGeometry *po_ogr_geom, int &num_points, int *&x);
  static bool               IsPointInsidePixelLineGeometry (OGRPoint point, OGRGeometry *po_ogr_geom);

  static bool               AddIntermediatePoints(OGRPolygon *p_polygon, int points_on_segmet=10);
  //static bool         ConvertOGRGeometryToArrayOfSegments (OGRGeometry *p_ogr_geom, int &num_segments, OGRLineString **pp_ls);

	VectorOperations	();
	~VectorOperations	();
	

public:
	bool						Intersects(OGREnvelope &envelope);
	OGRGeometry*				get_ogr_geometry_ref();
	OGREnvelope					GetEnvelope ();


protected:
	OGRGeometry					*p_ogr_geometry_;


protected:
	static OGRMultiPolygon*		ReadMultiPolygonFromOGRDataSource	(GDALDataset* poDS); 
	

};





}
#endif