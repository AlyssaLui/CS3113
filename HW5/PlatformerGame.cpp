#include "PlatformerGame.h"

PlatformerGame::PlatformerGame() {
	Init();
	Mix_PlayMusic(music, -1);
}

PlatformerGame::~PlatformerGame() {
	Mix_FreeChunk(coinSound);
	Mix_FreeChunk(jumpSound);
	Mix_FreeMusic(music);
	SDL_Quit();
}

void PlatformerGame::Init() {
	SDL_Init(SDL_INIT_VIDEO);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	coinSound = Mix_LoadWAV("coin.wav");
	jumpSound = Mix_LoadWAV("jump.wav");
	music = Mix_LoadMUS("Telekinesis.mp3");

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

	texture = loadTexture("classSprites.png");
	font = loadTexture("font.png");

	ifstream infile("GameMap.txt");
	string line;
	while (getline(infile, line)) {
		if (line == "[header]") {
			if (!readHeader(infile)) {
				return;
			}
		}
		else if (line == "[layer]") {
			readLayerData(infile);
		}
		else if (line == "[Object Layer 1]") {
			readEntityData(infile);
		}
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
		entities[0]->setAccelX(-0.3);
		entities[0]->setFacingLeft(true);
	}
	else if (keys[SDL_SCANCODE_RIGHT]) {
		entities[0]->setAccelX(0.3);
		entities[0]->setFacingLeft(false);
	}
	else {
		entities[0]->setAccelX(0.0);
	}

	if (event.key.keysym.scancode == SDL_SCANCODE_SPACE) {
		if (entities[0]->getCollidedBottom()){
			Mix_PlayChannel(-1, jumpSound, 0);
			entities[0]->setVelocityY(1.5f);
			entities[0]->setCollidedBottom(false);
		}
	}

	Render();
	return done;
}

void PlatformerGame::FixedUpdate() {
	entities[0]->FixedUpdate();

	entities[0]->setCollidedBottom(false);
	entities[0]->setCollidedTop(false);
	entities[0]->setCollidedLeft(false);
	entities[0]->setCollidedRight(false);

	collidesY(entities[0]);
	collidesX(entities[0]);

	for (int i = 1; i < entities.size(); i++) {
		if( entities[0]->collidesWith(*entities[i])) {
			if (entities[i]->getVisible()) {
				Mix_PlayChannel(-1, coinSound, 0);
			}
			entities[i]->setVisible(false);
		}
	}
}

void PlatformerGame::Render() {
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	float transX = -entities[0]->getX();
	float transY = 0.8f;
	if (transX > -1.0) {
		transX = -1.0;
	}
	if (transX < -2.0) {
		transX = -2.0;
	}
	glTranslatef(transX, transY, 0.0f);
	drawMap(texture);

	for (int i = 1; i < entities.size(); i++) {
		if (entities[i]->getVisible()) {
			entities[i]->drawRight(1.0);
		}
	}

	if (entities[0]->getFacingLeft()) {
		entities[0]->drawLeft(1.0);
	}
	else {
		entities[0]->drawRight(1.0);
	}

	//drawText(font, "Keys Collected: " + to_string(score), entities[0]->getX() - 1.0, 0.9, 0.1, 0.0, 0.0, 0.0, 1.0);
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

bool PlatformerGame::readHeader(ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
		
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		
		if (key == "width") {
		mapWidth = atoi(value.c_str());
		}
		else if (key == "height"){
			mapHeight = atoi(value.c_str());
		}
	}
	
	if (mapWidth == -1 || mapHeight == -1) {
		return false;
	}
	else { 
		levelData = new unsigned char*[mapHeight];
		for (int i = 0; i < mapHeight; ++i) {
			levelData[i] = new unsigned char[mapWidth];
		}
		return true;
	}
}

bool PlatformerGame::readLayerData(ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						levelData[y][x] = val - 1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}

bool PlatformerGame::readEntityData(ifstream &stream) {
	string line;
	string type;
	
	while (getline(stream, line)) {
		if (line == "") { break; }
		
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		
		if (key == "type") {
			type = value;
		}
		else if (key == "location") {	
			istringstream lineStream(value);
			string xPosition, yPosition;
			getline(lineStream, xPosition, ',');
			getline(lineStream, yPosition, ',');
					
			float placeX = atoi(xPosition.c_str()) / 16 * TILE_SIZE;
			float placeY = atoi(yPosition.c_str()) / 16 * -TILE_SIZE;
				
			placeEntity(type, placeX, placeY);
		}
	}
	return true;
}

void PlatformerGame::placeEntity(string type, float placeX, float placeY) {

	float uVal;
	float vVal;
	float width;
	float height;

	if (type == "Player") {
		uVal = (float)(98 % 16) / 16.0f;
		vVal = (float)(98 / 16) / 8.0f;
		width = 1.0 / 16.0f;
		height = 1.0 / 8.0f;	
		Entity* obj = new Entity(texture, placeX, placeY, uVal, vVal, width, height, true);
		entities.push_back(obj);
	}
	else if (type == "key") {
		uVal = (float)(86 % 16) / 16.0f;
		vVal = (float)(86 / 16) / 8.0f;
		width = 1.0 / 16.0f;
		height = 1.0 / 8.0f;
		Entity* obj = new Entity(texture, placeX, placeY, uVal, vVal, width, height, true);
		entities.push_back(obj);
	}

}

void PlatformerGame::drawMap(GLuint textureMap) {
	glBindTexture(GL_TEXTURE_2D, textureMap);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	
	float textureHeight = 1.0 / 8.0f;
	float textureWidth = 1.0 / 16.0f;
	vector<float> vertexData;
	vector<float> texCoordData;

	for (int y = 0; y < mapHeight; y++) {
		for (int x = 0; x < mapWidth; x++) {
			float uVal = (float)(levelData[y][x] % 16) / 16.0f;
			float vVal = (float)(levelData[y][x] / 16) / 8.0f;
			texCoordData.insert(texCoordData.end(), { uVal, vVal, 
													uVal, vVal + textureHeight, 
													uVal + textureWidth, vVal + textureHeight,
													uVal + textureWidth, vVal });

			vertexData.insert(vertexData.end(), { TILE_SIZE * x, -TILE_SIZE * y,
												TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
												(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
												(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y });
		}
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, mapWidth*mapHeight*4);
	
}

void PlatformerGame::worldToTileCoord(float worldX, float worldY, int *gridX, int *gridY) {

	*gridX = (int)(worldX  / TILE_SIZE);
	*gridY = (int)(-worldY / TILE_SIZE);
}

bool PlatformerGame::isSolid(unsigned char tile) {
	switch( tile ) {

	case 12: 
		return false;
		break;
	case 3:
	case 17: 
	case 18:
	case 34: 
		return true;
		break;
	default:
		return false;
		break;
	}
	return false;
}

float PlatformerGame::checkCollisionY(float x, float y) {
	int gridX;
	int gridY;
	worldToTileCoord(x, y, &gridX, &gridY);
	if ( gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight ) {
		return 0.0f;
	}
	if (isSolid(levelData[gridY][gridX])) {
		float yCoord = (gridY * TILE_SIZE);
		return -y - yCoord;
	}
	return 0.0f;
}

float PlatformerGame::checkCollisionX(float x, float y) {
	int gridX;
	int gridY;
	worldToTileCoord(x, y, &gridX, &gridY);
	if (gridX < 0 || gridX > mapWidth || gridY < 0 || gridY > mapHeight) {
		return 0.0f;
	}
	if (isSolid(levelData[gridY][gridX])) {
		float xCoord = (gridX * TILE_SIZE);
		return x - xCoord;
	}
	return 0.0f;
}

void PlatformerGame::collidesX(Entity* entity) {
	float adjust = checkCollisionX(entity->getX() - entity->getWidth()/2 , entity->getY());
	if (adjust != 0.0) {
		entity->setX(entity->getX() - adjust + TILE_SIZE);
		entity->setVelocityX(0.0f);
		entity->setCollidedLeft(true);
	}

	adjust = checkCollisionX(entity->getX() + entity->getWidth() / 2, entity->getY());
	if (adjust != 0.0) {
		entity->setX(entity->getX() - adjust );
		entity->setVelocityX(0.0f);
		entity->setCollidedRight(true);
	}
}

void PlatformerGame::collidesY(Entity* entity) {
	float adjust = checkCollisionY(entity->getX(), entity->getY() - (entity->getHeight() / 2));
	if (adjust != 0.0) {
		entity->setY(entity->getY() + (adjust));
		entity->setVelocityY(0.0f);
		entity->setCollidedBottom(true);
	}

	adjust = checkCollisionY(entity->getX(), entity->getY() + entity->getHeight() / 2);
	if (adjust != 0.0) {
		entity->setY(entity->getY() + (adjust) - TILE_SIZE);
		entity->setVelocityY(0.0f);
		entity->setCollidedTop(true);
	}
}

void PlatformerGame::drawText(GLuint textTexture, string text, float x, float y, float size, float r, float g, float b, float a) {

	glBindTexture(GL_TEXTURE_2D, textTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);

	vector<float> vertexData;
	vector<float> textCoordData;
	vector<float> colorData;

	float textureSize = 1.0 / 16.0f;

	for (int i = 0; i <text.size(); i++) {
		float textX = (float)(((int)text[i]) % 16) / 16.0f;
		float textY = (float)(((int)text[i]) / 16) / 16.0f;

		vertexData.insert(vertexData.end(), { (size*i) + (-0.4f*size), 0.5f*size,
			(size*i) + (-0.4f*size), -0.5f*size,
			(size*i) + (0.4f*size), -0.5f*size,
			(size*i) + (0.4f*size), 0.5f*size });

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });

		textCoordData.insert(textCoordData.end(), { textX, textY,
			textX, textY + textureSize,
			textX + textureSize, textY + textureSize,
			textX + textureSize, textY });

	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, text.size() * 4);

	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}