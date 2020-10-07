#include "NOU/App.h"
#include "NOU/Input.h"
#include "NOU/Entity.h"
#include "NOU/CCamera.h"
#include "Sprites/CSpriteRenderer.h"
#include "Sprites/CSpriteAnimator.h"

#include "imgui.h"

#include <memory>

using namespace nou;


int main() 
{ 

	App::Init("Ayi_Pranayanda_100765502_A2", 1000, 1000);
	App::SetClearColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	App::InitImgui();

	//Load shaders.
	auto v_sprite = std::make_unique<Shader>("shaders/texturedunlit.vert", GL_VERTEX_SHADER);
	auto f_sprite = std::make_unique<Shader>("shaders/texturedunlit.frag", GL_FRAGMENT_SHADER);
	
	auto prog_sprite = ShaderProgram({ v_sprite.get(), f_sprite.get() });
	
	//Load sprites
	//Ninja sprite
	//Movement
	Texture2D idleTex = Texture2D("idle.png", true);
	Texture2D runTex = Texture2D("run.png", true);
	Texture2D stealthTex = Texture2D("stealth.png", true);
	//Attacks
	Texture2D slashTex = Texture2D("slash.png", true);
	Texture2D vSlashTex = Texture2D("vSlash.png", true);
	Texture2D spinAttackTex = Texture2D("spinAttack.png", true);
	Texture2D counterAttackTex = Texture2D("counterAttack.png", true);
	//Damage/death
	Texture2D takeDamageTex = Texture2D("takeDamage.png", true);
	Texture2D dieTex = Texture2D("die.png", true);

	Material idleMat(prog_sprite);
	idleMat.AddTexture("albedo", idleTex);
	Material runMat(prog_sprite);
	runMat.AddTexture("albedo", runTex);
	Material stealthMat(prog_sprite);
	stealthMat.AddTexture("albedo", stealthTex);

	Material slashMat(prog_sprite);
	slashMat.AddTexture("albedo", slashTex);
	Material vSlashMat(prog_sprite);
	vSlashMat.AddTexture("albedo", vSlashTex);
	Material spinAttackMat(prog_sprite);
	spinAttackMat.AddTexture("albedo", spinAttackTex);
	Material counterAttackMat(prog_sprite);
	counterAttackMat.AddTexture("albedo", counterAttackTex);

	Material takeDamageMat(prog_sprite);
	takeDamageMat.AddTexture("albedo", takeDamageTex);
	Material dieMat(prog_sprite);
	dieMat.AddTexture("albedo", dieTex);

	//Load in ninja spritesheet


	


	return 0; 
} 
