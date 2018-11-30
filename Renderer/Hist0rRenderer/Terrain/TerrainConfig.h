#pragma once
class TerrainConfig
{
public:
	TerrainConfig();
	
	int scaleY;
	int scaleXZ;

	int positionY;
	int positionXZ;

	int lodRange[8];
	int lodMorphingArea[8];

	~TerrainConfig();
private:
	


};

