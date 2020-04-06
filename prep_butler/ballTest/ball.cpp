#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
	#include <SDL.h>
	#include <SDL_image.h>
#else
	#include <SDL2/SDL.h>
	#include <SDL2/SDL_image.h>
#endif

#include <iostream>
#include <math.h>

using std::cout;
using std::cin;
using std::endl;
using std::string;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TOTAL_PARTICLES = 2;




//Texture wrapper class
class LTexture
{
	public:
		//Initializes variables
		LTexture();

		//Deallocates memory
		~LTexture();

		//Loads image at specified path
		bool loadFromFile( std::string path );
		
		#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
		//Creates image from font string
		bool loadFromRenderedText( std::string textureText, SDL_Color textColor );
		#endif

		//Deallocates texture
		void free();

		//Set color modulation
		void setColor( Uint8 red, Uint8 green, Uint8 blue );

		//Set blending
		void setBlendMode( SDL_BlendMode blending );

		//Set alpha modulation
		void setAlpha( Uint8 alpha );
		
		//Renders texture at given point
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

		//Gets image dimensions
		int getWidth();
		int getHeight();

	private:
		//The actual hardware texture
		SDL_Texture* mTexture;

		//Image dimensions
		int mWidth;
		int mHeight;
};

//Scene textures
LTexture gDustTexture;
LTexture gDotTexture;

class Particle
{
	public:
		//Initialize position and animation
		Particle( int x, int y );

		//Shows the particle
		void render();

		//Checks if particle is dead
		bool isDead();

	private:
		//Offsets
		int mPosX, mPosY;
		
		//Velocify
		float velX, velY;

		//Current frame of animation
		int mFrame;

		//Type of particle
		LTexture *mTexture;
};

//The ball that will b o u w n s e
class Ball
{
    public:
		//The dimensions of the ball
		static const int BALL_WIDTH = 20;
		static const int BALL_HEIGHT = 20;

		//Maximum axis velocity of the ball
		static const int BALL_VEL = 10;

		//Initializes the variables and allocates particles
		Ball();

		//Deallocates particles
		~Ball();

		//Takes key presses and adjusts the ball's velocity
		void handleEvent( SDL_Event& e );

		//Moves the ball
		void move();

		//Shows the ball on the screen
		void render(int, int);

    private:
		//The particles
		Particle* particles[ TOTAL_PARTICLES ];

		//Shows the particles
		void renderParticles(int, int);

		//The X and Y offsets of the ball
		int mPosX, mPosY;

		//The velocity of the ball
		int mVelX, mVelY;
};

Ball::Ball()
{
    //Initialize the offsets
    mPosX = 0;
    mPosY = 0;

    //Initialize the velocity
    mVelX = 0;
    mVelY = 0;

    //Initialize particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ] = new Particle( mPosX, mPosY );
    }
}

Ball::~Ball()
{
    //Delete particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        delete particles[ i ];
    }
}

Particle::Particle( int x, int y )
{
    //Set offsets
    mPosX = x - 5 + ( rand() % 25 );
    mPosY = y - 5 + ( rand() % 25 );

    //Initialize animation
    mFrame = rand() % 5;


    mTexture = &gDustTexture;
}

void Particle::render()
{
    //Show image
	mTexture->render( mPosX, mPosY );

    //Show shimmer
    //if( mFrame % 2 == 0 )
    //{
		//gShimmerTexture.render( mPosX, mPosY );
    //}

    //Animate
    mFrame++;
}

bool Particle::isDead()
{
    return mFrame > 10;
}



bool initSdlWindow(SDL_Window* &gWindow, SDL_Renderer* &gRenderer) {

	bool success = true;
	if(SDL_Init(SDL_INIT_VIDEO) < 0 ) {
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	} else {
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if(gWindow == NULL) {
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl; 
		} else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			int imgFlags = IMG_INIT_PNG;
			if(!(IMG_Init(imgFlags) & imgFlags)) {
				cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError() << endl;
				success = false;
			}
		}
	}
	return(success);
}


void closeSdlWindow(SDL_Window* &gWindow ,SDL_Renderer* &gRenderer, SDL_Texture* &gTexture) {
	SDL_DestroyTexture( gTexture );
    gTexture = NULL;

    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(string imageName, SDL_Renderer* &gRenderer) {
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(imageName.c_str());
	if(loadedSurface == NULL) {
		cout << "Unable to load image \"" << imageName << "\"! SDL Error:" << SDL_GetError() << endl;
	} else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if(newTexture == NULL) {
			cout << "Unable to create texture from \"" << imageName << "\"! SDL Error: " << SDL_GetError() << endl;
		}
		SDL_FreeSurface(loadedSurface);
	}
	return(newTexture);
}



	SDL_Window* gWindow = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Texture* gTexture = NULL;

LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

LTexture::~LTexture()
{
	//Deallocate
	free();
}

void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void LTexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void LTexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void LTexture::render( int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool LTexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void Ball::render(int bX, int bY)
{
    //Show the dot
	gDotTexture.render( bX, bY );

	//Show particles on top of dot
	renderParticles(mPosX, mPosY);
}

void Ball::renderParticles(int bX, int bY)
{
	//Go through particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        //Delete and replace dead particles
        if( particles[ i ]->isDead() )
        {
            delete particles[ i ];
            particles[ i ] = new Particle( bX, bY );
        }
    }

    //Show particles
    for( int i = 0; i < TOTAL_PARTICLES; ++i )
    {
        particles[ i ]->render();
    }
}


bool init()
{
	freopen("debug.txt","w",stdout);
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load dust texture
	if( !gDustTexture.loadFromFile( "dust.png" ) )
	{
		printf( "Failed to load dust texture!\n" );
		success = false;
	}
		if( !gDotTexture.loadFromFile( "red.bmp" ) )
	{
		printf( "Failed to load dust texture!\n" );
		success = false;
	}
	
	//Set texture transparency
	gDustTexture.setAlpha( 255 );

	return success;
}

void close()
{
	//Free loaded images
	gDustTexture.free();
	gDotTexture.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

bool isWindowMoved( SDL_Event& e, bool alreadyPaused){
	if (e.type == SDL_WINDOWEVENT){
		if (e.window.event == SDL_WINDOWEVENT_MOVED){
			return true;
		}
	}
	/*else if (e.type == SDL_ExposeEvent){
			printf("SDL_VIDEOEXPOSE was triggered\n");
			return false;
		}*/
	//Event is NOT a windowevent, return whatever the pause state was.
	return alreadyPaused;
}



int main(int argc, char* argv[]) {
	bool quit = false;
	SDL_Event e;

	SDL_Rect ball;
	ball.x = SCREEN_WIDTH / 2 - 50;
	ball.y = 110;
	ball.w = 100;
	ball.h = 100;
	Ball b;

	float vel = 0;
	int deltaTicks = 0;
	int prevTime = 0;
	int currentTime;
	float deltaTime = 0;
	float deltaY;
	float ballY = 110;
	float bounciness = 0.5;
	//Max cap for the delta time
	const Uint32 max_delay = 32;
	
	bool gamePaused = false;
//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			printf( "Testing!\n");
	/*if(!initSdlWindow(gWindow, gRenderer)) {
		cout << "Failed to initialize!" << endl;
	} else {*/
		gTexture = loadTexture("amigaball.png", gRenderer);
		while(!quit) {
			while(SDL_PollEvent(&e) != 0) {
				if(e.type ==  SDL_QUIT) {
					quit = true;
				}
				/*if(e.type == SDL_VIDEOEXPOSE){
					printf("SDL_video beep bopp\n");
				}*/
				//gamePaused = isWindowMoved(e,gamePaused);
			}
			if (!gamePaused){
			currentTime = SDL_GetTicks();
			deltaTicks = currentTime - prevTime;
			deltaTicks = std::min<Uint32>(deltaTicks,max_delay);
			prevTime = currentTime;
			deltaTime = (float)deltaTicks/50;
			deltaY = (vel * deltaTime) + (.5 * 9.81 * (deltaTime * deltaTime));			
			ballY = ballY + deltaY;
			vel = vel + (9.81 * deltaTime);

			if(ballY > SCREEN_HEIGHT -100) {
				ballY = SCREEN_HEIGHT - 100;
				vel *= -1 * bounciness;
				if (vel < -10) {
					printf("Velocity: %f, Ball X: %i, Ball Y: %i\n",vel, ball.x, ball.y);
					b.render(ball.x, ball.y);
				}
			}
			if(ballY < 0) {
				ballY = 0;
				vel *= -1 * bounciness;
			}
			
			SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(gRenderer);

			ball.y = floor(ballY);

			SDL_RenderCopy(gRenderer, gTexture, NULL, &ball);

			SDL_RenderPresent(gRenderer);
		//}
		}//end of if window not being moved
	}
		}
	}
	closeSdlWindow(gWindow, gRenderer, gTexture);
	fclose(stdout);
	return(0);
}
