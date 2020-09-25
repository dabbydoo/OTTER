/*
Week 3 Tutorial Starter - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

/*
Ayi Pranayanda 100765502 week 3 tutorial assignment

Knight controls
Left Arrow: Left
Right Arrow: Right
Space Bar: Attack

Rogue controls
A Key: Left
D Key: Right
Left Shift: Attack

Rogue can be toggled to move autonomously in the gui.
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "Sprites/CSpriteRenderer.h"
#include "Sprites/CSpriteAnimator.h"
#include "CKnightFSM.h"
#include "CRogueFSM.h"
#include <iostream>

#include "imgui.h"

#include <memory>

using namespace nou;

//LERP function
template<typename T>
T LERP(const T& p0, const T& p1, float t)
{
	return (1.0f - t) * p0 + t * p1;
}

int main() 
{
	App::Init("Week 3 Tutorial - Sprites", 800, 800);
	App::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	App::InitImgui();

	//Load in some shaders.
	auto v_sprite = std::make_unique<Shader>("shaders/texturedunlit.vert", GL_VERTEX_SHADER);
	auto f_sprite = std::make_unique<Shader>("shaders/texturedunlit.frag", GL_FRAGMENT_SHADER);

	auto prog_sprite = ShaderProgram({ v_sprite.get(), f_sprite.get() });

	//Load in sprites.
	Texture2D boomTex = Texture2D("explosion.png", true);
	Texture2D knightTex = Texture2D("knight.png", true);
	Texture2D rogueTex = Texture2D("rogue.png", true);

	Material boomMat(prog_sprite);
	boomMat.AddTexture("albedo", boomTex);

	Material knightMat(prog_sprite);
	knightMat.AddTexture("albedo", knightTex);

	Material rogueMat(prog_sprite);
	rogueMat.AddTexture("albedo", rogueTex);
	
	//TODO: Load in explosion spritesheet.
	auto boomSheet = std::make_unique<Spritesheet>(boomTex, glm::vec2(222.0f, 222.0f));
	boomSheet->AddAnimation("boom", 0, 27, 30.0f);
	boomSheet->SetDefaultFrame(27);

	//Load in knight spritesheet, add animations.
	auto knightSheet = std::make_unique<Spritesheet>(knightTex, glm::vec2(64.0f, 64.0f));
	knightSheet->AddAnimation(CKnightFSM::idleClip, 0, 4, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::runClip, 5, 12, 12.0f);
	knightSheet->AddAnimation(CKnightFSM::attackClip, 19, 21, 12.0f);

	//Load in rogue spritesheet, add animations.
	auto rogueSheet = std::make_unique<Spritesheet>(rogueTex, glm::vec2(32.0f, 32.0f));
	rogueSheet->AddAnimation(CRogueFSM::idleClip, 0, 9, 12.0f);
	rogueSheet->AddAnimation(CRogueFSM::runClip, 20, 29, 12.0f);
	rogueSheet->AddAnimation(CRogueFSM::attackClip, 30, 39, 12.0f);

	//Set up our camera.
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Ortho(-400.0f, 400.0f, -400.0f, 400.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);

	//Variables for lerping position
	bool isLerping = false;
	glm::vec3 origPos = glm::vec3(-300.0f, 0.0f, 0.0f), newPos = glm::vec3(300.0f, 0.0f, 0.0f);
	float posTimer = 0.0f, posTimeLimit = 15.0f;
	bool lerpPos = true;

	//TODO: Create explosion entity.
	Entity okBoomer = Entity::Create();
	okBoomer.Add<CSpriteRenderer>(okBoomer, *boomSheet, boomMat);
	auto& boomAnim = okBoomer.Add<CSpriteAnimator>(okBoomer, *boomSheet);

	//Create the knight entity.
	Entity knightEntity = Entity::Create();
	knightEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	knightEntity.Add<CSpriteRenderer>(knightEntity, *knightSheet, knightMat);
	auto& knightAnim = knightEntity.Add<CSpriteAnimator>(knightEntity, *knightSheet);
	knightEntity.Add<CKnightFSM>(knightEntity);

	//Create the rogue entity
	Entity rogueEntity = Entity::Create();
	rogueEntity.transform.m_scale = glm::vec3(2.0f, 2.0f, 2.0f);
	rogueEntity.transform.m_pos = glm::vec3(-100.0f, 0.0f, 0.0f);
	rogueEntity.Add<CSpriteRenderer>(rogueEntity, *rogueSheet, rogueMat);
	auto& rogueAnim = rogueEntity.Add<CSpriteAnimator>(rogueEntity, *rogueSheet);
	rogueEntity.Add<CRogueFSM>(rogueEntity);

	App::Tick();

	//Disabling the depth buffer.
	//(The reason we do this is so we can naively draw sprites on top
	//of each other in the order we call Draw().)
	glDisable(GL_DEPTH_TEST);

	//Disable backface culling.
	//(The reason we disable culling is to make it simpler for us to 
	//flip sprites without making OpenGL think we're drawing polygons
	//that shouldn't be visible.)
	glDisable(GL_CULL_FACE);
	
	//This is our main loop.
	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		//Start of the frame.
		App::FrameStart();
		float deltaTime = App::GetDeltaTime();

		//TODO: Control our knight.
		bool moving = Input::GetKey(GLFW_KEY_RIGHT) || Input::GetKey(GLFW_KEY_LEFT);
		knightEntity.Get<CKnightFSM>().SetVariable("moving", moving);

		if (moving)
		{
			GLfloat speed = 100.0f;

			bool flip = Input::GetKey(GLFW_KEY_LEFT);

			knightEntity.transform.m_scale.x = (flip) ? -2.0f : 2.0f;
			knightEntity.transform.m_pos.x += (flip) ? -speed * deltaTime : speed * deltaTime;
		}

		//Control Rogue
		bool movingRogue = Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_A);
		rogueEntity.Get<CRogueFSM>().SetVariable("movingRogue", movingRogue);

		if (movingRogue)
		{
			GLfloat speedRogue = 50.0f;

			bool flip = Input::GetKey(GLFW_KEY_A);

			rogueEntity.transform.m_scale.x = (flip) ? -2.0f : 2.0f;
			rogueEntity.transform.m_pos.x += (flip) ? -speedRogue * deltaTime : speedRogue * deltaTime;
		}

		if (isLerping == true)
		{
			rogueEntity.Get<CRogueFSM>().SetVariable("movingRogue", isLerping);

			//LERP
			if(!rogueEntity.Get<CRogueFSM>().GetTrigger("attackingRogue"))
				posTimer += deltaTime;

			if (posTimer >= posTimeLimit)
			{
				posTimer = 0.0f;
				lerpPos = !lerpPos;
			}

		

			//Calculate t
			float tPos = posTimer / posTimeLimit;

			bool flip = false;

			if (lerpPos == true && !rogueEntity.Get<CRogueFSM>().GetTrigger("attackingRogue"))
			{
				if ((rogueEntity.transform.m_pos == origPos))
					flip = false;

				if ((int)rogueEntity.transform.m_pos.x == (int)knightEntity.transform.m_pos.x - 20)
				{
					rogueEntity.Get<CRogueFSM>().SetTrigger("attackingRogue");
				}

				rogueEntity.transform.m_pos = LERP(origPos, newPos, tPos);		

				rogueEntity.transform.m_scale.x = 2.0f;
				rogueEntity.transform.m_pos.x += 50.0f * deltaTime;
				
			}
			else if (lerpPos == false && !rogueEntity.Get<CRogueFSM>().GetTrigger("attackingRogue"))
			{
				if ((rogueEntity.transform.m_pos == newPos))
					flip = true;

				if ((int)rogueEntity.transform.m_pos.x == (int)knightEntity.transform.m_pos.x + 20)
				{
					rogueEntity.Get<CRogueFSM>().SetTrigger("attackingRogue");
				}

				rogueEntity.transform.m_pos = LERP(newPos, origPos, tPos);

				rogueEntity.transform.m_scale.x = -2.0f;
				rogueEntity.transform.m_pos.x += 50.0f * deltaTime;

			}
			if ((int)rogueEntity.transform.m_pos.x == (int)knightEntity.transform.m_pos.x)
			{
				rogueEntity.Get<CRogueFSM>().SetTrigger("attackingRogue");
			}
		}

		//Updates all the entities.
		camEntity.Get<CCamera>().Update();
		//TODO: Update explosion entity.
		knightEntity.Get<CKnightFSM>().Update();
		rogueEntity.Get<CRogueFSM>().Update();
		okBoomer.Get<CSpriteAnimator>().Update(deltaTime);
		knightEntity.Get<CSpriteAnimator>().Update(deltaTime);
		rogueEntity.Get<CSpriteAnimator>().Update(deltaTime);
		
		//Recomputes global matrices.
		//TODO: Update explosion entity.
		knightEntity.transform.RecomputeGlobal();
		rogueEntity.transform.RecomputeGlobal();
		okBoomer.transform.RecomputeGlobal();

		//Draws the sprites.
		//TODO: Draw explosion entity.
		okBoomer.Get<CSpriteRenderer>().Draw();
		knightEntity.Get<CSpriteRenderer>().Draw();
		rogueEntity.Get<CSpriteRenderer>().Draw();

		//For Imgui stuff...
		App::StartImgui();

		//TODO: Create Imgui panel.
		//TODO: Create Imgui button and play explosion on press.
		static bool panelOpen = true;
		ImGui::Begin("Coffee is great", &panelOpen, ImVec2(300, 200));

		if (ImGui::Button("Boom!"))
			okBoomer.Get<CSpriteAnimator>().PlayOnce("boom");

		//Attack with knight
		if (Input::GetKeyDown(GLFW_KEY_SPACE) || ImGui::Button("attack"))
			knightEntity.Get<CKnightFSM>().SetTrigger("attacking");

		//Attack with rogue
		if (Input::GetKeyDown(GLFW_KEY_LEFT_SHIFT) || ImGui::Button("attackRogue"))
			rogueEntity.Get<CRogueFSM>().SetTrigger("attackingRogue");


		//Lerping rogue
		if (ImGui::Button("ToggleLerpRogue"))
			isLerping = !isLerping;


		ImGui::End();
		App::EndImgui();

		//This sticks all the drawing we just did on the screen.
		App::SwapBuffers();
	}

	App::Cleanup();

	return 0; 
} 
