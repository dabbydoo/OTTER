/*
Week 5 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "CParticleSystem.h"
#include "NOU/GLTFLoader.h"
#include <cmath>;
#include "imgui.h"
#include <iostream>
#include <memory>
#include <ctime>

using namespace nou;

//Forward declaring our global resources for this demo.
std::unique_ptr<ShaderProgram> prog_texLit, prog_particles;
std::unique_ptr<Mesh> duckMesh, oceanMesh;
std::unique_ptr<Texture2D> duckTex, particleTex, oceanTex, particleWaterTex;
std::unique_ptr<Material> duckMat, particleMat, oceanMat, particleWaterMat;

//This function will load in our global resources.
//(It's only been separated to make main() a bit cleaner to look at.)
void LoadDefaultResources();

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	App::Init("Week 5 Tutorial - Particles", 800, 800);
	App::SetClearColor(glm::vec4(0.67f, 0.84f, 0.9f, 1.0f));

	App::InitImgui();

	LoadDefaultResources();

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);
	camEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//Creating the duck entity.
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, *duckMesh, *duckMat);
	duckEntity.transform.m_scale = glm::vec3(0.1f, 0.1f, 0.1f);
	duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Creating the duck entity2.
	Entity duckEntity2 = Entity::Create();
	duckEntity2.Add<CMeshRenderer>(duckEntity2, *duckMesh, *duckMat);
	duckEntity2.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity2.transform.m_pos = glm::vec3(0.0f, -1.0f, -2.0f);
	duckEntity2.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Creating the duck entity3.
	Entity duckEntity3 = Entity::Create();
	duckEntity3.Add<CMeshRenderer>(duckEntity3, *duckMesh, *duckMat);
	duckEntity3.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity3.transform.m_pos = glm::vec3(-2.0f, -1.0f, -4.0f);
	duckEntity3.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Creating the duck entity4.
	Entity duckEntity4 = Entity::Create();
	duckEntity4.Add<CMeshRenderer>(duckEntity4, *duckMesh, *duckMat);
	duckEntity4.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity4.transform.m_pos = glm::vec3(-10.0f, -1.0f, -8.0f);
	duckEntity4.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Creating the duck entity5.
	Entity duckEntity5 = Entity::Create();
	duckEntity5.Add<CMeshRenderer>(duckEntity5, *duckMesh, *duckMat);
	duckEntity5.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity5.transform.m_pos = glm::vec3(-8.0f, -1.0f, -15.0f);
	duckEntity5.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//Creating the duck entity6.
	Entity duckEntity6 = Entity::Create();
	duckEntity6.Add<CMeshRenderer>(duckEntity6, *duckMesh, *duckMat);
	duckEntity6.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	duckEntity6.transform.m_pos = glm::vec3(-15.0f, -1.0f, -15.0f);
	duckEntity6.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	
	// Creating the ocean entity.
	Entity oceanEntity = Entity::Create();
	oceanEntity.Add<CMeshRenderer>(oceanEntity, *oceanMesh, *oceanMat);
	oceanEntity.transform.m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	oceanEntity.transform.m_pos = glm::vec3(0.0f, -2.0f, 15.0f);
	oceanEntity.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//TODO: Set up our particle system.
	ParticleParam particleData;
	particleData.lifetime = 1.5f;
	particleData.startColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	particleData.endColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	particleData.startSize = 0.3f;
	particleData.maxParticles = 200;
	particleData.emissionRate = 100.0f;
	particleData.tanTheta = glm::tan(glm::radians(30.0f));

	ParticleParam particleDataWater;
	particleDataWater.lifetime = 1.5f;
	particleDataWater.startColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	particleDataWater.endColor = glm::vec4(0.94f, 0.97f, 1.0f, 0.0f);
	particleDataWater.startSize = 0.3f;
	particleDataWater.maxParticles = 200;
	particleDataWater.emissionRate = 100.0f;
	particleDataWater.tanTheta = glm::tan(glm::radians(30.0f));
	
	//Set up our particle system for duck enemies
	ParticleParam particleData2;
	particleData2.lifetime = 1.5f;
	particleData2.startColor = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
	particleData2.endColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	particleData2.startSize = 0.7f;
	particleData2.maxParticles = 200;
	particleData2.emissionRate = 200.0f;
	particleData2.tanTheta = glm::tan(glm::radians(30.0f));

	Entity particleEntity = Entity::Create();
	particleEntity.transform.m_pos = glm::vec3(0.2f, -0.2f, 0.0f);
	particleEntity.Add<CParticleSystem>(particleEntity, *particleMat, particleData);

	Entity particleWaterEntity = Entity::Create();
	particleWaterEntity.transform.m_pos = glm::vec3(0.2f, -0.2f, 0.0f);
	particleWaterEntity.Add<CParticleSystem>(particleWaterEntity, *particleWaterMat, particleDataWater);

	Entity particleEntity2 = Entity::Create();
	particleEntity2.transform.m_pos = glm::vec3(0.2f, -0.2f, 0.0f);
	particleEntity2.Add<CParticleSystem>(particleEntity2, *particleMat, particleData2);

	Entity particleEntity3 = Entity::Create();
	particleEntity3.transform.m_pos = glm::vec3(0.2f, -0.2f, 0.0f);
	particleEntity3.Add<CParticleSystem>(particleEntity3, *particleMat, particleData2);

	Entity particleEntity4 = Entity::Create();
	particleEntity4.transform.m_pos = glm::vec3(0.2f, -0.2f, 0.0f);
	particleEntity4.Add<CParticleSystem>(particleEntity4, *particleMat, particleData2);

	Entity particleEntity5 = Entity::Create();
	particleEntity5.transform.m_pos = glm::vec3(0.2f, -0.2f, 0.0f);
	particleEntity5.Add<CParticleSystem>(particleEntity5, *particleMat, particleData2);

	Entity particleEntity6 = Entity::Create();
	particleEntity6.transform.m_pos = glm::vec3(0.2f, -0.2f, 0.0f);
	particleEntity6.Add<CParticleSystem>(particleEntity6, *particleMat, particleData2);

	//Direction where duck is facing
	float facing = 0; //0 = right, 1 = in - right , 2 = in, 3 = in - left, 4 = left, 5 = left - out, 6 = out, 7 = right - out

	//Enemy duck burning
	bool onFire1 = false;
	bool onFire2 = false;
	bool onFire3 = false;
	bool onFire4 = false;
	bool onFire5 = false;

	bool useWater = false;
	float* slider;

	App::Tick();
	
	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();
		
		//Updates the camera.
		camEntity.Get<CCamera>().Update();


		
		/*rot = rot + 0.005;
		particleEntity.transform.m_rotation = glm::vec3(0.0f, 0.0f, rot);*/


		duckEntity.transform.RecomputeGlobal();
		duckEntity2.transform.RecomputeGlobal();
		duckEntity3.transform.RecomputeGlobal();
		duckEntity4.transform.RecomputeGlobal();
		duckEntity5.transform.RecomputeGlobal();
		duckEntity6.transform.RecomputeGlobal();
		oceanEntity.transform.RecomputeGlobal();
		

		//We draw particles with the depth buffer disabled
		//to prevent z-fighting.
		//Custom depth sorting is handled by our particle system.
		
		
		duckEntity.Get<CMeshRenderer>().Draw();
		duckEntity2.Get<CMeshRenderer>().Draw();
		duckEntity3.Get<CMeshRenderer>().Draw();
		duckEntity4.Get<CMeshRenderer>().Draw();
		duckEntity5.Get<CMeshRenderer>().Draw();
		duckEntity6.Get<CMeshRenderer>().Draw();

		oceanEntity.Get<CMeshRenderer>().Draw();

		
		/*std::cout << "PLAYERPOS X: " << duckEntity.transform.m_pos.x << std::endl;
		std::cout << "PLAYERPOS Z: " << duckEntity.transform.m_pos.z << std::endl;
		std::cout << "ENEMYPOS X: " << duckEntity2.transform.m_pos.x << std::endl;
		std::cout << "ENEMYPOS Z: " << duckEntity2.transform.m_pos.z << std::endl;*/
		

		camEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x, duckEntity.transform.m_pos.y + 2, duckEntity.transform.m_pos.z + 4.0f);

		auto rotate = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		
		//Left
		if (Input::GetKey(GLFW_KEY_A) && !Input::GetKey(GLFW_KEY_D) && !Input::GetKey(GLFW_KEY_W) && !Input::GetKey(GLFW_KEY_S))
		{
			facing = 4;

			duckEntity.transform.m_pos.x = duckEntity.transform.m_pos.x - 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			
		}

		//Right
		if (Input::GetKey(GLFW_KEY_D) && !Input::GetKey(GLFW_KEY_A) && !Input::GetKey(GLFW_KEY_W) && !Input::GetKey(GLFW_KEY_S))
		{
			facing = 0;

			duckEntity.transform.m_pos.x = duckEntity.transform.m_pos.x + 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			
		}


		//In
		if (Input::GetKey(GLFW_KEY_W) && !Input::GetKey(GLFW_KEY_D) && !Input::GetKey(GLFW_KEY_A) && !Input::GetKey(GLFW_KEY_S))
		{
			facing = 2;

			duckEntity.transform.m_pos.z = duckEntity.transform.m_pos.z - 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			
		}

		//Out
		if (Input::GetKey(GLFW_KEY_S) && !Input::GetKey(GLFW_KEY_D) && !Input::GetKey(GLFW_KEY_W) && !Input::GetKey(GLFW_KEY_A))
		{
			facing = 6;

			duckEntity.transform.m_pos.z = duckEntity.transform.m_pos.z + 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f));

			
		}

		//MULTI-PRESS
		if (Input::GetKey(GLFW_KEY_D) && Input::GetKey(GLFW_KEY_W))
		{
			facing = 1;

			duckEntity.transform.m_pos.x = duckEntity.transform.m_pos.x + 0.05;
			duckEntity.transform.m_pos.z = duckEntity.transform.m_pos.z - 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (Input::GetKey(GLFW_KEY_A) && Input::GetKey(GLFW_KEY_W))
		{
			facing = 3;

			duckEntity.transform.m_pos.x = duckEntity.transform.m_pos.x - 0.05;
			duckEntity.transform.m_pos.z = duckEntity.transform.m_pos.z - 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (Input::GetKey(GLFW_KEY_A) && Input::GetKey(GLFW_KEY_S))
		{
			facing = 5;

			duckEntity.transform.m_pos.x = duckEntity.transform.m_pos.x - 0.05;
			duckEntity.transform.m_pos.z = duckEntity.transform.m_pos.z + 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(235.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		if (Input::GetKey(GLFW_KEY_D) && Input::GetKey(GLFW_KEY_S))
		{
			facing = 7;

			duckEntity.transform.m_pos.z = duckEntity.transform.m_pos.z + 0.05;
			duckEntity.transform.m_pos.x = duckEntity.transform.m_pos.x + 0.05;
			duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(315.0f), glm::vec3(0.0f, 1.0f, 0.0f));;
		}

		if (Input::GetKey(GLFW_KEY_SPACE))
		{
			if (!useWater)
			{
				if (abs(duckEntity.transform.m_pos.x - duckEntity2.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity2.transform.m_pos.z) < 2.0f)
				{
					onFire1 = true;
					particleEntity2.transform.m_pos = duckEntity2.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity3.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity3.transform.m_pos.z) < 2.0f)
				{
					onFire2 = true;
					particleEntity3.transform.m_pos = duckEntity3.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity4.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity4.transform.m_pos.z) < 2.0f)
				{
					onFire3 = true;
					particleEntity4.transform.m_pos = duckEntity4.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity5.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity5.transform.m_pos.z) < 2.0f)
				{
					onFire4 = true;
					particleEntity5.transform.m_pos = duckEntity5.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity6.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity6.transform.m_pos.z) < 2.0f)
				{
					onFire5 = true;
					particleEntity6.transform.m_pos = duckEntity6.transform.m_pos;
				}
			}

			//
			if (useWater)
			{
				if (abs(duckEntity.transform.m_pos.x - duckEntity2.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity2.transform.m_pos.z) < 2.0f)
				{
					onFire1 = false;
					particleEntity2.transform.m_pos = duckEntity2.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity3.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity3.transform.m_pos.z) < 2.0f)
				{
					onFire2 = false;
					particleEntity3.transform.m_pos = duckEntity3.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity4.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity4.transform.m_pos.z) < 2.0f)
				{
					onFire3 = false;
					particleEntity4.transform.m_pos = duckEntity4.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity5.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity5.transform.m_pos.z) < 2.0f)
				{
					onFire4 = false;
					particleEntity5.transform.m_pos = duckEntity5.transform.m_pos;
				}

				if (abs(duckEntity.transform.m_pos.x - duckEntity6.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity6.transform.m_pos.z) < 2.0f)
				{
					onFire5 = false;
					particleEntity6.transform.m_pos = duckEntity6.transform.m_pos;
				}
			}
			//if (abs(duckEntity.transform.m_pos.x - duckEntity2.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity2.transform.m_pos.z) < 2.0f)
			//{
			//	onFire1 = true;
			//	particleEntity2.transform.m_pos = duckEntity2.transform.m_pos;
			//}

			//if (abs(duckEntity.transform.m_pos.x - duckEntity2.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity2.transform.m_pos.z) < 2.0f)
			//{
			//	onFire1 = true;
			//	particleEntity2.transform.m_pos = duckEntity2.transform.m_pos;
			//}

			//if (abs(duckEntity.transform.m_pos.x - duckEntity2.transform.m_pos.x) < 2.0f && abs(duckEntity.transform.m_pos.z - duckEntity2.transform.m_pos.z) < 2.0f)
			//{
			//	onFire1 = true;
			//	particleEntity2.transform.m_pos = duckEntity2.transform.m_pos;
			//}

			//right
			if (facing == 0)
			{
				if (!useWater)
				{
					particleEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x + 0.55, -0.37f, duckEntity.transform.m_pos.z);
					particleEntity.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				}
				if (useWater)
				{
					particleWaterEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x + 0.55, -0.37f, duckEntity.transform.m_pos.z);
					particleWaterEntity.transform.m_rotation = glm::angleAxis(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				}
			}
			//right-in
			if (facing == 1)
			{
				/*particleEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x + 0.55, -0.37f, duckEntity.transform.m_pos.z);
				particleEntity.transform.m_rotation.x = glm::radians(-90.0f);
				particleEntity.transform.m_rotation.y = glm::radians(-45.0f);*/
			}
			//in
			if (facing == 2)
			{
				if (!useWater)
				{
					particleEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x, -1.05f, duckEntity.transform.m_pos.z - 3.8);
					particleEntity.transform.m_rotation = glm::angleAxis(glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, .0f));
				}
				if (useWater)
				{
					particleWaterEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x, -1.05f, duckEntity.transform.m_pos.z - 3.8);
					particleWaterEntity.transform.m_rotation = glm::angleAxis(glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, .0f));
				}
			}
			//left-in
			if (facing == 3)
			{

			}
			//left
			if (facing == 4)
			{
				if (!useWater)
				{
					particleEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x - 0.55, -0.37f, duckEntity.transform.m_pos.z);
					particleEntity.transform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				}
				if (useWater)
				{
					particleWaterEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x - 0.55, -0.37f, duckEntity.transform.m_pos.z);
					particleWaterEntity.transform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
				}
			}
			//left-out
			if (facing == 5)
			{

			}
			//out
			if (facing == 6)
			{
				if (!useWater)
				{
					particleData.emissionRate = 200.0f;
					particleEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x + 0.2f, -2.0f, duckEntity.transform.m_pos.z - 3.8);
					particleEntity.transform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, .0f));
				}
				if (useWater)
				{
					particleDataWater.emissionRate = 200.0f;
					particleWaterEntity.transform.m_pos = glm::vec3(duckEntity.transform.m_pos.x + 0.2f, -2.0f, duckEntity.transform.m_pos.z - 3.8);
					particleWaterEntity.transform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, .0f));
				}
			}
			//right-out
			if (facing == 7)
			{

			}
			if (!useWater)
			{
				particleEntity.Get<CParticleSystem>().Update(deltaTime);
				particleEntity.transform.RecomputeGlobal();
				glDisable(GL_DEPTH_TEST);
				particleEntity.Get<CParticleSystem>().Draw();
				glEnable(GL_DEPTH_TEST);
			}
			else
			{
				particleWaterEntity.Get<CParticleSystem>().Update(deltaTime);
				particleWaterEntity.transform.RecomputeGlobal();
				glDisable(GL_DEPTH_TEST);
				particleWaterEntity.Get<CParticleSystem>().Draw();
				glEnable(GL_DEPTH_TEST);
			}
		}

		if (onFire1)
		{
			particleEntity2.Get<CParticleSystem>().Update(deltaTime);
			particleEntity2.transform.RecomputeGlobal();
			glDisable(GL_DEPTH_TEST);
			particleEntity2.Get<CParticleSystem>().Draw();
			glEnable(GL_DEPTH_TEST);
		}


		if (onFire2)
		{
			particleEntity3.Get<CParticleSystem>().Update(deltaTime);
			particleEntity3.transform.RecomputeGlobal();
			glDisable(GL_DEPTH_TEST);
			particleEntity3.Get<CParticleSystem>().Draw();
			glEnable(GL_DEPTH_TEST);
		}


		if (onFire3)
		{
			particleEntity4.Get<CParticleSystem>().Update(deltaTime);
			particleEntity4.transform.RecomputeGlobal();
			glDisable(GL_DEPTH_TEST);
			particleEntity4.Get<CParticleSystem>().Draw();
			glEnable(GL_DEPTH_TEST);
		}


		if (onFire4)
		{
			particleEntity5.Get<CParticleSystem>().Update(deltaTime);
			particleEntity5.transform.RecomputeGlobal();
			glDisable(GL_DEPTH_TEST);
			particleEntity5.Get<CParticleSystem>().Draw();
			glEnable(GL_DEPTH_TEST);
		}


		if (onFire5)
		{
			particleEntity6.Get<CParticleSystem>().Update(deltaTime);
			particleEntity6.transform.RecomputeGlobal();
			glDisable(GL_DEPTH_TEST);
			particleEntity6.Get<CParticleSystem>().Draw();
			glEnable(GL_DEPTH_TEST);
		}

		/*particleEntity.transform.m_pos.x = duckEntity.transform.m_pos.x;
		particleEntity.transform.m_pos.z = duckEntity.transform.m_pos.z;*/

		//For Imgui...
		App::StartImgui(); 
		
		ImGui::Text("Current Mode (Press to toggle): "); 
		ImGui::SameLine(); 
		if (!useWater)
		{
			if (ImGui::Button("Fire"))
			{
				useWater = true;
			}
		}
		if (useWater)
		{
			if (ImGui::Button("Water"))
			{
				useWater = false;
			}
		}
		
		//Put any Imgui controls you plan to use for your
		//particle system here (for the exercise).

		App::EndImgui();  

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0;
}

void LoadDefaultResources()
{
	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	//Lit and textured shader program.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> texLit = { v_texLit.get(), f_texLit.get() };
	prog_texLit = std::make_unique<ShaderProgram>(texLit);

	//Billboarded particles shader program.
	auto v_particles = std::make_unique<Shader>("shaders/particles.vert", GL_VERTEX_SHADER);
	auto g_particles = std::make_unique<Shader>("shaders/particles.geom", GL_GEOMETRY_SHADER);
	auto f_particles = std::make_unique<Shader>("shaders/particles.frag", GL_FRAGMENT_SHADER);

	std::vector<Shader*> particles = { v_particles.get(), g_particles.get(), f_particles.get() };
	prog_particles = std::make_unique<ShaderProgram>(particles);

	//Load in the ducky model.
	duckMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("duck/playerShip.gltf", *duckMesh);

	//Load in the ocean model.
	oceanMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("ocean/Ocean.gltf", *oceanMesh);

	//Load in textures.
	duckTex = std::make_unique<Texture2D>("duck/DuckCM.png");
	particleTex = std::make_unique<Texture2D>("fire.png");
	particleWaterTex = std::make_unique<Texture2D>("water.png");
	oceanTex = std::make_unique<Texture2D>("ocean/oceanTex.png");
	 
	//Make materials. 
	duckMat = std::make_unique<Material>(*prog_texLit);
	duckMat->AddTexture("albedo", *duckTex);

	oceanMat = std::make_unique<Material>(*prog_texLit);
	oceanMat->AddTexture("albedo", *oceanTex);

	particleMat = std::make_unique<Material>(*prog_particles);
	particleMat->AddTexture("albedo", *particleTex);

	particleWaterMat = std::make_unique<Material>(*prog_particles);
	particleWaterMat->AddTexture("albedo", *particleWaterTex);
}