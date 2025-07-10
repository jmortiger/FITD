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

// #define _RENDER_calcTransformedX(xF, zF) ((xF * cameraFovX) / zF) + cameraCenterX
// #define _RENDER_calcTransformedXCustom(xF, zF, cameraFovXF, cameraCenterXF) ((xF * cameraFovXF) / zF) + cameraCenterXF
#define _RENDER_calcTransformedCustom(valF, zF, cameraFovF, cameraCenterF) ((valF * cameraFovF) / zF) + cameraCenterF
// #define _RENDER_calcTransformed(valF, zF, coord) ((valF * cameraFov##coord) / Z) + cameraCenter##coord
#define _RENDER_calcTransformed(valF, zF, coord) _RENDER_calcTransformedCustom(valF, zF, cameraFov##coord, cameraCenter##coord)
#define _RENDER_calcTransformedX(xF, zF) _RENDER_calcTransformed(xF, zF, X)
#define _RENDER_calcTransformedY(yF, zF) _RENDER_calcTransformed(yF, zF, Y)

#endif
