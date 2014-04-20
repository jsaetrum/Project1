#include "object.h"

int start_size = 7000;

// file attribs
FILE *objfp;

int OBJ_Load( const char *filename )
{
   long size;
	errno_t err;

   err = fopen_s( &objfp, filename, "r" ); // TODO: fopen_s (make a macro) #ifdef _WIN32 etc. fopen_s / fopen
	if ( err )
	{
		fprintf( stderr, "File not found: %s\n", filename );
      return 0;
	}

   fseek( objfp, 0, SEEK_END );
   size = ftell( objfp );
   rewind( objfp );

   objbuffer = (char*)calloc( size, sizeof(char) );
   fread( objbuffer, sizeof(char), size, objfp ); // TODO: fread_s (make a macro)

   return 1;
}

void OBJ_GetVertexData( obj_t *obj )
{
   int nargs;
   char tag[3];
   vec3_t vertex;
   int i;
   long vdoffset = 0;

   obj->numMeshVertices = obj->numNormals = obj->numUVTextureCoords = 0;

   rewind(objfp);

   // count vertices
   while (fscanf(objfp, "%2s %*[^\n]", tag) != EOF)
   {
      if (!strcmp(tag, "v"))
         obj->numMeshVertices++;
      else if (!strcmp(tag, "vn"))
         obj->numNormals++;
      else if (!strcmp(tag, "vt"))
         obj->numUVTextureCoords++;
      else if (!strcmp(tag, "#") || !strcmp(tag, "o")) // temp
         vdoffset = ftell(objfp); // temp: tell where the interesting data is located

      //if (!strcmp(tag, "v") && !vdoffset)
      //   vdoffset = ftell(objfp); // tell where the interesting data is located
   }

   vdoffset++; // temp

   obj->vertMeshes = (vec3_t*)malloc(obj->numMeshVertices*sizeof(vec3_t));
   obj->vertNormals = (vec3_t*)malloc(obj->numNormals*sizeof(vec3_t));
   obj->vertUVTextures = (vec2_t*)malloc(obj->numUVTextureCoords*sizeof(vec2_t));

   fseek(objfp, vdoffset, SEEK_SET);
   i = 0;
   while((nargs = fscanf(objfp, "%2s %f %f %f", tag, &vertex[0], &vertex[1], &vertex[2])) != EOF)
   {
      if (nargs == 4)
      {
         if (!strcmp(tag, "v"))
            Vec3Copy(vertex, obj->vertMeshes[i++]);
         else if (!strcmp(tag, "vn"))
            Vec3Copy(vertex, obj->vertNormals[i++]);
      }
      else if (nargs == 3)
      {
         if (!strcmp(tag, "vt"))
            Vec2Copy(vertex, obj->vertUVTextures[i++]);
      }
   }

   //printf("v %f %f %f\n", obj->vertMeshes[0][0], obj->vertMeshes[0][1], obj->vertMeshes[0][2]);
}

void OBJ_Clean( obj_t *obj )
{
   free(obj->vertMeshes);
   free(obj->vertNormals);
   free(obj->vertUVTextures);
   fclose( objfp );
}

// Reads one line at a time and formats(converts?) it with sscanf
//debugging, testing. will only work when faces are like this: f n/n n/n n/n
// TODO: maybe write an "easier" function based on a slightly modified obj-format:
// e.g.:
//  # # # <--- numbers denoting the number of vertices in file (position, texture, normals and indices)
// This is useful in order to the elimination of reading the obj-file twice (in first counting the vertices in order to allocate
// the right amount of data). Also get rid of the / notation used for indices, instead list the indices like vertices in the
// obj-format.
// The function below (or other functions in whole or partial) may be used as a tool (isolated from this project)
// to convert to the new plain text file format.
int ReadVertexData( const char **name, obj_t *obj )
{
	FILE *f;
	int i,j, c, y, lineLen;
	char line[100];
	errno_t err;

   lineLen = 50;
	err = fopen_s( &f, *name, "r" );
	if ( err )
	{
		fprintf( stderr, "File not found: %s\n", *name );
		return -1;
	}
	memset(line, 0, sizeof(char) * lineLen );
	i = j = c = y = 0;
	do
	{
		 if( fgets( line, lineLen-1 ,f ) == NULL )
			 break;
		// POSITIONS-> 3 floats for each vertex
		 if( line[0] == 'v' && line[1] != 't' && line[1] != 'n'  )
		 {
			//sscanf_s( line, "%*c %f %f %f ", &obj->vertexPos[i],  &obj->vertexPos[i+1],  &obj->vertexPos[i+2] );
          sscanf_s( line, "%*c %f %f %f ", &obj->vertelem[i].position[0], &obj->vertelem[i].position[1], &obj->vertelem[i].position[2] );

         i++;
			//i += 3;
			obj->numPos += 3;
		 }
       // INDICES for positions and UV coordinates only( vertexTexCoords )
		 if( line[0] == 'f' )
		 {
			 sscanf_s( line, "%*c %hu%*c%hu %hu%*c%hu %hu%*c%hu ", 
             &obj->posIndices[j], &obj->texcoordIndices[y],  
				 &obj->posIndices[j+1], &obj->texcoordIndices[y+1],  
				 &obj->posIndices[j+2], &obj->texcoordIndices[y+2]);
	
          y += 3;
			 j += 3;
          obj->numPosIndices += 3;
          obj->numTexcoordsIndices += 3;
		 }
       // TEXTURE COORDINATES
       if( strncmp( &line[0], "vt", 2 ) == 0 )
		 {
          //sscanf_s( line,  "%*c%*c %f %f ", &obj->vertexTexcoords[c], &obj->vertexTexcoords[c+1] );
          sscanf_s( line,  "%*c%*c %f %f ", &obj->vertelem[c].UVcoord[0], &obj->vertelem[c].UVcoord[1] );

          c++;
			 //c += 2;
          obj->numTexcoords += 2; 
       }
	} while( !feof(f) );
	fclose( f );
	return 0;
}

// Bytes for mesh
int GetPosBytes( obj_t **obj, int num )
{
   int i, bytes_total;

   bytes_total = 0;
   for( i = 0; i < num; i++ )
      bytes_total += obj[i]->numPos * sizeof(float);

   return bytes_total;
}

//bytes for uv coordinates
int GetTexBytes( obj_t **obj, int num )
{
   int i, bytes_total;
   bytes_total = 0;
   for( i = 0; i < num; i++ )
      bytes_total += obj[i]->numTexcoords * sizeof(float);

   return bytes_total;
}
// this one and the one below: bytes for each index type
int GetPosIdxBytes( obj_t **obj, int num )
{
   int i, bytes_total;

   bytes_total = 0;
   for( i = 0; i < num; i++ )
      bytes_total += obj[i]->numPosIndices * sizeof(unsigned short);

   return bytes_total;
}

int GetTexIdxBytes( obj_t **obj, int num )
{
   int i, bytes_total;
   bytes_total = 0;
   for( i = 0; i < num; i++ )
      bytes_total += obj[i]->numTexcoordsIndices * sizeof(unsigned short);

   return bytes_total;
}

void AdjustToZeroIndexing( obj_t *o )
{
   int i;
   for( i = 0; i < o->numPosIndices; i++ )
	{
		if( o->posIndices[i] > 0 )
			o->posIndices[i]--; 
	}

   for( i = 0; i < o->numNormsIndices; i++ )
	{
      if( o->normIndices > 0 )
			o->normIndices[i]--; 
	}

   for( i = 0; i < o->numTexcoordsIndices; i++ )
	{
      if( o->texcoordIndices > 0 )
         o->texcoordIndices[i]--; 
	}
}

obj_t *LoadObj( char *name, float x, float y, float z  )
{
	int retval;
	obj_t *obj;

	obj = (obj_t*)malloc(sizeof(obj_t));
	obj->vertexPos = (float*)calloc( start_size, sizeof(float) );
   obj->vertexNorms = (float*)calloc( start_size, sizeof(float) );
	obj->vertexTexcoords = (float*)calloc( start_size, sizeof(float) );

   // test
   obj->vertelem = (vertelem_t*)malloc(4*sizeof(vertelem_t));


   obj->posIndices = (unsigned short*)calloc( start_size, sizeof(unsigned short) );
   obj->normIndices = (unsigned short*)calloc( start_size, sizeof(unsigned short) );
	obj->texcoordIndices = (unsigned short*)calloc( start_size, sizeof(unsigned short) );
   obj->start_pos[0] = x;
   obj->start_pos[1] = y;
   obj->start_pos[2] = z;

	// 3 attributes, each with its own indices
   obj->numNorms = obj->numNormsIndices = 0;
   obj->numPos = obj->numPosIndices = 0;
   obj->numTexcoords = obj->numTexcoordsIndices = 0;
	
	retval = ReadVertexData( &name, obj );

   AdjustToZeroIndexing( obj );
	
	return obj;
}


void DeleteObj( obj_t *obj )
{
	free( obj->vertexPos );
   free( obj->vertexTexcoords );
   free( obj->vertexNorms );

   free( obj->posIndices );
   free( obj->texcoordIndices );
   free( obj->normIndices );


	free( obj );
}