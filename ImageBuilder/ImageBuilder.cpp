// ImageBuilder.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


using namespace std;
using namespace GMT;

int _tmain(int argc, _TCHAR* argv[])
{
	if (!LoadGDAL(argc,argv)) return 0;
	GDALAllRegister();

	int nRetCode = 0;

	if (argc == 1)
	{
		cout<<"Usage:\n";
		//cout<<"ImageTillingShp <ImageFile> <TilesFolder> <TileType> <EdgesTillingType (INTERSECTS|WITHIN|MARK)> <VectorFile> <BackGroundImage>\n";
		cout<<"ImageBuilder [-tiles TilesFolder] [-zoom ZoomNum] [-type TileType (jpg|png)] [-file FileName] [-border VectorFile | lon1,lat1,lon2,lat2,...] [-tab -wld -prj -xml -map -kml (georeference files)]\n";
		cout<<"Example1:\n";
		cout<<"ImageBuilder -tiles c:\\tiles\\irs -zoom 14 -file c:\\1.jpg -border 36.841,55.379,37.2666,55.389,37.517,55.183 -tab -map -xml -wld -kml"<<endl;
		cout<<"Example2:\n";
		cout<<"ImageBuilder -tiles c:\\tiles\\irs -zoom 14 -file c:\\1.jpg -border c:\\1.shp -tab -map -xml -wld -kml"<<endl;

		return 0;
	}

	wstring strTileType		=  ReadConsoleParameter(L"-type",argc,argv);		
	//strImageType =  ReadConsoleParameter(L"-imageType",argc,argv);
	wstring strTilesFolder		=  ReadConsoleParameter(L"-tiles",argc,argv);
	//cout<<"AAa"<<strTilesFolder<<endl;
	//strPoints			=  ReadConsoleParameter(L"-points",argc,argv);
	wstring strImageFile		=  ReadConsoleParameter(L"-file",argc,argv);
	wstring strZoom				=  ReadConsoleParameter(L"-zoom",argc,argv);
	wstring strVectorFile		=  ReadConsoleParameter(L"-border",argc,argv);

	BOOL	bWldFile = ( ReadConsoleParameter(L"-wld",argc,argv,TRUE) != L"");
	BOOL	bTabFile = ( ReadConsoleParameter(L"-tab",argc,argv,TRUE) != L"");
	BOOL	bPrjFile = ( ReadConsoleParameter(L"-prj",argc,argv,TRUE) != L"");
	BOOL	bMapFile = ( ReadConsoleParameter(L"-map",argc,argv,TRUE) != L"");
	BOOL	bKmlFile = ( ReadConsoleParameter(L"-kml",argc,argv,TRUE) != L"");
	BOOL	bAuxFile = ( ReadConsoleParameter(L"-xml",argc,argv,TRUE) != L"");
	
	wstring	strWMS		=  ReadConsoleParameter(L"-wms",argc,argv,1);
	wstring	strWidth	=  ReadConsoleParameter(L"-width",argc,argv);
	wstring	strHeight	=  ReadConsoleParameter(L"-height",argc,argv);
	


	/*
	strImageFile		=	L"d:\\GeoMixerTools\\test_data\\result_z14.jpg";
	strZoom				=	L"14";
	strVectorFile		=	L"39.662,54.638,39.686,54.638,39.686,54.624,39.662,54.624";
	strTilesFolder		=	L"d:\\GeoMixerTools\\test_data\\copy";
	bWldFile = (bTabFile = (bMapFile = (bKmlFile = (bPrjFile = (bAuxFile = 1)))));
	*/

		

	/*
	//strTilesFolder = L"D:\\GeoMixerTools\\test_data\\copy";
	
	strTilesFolder = L"D:\\GeoMixerTools\\test_data\\image_cut_merc_tiles";
	strImageFile	= L"D:\\GeoMixerTools\\test_data\\17_.jpg";
	//strWMS = L"WMS";
	strZoom	= L"17";
	//strWidth = L"1000";
	//strHeight = L"1000";
	strVectorFile = L"39.662,54.638,39.686,54.638,39.686,54.624,39.662,54.624";
	bWldFile = (bTabFile = (bMapFile = (bKmlFile = (bPrjFile = (bAuxFile = 1)))));
	*/

	int			nZoom;
	double		dResolution;
	wstring		strPoints;


	if (strTilesFolder == L"")
	{
		cout<<"Error: missing \"-tiles\" parameter"<<endl;
		return 0;
	}

	if (strImageFile == L"")
	{
		cout<<"Error: missing \"-file\" parameter"<<endl;
		return 0;
	}

	if (strZoom == L"")
	{
		cout<<"Error: missing \"-zoom\" parameter"<<endl;
		return 0;
	}

	if (strVectorFile == L"")
	{
		cout<<"Error: missing \"-border\" parameter"<<endl;
		return 0;
	}

	
		//*/


		//strTilesFolder 	=	"I:\\GeoMixerTools\\test_data\\copy";
		//strZoom			= L"14";
		//strImageFile	= L"I:\\GeoMixerTools\\test_data\\result_z14.jpg";
		//strVectorFile	= L"I:\\GeoMixerTools\\test_data\\test_border\\test_border.tab";
		/*
		strTileType		=	"png";
		strTilesFolder 	=	"I:\\moscow_166";
		//strImageType	=	"irs";
		//strVectorFile		=	"\\\\192.168.4.28\\vectors\\moscow_ikonos_mercator.mif";
		//strVectorFile		=	"i:\\moscow_ikonos_mercator.mif";
		strZoom = L"14";
		strImageFile		=	"I:\\moscow_166\\z14.png";
		//strVectorFile			= L"I:\\moscow_166\\moscow_166.tab";
		//strImageType = L"jpeg";
		//strPoints		= L"4145350,7447760,4231469,7499316,4232159,7461749";

		//strPoints		= L"4195350,7447760,4201469,7479316,4222159,7461749";
		strPoints		= L"37.51,55.80,37.72,55.80,37.72,55.69,37.51,55.69";
		
		
		bWldFile = TRUE;
		bTabFile = TRUE;
		bPrjFile = TRUE;
		bMapFile = TRUE;
		bKmlFile = TRUE;

		*/


		if (!FileExists(strTilesFolder))
		{
			wcout<<L"Error: parameter -tiles: folder "<<strTilesFolder<<L" doesn't exist"<<endl;
			return 0;
		}

		if (strImageFile==L"") 
		{
			wcout<<L"Error: parameter -file: you must specify file name"<<endl;
			return 0;
		}
			
		if (strZoom==L"")
		{
			wcout<<L"Error: parameter -zoom: you must specify zoom"<<endl;
			return 0;
		}
		
		nZoom = (int)(_wtof(strZoom.data()));
		//dResolution = TileName::CalcResolutionByZoom((int)(_wtof(strZoom.data())));
		
		
		
		OGRRegisterAll();
		VectorBorder oVectorBorder;

		/*
		GeoReference oGeoRef;
		oGeoRef.SetDatum(GeoReference::WGS84);
		oGeoRef.SetProjection(GeoReference::MERCATOR);

		if (strVectorFile==L"")
		{
			cout<<"Error: parameter -border: you must specify border"<<endl;
			return 0;
		}

		int n1 = max(strVectorFile.find(L'\\'),strVectorFile.find(L'/'));
		int n2 = max(max(MakeLower(strVectorFile).find(L".tab"),MakeLower(strVectorFile).find(L".shp")),MakeLower(strVectorFile).find(L".mif"));
		if ((!FileExists(strVectorFile))&&(n1<0)&&(n2<0)) strPoints = strVectorFile;
		
		//cout<<1111<<endl;
		if (strPoints!=L"")
		{
			double	dblPoints[100];
			int		nNumOfPoints = 0;
			wstring	strNumber;
			while (strPoints.size()!=0)
			{
				if (strPoints[0]!=',')
				{
					strNumber+=strPoints[0];
					strPoints = strPoints.erase(0,1);

				}
				else
				{
					strPoints = strPoints.erase(0,1);
					dblPoints[nNumOfPoints]=_wtof(strNumber.data());
					nNumOfPoints++;
					strNumber=L"";
				}
			}
			dblPoints[nNumOfPoints]=_wtof(strNumber.data());
			nNumOfPoints++;
			if (strWMS==L"")
			{
				for (int i=0;i<nNumOfPoints/2;i++)
				{
					oGeoRef.ToXY(dblPoints[2*i],dblPoints[2*i+1],dblPoints[2*i],dblPoints[2*i+1]);
				}
	  		}
			oVectorBorder.InitByPoints(nNumOfPoints/2,dblPoints);
		}
		else
		{
			if (!VectorFile::OpenAndCreatePolygonInMercator(strVectorFile,oVectorBorder))
			{
				wcout<<L"Error: "<<strVectorFile<<L": can't read border polygon"<<endl;
				return FALSE;
			}
		}	
		//cout<<1111<<endl;
		
		if (strWMS!=L"")
		{
			if (strWidth == L"" || strHeight == L"")
			{
				cout<<"Erros: you must specify image size: -width and -height"<<endl;
				return 0;
			}
			int width	= (int)_wtof(strWidth.data());
			int height	= (int)_wtof(strHeight.data());
			RasterBuffer oBuffer;
			
			KosmosnimkiTileName oTileName(strTilesFolder,L"");
			if (!CombineTilesIntoBuffer(&oTileName,nZoom,oVectorBorder.GetEnvelope(),oBuffer)) return 0;
			
			//ToDo
			//oBuffer.ResizeAndConvertToRGB(width,height);
			//oBuffer.SaveToFile(strImageFile);
			return 1;
		}
		

		
		
		
		
		
		
		
		OGREnvelope oEnvelope  = oVectorBorder.GetEnvelope();
		
		OGREnvelope pixelEnvelope = oTileName.CalcPixelEnvelope(oEnvelope,nZoom);
		pixelEnvelope.MinX -=(((int)pixelEnvelope.MinX%oTileName.TILE_SIZE)%8);
		pixelEnvelope.MaxX += min(8-(((int)pixelEnvelope.MaxX%oTileName.TILE_SIZE)%8),oTileName.TILE_SIZE-((int)pixelEnvelope.MaxX%oTileName.TILE_SIZE) );
		pixelEnvelope.MinY -=(((int)pixelEnvelope.MinY%oTileName.TILE_SIZE)%8);
		pixelEnvelope.MaxY += min(8-(((int)pixelEnvelope.MaxY%oTileName.TILE_SIZE)%8),oTileName.TILE_SIZE-((int)pixelEnvelope.MaxY%oTileName.TILE_SIZE) );
	
		double ULX = - 20037508.342789243076588408880691,ULY = 20037508.342789243076588408880691;
		//oTileName.CalcCoordSysULxy(ULX,ULY);
		dResolution = oTileName.CalcResolutionByZoom(nZoom);
		oEnvelope.MinX = ULX + pixelEnvelope.MinX*dResolution;
		oEnvelope.MaxY = ULY - pixelEnvelope.MinY*dResolution;
		oEnvelope.MaxX = ULX + pixelEnvelope.MaxX*dResolution;
		oEnvelope.MinY = ULY - pixelEnvelope.MaxY*dResolution;

		OGREnvelope oEnvelopeLatLon;
		oGeoRef.ToLatLon(oEnvelope.MinX,oEnvelope.MaxY,oEnvelopeLatLon.MinX,oEnvelopeLatLon.MaxY);
		oGeoRef.ToLatLon(oEnvelope.MaxX,oEnvelope.MinY,oEnvelopeLatLon.MaxX,oEnvelopeLatLon.MinY);


		//AdjustBoundsForResolution(oEnvelope,dResolution*8);
		if (bWldFile)
		{
			wstring strWldExt = L".jgw";
			if ((MakeLower(strTileType) == L"png")||(MakeLower(strTileType) == L".png")) strWldExt = L".pgw";
			wstring strWldFile = RemoveExtension(strImageFile)+strWldExt;
			oGeoRef.WriteWldFile(oEnvelope.MinX,oEnvelope.MaxY,dResolution,strWldFile);
			
		}
		if (bTabFile)
		{
			wstring strTabFile = RemoveExtension(strImageFile)+L".tab";
			int nWidth	= floor(0.5+((oEnvelope.MaxX-oEnvelope.MinX)/dResolution));
			int nHeight = floor(0.5+((oEnvelope.MaxY-oEnvelope.MinY)/dResolution));
			oGeoRef.WriteTabFile(RemovePath(strImageFile),nWidth,nHeight,oEnvelope.MinX,oEnvelope.MaxY,dResolution,strTabFile);
			
		}
		if (bPrjFile)
		{
			wstring strPrjFile = RemoveExtension(strImageFile) + L".prj";
		}
		if (bAuxFile)
		{
			wstring strAuxFile = RemoveExtension(strImageFile) + L".jpg.aux.xml";
			oGeoRef.WriteAuxFile(strAuxFile);
		}
		if (bMapFile)
		{
			wstring strMapFile = RemoveExtension(strImageFile) + L".map";
			int nWidth	= floor(0.5+((oEnvelope.MaxX-oEnvelope.MinX)/dResolution));
			int nHeight = floor(0.5+((oEnvelope.MaxY-oEnvelope.MinY)/dResolution));
			oGeoRef.WriteMapFile(RemovePath(strImageFile),nWidth,nHeight,oEnvelope.MinX,oEnvelope.MaxY,dResolution,strMapFile);
		}

		if (bKmlFile)
		{
			wstring strKmlFile = RemoveExtension(strImageFile) + L".kml";
			oGeoRef.WriteKmlFile(strKmlFile,RemovePath(RemoveExtension(strImageFile)),RemovePath(strImageFile),oEnvelopeLatLon.MaxY,oEnvelopeLatLon.MinX,oEnvelopeLatLon.MinY,oEnvelopeLatLon.MaxX);
		}

		*/
	
	
	return 1;
}
