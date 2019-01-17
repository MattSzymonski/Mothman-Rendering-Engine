#ifndef COMMONVALS //If not defined, another files will not redefine it if it has been already defined
#define COMMONVALS

#include "stb_image.h"

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;
const int TEXTURES_PER_MATERIAL = 2;

const int SKYBOX_TEXUNIT = 0;
const int DIFFUSE_TEXUNIT = 1;
const int NORMAL_TEXUNIT = 2;
const int DIR_SHADOWMAP_TEXUNIT = 3;
const int HEIGHTMAP_TEXUNIT = 4;
const int OMNIDIR_SHADOWMAP_TEXUNIT = 5; //Range between 4 and omnilights count


const int WINDOW_SIZE_H = 1400;
const int WINDOW_SIZE_V = 800;

#endif