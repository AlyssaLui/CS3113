#include "SpaceInvaders.h"
using namespace std;


SpaceInvaders::SpaceInvaders() {

	Setup();
	bool done = false;
	score = 0;
	state = STATE_MAIN_MENU;
}

void SpaceInvaders::Setup() {
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_MODELVIEW);

	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(0.5, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	textSheet = loadTexture("font.png");
	movedirection = 1;

	Sprite kitty = Sprite(0.5, -0.7f, 103.0f / 256.0f, 0.0f, 50.0f / 256.0f, 92.0f / 256.0f, true, loadTexture("sprites.png"));
	kitties.push_back(kitty);

	for (int i = 0; i < 23; i++) {
		puppies.insert(puppies.end(), Sprite(-0.8f, 0.6f, 89.0f / 256.0f, 102.0f / 256.0f, 101.0f / 256.0f, 100.0f / 256.0f, true, loadTexture("sprites.png")));
	}

	int spriteCounter = 0;
	bool row2 = true;
	bool row3 = false;
	for (int k = 0; k < puppies.size(); k++) {
		if (spriteCounter < 7) {
			puppies[k].setX(puppies[k].getX() + spriteCounter*0.17);
			spriteCounter++;
		}
		else if (row2){
			spriteCounter = 0;
			for (int j = 7; j < puppies.size(); j++) {
				puppies[j].setY(puppies[j].getY() - 0.2f);
			}
			row2 = false;
			row3 = true;
		}
		else if (row3) {
			spriteCounter = 0;
			for (int i = 15; i < puppies.size(); i++) {
				puppies[i].setY(puppies[i].getY() - 0.2f);
			}
			row3 = false;
		}
	}

	for (int i = 0; i < MAX_BULLETS; i++) {
		Bullet newBullet(0.0f, 0.0f, 0.0f, 0.0f, 101.0f / 256.0f, 100.0f / 256.0f, loadTexture("sprites.png"));
		bullets.push_back(newBullet);
	}

	for (int numBones = 0; numBones < 10; numBones++) {
		bones.insert( bones.end(), Bullet(0.0f, 0.0f, 0.0f, 102.0f / 256.0f, 87.0f / 256.0f, 90.0f / 256.0f, loadTexture("sprites.png")));
	}
}

bool SpaceInvaders::Update() {

	SDL_Event event;	
	keys = SDL_GetKeyboardState(NULL);
	float lastFrameTicks = 0.0f;
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;
	cooldown++;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
	}

	if (keys[SDL_SCANCODE_S]) {
		state = STATE_GAME_LEVEL;
	}

	if (keys[SDL_SCANCODE_R]) {
		state = STATE_MAIN_MENU;
	}

	if (keys[SDL_SCANCODE_LEFT]) {
		if(( kitties[0].getX() - 0.003f ) > -1.0 ) {
			kitties[0].setX(kitties[0].getX() - 0.003f);
		}
	}

	if( keys[SDL_SCANCODE_RIGHT] ) {
		if ((kitties[0].getX() + 0.003f) < 1.0) {
			kitties[0].setX(kitties[0].getX() + 0.003f);
		}
	}
	
	if (keys[SDL_SCANCODE_SPACE]) {
		shootBullet(kitties[0]);
	}

	for (int k = 0; k < bullets.size(); k++) {
		for (int j = 0; j < puppies.size(); j++){	
			if (bullets[k].getX() > (puppies[j].getX() - 0.15*puppies[j].getWidth()) &&
				bullets[k].getX() < (puppies[j].getX() + 0.15*puppies[j].getWidth()) &&
				bullets[k].getY() > (puppies[j].getY() - 0.15*puppies[j].getHeight()) &&
				bullets[k].getY() < (puppies[j].getY() + 0.15*puppies[j].getHeight()) &&
				bullets[k].getVisible()) {
				score++;
				puppies.erase(puppies.begin() + j);
				bullets[k].setVisible(false);
				j--;
			}
		}
	}

	dropBone();


	if (score == 23) {
		state = STATE_GAME_OVER;
	}

	Render();
	
	return done;

}

void SpaceInvaders::Render() {

	glClear(GL_COLOR_BUFFER_BIT);

	switch (state) {
		case STATE_MAIN_MENU:
			renderMain();
			break;
		case STATE_GAME_LEVEL:
			renderLevel();
			break;
		case STATE_GAME_OVER:
			renderEnd();
			break;
	}

	SDL_GL_SwapWindow(displayWindow);

}

void SpaceInvaders::renderMain() {

	drawText( textSheet, "Kitty vs. Puppies", -0.8f, 0.1f, 0.1f, 1.0f, 0.5f, 0.4f, 1.0f);
	drawText( textSheet, "Press S to start", -0.8f, 0.0f, 0.1f, 1.0f, 0.5f, 0.4f, 1.0f);

}

void SpaceInvaders::renderLevel() {

	glClearColor(0.9, 0.5, 0.5, 1.0);
	drawText(textSheet, "Score: ", -0.8f, 0.8f, 0.1f, 1.0f, 0.5f, 0.4f, 1.0f);
	drawText(textSheet, to_string(score), 0.0f, 0.8f, 0.1f, 1.0f, 0.5f, 0.4f, 1.0f);

	kitties[0].drawSprite(0.2f);
		
	for( int k = 0; k < puppies.size(); k++ ) {
		puppies[k].drawSprite(0.2f);
	}

	for (int i = 0; i < bullets.size(); i++) {
		if (bullets[i].getVisible()) {
			bullets[i].setY(bullets[i].getY() + 8.0*0.001);
		}
	}
	
	for (int j = 0; j < bullets.size(); j++) {
		if (bullets[j].getVisible()) {
			bullets[j].draw(0.05f);
		}
	}

	for (int c = 0; c < bones.size(); c++) {
		if (bones[c].getVisible()) {
			bones[c].setY(bones[c].getY() - 5.0*0.001);
		}
	}

	for (int d = 0; d < bones.size(); d++) {
		if (bones[d].getVisible()) {
			bones[d].draw(0.05f);
		}
	}

	float minX = puppies[0].getX();	
	float maxX = puppies[0].getX();
	for (int counter = 0; counter < puppies.size(); counter++ ) {
		if (puppies[counter].getX() > maxX ) {
			maxX = puppies[counter].getX();
		}
	}
	
	if (maxX > 0.9){
		movedirection = 0;
	}
	else if (minX < -0.9){
		movedirection = 1;
	}
	
	if (movedirection == 1) {
		for (int a = 0; a < puppies.size(); a++){
			puppies[a].setX(puppies[a].getX() + 0.003);
		}
	}
	else if (movedirection == 0) {
		for (int b = 0; b < puppies.size(); b++){
			puppies[b].setX(puppies[b].getX() - 0.003);
		}
	}


}

void SpaceInvaders::renderEnd() {

	GLuint textSheet = loadTexture("font.png");
	drawText(textSheet, "Game Over", -0.8f, 0.3f, 0.1f, 1.0f, 0.5f, 0.4f, 1.0f);
	drawText(textSheet, "Press R to restart", -0.8f, -0.1f, 0.1f, 1.0f, 0.5f, 0.4f, 1.0f);
}

GLuint SpaceInvaders::loadTexture(const char *image_path)
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

void SpaceInvaders::drawText( GLuint textTexture, string text, float x, float y, float size, float r, float g, float b, float a ) {
	
	glBindTexture(GL_TEXTURE_2D, textTexture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glLoadIdentity();
	glTranslatef( x, y, 0.0 );

	vector<float> vertexData;			
	vector<float> textCoordData;				
	vector<float> colorData;
	
	float textureSize = 1.0/16.0f;

	for( int i=0; i <text.size(); i++ ) {
		float textX = (float) (((int)text[i]) % 16 ) / 16.0f;
		float textY = (float) (((int)text[i]) / 16 ) / 16.0f;		
			
		vertexData.insert(vertexData.end(),	{	(size*i) + (-0.4f*size), 0.5f*size,
												(size*i) + (-0.4f*size), -0.5f*size,
												(size*i) + (0.4f*size), -0.5f*size,
												(size*i) + (0.4f*size), 0.5f*size });
		
		colorData.insert( colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
		
		textCoordData.insert( textCoordData.end(), {textX, textY, 
													textX, textY + textureSize, 
													textX + textureSize, textY + textureSize, 
													textX + textureSize, textY });
		
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer (2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, textCoordData.data());
	glEnableClientState( GL_TEXTURE_COORD_ARRAY);
						
	glDrawArrays(GL_QUADS, 0, text.size() * 4);
		
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

}

void SpaceInvaders::shootBullet( Sprite obj ) {
	if (cooldown > 20) {
		for (int i = 0; i < bullets.size(); i++) {
			if (bullets[i].getY() > 1.0f) {
				bullets[i].setVisible(false);
			}
		}
		int idx = 0;
		while ((bullets[idx].getVisible()) && idx < MAX_BULLETS - 1) {
			idx++;
			if (idx == MAX_BULLETS) {
				idx = 0;
			}
		}

		bullets[idx].setVisible(true);
		bullets[idx].setX(obj.getX());
		bullets[idx].setY(obj.getY()+ 0.001);

		cooldown = 0;
	}
}

void SpaceInvaders::dropBone() {
	if (dropBuffer > 30) {
		for (int i = 0; i < bones.size(); i++) {
			if (bones[i].getY() < -1.0) {
				bones[i].setVisible(false);
			}
		}
		int index = 0;
		while ((bones[index].getVisible()) && index < bones.size()){
			index++;
			if (index == bones.size()) {
				index = 0;
			}
		}
		
		int randNum = rand() % (puppies.size());
		bones[index].setX(puppies[randNum].getX());
		bones[index].setY(puppies[randNum].getY());
		bones[index].setVisible(true);

		dropBuffer = 0;
	}
}