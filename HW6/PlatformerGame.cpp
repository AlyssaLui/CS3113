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

	for (int i = 0; i < 15; i++) {
		Entity* asteroid = new Entity(texture, 224.0 / 1024.0, 664.0 / 1024.0, 101.0 / 1024.0, 84.0 / 1024.0, true);
		asteroid->setX(rand() % 3 -1);
		asteroid->setY(rand() % 3 -1);
		asteroid->setAccelX((rand() % 3 - 1) *0.03);
		asteroid->setAccelY((rand() % 3 - 1) *0.03);
		asteroids.push_back(asteroid);
	}

}

bool PlatformerGame::UpdateandRender() {
	SDL_Event event;
	keys = SDL_GetKeyboardState(NULL);

	float lastFrameTicks = 0.0f;
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
		rocket->setAccelX(-0.1);

	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		rocket->setAccelX(0.1);

	}
	else if (keys[SDL_SCANCODE_UP]) {
		rocket->setAccelY(0.1);
	}
	else if (keys[SDL_SCANCODE_DOWN]) {
		rocket->setAccelY(-0.1);
	}
	else if (keys[SDL_SCANCODE_A]) {
		rocket->setRotation(rocket->getRotation() + (elapsed*0.05));
	}
	else if (keys[SDL_SCANCODE_D]) {
		rocket->setRotation(rocket->getRotation() - (elapsed*0.05));
	}
	else {
		rocket->setAccelX(0.0);
		rocket->setAccelY(0.0);
	}


	if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
		// Shoot
	}

	Render();
	return done;
}

void PlatformerGame::FixedUpdate() {
	
	rocket->FixedUpdate();
	for (int i = 0; i < asteroids.size(); i++) {
		asteroids[i]->FixedUpdate();
		for (int j = 0; j < asteroids.size(); j++) {
			if (i != j) {
				asteroids[i]->satCollidesWith(*asteroids[j]);
			}
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
