#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Laborator3_Vis2D : public SimpleScene
{
	public:
		struct ViewportSpace
		{
			ViewportSpace() : x(0), y(0), width(1), height(1) {}
			ViewportSpace(int x, int y, int width, int height)
				: x(x), y(y), width(width), height(height) {}
			int x;
			int y;
			int width;
			int height;
		};

		struct LogicSpace
		{
			LogicSpace() : x(0), y(0), width(1), height(1) {}
			LogicSpace(float x, float y, float width, float height)
				: x(x), y(y), width(width), height(height) {}
			float x;
			float y;
			float width;
			float height;
		};

		struct Enemies {

			Enemies() : x(0), y(0), angle(0), lifes(0), color(0, 0, 1) {}
			Enemies(float x, float y, float angle, int lifes)
				: x(x), y(y), angle(angle), lifes(lifes), color(color) {}
			//coordonatele inamicilor
			float x;
			float y;
			//unghiul inamicilor
			float angle;
			int lifes;
			glm::vec3 color;
		};

		struct Projectiles {
			Projectiles() : x(0), y(0), angle(0) {}
			Projectiles(float x, float y, float angle)
				: x(x), y(y), angle(angle) {}
			//coordonatele proiectilelor
			float x;
			float y;
			float angle;
		};

	public:
		Laborator3_Vis2D();
		~Laborator3_Vis2D();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void DrawScene(glm::mat3 visMatrix);

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;

		// Sets the logic space and view space
		// logicSpace: { x, y, width, height }
		// viewSpace: { x, y, width, height }
		glm::mat3 VisualizationTransf2D(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);
		glm::mat3 VisualizationTransf2DUnif(const LogicSpace &logicSpace, const ViewportSpace &viewSpace);

		glm::mat3 VisualizationTransfInverse(const LogicSpace & logicSpace, const ViewportSpace & viewSpace);

		void SetViewportArea(const ViewportSpace &viewSpace, glm::vec3 colorColor = glm::vec3(0), bool clear = true);

	protected:
		float length;
		ViewportSpace viewSpace;
		LogicSpace logicSpace;
		glm::mat3 modelMatrix, visMatrix;
		glm::mat3 anemyMatrix;
		float scaleX, scaleY;		//pentru scalarea navei
		float dX, dY;				//pentru mouseMove
		float currentX, currentY;	//pentru mouseMove
		float auxX, auxY;			//pentru translatie si scalare a navei
									//pentru setarea axelor in centrul ecranului
		float spaceshipX, spaceshipY;	//coordonatele navei in centru
		float rotation;				//pentru rotirea in jurul mouse-ului
		std::vector<Enemies> enemiesVector;		//vector de inamici
		std::vector<Projectiles> projectilesVector;		//vector de proiectile
		float scaleXEnemy, scaleYEnemy;		//pentru scalarea inamicilor
		float scaleXProjectiles, scaleYProjectiles;		//pentru scalarea proiectilelor
		float collisionDistance;			//distanta minima pentru coliziune
		//std::vector<Enemies>::iterator eIt;
		//std::vector<Projectiles>::iterator pIt;
		int nrOfEnemies = 0, nrOfProjectiles = 0;
		int count = 0;		//contor pentru a miscora timpul de aparitie a inamicilor pe ecran
		bool eraseEnemy = false, eraseProjectile = false;
		int lifesOfShip = 3;
		float speedOfShip = 2;
		glm::vec3 finalColor = glm::vec3(1, 0, 0) ;

};
