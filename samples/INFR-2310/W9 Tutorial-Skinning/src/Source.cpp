/*
Week 9 Tutorial Sample - Created for INFR 2310 at Ontario Tech.
(c) Atiya Nova and Samantha Stahlke 2020
*/

#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "NOU/CMeshRenderer.h"
#include "NOU/GLTFLoader.h"

#include "CSkinnedMeshRenderer.h"
#include "CAnimator.h"
#include "GLTFLoaderSkinning.h"

#include "Logging.h"
#include "GLM/gtx/matrix_decompose.hpp"
#include "imgui.h"
#include <iostream>

#include <memory>

using namespace nou;

int main()
{
	App::Init("Week 9 Tutorial - Skinning", 600, 600);
	App::SetClearColor(glm::vec4(0.2f, 0.0f, 0.4f, 1.0f));

	App::InitImgui();

	//Loading shaders...
	auto v_texLit = std::make_unique<Shader>("shaders/texturedlit.vert", GL_VERTEX_SHADER);
	auto f_texLit = std::make_unique<Shader>("shaders/texturedlit.frag", GL_FRAGMENT_SHADER);

	ShaderProgram prog_texLit = ShaderProgram({ v_texLit.get(), f_texLit.get() });

	auto v_skinning = std::make_unique<Shader>("shaders/gpuskinning.vert", GL_VERTEX_SHADER);
	auto f_skinning = std::make_unique<Shader>("shaders/gpuskinning.frag", GL_FRAGMENT_SHADER);

	ShaderProgram prog_skinning = ShaderProgram({ v_skinning.get(), f_skinning.get() });

	//Loading textures...
	Texture2D jointTex("textures/joint_texture.png");
	Texture2D plainTex("textures/plain_texture.png");

	//Setting up materials...
	Material boiMat(prog_skinning);
	boiMat.AddTexture("albedo", plainTex);

	Material jointMat(prog_texLit);
	jointMat.AddTexture("albedo", jointTex);

	//Load a basic box mesh for drawing our skeleton.
	auto boxMesh = std::make_unique<Mesh>();
	GLTF::LoadMesh("models/box/BoxTextured.gltf", *boxMesh);
	  
	////Load our skinned boy mesh (in base pose).
	//auto boiMesh = std::make_unique<SkinnedMesh>();
	//GLTF::LoadSkinnedMesh("models/boi/Base.gltf", *boiMesh);

	////Load our animation.
	//auto walkAnim = std::make_unique<SkeletalAnim>();
	//GLTF::LoadAnimation("models/boi/Walk.gltf", *(walkAnim.get()));

	//Load our skinned boy mesh (in base pose).
	auto boiMesh = std::make_unique<SkinnedMesh>();
	GLTF::LoadSkinnedMesh("models/jellyFish/JellyFish_Base.gltf", *boiMesh);

	//Load our animation.
	auto walkAnim = std::make_unique<SkeletalAnim>();
	GLTF::LoadAnimation("models/jellyFish/JellyFish_Idle.gltf", *(walkAnim.get()));

	//Make our camera...
	Entity camEntity = Entity::Create();
	auto& cam = camEntity.Add<CCamera>(camEntity);
	cam.Perspective(60.0f, 1.0f, 0.1f, 100.0f);
	camEntity.transform.m_pos = glm::vec3(0.0f, 0.0f, 2.0f);

	//Make an entity for the boi...
	Entity boiEntity = Entity::Create();
	//Skinned mesh renderer.
	boiEntity.Add<CSkinnedMeshRenderer>(boiEntity, *boiMesh, boiMat);
	boiEntity.transform.m_pos = glm::vec3(0.0f, -0.75f, 0.0f);
	boiEntity.transform.m_scale = glm::vec3(0.5f, 0.5f, 0.5f);
	//Skeletal animator.
	auto& skinnedAnimator = boiEntity.Add<CAnimator>(boiEntity, *walkAnim);

	//Make an entity for drawing our debug skeleton (just a box at each joint).
	Entity jointEntity = Entity::Create();
	jointEntity.Add<CMeshRenderer>(jointEntity, *boxMesh, jointMat);
	//Parented to the entity our skeleton is attached to.
	jointEntity.transform.SetParent(&(boiEntity.transform));
	jointEntity.transform.m_scale = glm::vec3(0.05f, 0.05f, 0.05f);
	
	//To spin our boi every frame. 
	float anglePerSecond = 30.0f;

	std::vector<std::function<void()>> imGuiCallbacks;
	SkeletalAnimClip* boiEntityClip = boiEntity.Get<CAnimator>().GetClip();

	//ImGui controls
	imGuiCallbacks.push_back([&]() {

		///Play-Stop Anim
		ImGui::Text("Toggle Animation: ");
		ImGui::SameLine();
		if (!boiEntityClip->getIsPlaying())
		{
			if (ImGui::Button("Play Animation"))
				boiEntityClip->setIsPlaying(true);
		}
		if (boiEntityClip->getIsPlaying())
		{
			if (ImGui::Button("Pause Animation"))
				boiEntityClip->setIsPlaying(false);
		}

		///Loop?
		ImGui::Text("Toggle Animation Loop: ");
		ImGui::SameLine();
		if (!boiEntityClip->getIsLooping())
		{
			if (ImGui::Button("Play Animation Once"))
				boiEntityClip->setIsLooping(true);
		}
		if (boiEntityClip->getIsLooping())
		{
			if (ImGui::Button("Loop Animation"))
				boiEntityClip->setIsLooping(false);
		}

		///Restart
		ImGui::Text("Restart Animation: ");
		ImGui::SameLine();
		if (ImGui::Button("Restart"))
			boiEntityClip->restartAnimClip();

		if (ImGui::SliderFloat("Speed", &boiEntityClip->m_speed, 0.0f, 4.0f)) {

		}

		/*if (ImGui::CollapsingHeader("Scene Level Lighting Settings"))
		{
			if (ImGui::ColorPicker3("Ambient Color", glm::value_ptr(ambientCol))) {
				shader->SetUniform("u_AmbientCol", ambientCol);
			}
			if (ImGui::SliderFloat("Fixed Ambient Power", &ambientPow, 0.01f, 1.0f)) {
				shader->SetUniform("u_AmbientStrength", ambientPow);
			}
		}
		if (ImGui::CollapsingHeader("Light Level Lighting Settings"))
		{
			if (ImGui::SliderFloat3("Light Pos", glm::value_ptr(lightPos), -10.0f, 10.0f)) {
				shader->SetUniform("u_LightPos", lightPos);
			}
			if (ImGui::ColorPicker3("Light Col", glm::value_ptr(lightCol))) {
				shader->SetUniform("u_LightCol", lightCol);
			}
			if (ImGui::SliderFloat("Light Ambient Power", &lightAmbientPow, 0.0f, 1.0f)) {
				shader->SetUniform("u_AmbientLightStrength", lightAmbientPow);
			}
			if (ImGui::SliderFloat("Light Specular Power", &lightSpecularPow, 0.0f, 1.0f)) {
				shader->SetUniform("u_SpecularLightStrength", lightSpecularPow);
			}
		}
		if (ImGui::CollapsingHeader("Material Level Lighting Settings"))
		{
			if (ImGui::SliderFloat("Shininess", &shininess, 0.1f, 128.0f)) {
				shader->SetUniform("u_Shininess", shininess);
			}
		}*/
		
	});

	App::Tick();

	while (!App::IsClosing() && !Input::GetKey(GLFW_KEY_ESCAPE))
	{
		App::FrameStart();

		float deltaTime = App::GetDeltaTime();

		camEntity.Get<CCamera>().Update();

		//Spin de boi.
		float degreeSpin = anglePerSecond * deltaTime;
		boiEntity.transform.m_rotation = glm::angleAxis(glm::radians(degreeSpin), glm::vec3(0.0f, 1.0f, 0.0f)) * boiEntity.transform.m_rotation;
		boiEntity.transform.RecomputeGlobal();

		//Update the animator, and draw the boi.
		boiEntity.Get<CAnimator>().Update(deltaTime);
		boiEntity.Get<CSkinnedMeshRenderer>().Draw();

		/*if (isRestart)
			isRestart = false;*/

		//As a debug utility/demo: Draw our joints.
		glDisable(GL_DEPTH_TEST); 

		auto& jointTransform = jointEntity.transform;
		
		glm::vec3 scale, skew;
		glm::vec4 persp;

		const Skeleton& skeleton = boiEntity.Get<CSkinnedMeshRenderer>().GetSkeleton();

		for (auto& joint : skeleton.m_joints)
		{
			glm::decompose(joint.m_global,
						   scale,
						   jointTransform.m_rotation,
						   jointTransform.m_pos,
						   skew,
						   persp);

			jointTransform.RecomputeGlobal();
			jointEntity.Get<CMeshRenderer>().Draw();
		}

		
		
		glEnable(GL_DEPTH_TEST);


		App::StartImgui();

		//Put any ImGUI code you need in here.
		//(Don't forget to call Imgui::Begin and Imgui::End!)
		if(ImGui::Begin("Animation Controller")) {
			// Render our GUI stuff
			for (auto& func : imGuiCallbacks) {
				func();
			}
			ImGui::End();
		}

		

		App::EndImgui();

		App::SwapBuffers();
	}

	App::Cleanup();

	return 0;
}