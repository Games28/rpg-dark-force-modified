#include "Map.h"

bool Map::mapHasWallAt(float x, float y)
{
	if (x < 0 || x >= MAP_NUM_COLS_X * TILE_SIZE || y < 0 || y >= MAP_NUM_ROWS_Y * TILE_SIZE) {
		return true;
	}
	int mapGridIndexX = (int)floor(x / TILE_SIZE);
	int mapGridIndexY = (int)floor(y / TILE_SIZE);
	//return Floatheightmap[mapGridIndexY][mapGridIndexX] != 0;
	float height = 0;
	for (int i = 0; i < (int)fMaps.size(); i++)
	{
		height += fMaps[i][mapGridIndexY * MapX + mapGridIndexX];
	}
	return height != 0;
}

// Joseph21 - a couple of convenience functions to safely compare floats using an error margin
// Note that prototypes for these functions are not needed, since they are not part of class Map

#define F_EPSILON   0.0001f    // error margin for comparing floats

bool floatEqual(    float a, float b ) { return abs(a - b) < F_EPSILON; }
bool floatGrtEqual( float a, float b ) { return  floatEqual( a, b ) || a > b; }
bool floatLssEqual( float a, float b ) { return  floatEqual( a, b ) || a < b; }
bool floatGreater(  float a, float b ) { return !floatEqual( a, b ) && a > b; }
bool floatLess(     float a, float b ) { return !floatEqual( a, b ) && a < b; }

bool Map::isInsideMap(float x, float y)
{
    // the boundaries of the map are considered to be part of the map
    return floatGrtEqual( x, 0.0f ) && floatLssEqual( x, MAP_NUM_COLS_X * TILE_SIZE ) &&
           floatGrtEqual( y, 0.0f ) && floatLssEqual( y, MAP_NUM_ROWS_Y * TILE_SIZE );
}

bool Map::isOnMapBoundary(float x, float y)
{
    return floatEqual( x, 0.0f ) || floatEqual( x, MAP_NUM_COLS_X * TILE_SIZE ) ||
           floatEqual( y, 0.0f ) || floatEqual( y, MAP_NUM_ROWS_Y * TILE_SIZE );
}

bool Map::isOutSideMap(float x, float y)
{
	return !isInsideMap( x, y );
}

void Map::renderMapGrid(olc::PixelGameEngine* PGEptr)
{
    // fill background for minimap
    PGEptr->FillRect( 0, 0, (int)MAP_NUM_COLS_X * TILE_SIZE * MINIMAP_SCALE_FACTOR, (int)MAP_NUM_ROWS_Y * TILE_SIZE * MINIMAP_SCALE_FACTOR, olc::DARK_YELLOW );

    // draw each tile
	for (int i = 0; i < MAP_NUM_ROWS_Y; i++) {
		for (int j = 0; j < MAP_NUM_COLS_X; j++) {
			int tileX = j * TILE_SIZE;
			int tileY = i * TILE_SIZE;
            // colour different for different heights
			olc::Pixel p;
			switch (heightmap[i][j]) {
			    case 0:  p = olc::VERY_DARK_GREEN; break;
			    case 1:  p = olc::WHITE;           break;
			    case 2:  p = olc::GREY;            break;
			    case 3:  p = olc::DARK_GREY;       break;
			    case 4:  p = olc::VERY_DARK_GREY;  break;
			    default: p = olc::BLUE;  break;
			}
            // render this thile
			PGEptr->FillRect(
				tileX * MINIMAP_SCALE_FACTOR     + 1,
				tileY * MINIMAP_SCALE_FACTOR     + 1,
				TILE_SIZE * MINIMAP_SCALE_FACTOR - 1,
				TILE_SIZE * MINIMAP_SCALE_FACTOR - 1,
				p
			);
		}
	}
}

int Map::getFromHeightMap( int x, int y )
{
    // Joseph21 - if coordinate is out of bounds, return 0, else return value of height map
    if (x < 0 || x >= MAP_NUM_COLS_X || y < 0 || y >= MAP_NUM_ROWS_Y)
        return 0;
    else
        return heightmap[y][x];
}

void Map::setTextures()
{
	
		
	
	
}

int Map::getTextureMap(int i, int j, int Height)
{
	int textureid = 0;

	switch (Height)
	{
	case 1:
		textureid = texturemapOne[i][j];
		break;
	case 2:
		textureid = texturemapTwo[i][j];
		break;
	case 3:
		textureid = texturemapThree[i][j];
		break;
	case 4:
		textureid = texturemapFour[i][j];
		break;
	}
	return textureid;
}

int Map::gettexture(int x, int y,int layer)
{
	
	int texture = 0;
	int indexX = x;
	int indexY = y;
	int offsetX = 1;
	int offsetY = 1;
	bool xzero = false;
	bool yzero = false;
	
	if (indexX < 0 || indexX >= MAP_NUM_COLS_X || indexY < 0 || indexY >= MAP_NUM_ROWS_Y)
	{
		return texture;
	}
	else
	{
		
		switch (layer)
		{
		case 1:
			texture = iTextures[0][indexY * MAP_NUM_COLS_X + indexX];
			break;
		case 2:
			texture = iTextures[1][indexY * MAP_NUM_COLS_X + indexX];
			break;
		case 3:
			texture = iTextures[2][indexY * MAP_NUM_COLS_X + indexX];
			break;
		}
		
		
		
		return texture;
	}
}

float Map::FloatgetfromHeightmap(int x, int y, int level)
{
	//new
	
	if (x < 0 || x >= MAP_NUM_COLS_X || y < 0 || y >= MAP_NUM_ROWS_Y)
	{
		return 0;
	
	}
	else
	{
		return fMaps[level ][y * MapX + x];
		
		
	 
	}

}

float Map::floatheightmap(int x, int y)
{
	float result = -1.0f;
	if (x < 0 || x >= MAP_NUM_COLS_X || y < 0 || y >= MAP_NUM_ROWS_Y)
	{
		return 0;

	}
	else
	{
		result = 0;
		for (int i = 0; i < (int)fMaps.size(); i++)
		{

			result += fMaps[i][y * MapX + x];
		}

		return result;
	}
}

void Map::addMapLayer( const std::string& sUserMap)
{
	if (MapX * MapY != (int)sUserMap.length())
	{
		std::cout << "ERROR: InitMap() -->  mismatch between map dimensions and length of map string" << std::endl;
	}
	
	std::string sMap = sUserMap;

	float* fMap = new float[MapX * MapY];

	for (int y = 0; y < MapY; y++)
	{
		for (int x = 0; x < MapX; x++)
		{
			switch (sMap[y * MapX + x]) {
			case GRND_FLOOR: fMap[y * MapX + x] = 0.0f; break;
			case FRST_FLOOR: fMap[y * MapX + x] = 1.0f; break;

			case FLOOR_1QRTR: fMap[y * MapX + x] = 0.25f; break;
			case FLOOR_HALVE: fMap[y * MapX + x] = 0.50f; break;
			case FLOOR_3QRTR: fMap[y * MapX + x] = 0.75f; break;

			case         '1': fMap[y * MapX + x] = 0.10f; break;
			case         '2': fMap[y * MapX + x] = 0.20f; break;
			case         '3': fMap[y * MapX + x] = 0.30f; break;
			case         '4': fMap[y * MapX + x] = 0.40f; break;
			case         '5': fMap[y * MapX + x] = 0.50f; break;
			case         '6': fMap[y * MapX + x] = 0.60f; break;
			case         '7': fMap[y * MapX + x] = 0.70f; break;
			case         '8': fMap[y * MapX + x] = 0.80f; break;
			case         '9': fMap[y * MapX + x] = 0.90f; break;

			default: std::cout << "ERROR: AddLayer() --> unknown sMap value: " << sMap[y * MapX + x] << std::endl;
			}
		}
	}
	sMaps.push_back(sMap);
	fMaps.push_back(fMap);
}

void Map::addTextures(const std::string& sUserTexture)
{
	if (MapX * MapY != (int)sUserTexture.length())
	{
		std::cout << "ERROR: InitMap() -->  mismatch between map dimensions and length of map string" << std::endl;
	}

	std::string sTexture = sUserTexture;

	int* ITexture = new int[MapX * MapY];

	for (int y = 0; y < MapY; y++)
	{
		for (int x = 0; x < MapX; x++)
		{
			switch (sTexture[y * MapX + x]) {
			case TEXTURE_BLANK: ITexture[y * MapX + x] = -1; break;
			case TEXTURE_ZERO: ITexture[y * MapX + x] = 0; break;
			case TEXTURE_ONE: ITexture[y * MapX + x] = 1; break;
			case TEXTURE_TWO: ITexture[y * MapX + x] = 2; break;
			case TEXTURE_THREE: ITexture[y * MapX + x] = 3; break;
			case TEXTURE_FOUR: ITexture[y * MapX + x] = 4; break;
			case TEXTURE_FIVE: ITexture[y * MapX + x] = 5; break;
			

			default: std::cout << "ERROR: AddLayer() --> unknown sTexture value: " << sTexture[y * MapX + x] << std::endl;
			}
		}
	}
	iTextures.push_back(ITexture);
	sTextures.push_back(sTexture);
}

void Map::InitMap(int sizex, int sizey)
{
	MapX = sizex;
	MapY = sizey;
}


