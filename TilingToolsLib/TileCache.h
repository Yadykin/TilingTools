#ifndef GMX_TILE_CACHE_H
#define GMX_TILE_CACHE_H
#pragma once
#include	"stdafx.h"


namespace gmx
{

class TileCache
{
public:
  static const __int64 DEFAULT_CACHE_MAX_SIZE = 800000000;
	TileCache(__int64 max_size=0);
	bool		AddTile(int z, int x, int y, BYTE *p_data, unsigned int size);
	bool		GetTile(int z, int x, int y, BYTE *&p_data, unsigned int &size);
  bool    FindTile(int z, int x, int y);
  unsigned __int64 cache_size();
	~TileCache(void);

protected:
	map<string,BYTE*>			tile_data_map_;
	map<string,unsigned int>	tile_size_map_;
  __int64 cache_size_;
  __int64 cache_max_size_;


};


}

#endif