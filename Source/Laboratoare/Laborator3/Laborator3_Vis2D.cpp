#include "Laborator3_Vis2D.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"
#include <math.h> 


using namespace std;

Laborator3_Vis2D::Laborator3_Vis2D()
{
}

Laborator3_Vis2D::~Laborator3_Vis2D()
{
}

void Laborator3_Vis2D::Init()
{
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	cameraInput->SetActive(false);

	logicSpace.x = 0;		// logic x
	logicSpace.y = 0;		// logic y
	logicSpace.width = 12.8;	// logic width
	logicSpace.height = 7.2;	// logic height

	glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
	length = 0.99f;

	//am aflat coordonatele centrului navei
	auxX = corner.x + length / 2;
	auxY = corner.y + length / 2;

	Mesh* triangle1 = Object2D::CreateTriangle("triangle1", corner, length, true);
	Mesh* lifes = Object2D::CreateLifes("lifes", corner, length, true);
	Mesh* enemiesOneLife = Object2D::CreateTriangle("enemyOneLife", corner, length, true);
	//Mesh* enemiesTwoLifes = Object2D::CreateTriangle("enemyTwoLifes", corner, length, true);
	Mesh* projectiles = Object2D::CreateProjectiles("projectiles", corner, length, true);
	AddMeshToList(triangle1);
	AddMeshToList(lifes);
	AddMeshToList(enemiesOneLife);
	//	AddMeshToList(enemiesTwoLifes);
	AddMeshToList(projectiles);

	//pentru scalarea navei
	scaleX = 0.5;
	scaleY = 0.5;

	//pentru scalarea inamicilor
	scaleXEnemy = 0.35;
	scaleYEnemy = 0.35;

	//pentru scalarea proiectulor
	scaleXProjectiles = 1;
	scaleYProjectiles = 1;

	//coordonatele navei -  4 inainte
	spaceshipX = logicSpace.width / 2;
	spaceshipY = logicSpace.height / 2;

	//pentru rotatie
	rotation = 0;

	//distanta minima pentru coliziuni
	collisionDistance = 0;


}

// 2D visualization matrix
glm::mat3 Laborator3_Vis2D::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;

	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Laborator3_Vis2D::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

glm::mat3 Laborator3_Vis2D::VisualizationTransfInverse(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smax;
	sx = logicSpace.width / viewSpace.width;
	sy = logicSpace.height / viewSpace.height;
	if (sx > sy)
		smax = sx;
	else
		smax = sy;
	tx = logicSpace.x - smax * viewSpace.x + (logicSpace.width - smax * viewSpace.width) / 2;
	ty = logicSpace.y - smax * viewSpace.y + (logicSpace.height - smax * viewSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smax, 0.0f, tx,
		0.0f, smax, ty,
		0.0f, 0.0f, 1.0f));
}

void Laborator3_Vis2D::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	camera->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	camera->Update();
}

void Laborator3_Vis2D::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}


int position = 0;
float timeAux = 0;
float deltaX = 0;
float deltaY = 0;
int positionProjectile = 0;

void Laborator3_Vis2D::Update(float deltaTimeSeconds)
{

	
	if (lifesOfShip <= 0)
	{
		rotation = 0;
		speedOfShip = 0;

		SetViewportArea(viewSpace, finalColor, true);
	}
	else {
		glm::vec3 corner = glm::vec3(0.001, 0.001, 0);

		glm::ivec2 resolution = window->GetResolution();

		// Sets the screen area where to draw - the left half of the window
		viewSpace = ViewportSpace(0, 0, resolution.x, resolution.y);
		SetViewportArea(viewSpace, glm::vec3(0, 0, 0), true);

		// Compute the 2D visualization matrix
		visMatrix = glm::mat3(1);
		visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);

		//calculez pozitiile random pe care trebuie sa apara inamicii
		double time = Engine::GetElapsedTime();

		if (time > timeAux/* && position < 100*/) {

			Enemies enemy;
			enemy.x = sin(2 * 3.14 / 180 * rand()) * 4 + spaceshipX;
			enemy.y = cos(2 * 3.14 / 180 * rand()) * 4 + spaceshipY;
			enemy.angle = atan2f(spaceshipY - enemy.y, spaceshipX - enemy.x);
			if (count < 15) {
				timeAux += 2 - count *0.1f;
				count++;
			}
			else if (count == 15) {
				timeAux += 0.5;
			}



			//in functie paritatea valorii random, cream inamici cu 1 sau 2 vieti
			if (rand() % 2 == 0) {
				//inamicii cu o viata vor avea culoarea albastra
				enemy.lifes = 1;
				enemy.color = glm::vec3(0, 0, 1);
			}
			else {
				//inamicii cu 2 vieti vor avea culoarea galbena
				enemy.lifes = 2;
				enemy.color = glm::vec3(1, 1, 0);
			}

			enemiesVector.push_back(enemy);
			nrOfEnemies++;
		}

		//pentru rotatia inamicilor 
		for (int i = 0; i < enemiesVector.size(); i++) {
			enemiesVector[i].x += cos(enemiesVector[i].angle) * 3.14 / 180 * 0.6;
			enemiesVector[i].y += sin(enemiesVector[i].angle) * 3.14 / 180 * 0.6;
			enemiesVector[i].angle = atan2f(spaceshipY - enemiesVector[i].y, spaceshipX - enemiesVector[i].x);
		}


		for (int i = 0; i < projectilesVector.size(); i++) {
			projectilesVector[i].x += 0.5 * cos(projectilesVector[i].angle);
			projectilesVector[i].y += 0.5 * sin(projectilesVector[i].angle);
			modelMatrix = visMatrix * Transform2D::Translate(projectilesVector[i].x, projectilesVector[i].y)
				* Transform2D::Rotate(projectilesVector[i].angle);
			RenderMesh2D(meshes["projectiles"], modelMatrix, glm::vec3(0, 1, 1));
		}

		//cout << "INAMICI: " << nrOfEnemies << endl;
	//	cout << "PROIECTILE: " << numberOfProjectiles;
		//cout << "ENEMIES: " << enemiesVector.capacity() << endl;
	//	cout << "PROJECTILES" << projectilesVector.capacity() << endl;
		float deltaDistance = 0.5f;
		float first, second;
		for (int i = 0; i < enemiesVector.size();) {
			bool deletedEnemy = false;
			for (int j = 0; j < projectilesVector.size();) {
				first = pow(enemiesVector[i].x - projectilesVector[j].x, 2);
				second = pow(enemiesVector[i].y - projectilesVector[j].y, 2);
				collisionDistance = sqrt(first + second);
				if (collisionDistance <= deltaDistance) {

					if (enemiesVector[i].lifes == 1) {
						enemiesVector.erase(enemiesVector.begin() + i);
						//nrOfEnemies--;
						deletedEnemy = true;
						//eraseEnemy = true;
					}

					//nu sterge inamicul daca are 2 vieti
					else if (enemiesVector[i].lifes == 2) {
						enemiesVector[i].lifes--;
						enemiesVector[i].color = glm::vec3(1, 0, 0);

					}
					projectilesVector.erase(projectilesVector.begin() + j);
					//	eraseProjectile = true;
					nrOfProjectiles--;
				}
				//nu se sterg proiectilele doar daca distanta de coliziune > 0.2
				else if (collisionDistance > deltaDistance) {
					j++;
				}
			}
			if (deletedEnemy == false)
				i++;
		}

		//nava - inamici
		for (int i = 0; i < enemiesVector.size();) {

			collisionDistance = sqrt(pow(spaceshipX - enemiesVector[i].x, 2) + pow(spaceshipY - enemiesVector[i].y, 2));

			if (collisionDistance <= deltaDistance) {
				enemiesVector.erase(enemiesVector.begin() + i);
				if (lifesOfShip != 0)
					lifesOfShip--;
			}
			else {
				i++;
			}
		}

		//COLIZIUNE INAMICI - PROIECTILE

		/*int check = 0;
		for (eIt = enemiesVector.begin(); eIt != enemiesVector.end();) {
			check = 0;
			for (pIt = projectilesVector.begin(); pIt != projectilesVector.end();) {
				collisionDistance = sqrt((pow(abs(eIt->x - pIt->x), 2) + pow(abs(eIt->y - pIt->y), 2)));

				//daca distanta este mai mica sau egala cu 0.2, atunci inseamna ca s-a produs o coliziune
				if (collisionDistance <= deltaDistance) {
					//daca un proiectil a atins un inamic cu o viata
					cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAa" << endl;
					if (eIt->lifes == 1) {
						//dispar si inamicii si proiectilele
						cout << "BBBBBBBBBBBBBBBBBBBBBBBB" << endl;
						eIt = enemiesVector.erase(eIt);
						check = 1;		//am sters un inamic
					}
					//daca are 2 vieti
					else {
						eIt->lifes--;
						eIt->color = glm::vec3(1, 0, 0);
					}
					pIt = projectilesVector.erase(pIt);
				}
				//daca nu s-a produs o coliziune
				else {
					++pIt;
				}
				if (check == 1) {
					break;
				}
			}

			if (check == 0) {
				++eIt;
			}
		}*/

		/*	int enemyPosition = -1;
			int projectilePosition = -1;
			for (int i = 0; i < enemiesVector.size(); i++) {
				for (int j = 0; j < projectilesVector.size(); j++) {
					collisionDistance = sqrt((pow(abs(eIt->x - pIt->x), 2) + pow(abs(eIt->y - pIt->y), 2)));

					if (collisionDistance <= deltaDistance) {
						enemyPosition = i;
						projectilePosition = j;
						enemiesVector[i].lifes--;
					}
				}
			}

			if (enemyPosition != -1 && projectilePosition != -1) {
				if (enemiesVector[enemyPosition].lifes < 1) {
					enemiesVector.erase(enemiesVector.begin() + enemyPosition);
				}
				projectilesVector.erase(projectilesVector.begin() + projectilePosition);
			}*/

			//COLIZIUNE NAVA - INAMICI
		/*	for (eIt = enemiesVector.begin(); eIt != enemiesVector.end(); eIt++) {
				collisionDistance = sqrt(abs((pow(eIt->x - spaceshipX, 2)) + pow(eIt->y - spaceshipY, 2)));

				//daca distanta dintre nava si inamici este mai mica sau egala cu 0.2, atunci apare coliziunea
				if (collisionDistance <= deltaDistance) {
					//stergem inamicul din vector si o viata
					enemiesVector.erase(eIt);
				}
			}*/
	}
	DrawScene(visMatrix);

}

void Laborator3_Vis2D::FrameEnd()
{

}

void Laborator3_Vis2D::DrawScene(glm::mat3 visMatrix)
{


	//nava
	modelMatrix = visMatrix * Transform2D::Translate(spaceshipX, spaceshipY);
	modelMatrix *= Transform2D::Rotate(rotation);			//rotim nava dupa mouse
	modelMatrix *= Transform2D::Scale(scaleX, scaleY);		//scalam nava
	RenderMesh2D(meshes["triangle1"], modelMatrix, glm::vec3(1, 0, 1));

	for (int i = 0; i < lifesOfShip; i++)
	{
	
	//prima viata

		float dist =i* 0.12f;
	modelMatrix = visMatrix * Transform2D::Translate(logicSpace.width - length+dist, logicSpace.height - length);
	modelMatrix *= Transform2D::Scale(scaleX, scaleY);
	RenderMesh2D(meshes["lifes"], modelMatrix, glm::vec3(1, 0, 0));
}
	//a doua viata
	
	//modelMatrix = visMatrix * Transform2D::Translate(logicSpace.width - length + dist1, logicSpace.height - length);
	//modelMatrix *= Transform2D::Scale(scaleX, scaleY);
	//RenderMesh2D(meshes["lifes"], modelMatrix, glm::vec3(1, 0, 0));

	////a treia viata
	//float dist2 = 0.24f;
	//modelMatrix = visMatrix * Transform2D::Translate(logicSpace.width - length + dist2, logicSpace.height - length);
	//modelMatrix *= Transform2D::Scale(scaleX, scaleY);
	//RenderMesh2D(meshes["lifes"], modelMatrix, glm::vec3(1, 0, 0));

	//inamicii
	for (int i = 0; i < enemiesVector.size(); i++) {
		modelMatrix = visMatrix * Transform2D::Translate(enemiesVector[i].x, enemiesVector[i].y);
		modelMatrix *= Transform2D::Scale(scaleXEnemy, scaleYEnemy);
		modelMatrix *= Transform2D::Rotate(enemiesVector[i].angle);
		RenderMesh2D(meshes["enemyOneLife"], modelMatrix, enemiesVector[i].color);
		//	RenderMesh2D(meshes["enemyTwoLifes"], modelMatrix, enemiesVector[i].color);
	}
}

void Laborator3_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
	//TODO move the logic window with W, A, S, D (up, left, down, right)
	//TODO zoom in and zoom out logic window with Z and X
	if (window->KeyHold(GLFW_KEY_W)) {
		spaceshipY += speedOfShip * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_A)) {
		spaceshipX -= speedOfShip * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_S)) {
		spaceshipY -= speedOfShip * deltaTime;
	}
	if (window->KeyHold(GLFW_KEY_D)) {
		spaceshipX += speedOfShip * deltaTime;
	}
}

void Laborator3_Vis2D::OnKeyPress(int key, int mods)
{
	//add key release event
};

void Laborator3_Vis2D::OnKeyRelease(int key, int mods)
{
	// add key release event
};

float degrees;
void Laborator3_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	visMatrix = glm::mat3(1);
	visMatrix *= VisualizationTransfInverse(logicSpace, viewSpace);
	glm::vec3 vectorMouse = visMatrix * glm::vec3(mouseX, viewSpace.height - mouseY, 1);

	rotation = atan2f((-spaceshipY + vectorMouse.y), (-spaceshipX + vectorMouse.x));
};

void Laborator3_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	if (IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) {
		//proiectile
		Projectiles projectile;
		projectile.angle = rotation;
		projectile.x = spaceshipX;
		projectile.y = spaceshipY;
		projectilesVector.push_back(projectile);
		nrOfProjectiles++;
	}
};

void Laborator3_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3_Vis2D::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}