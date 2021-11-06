#include "common.h"

/* Projection:
 
 Z += cameraPerspective;
 float transformedX = ((X * cameraFovX) / Z) + cameraCenterX;
 float transformedY = ((Y * cameraFovY) / Z) + cameraCenterY;
 
 {X, Y, Z, 1}
 
 {
    ((X * cameraFovX) / Z) + cameraCenterX
    ((Y * cameraFovY) / Z) + cameraCenterY
    Z + cameraPerspective
    ?
 }
 
 ((X * CameraFovX) + (Z+CameraPersp) * cameraCenterX) / (Z+cameraPersp)
 
 
 
 */

struct rendererPointStruct
{
    float X;
    float Y;
    float Z;
};

#define NUM_MAX_VERTEX_IN_PRIM 64

struct primEntryStruct
{
	u8 polyType;
	u8 color;
	u16 size;
	u16 numOfVertices;
	primTypeEnum type;
	rendererPointStruct vertices[NUM_MAX_VERTEX_IN_PRIM];
};

#define NUM_MAX_PRIM_ENTRY 500

primEntryStruct primTable[NUM_MAX_PRIM_ENTRY];

u32 positionInPrimEntry = 0;

int BBox3D1=0;
int BBox3D2=0;
int BBox3D3=0;
int BBox3D4=0;

int renderVar1=0;

int numOfPrimitiveToRender=0;

char renderBuffer[3261];

char* renderVar2=NULL;

int modelFlags = 0;

int modelCosAlpha;
int modelSinAlpha;
int modelCosBeta;
int modelSinBeta;
int modelCosGamma;
int modelSinGamma;

bool noModelRotation;

int renderX;
int renderY;
int renderZ;

int numOfPoints;
int numOfBones;

s16 pointBuffer[NUM_MAX_POINT_IN_POINT_BUFFER*3];
s16 cameraSpaceBuffer[NUM_MAX_POINT_IN_POINT_BUFFER*3];
s16 bonesBuffer[NUM_MAX_BONES];

bool boneRotateX;
bool boneRotateY;
bool boneRotateZ;

int boneRotateXCos;
int boneRotateXSin;
int boneRotateYCos;
int boneRotateYSin;
int boneRotateZCos;
int boneRotateZSin;

char primBuffer[30000];

int renderVar3;

#ifndef AITD_UE4
void fillpoly(s16 * datas, int n, char c);
#endif

/*

 
 
*/

void transformPoint(float* ax, float* bx, float* cx)
{
    int X = (int)*ax;
    int Y = (int)*bx;
    int Z = (int)*cx;
    {
        int* ax = &X;
        int* bx = &Y;
        int* cx = &Z;

        {
            int x;
            int y;
            int z;

            if(transformUseY)
            {
                x = (((((*ax) * transformYSin) - ((*cx) * transformYCos))) / 0x10000)<<1;
                z = (((((*ax) * transformYCos) + ((*cx) * transformYSin))) / 0x10000)<<1;
            }
            else
            {
                x = (*ax);
                z = (*cx);
            }

            //si = x
            //ax = z

            if(transformUseX)
            {
                int tempY = (*bx);
                int tempZ = z;
                y = ((((tempY * transformXSin ) - (tempZ * transformXCos))) / 0x10000)<<1;
                z = ((((tempY * transformXCos ) + (tempZ * transformXSin))) / 0x10000)<<1;
            }
            else
            {
                y = (*bx);
            }

            // cx = y
            // bx = z

            if(transformUseZ)
            {
                int tempX = x;
                int tempY = y;
                x = ((((tempX * transformZSin) - ( tempY * transformZCos))) / 0x10000)<<1;
                y = ((((tempX * transformZCos) + ( tempY * transformZSin))) / 0x10000)<<1;
            }

            *ax = x;
            *bx = y;
            *cx = z;
        }
    }

    *ax = (float)X;
    *bx = (float)Y;
    *cx = (float)Z;
}

void InitGroupeRot(int transX,int transY,int transZ)
{
    if(transX)
    {
        boneRotateXCos = cosTable[transX&0x3FF];
        boneRotateXSin = cosTable[(transX+0x100)&0x3FF];

        boneRotateX = true;
    }
    else
    {
        boneRotateX = false;
    }

    if(transY)
    {
        boneRotateYCos = cosTable[transY&0x3FF];
        boneRotateYSin = cosTable[(transY+0x100)&0x3FF];

        boneRotateY = true;
    }
    else
    {
        boneRotateY = false;
    }

    if(transZ)
    {
        boneRotateZCos = cosTable[transZ&0x3FF];
        boneRotateZSin = cosTable[(transZ+0x100)&0x3FF];

        boneRotateZ = true;
    }
    else
    {
        boneRotateZ = false;
    }
}

void RotateList(s16* pointPtr, int numOfPoint)
{
    for(int i=0;i<numOfPoint;i++)
    {
        int x = *(s16*)pointPtr;
        int y = *(s16*)(pointPtr+1);
        int z = *(s16*)(pointPtr+2);

        if(boneRotateY)
        {
            int tempX = x;
            int tempZ = z;

            x = ((((tempX * boneRotateYSin) - (tempZ * boneRotateYCos)))>>16)<<1;
            z = ((((tempX * boneRotateYCos) + (tempZ * boneRotateYSin)))>>16)<<1;
        }

        if(boneRotateX)
        {
            int tempY = y;
            int tempZ = z;
            y = ((((tempY * boneRotateXSin ) - (tempZ * boneRotateXCos)))>>16)<<1;
            z = ((((tempY * boneRotateXCos ) + (tempZ * boneRotateXSin)))>>16)<<1;
        }

		if(boneRotateZ)
		{
			int tempX = x;
			int tempY = y;
			x = ((((tempX * boneRotateZSin) - ( tempY * boneRotateZCos)))>>16)<<1;
			y = ((((tempX * boneRotateZCos) + ( tempY * boneRotateZSin)))>>16)<<1;
		}

        *(s16*)(pointPtr) = x;
        *(s16*)(pointPtr+1) = y;
        *(s16*)(pointPtr+2) = z;

        pointPtr+=3;
    }
}

void RotateGroupeOptimise(sGroup* ptr)
{
    if (ptr->m_numGroup) // if group number is 0
    {
        int baseBone = ptr->m_start;
        int numPoints = ptr->m_numVertices;

        RotateList(pointBuffer + baseBone * 3, numPoints);
    }
}

void RotateGroupe(sGroup* ptr)
{
    int baseBone = ptr->m_start;
    int numPoints = ptr->m_numVertices;
    int temp;
    int temp2;

    RotateList(pointBuffer + baseBone * 3, numPoints);

    temp = ptr->m_numGroup; // group number

    temp2 = numOfBones - temp;

    do
    {
        if (ptr->m_orgGroup == temp) // is it on of this group child
        {
            RotateGroupe(ptr); // yes, so apply the transformation to him
        }

        ptr++;
    } while (--temp2);
}

void TranslateGroupe(int transX, int transY, int transZ, sGroup* ptr)
{
    s16* ptrSource = &pointBuffer[ptr->m_start * 3];

    for (int i = 0; i < ptr->m_numVertices; i++)
    {
        *(ptrSource++) += transX;
        *(ptrSource++) += transY;
        *(ptrSource++) += transZ;
    }
}

void ZoomGroupe(int zoomX, int zoomY, int zoomZ, sGroup* ptr)
{
    s16* ptrSource = &pointBuffer[ptr->m_start * 3];

    for (int i = 0; i < ptr->m_numVertices; i++)
    {
        *(ptrSource++) = (*(ptrSource) * (zoomX + 256)) / 256;
        *(ptrSource++) = (*(ptrSource) * (zoomY + 256)) / 256;
        *(ptrSource++) = (*(ptrSource) * (zoomZ + 256)) / 256;
    }
}

int AnimNuage(int x,int y,int z,int alpha,int beta,int gamma, sBody* pBody)
{
    renderX = x - translateX;
    renderY = y;
    renderZ = z - translateZ;

    ASSERT(pBody->m_vertices.size()<NUM_MAX_POINT_IN_POINT_BUFFER);

    for (int i = 0; i < pBody->m_vertices.size(); i++)
    {
        pointBuffer[i * 3 + 0] = pBody->m_vertices[i].x;
        pointBuffer[i * 3 + 1] = pBody->m_vertices[i].y;
        pointBuffer[i * 3 + 2] = pBody->m_vertices[i].z;
    }

    numOfPoints = pBody->m_vertices.size();
    numOfBones = pBody->m_groupOrder.size();
    ASSERT(numOfBones<NUM_MAX_BONES);

    if(pBody->m_flags & INFO_OPTIMISE)
    {
        for(int i=0;i<pBody->m_groupOrder.size();i++)
        {
            int boneDataOffset = pBody->m_groupOrder[i];
            sGroup* pGroup = &pBody->m_groups[pBody->m_groupOrder[i]];

            switch(pGroup->m_state.m_type)
            {
            case 1:
                if(pGroup->m_state.m_delta[0] || pGroup->m_state.m_delta[1] || pGroup->m_state.m_delta[2])
                {
                    TranslateGroupe(pGroup->m_state.m_delta[0], pGroup->m_state.m_delta[1], pGroup->m_state.m_delta[2], pGroup);
                }
                break;
            case 2:
                if (pGroup->m_state.m_delta[0] || pGroup->m_state.m_delta[1] || pGroup->m_state.m_delta[2])
                {
                    ZoomGroupe(pGroup->m_state.m_delta[0], pGroup->m_state.m_delta[1], pGroup->m_state.m_delta[2], pGroup);
                }
                break;
            }

            InitGroupeRot(pGroup[0].m_state.m_rotateDelta[0], pGroup[0].m_state.m_rotateDelta[1], pGroup[0].m_state.m_rotateDelta[2]);
            RotateGroupeOptimise(pGroup);
        }
    }
    else
    {
        pBody->m_groups[0].m_state.m_delta[0] = alpha;
        pBody->m_groups[0].m_state.m_delta[1] = beta;
        pBody->m_groups[0].m_state.m_delta[2] = gamma;

        for(int i=0;i<pBody->m_groups.size();i++)
        {
            int boneDataOffset = pBody->m_groupOrder[i];
            sGroup* pGroup = &pBody->m_groups[pBody->m_groupOrder[i]];

            int transX = pGroup->m_state.m_delta[0];
            int transY = pGroup->m_state.m_delta[1];
            int transZ = pGroup->m_state.m_delta[2];

            if(transX || transY || transZ)
            {
                switch(pGroup->m_state.m_type)
                {
                case 0:
                    { 
                        InitGroupeRot(transX,transY,transZ);
                        RotateGroupe(pGroup);
                        break;
                    }
                case 1:
                    {
                        TranslateGroupe(transX,transY,transZ, pGroup);
                        break;
                    }
                case 2:
                    {
                        ZoomGroupe(transX,transY,transZ, pGroup);
                        break;
                    }
                }
            }
        }
    }

    for(int i=0;i<pBody->m_groups.size();i++)
    {
        sGroup* pGroup = &pBody->m_groups[i];

        int j;

        int point1;
        int point2;

        s16* ptr1;
        s16* ptr2;

        int number;

        int ax;
        int bx;
        int dx;

        point1 = pGroup->m_baseVertices * 6;
        point2 = pGroup->m_start * 6;

        ASSERT(point1%2 == 0);
        ASSERT(point2%2 == 0);

        point1/=2;
        point2/=2;

        ASSERT(point1/3<NUM_MAX_POINT_IN_POINT_BUFFER);
        ASSERT(point2/3<NUM_MAX_POINT_IN_POINT_BUFFER);

        ptr1 = (s16*)&pointBuffer[point1];
        ptr2 = (s16*)&pointBuffer[point2];

        number = pGroup->m_numVertices;

        ax = ptr1[0];
        bx = ptr1[1];
        dx = ptr1[2];

        for(j=0;j<number;j++)
        {
            *(ptr2++) += ax;
            *(ptr2++) += bx;
            *(ptr2++) += dx;
        }
    }

    if(modelFlags & INFO_OPTIMISE)
    {
        InitGroupeRot(alpha,beta,gamma);
        RotateList(pointBuffer,numOfPoints);
    }

    {
        char* ptr = (char*)pointBuffer;
        s16* outPtr = cameraSpaceBuffer;
        int k = numOfPoints;


        float* outPtr2;

        for(int i=0;i<numOfPoints;i++)
        {
            float X = *(s16*)ptr;
            float Y = *(s16*)(ptr+2);
            float Z = *(s16*)(ptr+4);
            ptr+=6;

            X += renderX;
            Y += renderY;
            Z += renderZ;

#if defined(AITD_UE4)
            *(outPtr++) = (s16)X;
            *(outPtr++) = (s16)Y;
            *(outPtr++) = (s16)Z;
#else
            if(Y>10000) // depth clamp
            {
                *(outPtr++) = -10000;
                *(outPtr++) = -10000;
                *(outPtr++) = -10000;
            }
            else
            {
                Y -= translateY;

                transformPoint(&X,&Y,&Z);

                *(outPtr++) = (s16)X;
                *(outPtr++) = (s16)Y;
                *(outPtr++) = (s16)Z;
            }
#endif
        }

        ptr = (char*)cameraSpaceBuffer;
        outPtr2 = renderPointList;

        do
        {
            float X;
            float Y;
            float Z;

            X = *(s16*)ptr;
            ptr+=2;
            Y = *(s16*)ptr;
            ptr+=2;
            Z = *(s16*)ptr;
            ptr+=2;

#if defined(AITD_UE4)
            *(outPtr2++) = X;
            *(outPtr2++) = Y;
            *(outPtr2++) = Z;
#else
            Z += cameraPerspective;

            if( Z<=50 ) // clipping
            {
                *(outPtr2++) = -10000;
                *(outPtr2++) = -10000;
                *(outPtr2++) = -10000;
            }
            else
            {
                float transformedX = ((X * cameraFovX) / Z) + cameraCenterX;
                float transformedY;

                *(outPtr2++) = transformedX;

                if(transformedX < BBox3D1)
                    BBox3D1 = (int)transformedX;

                if(transformedX > BBox3D3)
                    BBox3D3 = (int)transformedX;

                transformedY = ((Y * cameraFovY) / Z) + cameraCenterY;

                *(outPtr2++) = transformedY;

                if(transformedY < BBox3D2)
                    BBox3D2 = (int)transformedY;

                if(transformedY > BBox3D4)
                    BBox3D4 = (int)transformedY;

                *(outPtr2++) = Z; 
            }
#endif

            k--;
            if(k==0)
            {
                return(1);
            }

        }while(renderVar1 == 0);
    }

    return(0);
}

/*
x - cameraX
y
z - cameraZ
 
*/

int RotateNuage(int x,int y,int z,int alpha,int beta,int gamma, sBody* pBody)
{
    float* outPtr;

    renderX = x - translateX;
    renderY = y;
    renderZ = z - translateZ;

    if(!alpha && !beta && !gamma)
    {
        noModelRotation = true;
    }
    else
    {
        noModelRotation = false;

        modelCosAlpha = cosTable[alpha&0x3FF];
        modelSinAlpha = cosTable[(alpha+0x100)&0x3FF];

        modelCosBeta = cosTable[beta&0x3FF];
        modelSinBeta = cosTable[(beta+0x100)&0x3FF];

        modelCosGamma = cosTable[gamma&0x3FF];
        modelSinGamma = cosTable[(gamma+0x100)&0x3FF];
    }

    outPtr = renderPointList;

    for(int i=0; i<pBody->m_vertices.size(); i++)
    {
        float X = pBody->m_vertices[i].x;
        float Y = pBody->m_vertices[i].y;
        float Z = pBody->m_vertices[i].z;

        if(!noModelRotation)
        {
			// Y rotation
			{
				float tempX = X;
				float tempZ = Z;

				X = (((modelSinBeta * tempX) - (modelCosBeta * tempZ))/65536.f)*2.f;
				Z = (((modelCosBeta * tempX) + (modelSinBeta * tempZ))/65536.f)*2.f;
			}

			// Z rotation
            {
                float tempX = X;
                float tempY = Y;

                X = (((modelSinGamma * tempX) - (modelCosGamma * tempY))/65536.f)*2.f;
                Y = (((modelCosGamma * tempX) + (modelSinGamma * tempY))/65536.f)*2.f;
            }

			// X rotation
            {
                float tempY = Y;
                float tempZ = Z;

                Y = (((modelSinAlpha * tempY) - (modelCosAlpha * tempZ))/65536.f)*2.f;
                Z = (((modelCosAlpha * tempY) + (modelSinAlpha * tempZ))/65536.f)*2.f;
            }
        }

        X += renderX;
        Y += renderY;
        Z += renderZ;

#if defined(AITD_UE4)
        *(outPtr++) = X;
        *(outPtr++) = Y;
        *(outPtr++) = Z;
#else
        if(Y>10000) // depth clamp
        {
            *(outPtr++) = -10000;
            *(outPtr++) = -10000;
            *(outPtr++) = -10000;
        }
        else
        {
            float transformedX;
            float transformedY;

            Y -= translateY;

            transformPoint(&X,&Y,&Z);

            Z += cameraPerspective;

            transformedX = ((X * cameraFovX) / Z) + cameraCenterX;

            *(outPtr++) = transformedX;

            if(transformedX < BBox3D1)
                BBox3D1 = (int)transformedX;

            if(transformedX > BBox3D3)
                BBox3D3 = (int)transformedX;

            transformedY = ((Y * cameraFovY) / Z) + cameraCenterY;

            *(outPtr++) = transformedY;

            if(transformedY < BBox3D2)
                BBox3D2 = (int)transformedY;

            if(transformedY > BBox3D4)
                BBox3D4 = (int)transformedY;

            *(outPtr++) = Z; 

            /*  *(outPtr++) = X;
            *(outPtr++) = Y;
            *(outPtr++) = Z; */

        }
#endif
    }
    return(1);
}

char* primVar1;
char* primVar2;

void primFunctionDefault(int primType,char** ptr,char** out)
{
    printf("UnHandled primType %d\n",primType);
    assert(0);
}

void processPrim_Line(int primType, char** ptr, char** out) // line tested
{
    float depth = 32000.f;
    primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

    (*ptr)++;
    u8 lineColor = **ptr;
    (*ptr)++;
    (*ptr)++;

    pCurrentPrimEntry->type = primTypeEnum_Line;
    pCurrentPrimEntry->color = lineColor;


    for(int i=0;i<2;i++)
    {
        u16 pointIndex;

        pointIndex = *(u16*)(*ptr);
        (*ptr)+=2;

        ASSERT((pointIndex%2) == 0);

        pCurrentPrimEntry->vertices[i].X = renderPointList[pointIndex/2];
        pCurrentPrimEntry->vertices[i].Y = renderPointList[(pointIndex/2)+1];
        pCurrentPrimEntry->vertices[i].Z = renderPointList[(pointIndex/2)+2];

        if(depth>pCurrentPrimEntry->vertices[i].Z)
            depth = pCurrentPrimEntry->vertices[i].Z;
    }

    if(depth > 0)
    {
        positionInPrimEntry++;
        numOfPrimitiveToRender++;
		ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);
    }
}

void processPrim_Line(int primType, sPrimitive* ptr, char** out)
{
}

void processPrim_Poly(int primType, sPrimitive* ptr, char** out)
{
    int i;
    int numOfPointInPoly;
    u8 polyColor;
    u8 polyType;
    float depth = 32000.f;

    primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

    ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);

    numOfPointInPoly = ptr->m_points.size();
    polyType = ptr->m_subType;
    polyColor = ptr->m_color;

    pCurrentPrimEntry->type = primTypeEnum_Poly;
    pCurrentPrimEntry->numOfVertices = numOfPointInPoly;
    pCurrentPrimEntry->color = polyColor;
    pCurrentPrimEntry->polyType = polyType;

    ASSERT(numOfPointInPoly < NUM_MAX_VERTEX_IN_PRIM);

    for (i = 0; i < numOfPointInPoly; i++)
    {
        u16 pointIndex;

        pointIndex = ptr->m_points[i] * 6;

        ASSERT((pointIndex % 2) == 0);

        pCurrentPrimEntry->vertices[i].X = renderPointList[pointIndex / 2];
        pCurrentPrimEntry->vertices[i].Y = renderPointList[(pointIndex / 2) + 1];
        pCurrentPrimEntry->vertices[i].Z = renderPointList[(pointIndex / 2) + 2];

        if (pCurrentPrimEntry->vertices[i].Z < depth)
            depth = pCurrentPrimEntry->vertices[i].Z;

    }

#if !defined(AITD_UE4)
    if (depth > 100)
#endif
    {
        positionInPrimEntry++;

        numOfPrimitiveToRender++;
        ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);
    }
}

void processPrim_Point(int primType, sPrimitive* ptr, char** out)
{
}

void processPrim_Sphere(int primType, sPrimitive* ptr, char** out)
{
}


void processPrim_Poly(int primType, char** ptr, char** out) // poly
{
    int i;
    int numOfPointInPoly;
    u8 polyColor;
    u8 polyType;
    float depth = 32000.f;

    primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

    ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);

    numOfPointInPoly = **ptr;
    (*ptr)++;

    polyType = **ptr;
    (*ptr)++;

    polyColor = **ptr;
    (*ptr)++;

    pCurrentPrimEntry->type = primTypeEnum_Poly;
    pCurrentPrimEntry->numOfVertices = numOfPointInPoly;
    pCurrentPrimEntry->color = polyColor;
    pCurrentPrimEntry->polyType = polyType;

	ASSERT(numOfPointInPoly < NUM_MAX_VERTEX_IN_PRIM);

    for(i=0;i<numOfPointInPoly;i++)
    {
        u16 pointIndex;

        pointIndex = *(u16*)(*ptr);
        (*ptr)+=2;

        ASSERT((pointIndex%2) == 0);

        pCurrentPrimEntry->vertices[i].X = renderPointList[pointIndex/2];
        pCurrentPrimEntry->vertices[i].Y = renderPointList[(pointIndex/2)+1];
        pCurrentPrimEntry->vertices[i].Z = renderPointList[(pointIndex/2)+2];

        if(pCurrentPrimEntry->vertices[i].Z < depth)
            depth = pCurrentPrimEntry->vertices[i].Z;

    }

#if !defined(AITD_UE4)
    if(depth > 100)
#endif
    {
        positionInPrimEntry++;

        numOfPrimitiveToRender++;
		ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);
    }
}

void processPrim_Point(int primType, char** ptr, char** out) // point
{
    u8 pointColor;
    u16 pointIndex;
    float depth = 32000.f;
    primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

    (*ptr)++;
    pointColor = **ptr;
    (*ptr)++;
    (*ptr)++;

	switch(primType)
	{
		case primTypeEnum_Point:
			pCurrentPrimEntry->type = primTypeEnum_Point;
			break;
		case primTypeEnum_BigPoint:
			pCurrentPrimEntry->type = primTypeEnum_BigPoint;
			break;
		case primTypeEnum_Zixel:
			pCurrentPrimEntry->type = primTypeEnum_Zixel;
			break;
		default:
			assert(0);
			break;
	}
    pCurrentPrimEntry->type = primTypeEnum_Point;
    pCurrentPrimEntry->color = pointColor;

    pointIndex = *(u16*)(*ptr);
    (*ptr)+=2;

    ASSERT((pointIndex%2) == 0);

    pCurrentPrimEntry->vertices[0].X = renderPointList[pointIndex/2];
    pCurrentPrimEntry->vertices[0].Y = renderPointList[(pointIndex/2)+1];
    depth = pCurrentPrimEntry->vertices[0].Z = renderPointList[(pointIndex/2)+2];

    if(depth > 0)
    {
        positionInPrimEntry++;
        numOfPrimitiveToRender++;
		ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);
    }
}

void processPrim_Sphere(int primType, char** ptr, char** out) // sphere
{
    u8 discColor;
    u16 discSize;
    u16 pointIndex;
    float depth = 32000.f;
    primEntryStruct* pCurrentPrimEntry = &primTable[positionInPrimEntry];

    (*ptr)++;
    discColor = **ptr;
    (*ptr)++;
    (*ptr)++;
    discSize = *(u16*)(*ptr);
    (*ptr)+=2;

    pCurrentPrimEntry->type = primTypeEnum_Sphere;
    pCurrentPrimEntry->color = discColor;
    pCurrentPrimEntry->size = discSize;

    pointIndex = *(u16*)(*ptr);
    (*ptr)+=2;

    ASSERT((pointIndex%2) == 0);

    pCurrentPrimEntry->vertices[0].X = renderPointList[pointIndex/2];
    pCurrentPrimEntry->vertices[0].Y = renderPointList[(pointIndex/2)+1];
    depth = pCurrentPrimEntry->vertices[0].Z = renderPointList[(pointIndex/2)+2];

    if(depth > 0)
    {
        positionInPrimEntry++;
        numOfPrimitiveToRender++;
		ASSERT(positionInPrimEntry < NUM_MAX_PRIM_ENTRY);
    }
}

void primType5(int primType, char** ptr, char** out) // draw out of hardClip
{
	printf("ignoring prim type 5\n");
	return;

    int pointNumber;
    s16 ax2;

    primVar1 = *out;

    *(s16*)(*out) = *(s16*)(*ptr);
    *out+=2;
    *ptr+=3;

    pointNumber = *(s16*)(*ptr);
    *ptr+=2;

    // here, should check for clip on X Y Z

    *(float*)(*out) = renderPointList[pointNumber/2]; // X
    *out+=sizeof(float);
    *(float*)(*out) = renderPointList[pointNumber/2+1]; // Y
    *out+=sizeof(float);
    *(float*)(*out) = renderPointList[pointNumber/2+2]; // Z
    ax2 = (s16)(*(float*)(*out));
    *out+=sizeof(float);

    primVar2 = *out;

    {
        numOfPrimitiveToRender++;

        *out = renderVar2;

        *(s16*)(*out) = ax2;
        *out+=2;
        *(s16*)(*out) = ax2;
        *out+=2;
        *(s16*)(*out) = primType;
        *out+=2;

        *(char**)(*out) = primVar1;
        *out+=4;

        renderVar2 = *out;
        *out = primVar2;
    }
}

void line(int x1, int y1, int x2, int y2, char c);

void renderLine(primEntryStruct* pEntry) // line
{
    osystem_draw3dLine( pEntry->vertices[0].X,pEntry->vertices[0].Y,pEntry->vertices[0].Z,
						pEntry->vertices[1].X,pEntry->vertices[1].Y,pEntry->vertices[1].Z,
						pEntry->color);
}

void renderPoly(primEntryStruct* pEntry) // poly
{
    osystem_fillPoly((float*)pEntry->vertices,pEntry->numOfVertices, pEntry->color, pEntry->polyType);
}

void renderZixel(primEntryStruct* pEntry) // point
{
    float transformedSize;

    transformedSize = ((5.f * (float)cameraFovX) / (float)(pEntry->vertices[0].Z+cameraPerspective));

    osystem_drawPoint(pEntry->vertices[0].X,pEntry->vertices[0].Y,pEntry->vertices[0].Z,pEntry->color,transformedSize);
}

void renderPoint(primEntryStruct* pEntry) // point
{
    float transformedSize;
	
    transformedSize = ((5.f * (float)cameraFovX) / (float)(pEntry->vertices[0].Z+cameraPerspective));
	
    osystem_drawPoint(pEntry->vertices[0].X,pEntry->vertices[0].Y,pEntry->vertices[0].Z,pEntry->color,1);
}

void renderBigPoint(primEntryStruct* pEntry) // point
{
    float transformedSize;
	
    transformedSize = ((5.f * (float)cameraFovX) / (float)(pEntry->vertices[0].Z+cameraPerspective));
	
    osystem_drawPoint(pEntry->vertices[0].X,pEntry->vertices[0].Y,pEntry->vertices[0].Z,pEntry->color,4);
}

void renderSphere(primEntryStruct* pEntry) // sphere
{
    float transformedSize;

    transformedSize = (((float)pEntry->size * (float)cameraFovX) / (float)(pEntry->vertices[0].Z+cameraPerspective));

    osystem_drawSphere(pEntry->vertices[0].X,pEntry->vertices[0].Y,pEntry->vertices[0].Z,pEntry->color,transformedSize*2);
}


void defaultRenderFunction(primEntryStruct* buffer)
{
    printf("Unsupported renderType\n");
}

typedef void (*renderFunction)(primEntryStruct* buffer);

renderFunction renderFunctions[]={
    renderLine, // line
    renderPoly, // poly
    renderPoint, // point
    renderSphere, // sphere
    renderBigPoint,
	renderZixel,
};

int AffObjet(int x,int y,int z,int alpha,int beta,int gamma,void* modelPtr)
{
    sBody* pBody = getBodyFromPtr(modelPtr);
    char* ptr = (char*)modelPtr;
    int numPrim;
    int i;
    char* out;

    // reinit the 2 static tables
    positionInPrimEntry = 0;
    //

    BBox3D1 = 0x7FFF;
    BBox3D2 = 0x7FFF;

    BBox3D3 = -0x7FFF;
    BBox3D4 = -0x7FFF;

    renderVar1 = 0;

    numOfPrimitiveToRender = 0;

    renderVar2 = renderBuffer;

    modelFlags = READ_LE_S16(ptr); ptr+=2;
    ptr+=12; // skip the ZV

    ptr+=READ_LE_S16(ptr) + 2; // skip scratch buffer

    if(modelFlags&INFO_ANIM)
    {
        pBody->sync();
        if(!AnimNuage(x,y,z,alpha,beta,gamma, pBody))
        {
            BBox3D3 = -32000;
            BBox3D4 = -32000;
            BBox3D1 = 32000;
            BBox3D2 = 32000;
            return(2);
        }
    }
    else
        if(!(modelFlags&INFO_TORTUE))
        {
            if(!RotateNuage(x,y,z,alpha,beta,gamma, pBody))
            {
                BBox3D3 = -32000;
                BBox3D4 = -32000;
                BBox3D1 = 32000;
                BBox3D2 = 32000;
                return(2);
            }
        }
        else
        {
            printf("unsupported model type prerenderFlag4 in renderer !\n");

            BBox3D3 = -32000;
            BBox3D4 = -32000;
            BBox3D1 = 32000;
            BBox3D2 = 32000;
            return(2);
        }

        numPrim = pBody->m_primitives.size();

        if(!numPrim)
        {
            BBox3D3 = -32000;
            BBox3D4 = -32000;
            BBox3D1 = 32000;
            BBox3D2 = 32000;
            return(2);
        }

        out = primBuffer;

		// create the list of all primitives to render
        for(i=0;i<numPrim;i++)
        {
            sPrimitive* pPrimitive = &pBody->m_primitives[i];
            u8 primType = pPrimitive->m_type;

			switch(primType)
			{
			case primTypeEnum_Line:
				processPrim_Line(primType, pPrimitive,&out);
				break;
			case primTypeEnum_Poly:
				processPrim_Poly(primType, pPrimitive,&out);
				break;
			case primTypeEnum_Point:
			case primTypeEnum_BigPoint:
			case primTypeEnum_Zixel:
				processPrim_Point(primType, pPrimitive,&out);
				break;
			case primTypeEnum_Sphere:
				processPrim_Sphere(primType, pPrimitive,&out);
				break;
            case processPrim_PolyTexture9:
            case processPrim_PolyTexture10:
                processPrim_Poly(primType, pPrimitive, &out);
                ptr += 3 * 2;
                break;
			default:
				return 0;
				assert(0);
			}

        }

#if 0
        // TODO: poly sorting by depth
#ifdef USE_GL2
        source = renderBuffer;
#else
        inBuffer = renderBuffer;
        outBuffer = sortedBuffer;

        for(i=0;i<numOfPolyToRender;i++)
        {
            int j;
            int bestIdx;
            int bestDepth = -32000;
            char* readBuffer = renderBuffer;

            for(j=0;j<numOfPolyToRender;j++)
            {
                int depth = READ_LE_S16(readBuffer);

                if(depth>bestDepth)
                {
                    bestIdx = j;
                    bestDepth = depth;
                }

                readBuffer+=10;
            }

            memcpy(outBuffer,renderBuffer+10*bestIdx,10);
            *(s16*)(renderBuffer+10*bestIdx) = -32000;
            outBuffer+=10;
        }
        source = sortedBuffer;

#endif
#endif

        //  

        if(!numOfPrimitiveToRender)
        {
            BBox3D3 = -32000;
            BBox3D4 = -32000;
            BBox3D1 = 32000;
            BBox3D2 = 32000;
            return(1); // model ok, but out of screen
        }

        //  source += 10 * 1;
        for(i=0;i<numOfPrimitiveToRender;i++)
        {
            renderFunctions[primTable[i].type](&primTable[i]);
        }

        //DEBUG
        /*  for(i=0;i<numPointInPoly;i++)
        {
        int x;
        int y;

        x = renderPointList[i*3];
        y = renderPointList[i*3+1];

        if(x>=0 && x < 319 && y>=0 && y<199)
        {
        screen[y*320+x] = 15;
        }
        }*/
        //

        osystem_flushPendingPrimitives();
        return(0);
}

void computeScreenBox(int x, int y, int z, int alpha, int beta, int gamma, char* bodyPtr)
{
    sBody* pBody = getBodyFromPtr(bodyPtr);

    BBox3D1 = 0x7FFF;
    BBox3D2 = 0x7FFF;

    BBox3D3 = -0x7FFF;
    BBox3D4 = -0x7FFF;

    renderVar1 = 0;

    numOfPrimitiveToRender = 0;

    renderVar2 = renderBuffer;

    modelFlags = pBody->m_flags;

    if(modelFlags&INFO_ANIM)
    {
        pBody->sync();
        AnimNuage(x,y,z,alpha,beta,gamma, pBody);
    }
}

void sBody::sync()
{
    u8* ptr = (u8*)m_raw;

    ptr += 2; // skip the flag
    ptr += 12; // skip the ZV
    ptr += READ_LE_S16(ptr) + 2; // skip scratch buffer
    ptr += READ_LE_S16(ptr) * 6 + 2; // skip vertices
    u16 numGroups = READ_LE_U16(ptr);
    ptr += numGroups * 2 + 2; // skip group order

    assert(numGroups == m_groups.size());

    for (int i = 0; i < numGroups; i++)
    {
        m_groups[i].m_state.m_type = READ_LE_S16(ptr + 8);
        m_groups[i].m_state.m_delta[0] = READ_LE_S16(ptr + 10);
        m_groups[i].m_state.m_delta[1] = READ_LE_S16(ptr + 12);
        m_groups[i].m_state.m_delta[2] = READ_LE_S16(ptr + 14);
        ptr += 16;
        if (m_flags & INFO_OPTIMISE)
        {
            m_groups[i].m_state.m_rotateDelta[0] = READ_LE_S16(ptr + 0);
            m_groups[i].m_state.m_rotateDelta[1] = READ_LE_S16(ptr + 2);
            m_groups[i].m_state.m_rotateDelta[2] = READ_LE_S16(ptr + 4);
            ptr += 8;
        }
    }
}
