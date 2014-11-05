#include "PlatformerGame.h"

PlatformerGame::PlatformerGame() {
	Init();
}

PlatformerGame::~PlatformerGame() {
	SDL_Quit();
}

void PlatformerGame::Init() {
	SDL_Init(SDL_INIT_VIDEO);

	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	keys = SDL_GetKeyboardState(NULL);

	texture = loadTexture("asteroidsprites.png");
	rocket = new Entity(texture, 325.0 / 1024.0, 739.0 / 1024.0, 98.0 / 1024.0, 75.0 / 1024.0, true);
	rocket->setX(0.0);
	rocket->setY(0.0);

	for (int i = 0; i < 20; i++) {
		Entity* asteroid = new Entity(texture, 224.0 / 1024.0, 664.0 / 1024.0, 101.0 / 1024.0, 84.0 / 1024.0, true);
		asteroid->setX(rand() % 3 -1);
		asteroid->setY(rand() % 3 -1);
		asteroid->setAccelX((rand() % 3 - 1) *0.5);
		asteroid->setAccelY((rand() % 3 - 1) *0.5);
		asteroids.push_back(asteroid);
	}

}

bool PlatformerGame::UpdateandRender() {
	SDL_Event event;
	keys = SDL_GetKeyboardState(NULL);

	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}

	if (keys[SDL_SCANCODE_LEFT]) {
		rocket->setRotation(rocket->getRotation() + 50 * (elapsed));

	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		rocket->setRotation(rocket->getRotation() - 50 * (elapsed));

	}
	else if (keys[SDL_SCANCODE_UP]) {
		rocket->setAccelY(1.0);
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		rocket->setAccelY(-1.0);
	}
	
	else {
		rocket->setAccelX(0.0);
		rocket->setAccelY(0.0);
	}

	Render();
	return done;
}

void PlatformerGame::FixedUpdate() {
	
	if (rocket->getWidth() == (112.0 / 1024.0)) {
		rocket->setU(325.0/1024.0);
		rocket->setV(739.0 / 1024.0);
		rocket->setWidth(98.0 / 1024.0);
		rocket->setHeight(75.0 / 1024.0);
	}
	rocket->FixedUpdate();
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i]->FixedUpdate();
		if (asteroids[i]->getX() > 1.5 || asteroids[i]->getX() < -1.5) {
			asteroids[i]->setX(-asteroids[i]->getX());
		}
		if (asteroids[i]->getY() > 1.5 || asteroids[i]->getY() < -1.5) {
			asteroids[i]->setY(-asteroids[i]->getY());
		}

		for (int j = 0; j < asteroids.size(); j++) {
			if (i != j) {
				asteroids[i]->satCollidesWith(*asteroids[j]);
			}
		}
		if (asteroids[i]->satCollidesWith(*rocket)) {
			rocket->setU(0.0);
			rocket->setV(941.0 / 1024.0);
			rocket->setWidth(112.0 / 1024.0);
			rocket->setHeight(75.0 / 1024.0);
		}
	}


}

void PlatformerGame::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	rocket->render();
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i]->render();	
	}
	
	SDL_GL_SwapWindow(displayWindow);
}

GLuint PlatformerGame::loadTexture(const char *image_path)
{
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);

	return textureID;
}
