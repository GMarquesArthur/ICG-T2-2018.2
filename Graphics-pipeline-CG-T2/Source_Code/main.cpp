#include "main.h"
#include "color.h"
#include "pipeline.h"
#include <vector>
#include <stdio.h>
using namespace glm;



//-----------------------------------------------------------------------------

void drawObj(objeto obj, color c){

	for(int i = 0;i < obj.indices.size();i++)
		Reta(int(obj.vertices[obj.indices[i][0]][0]),int(obj.vertices[obj.indices[i][0]][1]),int(obj.vertices[obj.indices[i][1]][0]),int(obj.vertices[obj.indices[i][1]][1]),c);
}

void MyGlDraw(void)
{

	color c;
		c.r = 255;
		c.g = 255;
		c.b = 255;
		c.a = 255;

		c.rf = 255;
		c.gf = 255;
		c.bf = 255;
		c.af = 255;

objeto cube;

objetoloader(&cube,"monkey.obj");

pipeline(&cube);
	

drawObj(cube,c);

}


//-----------------------------------------------------------------------------
int main(int argc, char **argv){
	// Inicializações.
	InitOpenGL(&argc, argv);
	InitCallBacks();
	InitDataStructures();

	// Ajusta a função que chama as funções do mygl.h

	DrawFunc = MyGlDraw;	

	// Framebuffer scan loop.
	glutMainLoop();

	return 0;
}

