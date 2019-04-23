#include "pipeline.h"

using namespace std;

using namespace glm;

bool objetoloader(objeto * obj,char* path){

	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path, 0/*aiProcess_JoinIdenticalVertices | aiProcess_SortByPType*/);
	if( !scene) {
		fprintf( stderr, importer.GetErrorString());
		getchar();
		return false;
	}
	const aiMesh* mesh = scene->mMeshes[0]; // In this simple example code we always use the 1rst mesh (in OBJ files there is often only one anyway)

	// Fill vertices positions
	obj->vertices.reserve(mesh->mNumVertices);
	for(unsigned int i=0; i<mesh->mNumVertices; i++){
		aiVector3D pos = mesh->mVertices[i];
		obj->vertices.push_back(vec4(pos.x, pos.y, pos.z,1));
	}


	// Fill face indices
	obj->indices.reserve(3*mesh->mNumFaces);
	for (unsigned int i=0; i<mesh->mNumFaces; i++){
		// Assume the model has only triangles.
		obj->indices.push_back(vec2(mesh->mFaces[i].mIndices[0],mesh->mFaces[i].mIndices[1]));
       		obj->indices.push_back(vec2(mesh->mFaces[i].mIndices[1],mesh->mFaces[i].mIndices[2]));
	}

	return true;
}





void pipeline(struct objeto* obj){


    //Model  Esp. Obj. --> Esp. Univ. 
    //Rotação
    float Angle = 45.0f;

    mat4 x_Rotetion = mat4(1,0,0,0,
                           0,cos(Angle),-sin(Angle),0,
                           0,sin(Angle),cos(Angle),0,
                           0,0,0,1 );

    mat4 y_Rotetion = mat4(cos(Angle),0,sin(Angle),0,
                           0,1,0,0,
                           -sin(Angle),0,cos(Angle),0,
                           0,0,0,1 );

    mat4 z_Rotetion = mat4(cos(Angle),-sin(Angle),0,0,
                           sin(Angle),cos(Angle),0,0,
                           0,0,1,0,
                           0,0,0,1 );



    //Shear
    float mx = 0.0f;
    float my = 0.0f;
    
    mat4 shear = mat4(    1,0,mx,0,
                          0,1,my,0,
                          0,0,1,0,
                          0,0,0,1 );



    //Escala
    float sx = 1.5f;
    float sy = 1.5f;
    float sz = 1.5f;
    mat4 scale = mat4(    sx,0,0,0,
                          0,sy,0,0,
                          0,0,sz,0,
                          0,0,0,1 );




    //Translação
    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;
    mat4 translate = mat4(1,0,0,dx,
                          0,1,0,dy,
                          0,0,1,dz,
                          0,0,0,1 );


                        

    mat4 M_model= scale*y_Rotetion;  


    //Parametros da camera

    vec3 camera_pos =vec3(0,0,5);   //Posicao da camera no universo.
    vec3 camera_lookat =vec3(0,0,0);  // Ponto para onde a camera esta olhando.
    vec3 camera_up = vec3(0,1,0);  // 'up' da camera no espaco do universo.



    //Calculo do sistema ortonormal gerado a partir dos parametros da camera

    vec3 camera_dir = vec3(camera_lookat - camera_pos);

    vec3 z_camera = normalize(-(camera_dir));
    vec3 x_camera = normalize(cross(camera_up,z_camera));
    vec3 y_camera = cross(z_camera,x_camera);

    //Construcao da matriz view: Esp. Univ. --> Esp. Cam.

    mat4 Bt  =  mat4(x_camera.x,x_camera.y,x_camera.z,0,
                    y_camera.x,y_camera.y,y_camera.z,0,
                    z_camera.x,z_camera.y,z_camera.z,0,
                    0,0,0,1);

    mat4 T = mat4(1,0,0,-camera_pos.x,
                0,1,0,-camera_pos.y,
                0,0,1,-camera_pos.z,
                0,0,0,1);

    mat4 M_view =  T * Bt ; 

   
    //Construcao da matriz de Projecao: Esp. Cam. --> Esp. Recorte

    float d = 2 ; // distância do centro de projeção para o viewplane

    mat4 M_projection = mat4(1,0,0,0,
                             0,1,0,0,
                             0,0,1,d,
                             0,0,-(1/d),0);

    
    //contrução da M_viewport Esp. Canônico --> o Esp. de tela.

    mat4 Scale_1 = mat4(1,0,0,0,
                        0,-1,0,0,
                        0,0,1,0,
                        0,0,0,1);



    mat4 traslete_1 = mat4( 1,0,0,1,
                            0,1,0,1,
                            0,0,1,0,
                            0,0,0,1);



    int w = 512;
    int h = 512;

    mat4 Scale_2= mat4(w/2,0,0,0,
                       0,h/2,0,0,
                       0,0,1,0,
                       0,0,0,1);




    mat4 M_viewport =  Scale_1*traslete_1*Scale_2;
 
    //Construcao da matriz de ModelView: Esp. Obj. --> Esp. Cam.

    mat4 M_model_view =  M_model * M_view ;

   //Construcao da matriz ModelViewProjection: Esp. Obj. --> Esp. Recorte

    mat4 M_model_view_proj =M_model_view * M_projection ;


    for(int i = 0; i < obj->vertices.size(); i++){
        
        obj->vertices[i] = vec4(obj->vertices[i] * M_model_view_proj );
    }

     //Esp. Recorte --> Esp. Canonico
    for(int i = 0; i < obj->vertices.size(); i++){
        
        obj->vertices[i] = vec4(obj->vertices[i] / obj->vertices[i].w);
    }


    //Esp. Canonico --> Esp. de Tela

    for(int i = 0; i < obj->vertices.size(); i++){
        
        obj->vertices[i] = vec4(floor(obj->vertices[i] * M_viewport) );
    }  



};


