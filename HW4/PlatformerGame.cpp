#include "PlatformerGame.h"

PlatformerGame::PlatformerGame() {
	Init();
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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	keys = SDL_GetKeyboardState(NULL);

	GLuint sprites = loadTexture("sprites.png");

	Entity player = Entity(sprites, 0.0f, 0.0f, 64.0 / 256.0f, 64.0 / 256.0f, true, false);
	player.setX(0.0f);
	player.setY(-0.5f);
	player.setScale(0.3);
	//player.insertUVPoints(0.0f, (66.0 / 256.0f));					//Facing left - 0,1
	//player.insertUVPoints((66.0 / 256.0f), (66.0 / 256.0f));		//Facing left idle - 2,3
	//player.insertUVPoints(0.0f, (66.0 / 256.0f));					//Facing left - 4,5
	//player.insertUVPoints(0.0f, (132.0 / 25.0f));					//Facing right - 6,7
	//player.insertUVPoints((66.0 / 256.0f), 0.0f);					//Facing right idle - 8,9
	//player.insertUVPoints((66.0 / 256.0f), (132.0 / 256.0f));		//Facing right - 10,11
	dynamicEntities.push_back(player);

	Entity floor = Entity(sprites, 0.0f, 198.0 / 256.0f, 48.0 / 256.0f, 48.0 / 256.0f, true, true);
	floor.setY(-0.9f);
	floor.setX(-0.9f);
	floor.setScale(0.3);
	for (int floorCount = 0; floorCount < 18; floorCount++) {		
		floor.setX(floor.getX() + (48.0 / 256.0)*0.5);
		staticEntities.push_back(floor);
	}

	Entity floor1 = Entity(sprites, 0.0f, 198.0 / 256.0f, 48.0 / 256.0f, 48.0 / 256.0f, true, true);
	floor1.setY(-0.3f);
	floor1.setX(-0.9);
	floor1.setScale(0.3);
	for (int floorCounter = 0; floorCounter < 9; floorCounter++) {	
		floor1.setX(floor1.getX() + (48.0 / 256.0)*0.5);
		staticEntities.push_back(floor1);
	}

	Entity wallL = Entity(sprites, 50.0 / 256.0f, 198.0 / 256.0f, 48.0 / 256.0f, 48.0 / 256.0f, true, true);
	wallL.setX(-0.9);
	wallL.setY(-0.9);
	wallL.setScale(0.3);
	for (int wallCount = 0; wallCount < 20; wallCount++) {		
		wallL.setY(wallL.getY() + (48.0 / 256.0)*0.5);
		staticEntities.push_back(wallL);
	}

	Entity wallR = Entity(sprites, 50.0 / 256.0f, 198.0 / 256.0f, 48.0 / 256.0f, 48.0 / 256.0f, true, true);
	wallR.setX(0.9);
	wallR.setY(-0.9);
	wallR.setScale(0.3);
	for (int wallCounter = 0; wallCounter < 20; wallCounter++) {	
		wallR.setY(wallR.getY() + (48.0 / 256.0)*0.5);
		staticEntities.push_back(wallR);
	}

	Entity floor2 = Entity(sprites, 0.0f, 198.0 / 256.0f, 48.0 / 256.0f, 48.0 / 256.0f, true, true);
	floor2.setX(0.0f);
	floor2.setY(0.3f);
	floor2.setScale(0.3);
	for (int floorCounter1 = 0; floorCounter1 < 9; floorCounter1++) {	
		floor2.setX(floor2.getX() + (48.0 / 256.0)*0.5);
		staticEntities.push_back(floor2);
	}

	Entity floor3 = Entity(sprites, 0.0f, 198.0 / 256.0f, 48.0 / 256.0f, 48.0 / 256.0f, true, true);
	floor3.setY(0.9f);
	floor3.setX(-0.9f);
	floor3.setScale(0.3);
	for (int floorCount3 = 0; floorCount3 < 18; floorCount3++) {
		floor3.setX(floor3.getX() + (48.0 / 256.0)*0.5);
		staticEntities.push_back(floor3);
	}

	// Creating target entities
	Entity target = Entity(sprites, 100.0 / 256.0f, 198.0 / 256.0f, 48.0 / 256.0f, 48.0 / 256.0f, true, true);
	target.setScale(0.5);
	target.setX(staticEntities[staticEntities.size() - 20].getX()); 
	target.setY(staticEntities[staticEntities.size() - 20].getY() + staticEntities[staticEntities.size() - 20].getHeight() * 0.5);
	target.setVisible(true);
	dynamicEntities.push_back(target);


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
		dynamicEntities[0].FixedUpdate();
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}

	if (keys[SDL_SCANCODE_LEFT]) {
		dynamicEntities[0].setAccelX(-0.1);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		dynamicEntities[0].setAccelX(0.1);
	}
	else {
		dynamicEntities[0].setAccelX(0.0);
	}

	if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
		if (dynamicEntities[0].getCollidedBottom()) {
			dynamicEntities[0].setVelocityY(2.0f);
			dynamicEntities[0].setCollidedBottom(false);
		}
	}


	Render();
	return done;
}

void PlatformerGame::FixedUpdate() {
	
	if (dynamicEntities[0].getX() < staticEntities[30].getX() + (staticEntities[18].getWidth()* 0.5)) {
		float xPenetration = fabs(fabs(dynamicEntities[0].getX() - staticEntities[30].getX()) - dynamicEntities[0].getWidth()*0.5 - staticEntities[30].getWidth()*0.5);
		dynamicEntities[0].setX(dynamicEntities[0].getX() + xPenetration*0.01 );
		dynamicEntities[0].setVelocityX(0.0);
	}
	else if (dynamicEntities[0].getX() > staticEntities[50].getX() - (staticEntities[50].getWidth()* 0.5)) {
		float xPenetration1 = fabs(fabs(dynamicEntities[0].getX() - staticEntities[50].getX()) - dynamicEntities[0].getWidth()*0.5 - staticEntities[50].getWidth()*0.5);
		dynamicEntities[0].setX(dynamicEntities[0].getX() - xPenetration1*0.01 );
		dynamicEntities[0].setVelocityX(0.0);
	}

	for (int j = 0; j < staticEntities.size(); j++) {
		if (dynamicEntities[0].collidesWith(staticEntities[j])) {
			dynamicEntities[0].setCollidedBottom(true);
			float yPenetration = fabs(fabs(dynamicEntities[0].getY() - staticEntities[j].getY()) - dynamicEntities[0].getHeight()*0.5*dynamicEntities[0].getScale() - staticEntities[j].getHeight()*0.5*staticEntities[j].getScale());

			if (dynamicEntities[0].getY() > staticEntities[j].getY()) {
				dynamicEntities[0].setY(dynamicEntities[0].getY() + yPenetration + 0.001f);
				dynamicEntities[0].setVelocityY(0.0);
			}
			else {
				dynamicEntities[0].setY(dynamicEntities[0].getY() - yPenetration - 0.001f);
				dynamicEntities[0].setVelocityY(0.0);
			}
		}
	}

	if (dynamicEntities[0].collidesWith(dynamicEntities[1])) {
		dynamicEntities[1].setVisible(false);
	}
}

void PlatformerGame::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	dynamicEntities[0].draw(0.3);
	if (dynamicEntities[1].getVisible()) {
		dynamicEntities[1].draw(0.5);
	}

	for (int i = 0; i < staticEntities.size(); i++) {
		staticEntities[i].draw(0.3);
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
