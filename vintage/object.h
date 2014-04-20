#ifndef OBJECT_H
#define OBJECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

#define NUMOBJECTS 1

// handles for buffer objects used to store mesh on the graphics card
unsigned int verticesVbo;
unsigned int vao;
unsigned int indicesVbo;

char *objbuffer;

typedef struct
{
   vec3_t position;
   //vec3_t vertNormals;
   vec2_t UVcoord;
} vertelem_t;

typedef struct
{
   int numMeshVertices;
   int numNormals;
   int numUVTextureCoords;
   vec3_t *vertMeshes;
   vec3_t *vertNormals;
   vec2_t *vertUVTextures;

   vertelem_t *vertelem;
   //vec3_t *vertUVWTextures;  // for future use
// for debugging
      
	float *vertexPos;
   float *vertexNorms;
   float *vertexTexcoords;

	unsigned short *posIndices;
   unsigned short *normIndices;
   unsigned short *texcoordIndices;

   int numPos;
   int numNorms;
   int numTexcoords;

   // for new format
   int numPosIndices;
   int numNormsIndices;
   int numTexcoordsIndices;
// for debugging

   vec3_t start_pos;
	//etc..
} obj_t;

//from a file
int OBJ_Load( const char *filename );
void OBJ_GetVertexData( obj_t *obj );
void OBJ_Clean( void );

int GetPosBytes( obj_t **obj, int num_objects );
int GetTexBytes( obj_t **obj, int num_objects );

int GetPosIdxBytes( obj_t **obj, int num_objects );
int GetTexIdxBytes( obj_t **obj, int num_objects );

void DeleteObj( obj_t *obj );
obj_t *LoadObj( char *name, float x, float y, float z );

#endif
