#ifndef _RENDERER_H_
#define _RENDERER_H_

extern int BBox3D1;
extern int BBox3D2;
extern int BBox3D3;
extern int BBox3D4;

#define NUM_MAX_POINT_IN_POINT_BUFFER 800
#define NUM_MAX_BONES 50

extern s16 pointBuffer[NUM_MAX_POINT_IN_POINT_BUFFER * 3];
extern int numOfPoints;

void transformPoint(float* ax, float* bx, float* cx);

int AffObjet(int x, int y, int z, int alpha, int beta, int gamma, void* modelPtr);

void computeScreenBox(int x, int y, int z, int alpha, int beta, int gamma, char* bodyPtr);

#endif
