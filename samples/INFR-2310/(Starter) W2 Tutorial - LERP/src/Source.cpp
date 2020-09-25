/*
Week 2 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/GLTFLoader.h"

#include "Logging.h"

#include <memory>

using namespace nou;

//TODO: Write templated LERP function.
//LERP function
template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

int main() 
{
	App::Init("Week 1 Tutorial - LERP", 1000, 1000);
	App::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	//Load in some shaders.
	//Smart pointers will automatically deallocate memory when they go out of scope.
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	auto prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	//Load duck model and texture
	Mesh duckMesh;
	GLTF::LoadMesh("duck/Duck.gltf", duckMesh);

	Material duckMat(prog_texLit);
	Texture2D duckTex = Texture2D("duck/DuckCM.png");
	
	duckMat.AddTexture("albedo", duckTex);


	//Load bot model and texture
	Mesh botMesh;
	GLTF::LoadMesh("duck/bot.gltf", botMesh);

	Material botMat(prog_texLit);


	Texture2D botBase = Texture2D("duck/color.png");
	botMat.AddTexture("botBase", botBase);


	//Set up camera entity and transform
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 4.0f);

	//Create duck entity
	Entity duckEntity = Entity::Create();
	duckEntity.Add<CMeshRenderer>(duckEntity, duckMesh, duckMat);
	//duckEntity.transform.m_scale = glm::vec3(0.005f, 0.005f, 0.005f);
	//duckEntity.transform.m_pos = glm::vec3(0.0f, -1.0f, 0.0f);
	//duckEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	//DUCK
	//Variables for lerping position
	glm::vec3 origPos = glm::vec3(0.0f, -1.0f, 0.0f), newPos = glm::vec3(0.0f);
	float posTimer = 0.0f, posTimeLimit = 2.5f;
	bool duckyGoPlaces = true;
	//variable for lerping color
	glm::vec3 origCol = glm::vec3(1.0f, 1.0f, 1.0f), newCol = glm::vec3(0.0f, 1.0f, 0.0f);
	float colTimer = 0.0f, colTimeLimit = 5.0f;
	bool duckyGoColor = true;
	//variable for lerping rotate
	glm::vec3 origRot = glm::vec3(0.0f, -1.5f, 0.0f), newRot = glm::vec3(0.0f, 4.5f, 0.0f);
	float rotTimer = 0.0f, rotTimeLimit = 2.5f;
	bool duckyGoRotate = true;
	//variable for lerping scale
	glm::vec3 origSca = glm::vec3(0.0005f, 0.0005f, 0.0005f), newSca = glm::vec3(0.01f, 0.01f, 0.01f);
	float scaTimer = 0.0f, scaTimeLimit = 5.0f;
	bool duckyGoScale = true;

	//Create Bot entity
	Entity botEntity = Entity::Create();
	botEntity.Add<CMeshRenderer>(botEntity, botMesh, botMat);
	/*botEntity.transform.m_scale = glm::vec3(0.015f, 0.015f, 0.015f);
	botEntity.transform.m_pos = glm::vec3(0.0f, -2.0f, -3.0f);
	botEntity.transform.m_rotation = glm::angleAxis(glm::radians(-30.0f), glm::vec3(-0.015f, 0.015f, 0.0f));*/

	//Bot
	//Variables for lerping position
	glm::vec3 botOriPos = glm::vec3(0.0f, -2.0f, -4.0f), botNewPos = glm::vec3(0.0f, 2.0f, -4.0f);
	float botPosTimer = 0.0f, botPosTimeLimit = 2.5f;
	bool botGo = true;
	//variable for lerping color
	glm::vec3 botOrigCol = glm::vec3(1.0f, 1.0f, 1.0f), botNewCol = glm::vec3(1.0f, 1.0f, 0.0f);
	float botColTimer = 0.0f, botColTimeLimit = 2.0f;
	bool botGoColor = true;
	//variable for lerping rotate
	glm::vec3 botOrigRot = glm::vec3(2.0f, 0.0f, 0.0f), botNewRot = glm::vec3(-2.0f, 0.0f, -2.0f);
	float botRotTimer = 0.0f, botRotTimeLimit = 5.0f;
	bool botGoRotate = true;
	//variable for lerping scale
	glm::vec3 botOrigSca = glm::vec3(0.02f, 0.02f, 0.02f), botNewSca = glm::vec3(0.015f, 0.015f, 0.015f);
	float botScaTimer = 0.0f, botScaTimeLimit = 5.0f;
	bool botGoScale = true;

	
	App::Tick();

	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: LERP stuff!
		//Duck
		posTimer += deltaTime;
		colTimer += deltaTime;
		rotTimer += deltaTime;
		scaTimer += deltaTime;

		if (posTimer >= posTimeLimit)
		{
			posTimer = 0.0f;
			duckyGoPlaces = !duckyGoPlaces;
		}

		if (colTimer >= colTimeLimit)
		{
			colTimer = 0.0f;
			duckyGoColor = !duckyGoColor;
		}

		//if (rotTimer >= rotTimeLimit)
		//{
		//	rotTimer = 0.0f;
		//	duckyGoRotate = !duckyGoRotate;
		//}

		if (scaTimer >= scaTimeLimit)
		{
			scaTimer = 0.0f;
			duckyGoScale = !duckyGoScale;
		}

		//Bot
		botPosTimer += deltaTime;
		botColTimer += deltaTime;
		botRotTimer += deltaTime;
		botScaTimer += deltaTime;

		if (botPosTimer >= posTimeLimit)
		{
			botPosTimer = 0.0f;
			botGo = !botGo;
		}

		if (botColTimer >= botColTimeLimit)
		{
			botColTimer = 0.0f;
			botGoColor = !botGoColor;
		}

		/*if (botRotTimer >= botRotTimeLimit)
		{
			botRotTimer = 0.0f;
			botGoRotate = !botGoRotate;
		}*/

		if (botScaTimer >= botScaTimeLimit)
		{
			botScaTimer = 0.0f;
			botGoScale = !botGoScale;
		}
		
		//Calculate t
		float tPos = posTimer / posTimeLimit;
		float tCol = colTimer / colTimeLimit;
		float tRot = rotTimer / rotTimeLimit;
		float tSca = scaTimer / scaTimeLimit;

		if (duckyGoPlaces == true)
		{
			duckEntity.transform.m_pos = LERP(origPos, newPos, tPos);
		}
		else if (duckyGoPlaces == false)
		{
			duckEntity.transform.m_pos = LERP(newPos, origPos, tPos);
		}
		
		if (duckyGoColor == true)
		{
			duckMat.m_color = LERP(origCol, newCol, tCol);
		}
		else if (duckyGoColor == false)
		{
			duckMat.m_color = LERP(newCol, origCol, tCol);
		}

		if (duckyGoRotate == true)
		{
			duckEntity.transform.m_rotation = LERP(origRot, newRot, tRot);
		}
		else if (duckyGoRotate == false)
		{
			duckEntity.transform.m_rotation = LERP(newRot, origRot, tRot);
		}

		if (duckyGoScale == true)
		{
			duckEntity.transform.m_scale = LERP(origSca, newSca, tSca);
		}
		else if (duckyGoScale == false)
		{
			duckEntity.transform.m_scale = LERP(newSca, origSca, tSca);
		}

		//Calculate t BOT
		float botTPos = botPosTimer / botPosTimeLimit;
		float botTCol = botColTimer / botColTimeLimit;
		float botTRot = botRotTimer / botRotTimeLimit;
		float botTSca = botScaTimer / botScaTimeLimit;

		if (botGo == true)
		{
			botEntity.transform.m_pos = LERP(botOriPos, botNewPos, botTPos);
		}
		else if (botGo == false)
		{
			botEntity.transform.m_pos = LERP(botNewPos, botOriPos, botTPos);
		}

		if (botGoColor == true)
		{
			botMat.m_color = LERP(botOrigCol, botNewCol, botTCol);
		}
		else if (botGoColor == false)
		{
			botMat.m_color = LERP(botNewCol, botOrigCol, botTCol);
		}

		if (botGoRotate == true)
		{
			botEntity.transform.m_rotation = LERP(botOrigRot, botNewRot, botTRot);
		}
		else if (botGoRotate == false)
		{
			botEntity.transform.m_rotation = LERP(botNewRot, botOrigRot, botTRot);
		}

		if (botGoScale == true)
		{
			botEntity.transform.m_scale = LERP(botOrigSca, botNewSca, botTSca);
		}
		else if (botGoScale == false)
		{
			botEntity.transform.m_scale = LERP(botNewSca, botOrigSca, botTSca);
		}

		//TODO: Update camera, update duck transform, draw duck...
		camEntity.Get<CCamera>().Update();

		//Duck
		duckEntity.transform.RecomputeGlobal();
		duckEntity.Get<CMeshRenderer>().Draw();

		//Bot
		botEntity.transform.RecomputeGlobal();
		botEntity.Get<CMeshRenderer>().Draw();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
