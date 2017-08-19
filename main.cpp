#include<iostream>

#include "Camera/vCamera.h"
#include "Transforms/Transforms.h"
#include "Window/Display.h"
#include "Window/Fps.h"
#include "Input/input.h"
#include "Texture/TextureGL.h"
#include "shaders/Shader.h"
#include "Geometry/Mesh.h"


#include "util.h"

int main(void)
{

	//3- reconstruc and render
	Fps FramePerSc;
	input in;
	in.HideMouse(false);
	Display _display(512,512,"MVS visualization");


	Shader _Simple("shader programs/phong");


	GLint attribut1[3];




	attribut1[0] = _Simple.GetAttributeLocation("position");
	attribut1[1] = _Simple.GetAttributeLocation("texCoord");
	attribut1[2] = _Simple.GetAttributeLocation("normal");


	Mesh Sphere2("resources/Models/smvs2.obj",attribut1);

	Transforms modelpos;
	Transforms modelpos2;

	glm::vec3 CamPos    = glm::vec3(0.865511775, -5.935105801, 38.43729019);
	glm::vec3 CamFront  = glm::vec3(0.0,0.0,1.0);
	vCamera   camera(CamPos,glm::vec3(0.0,0.0,0.0),60.0f,512/512,0.01f,10000.0f);





	float phi   = 0.0;
	float theta = -90.0;
	float mouseSpeed = 1.8;
	float walkspeed  = 4.0;




	FramePerSc.Start(); // start counting fps
	while(!in.QUIT())
	{
		_display.Clear(0.0f,0.0,0.0f,0.0f);
		in.updateEvents();

		if(in.mouseMotion() && in.getMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		{
			// calculate the new angle across the vertical and horizantal vectors
			phi   += in.getMouseY() * mouseSpeed * FramePerSc.deltatime();
			theta += in.getMouseX() * mouseSpeed * FramePerSc.deltatime();

			if(phi > 89.0f)
				phi =  89.0f;
			if(phi < -89.0f)
				phi = -89.0f;

			float phiRadian   = DegreeToRad(phi);
			float thetaRadian = DegreeToRad(theta);

			 CamFront = glm::normalize(glm::vec3(
				cos(phiRadian) * cos(thetaRadian),
			    sin(phiRadian),
			    cos(phiRadian) * sin(thetaRadian)
			));


				camera.setTarget(CamPos + CamFront);
				camera.ViewMatrixUpdate();
			//modelpos.rotate(phiRadian,thetaRadian,0.0);
			//modelpos.update();
			//camera.setModelMatrix(modelpos.getModelMatrix());
		}

		if(in.getKey(SDL_SCANCODE_UP) == KEY_DOWN)
		{
			//walkspeed++;
			//modelpos.translate(0.0,0.0, walkspeed * FramePerSc.deltatime());
			//modelpos.update();
			//camera.setModelMatrix(modelpos.getModelMatrix());

			CamPos += walkspeed * FramePerSc.deltatime() * CamFront;
			camera.setPosition(CamPos);
			camera.setTarget(CamPos + CamFront);
			camera.ViewMatrixUpdate();
		}

		if(in.getKey(SDL_SCANCODE_DOWN) == KEY_DOWN)
		{
			//walkspeed--;
			//modelpos.translate(0.0,0.0,walkspeed * FramePerSc.deltatime());
			//modelpos.update();
			//camera.setModelMatrix(modelpos.getModelMatrix());
			CamPos -=  walkspeed * FramePerSc.deltatime() * CamFront;
			camera.setPosition(CamPos);
			camera.setTarget(CamPos + CamFront);
			camera.ViewMatrixUpdate();
		}

		if(in.getKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		{
			//walkspeed++;
			//modelpos.translate(0.0,0.0, walkspeed * FramePerSc.deltatime());
			//modelpos.update();
			//camera.setModelMatrix(modelpos.getModelMatrix());

			CamPos += glm::normalize(glm::cross(CamFront, glm::vec3(0.0,1.0,0.0))) * walkspeed * FramePerSc.deltatime();
			camera.setPosition(CamPos);
			camera.setTarget(CamPos + CamFront);
			camera.ViewMatrixUpdate();
		}

		if(in.getKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			//walkspeed--;
			//modelpos.translate(0.0,0.0,walkspeed * FramePerSc.deltatime());
			//modelpos.update();
			//camera.setModelMatrix(modelpos.getModelMatrix());
			CamPos -= glm::normalize(glm::cross(CamFront, glm::vec3(0.0,1.0,0.0))) * walkspeed * FramePerSc.deltatime();
			camera.setPosition(CamPos);
			camera.setTarget(CamPos + CamFront);
			camera.ViewMatrixUpdate();
		}


		if(in.getKey(SDL_SCANCODE_Q) == KEY_DOWN)
		{
			 in.exit();
		}


		modelpos2.rotate(0.0,0.0,90.0);
		modelpos2.update();
		camera.setModelMatrix(modelpos2.getModelMatrix());
		camera.update();

		_Simple.bind();
			Sphere2.Draw();

			if(in.getKey(SDL_SCANCODE_S) == KEY_DOWN)
			{

				BYTE img[3 * 512 * 512];
				glReadBuffer(GL_BACK);
				glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, img);

				// write this images down
				FIBITMAP* image = FreeImage_ConvertFromRawBits(img, 512, 512, 3*512, 24, 0xFF0000, 0x00FF00, 0x0000FF, false);
				FreeImage_Save(FIF_BMP, image, "resources/output.bmp", 0);


			}

		_Simple.Update(camera);
		_Simple.deleteShader();
		FramePerSc.Count();// count
		_display.Update();
	}
	return 0;
}
