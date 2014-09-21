//Alyssa Lui
//Game Programming
//Assignment: Pong

#include "Entity.h"

SDL_Window* displayWindow;

GLuint LoadTexture(const char *image_path)
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


void Setup(){
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_MODELVIEW);

}

void Render() {
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);
}

void drawGame(Entity c1, Entity c2, Entity b) {
	c1.draw();
	c2.draw();
	b.draw();
}

int main(int argc, char *argv[])
{
	Setup();
	
	float lastFrameTicks = 0.0f;
	float angle = 0.0f;
	
	Entity catOne(0.8, 0.0, 0.4, 0.4, LoadTexture("kitty.png"));
	Entity catTwo(-0.8, 0.0, 0.4, 0.4, LoadTexture("kittyLeft.png"));
	Entity ball(0.0, 0.0, 0.1, 0.1, LoadTexture("ball.png"));
	ball.changeDirection( -0.2, -0.1 );
	ball.setSpeed( 2.0 );

	Render();
	
	bool done = false;
	SDL_Event event;
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	
	while (!done) {	
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}	

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;
		//angle += elapsed;
		

		ball.setX( ball.getX() + (elapsed * ball.getDirectionX() * ball.getSpeed() ));
		ball.setY( ball.getY() + (elapsed * ball.getDirectionY() * ball.getSpeed() ));
		
		//Checks if the ball hits the floor or ceiling
		if( ball.getY() > 1.0 || ball.getY() < -1.0 ) {
			ball.changeDirection( ball.getDirectionX(), (-1.0)*ball.getDirectionY() ); 
		}

		//Checks if the ball hits the paddle
		if( (ball.getX()-(ball.getWidth()/2)) < (catTwo.getX()+(catTwo.getWidth()/2)) && (ball.getY()>(catTwo.getY() - (catTwo.getWidth()/2))) && (ball.getY()<(catTwo.getY() + (catTwo.getWidth()/2))) ) {
			ball.changeDirection( (-1.0)*ball.getDirectionX(), ball.getDirectionY() ); 
		}
		if( (ball.getX()-(ball.getWidth()/2)) > (catOne.getX()-(catOne.getWidth()/2)-0.1) && (ball.getY()>(catOne.getY() - (catOne.getWidth()/2))) && (ball.getY()<(catOne.getY() + (catOne.getWidth()/2))) ) {
			ball.changeDirection( (-1.0)*ball.getDirectionX(), ball.getDirectionY() ); 
		}

		else if( ball.getX() < -1.33 ) {
			glClearColor(0.7, 0.5, 0.5, 1.0);
			ball.setX( 0.0 );
			ball.setY( 0.0 );
		}
		else if( ball.getX() > 1.33 ) {
			glClearColor(0.03, 0.72, 0.7, 1.0);
			ball.setX( 0.0 );
			ball.setY( 0.0 );
		}
		
		if( keys[SDL_SCANCODE_UP] ) {
			if( (catOne.getY() + elapsed) < 1.0 ) {
				catOne.setY( catOne.getY() + elapsed ) ;
			}
		}
		else if( keys[SDL_SCANCODE_DOWN] ) {
			if( ((catOne.getY()-0.2)+ elapsed) > -1.0 ) {
				catOne.setY( catOne.getY() - elapsed ) ;
			}
		}
		else if( keys[SDL_SCANCODE_W] ) {
			if( (catTwo.getY() + elapsed) < 1.0 ) {
				catTwo.setY( catTwo.getY() + elapsed ) ;
			}
		}
		else if( keys[SDL_SCANCODE_S] ) {
			if( ((catTwo.getY()-0.2)+ elapsed) > -1.0 ) {
				catTwo.setY( catTwo.getY() - elapsed ) ;
			}
		}
		glClear(GL_COLOR_BUFFER_BIT);
		drawGame(catOne, catTwo, ball);

		SDL_GL_SwapWindow(displayWindow);
	}

	SDL_Quit();
	return 0;
}