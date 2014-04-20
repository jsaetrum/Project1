// whats going on here: break vbo functions into more edible pieces
void OGL_InitPositionVBO( obj_t **objects, int mesh_bytes )
{
   int i, offset;
   // BUFFER FOR VERTEX mesh ( attribute 0 )
   glGenBuffers( 1, &verticesVbo );
	glBindBuffer( GL_ARRAY_BUFFER, verticesVbo );

   offset = 0;
   // use bufferdata as malloc() , because multiple calls to this will reallocate and discard previously allocated data
   glBufferData( GL_ARRAY_BUFFER, mesh_bytes , 0, GL_STREAM_DRAW );
	for( i = 0; i < NUMOBJECTS; i++)
   {    
      glBufferSubData( GL_ARRAY_BUFFER, offset, objects[i]->numPos*sizeof(float), objects[i]->vertexPos );
      offset += objects[i]->numPos * sizeof(float);
   }
   
   glEnableVertexAttribArray( 0 );
   glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, NULL );
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
}

void OGL_InitTextureVBO( obj_t **objects, int texcoords_bytes )
{
   int i, offset;
   // BUFFER FOR VERTEX mesh ( attribute 0 )
   glGenBuffers( 1, &vt_vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vt_vbo );

   offset = 0;
   // use bufferdata as malloc() , because multiple calls to this will reallocate and discard previously allocated data
   glBufferData( GL_ARRAY_BUFFER, texcoords_bytes , 0, GL_STREAM_DRAW );
	for( i = 0; i < NUMOBJECTS; i++)
   {    
      glBufferSubData( GL_ARRAY_BUFFER, offset, objects[i]->numTexcoords*sizeof(float), objects[i]->vertexTexcoords );
      offset += objects[i]->numTexcoords * sizeof(float);
   }
   
   glEnableVertexAttribArray( 1 );
   glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 0, NULL );
   glBindBuffer( GL_ARRAY_BUFFER, 0 );
}