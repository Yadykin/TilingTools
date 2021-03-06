#include "StdAfx.h"
#include "TileName.h"


namespace gmx
{


StandardTileName::StandardTileName (string base_folder, string str_template)
{
	if (!ValidateTemplate(str_template)) return;
	if (!GMXFileSys::FileExists(base_folder)) return;

  if (!TileName::TileTypeByExtension(GMXFileSys::GetExtension(str_template),tile_type_))
  {
    cout<<"ERROR: can't parse tile type from input template: "<<str_template<<endl;
    return;
  }

	base_folder_	= base_folder;
	zxy_pos_[0] = (zxy_pos_[1] = (zxy_pos_[2] = 0));

	if (str_template[0] == L'/' || str_template[0] == L'\\') 	str_template = str_template.substr(1,str_template.length()-1);
	GMXString::ReplaceAll(str_template,"\\","/");
	str_template_ = str_template;
		
	//ReplaceAll(strTemplate,"\\","\\\\");
	int n = 0;
	int num = 2;
	//int k;
	while (str_template.find(L'{',n)!=std::string::npos)
	{
		string str = str_template.substr(str_template.find(L'{',n),str_template.find(L'}',n)-str_template.find(L'{',n)+1);
		if (str == "{z}")
			zxy_pos_[0] = (zxy_pos_[0] == 0) ? num : zxy_pos_[0];
		else if (str == "{x}")
			zxy_pos_[1] = (zxy_pos_[1] == 0) ? num : zxy_pos_[1];
		else if (str == "{y}")
			zxy_pos_[2] = (zxy_pos_[2] == 0) ? num : zxy_pos_[2];
		num++;
		n = str_template.find(L'}',n) + 1;
	}

	GMXString::ReplaceAll(str_template,"{z}","(\\d+)");
	GMXString::ReplaceAll(str_template,"{x}","(\\d+)");
	GMXString::ReplaceAll(str_template,"{y}","(\\d+)");
	rx_template_ = ("(.*[\\/])" + str_template) + "(.*)";
  //rx_template_ = str_template;
}

bool	StandardTileName::ValidateTemplate	(string str_template)
{

	if (str_template.find("{z}",0)==string::npos)
	{
		//cout<<"Error: bad tile name template: missing {z}"<<endl;
		return FALSE;
	}
	if (str_template.find("{x}",0)==string::npos)
	{
		//cout<<"Error: bad tile name template: missing {x}"<<endl;
		return FALSE;
	}
	if (str_template.find("{y}",0)==string::npos) 
	{
		//cout<<"Error: bad tile name template: missing {y}"<<endl;
		return FALSE;
	}

	if (str_template.find(".",0)==string::npos) 
	{
		//cout<<"Error: bad tile name template: missing extension"<<endl;
		return FALSE;
	}
		
  TileType tt;
  if (!TileName::TileTypeByExtension(GMXFileSys::GetExtension(str_template),tt))
  {
		cout<<"ERROR: not valid tile type in template: "<<str_template<<endl;
		return FALSE;
	}
	return TRUE;
}

string	StandardTileName::GetTileName (int zoom, int nX, int nY)
{
	string tile_name = str_template_;
	GMXString::ReplaceAll(tile_name,"{z}",GMXString::ConvertIntToString(zoom));
	GMXString::ReplaceAll(tile_name,"{x}",GMXString::ConvertIntToString(nX));
	GMXString::ReplaceAll(tile_name,"{y}",GMXString::ConvertIntToString(nY));
	return tile_name;
}

bool StandardTileName::ExtractXYZFromTileName (string tile_name, int &z, int &x, int &y)
{
	if (!regex_match(tile_name,rx_template_)) return FALSE;
	match_results<string::const_iterator> mr;
	regex_search(tile_name, mr, rx_template_);
  for (int i=0;i<mr.size();i++)
  {
    string str = mr[i].str();
    i=i;
  }

	if ((mr.size()<=zxy_pos_[0])||(mr.size()<=zxy_pos_[1])||(mr.size()<=zxy_pos_[2])) return FALSE;
	z = (int)atof(mr[zxy_pos_[0]].str().c_str());
	x = (int)atof(mr[zxy_pos_[1]].str().c_str());
	y = (int)atof(mr[zxy_pos_[2]].str().c_str());
		
	return TRUE;
}


bool StandardTileName::CreateFolder (int zoom, int x, int y)
{
	string tile_name = GetTileName(zoom,x,y);
	int n = 0;
	while (tile_name.find("/",n)!=std::string::npos)
	{
		if (!GMXFileSys::FileExists(GMXFileSys::GetAbsolutePath(base_folder_,tile_name.substr(0,tile_name.find("/",n)))))
			if (!GMXFileSys::CreateDir(GMXFileSys::GetAbsolutePath(base_folder_,tile_name.substr(0,tile_name.find("/",n))).c_str())) return FALSE;	
		n = (tile_name.find("/",n)) + 1;
	}
	return TRUE;
}


bool ESRITileName::CreateFolder (int zoom, int x, int y)
{
	string tile_name = GetTileName(zoom,x,y);
	int n = 0;
	while (tile_name.find("/",n)!=std::string::npos)
	{
		if (!GMXFileSys::FileExists(GMXFileSys::GetAbsolutePath(base_folder_,tile_name.substr(0,tile_name.find("/",n)))))
			if (!GMXFileSys::CreateDir(GMXFileSys::GetAbsolutePath(base_folder_,tile_name.substr(0,tile_name.find("/",n))).c_str())) return FALSE;	
		n = (tile_name.find("/",n)) + 1;
	}
	return TRUE;
}


bool	ESRITileName::ValidateTemplate	(string str_template)
{
  str_template = GMXString::MakeLower(str_template);
	if (str_template.find("{l}",0)==string::npos)
	{
		//cout<<"Error: bad tile name template: missing {L}"<<endl;
		return FALSE;
	}
	if (str_template.find("{c}",0)==string::npos)
	{
		//cout<<"Error: bad tile name template: missing {C}"<<endl;
		return FALSE;
	}
	if (str_template.find("{r}",0)==string::npos) 
	{
		//cout<<"Error: bad tile name template: missing {R}"<<endl;
		return FALSE;
	}

	if (str_template.find(".",0)==string::npos) 
	{
		//cout<<"Error: bad tile name template: missing extension"<<endl;
		return FALSE;
	}
		
	TileType tt;
  if (!TileName::TileTypeByExtension(GMXFileSys::GetExtension(str_template),tt))
  {
		cout<<"ERROR: not valid tile type in template: "<<str_template<<endl;
		return FALSE;
	}
	return TRUE;
}


ESRITileName::ESRITileName (string base_folder, string str_template)
{
  GMXString::ReplaceAll(str_template,"\\","/");
  GMXString::ReplaceAll(str_template,"{l}","{L}");
  GMXString::ReplaceAll(str_template,"{r}","{R}");
  GMXString::ReplaceAll(str_template,"{c}","{C}");

	if (!ValidateTemplate(str_template)) return;
	if (!GMXFileSys::FileExists(base_folder)) return;

	if (!TileName::TileTypeByExtension(GMXFileSys::GetExtension(str_template),tile_type_))
  {
    cout<<"ERROR: can't parse tile type from input template: "<<str_template<<endl;
    return;
  }
	
  base_folder_	= base_folder;
	
	if (str_template[0] == L'/') 	str_template = str_template.substr(1,str_template.length()-1);
  str_template_ = str_template;
		
  GMXString::ReplaceAll(str_template,"{L}","(L\\d{2})");
  GMXString::ReplaceAll(str_template,"{C}","(C[A-Fa-f0-9]{8,8})");
  GMXString::ReplaceAll(str_template,"{R}","(R[A-Fa-f0-9]{8,8})");
	rx_template_ = ("(.*)" + str_template) + "(.*)";
}


string	ESRITileName::GetTileName (int zoom, int nX, int nY)
{
	string tile_name = str_template_;
	GMXString::ReplaceAll(tile_name,"{L}","L"+GMXString::ConvertIntToString(zoom,FALSE,2));
	GMXString::ReplaceAll(tile_name,"{C}","C"+GMXString::ConvertIntToString(nX,TRUE,8));
	GMXString::ReplaceAll(tile_name,"{R}","R"+GMXString::ConvertIntToString(nY,TRUE,8));

  return tile_name;
}


bool ESRITileName::ExtractXYZFromTileName (string tile_name, int &z, int &x, int &y)
{
	if (!regex_match(tile_name,rx_template_)) return FALSE;
	match_results<string::const_iterator> mr;
	regex_search(tile_name, mr, rx_template_);
  
  for (int i=0;i<mr.size();i++)
  {
    if (mr[i].str()[0]=='L') z = (int)atof(mr[i].str().c_str());
    if (mr[i].str()[0]=='R') y = (int)atof(mr[i].str().c_str());
    if (mr[i].str()[0]=='C') x = (int)atof(mr[i].str().c_str());
  }

 	return TRUE;
}


KosmosnimkiTileName::KosmosnimkiTileName (string tiles_folder, TileType tile_type)
{
	base_folder_	= tiles_folder;
	tile_type_		= tile_type;
}


	
string	KosmosnimkiTileName::GetTileName (int zoom, int x, int y)
{
	if (zoom>0)
	{
		x = x-(1<<(zoom-1));
		y = (1<<(zoom-1))-y-1;
	}
	sprintf(buf,"%d\\%d\\%d_%d_%d.%s",zoom,x,zoom,x,y,this->ExtensionByTileType(tile_type_).c_str());
	return buf;
}

bool KosmosnimkiTileName::ExtractXYZFromTileName (string tile_name, int &z, int &x, int &y)
{
	tile_name = GMXFileSys::RemovePath(tile_name);
	tile_name = GMXFileSys::RemoveExtension(tile_name);
	int k;

	regex pattern("[0-9]{1,2}_-{0,1}[0-9]{1,7}_-{0,1}[0-9]{1,7}");
	//wregex pattern("(\d+)_-?(\d+)_-{0,1}[0-9]{1,7}");

	if (!regex_match(tile_name,pattern)) return FALSE;

	z = (int)atof(tile_name.substr(0,tile_name.find('_')).c_str());
	tile_name = tile_name.substr(tile_name.find('_')+1);
		
	x = (int)atof(tile_name.substr(0,tile_name.find('_')).c_str());
	y = (int)atof(tile_name.substr(tile_name.find('_')+1).c_str());

	if (z>0)
	{
		x+=(1<<(z-1));
		y=(1<<(z-1))-y-1;
	}

	return TRUE;
}


bool KosmosnimkiTileName::CreateFolder (int zoom, int x, int y)
{
	if (zoom>0)
	{
		x = x-(1<<(zoom-1));
		y = (1<<(zoom-1))-y-1;
	}

	sprintf(buf,"%d",zoom);
	string str = GMXFileSys::GetAbsolutePath(base_folder_, buf);
	if (!GMXFileSys::FileExists(str))
	{
		if (!GMXFileSys::CreateDir(str.c_str())) return FALSE;	
	}

	sprintf(buf,"%d",x);
	str = GMXFileSys::GetAbsolutePath(str,buf);
	if (!GMXFileSys::FileExists(str))
	{
		if (!GMXFileSys::CreateDir(str.c_str())) return FALSE;	
	}
		
	return TRUE;
}


}