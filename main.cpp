//declare libraries
#include <SDL.h>
#include <SDL_image.h>
#include<SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include<vector>
#include <SDL_ttf.h>
#include <sstream>
#include<fstream>
//Declare variables
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;
class LTexture
{
public:
	LTexture();
	~LTexture();
	bool loadFromFile(std::string path);
	void free();
	void render(int x, int y);
	int getWidth();
	int getHeight();
	#if defined(SDL_TTF_MAJOR_VERSION)
		
		bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
	#endif
private:
	SDL_Texture* mTexture;
	int mWidth;
	int mHeight;
};
bool init();
bool loadMedia();
void close();
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
LTexture TextureTile0, TextureTile2, TextureTile4, TextureTile8, TextureTile16, TextureTile32, TextureTile64, TextureTile128, TextureTile256, TextureTile512, TextureTile1024, TextureTile02048;
LTexture gBackgroundTexture;
LTexture Button, ButtonDown;
LTexture ScoreTexture, BestTexture;
LTexture Score, Best;
LTexture youwin, youlose;
TTF_Font* gFont = NULL;
Mix_Music* gMusic = NULL;
Mix_Chunk* gwin = NULL;
Mix_Chunk* glose = NULL;
SDL_Color textColor = { 0, 0, 0, 255 };
int score = 0, scorepermove = 0, best = 0;
int** board;
bool win = false, lose = false;

//constructor 
LTexture::LTexture()
{
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

//Destruction
LTexture::~LTexture()
{
	free();
}

//Load image from file
bool LTexture::loadFromFile(std::string path)
{
	free();
	SDL_Texture* newTexture = NULL;
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	}
	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		}
		else
		{
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	mTexture = newTexture;
	return mTexture != NULL;
}

//Freeing class variables
void LTexture::free()
{
	if (mTexture != NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

//Upload image to window at given coordinates
void LTexture::render(int x, int y)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth, mHeight };
	SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
}

//Take the size
int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}

//Load an image from a text
#if defined(SDL_TTF_MAJOR_VERSION)
bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
	if (textSurface != NULL)
	{
		//Create texture from surface pixels
		mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (mTexture == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	else
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}


	//Return success
	return mTexture != NULL;
}
#endif

//Initialize sdl, audio, tff
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0 < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
		{
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL)
			{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}

				//Initialize SDL_ttf
				if (TTF_Init() == -1)
				{
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = false;
				}
			}
		}
	}

	return success;
}

//Load resources
bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load Foo' texture
	TextureTile0.loadFromFile("res/0.png");
	TextureTile2.loadFromFile("res/2.png");
	TextureTile4.loadFromFile("res/4.png");
	TextureTile8.loadFromFile("res/8.png");
	TextureTile16.loadFromFile("res/16.png");
	TextureTile32.loadFromFile("res/32.png");
	TextureTile64.loadFromFile("res/64.png");
	TextureTile128.loadFromFile("res/128.png");
	TextureTile256.loadFromFile("res/256.png");
	TextureTile512.loadFromFile("res/512.png");
	TextureTile1024.loadFromFile("res/1024.png");
	TextureTile02048.loadFromFile("res/2048.png");
	youwin.loadFromFile("res/youwin.png");
	youlose.loadFromFile("res/youlose.png");
	gBackgroundTexture.loadFromFile("res/background.png");
	Button.loadFromFile("res/button.png");
	ButtonDown.loadFromFile("res/buttondown.png");
	Score.loadFromFile("res/scorebutton.png");
	Best.loadFromFile("res/bestbutton.png");
	gFont = TTF_OpenFont("res/arial.ttf", 20);
	gMusic = Mix_LoadMUS("res/music.wav");
	gwin = Mix_LoadWAV("res/win music.wav");
	glose = Mix_LoadWAV("res/lose music.wav");
	return success;
}

//freeing resources
void close()
{
	//Free loaded images
	TextureTile0.free();
	TextureTile2.free();
	TextureTile4.free();
	TextureTile8.free();
	TextureTile16.free();
	TextureTile32.free();
	TextureTile64.free();
	TextureTile128.free();
	TextureTile256.free();
	TextureTile512.free();
	TextureTile1024.free();
	TextureTile02048.free();
	gBackgroundTexture.free();
	Button.free();
	ButtonDown.free();
	Score.free();
	Best.free();
	youwin.free();
	youlose.free();
	//Destroy window, renderer, music and ttf 
	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	Mix_FreeMusic(gMusic);
	Mix_FreeChunk(gwin);
	Mix_FreeChunk(glose);
	TTF_CloseFont(gFont);
	//Quit SDL subsystems
	TTF_Quit();
	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}

//Create random tile in virtual board
void randomtile(int** board, int check)
{
	int count = 0;
	srand(time(NULL));
	int value;
	if (check == 0)
	{ 
		value = (rand() % (4 - 1 + 1) + 1); 
		if (value == 4) value = 4;
		else value = 2;
	}
	else value = 2;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 0) count++;
		}
	}
	int res = rand() % count;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 0) {
				if (res == 0) { board[i][j] = value; res = -1; }
				else res--;
			}
		}
	}
}

//Load Tile in virtual board to real board in window
void renderTile(int value,int i,int j)
{
	switch (value)
	{
	case 0: TextureTile0.render(131 + 97 * i, 198 + 97 * j); break;
	case 2: TextureTile2.render(131 + 97 * i, 198 + 97 * j); break;
	case 4: TextureTile4.render(131 + 97 * i, 198 + 97 * j); break;
	case 8: TextureTile8.render(131 + 97 * i, 198 + 97 * j); break;
	case 16: TextureTile16.render(131 + 97 * i, 198 + 97 * j); break;
	case 32: TextureTile32.render(131 + 97 * i, 198 + 97 * j); break;
	case 64: TextureTile64.render(131 + 97 * i, 198 + 97 * j); break; 
	case 128: TextureTile128.render(131 + 97 * i, 198 + 97 * j); break;
	case 256: TextureTile256.render(131 + 97 * i, 198 + 97 * j); break;
	case 512: TextureTile512.render(131 + 97 * i, 198 + 97 * j); break;
	case 1024: TextureTile1024.render(131 + 97 * i, 198 + 97 * j); break;
	case 2048: TextureTile02048.render(131 + 97 * i, 198 + 97 * j); break;
	}
}

//Push tiles in virtual board
void push(std::vector <int>& notmain,int &scorepermove)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = i + 1; j < 4; j++)
		{
			if ((notmain[i] == notmain[j]) && (notmain[i] != 0))
			{
				notmain[i] = notmain[i] * 2;
				scorepermove += notmain[i];
				notmain[j] = 0;
				break;
			}
			else
				if (notmain[j] != 0) break;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (notmain[i] == 0)
		{
			notmain.erase(notmain.begin() + i);
			int temp = 0;
			notmain.push_back(temp);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (notmain[i] == 0)
		{
			notmain.erase(notmain.begin() + i);
			int temp = 0;
			notmain.push_back(temp);
		}
	}
}

void moveright(int** board,int &scorepermove)
{
	for (int i = 0; i < 4; i++)
	{
		std::vector<int> notmain;
		for (int j = 3; j >= 0; j--)
		{
			int temp = board[i][j];
			notmain.push_back(temp);
		}
		push(notmain,scorepermove);
		for (int j = 3; j >= 0; j--)
		{
			board[i][j] = notmain[0];
			notmain.erase(notmain.begin());
		}
	}
}

void moveup(int** board,int &scorepermove)
{
	
	for (int j = 0; j < 4; j++)
	{
		std::vector<int> notmain;
		for (int i = 0; i < 4; i++)
		{
			int temp = board[i][j];
			notmain.push_back(temp);
		}
		push(notmain,scorepermove);
		for (int i = 0; i < 4; i++)
		{
			board[i][j] = notmain[0];
			notmain.erase(notmain.begin());
		}
	}
}

void moveleft(int** board,int &scorepermove)
{
	for (int i = 0; i < 4; i++)
	{
		std::vector<int> notmain;
		for (int j = 0; j < 4; j++)
		{
			int temp = board[i][j];
			notmain.push_back(temp);
		}
		push(notmain,scorepermove);
		for (int j = 0; j < 4; j++)
		{
			board[i][j] = notmain[0];
			notmain.erase(notmain.begin());
		}
	}
}

void movedown(int** board,int &scorepermove)
{
	for (int j = 0; j < 4; j++)
	{
		std::vector<int> notmain;
		for (int i = 3; i >= 0; i--)
		{
			int temp = board[i][j];
			notmain.push_back(temp);
		}
		push(notmain,scorepermove);
		for (int i = 3; i >= 0; i--)
		{
		board[i][j] = notmain[0];
			notmain.erase(notmain.begin());
		}
	}
}

//check the resemblance
bool checktheresemblance(int** board, int** thepreviousboard)
{
	bool check = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] != thepreviousboard[i][j]) check = false;
		}
	}
	return check;
}

// update value of board to the previous board
void updateboard(int** board, int** thepreviousboard)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			thepreviousboard[i][j] = board[i][j];
		}
	}
}

//all the event of new game button 
void handleEvent(SDL_Event* e)
{
	//If mouse event happened
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState(&x, &y);

		//Check if mouse is in button
		bool inside = true;

		//Mouse is left of the button
		if (x < 415)
		{
			inside = false;
		}
		//Mouse is right of the button
		else if (x > 415 + 106)
		{
			inside = false;
		}
		//Mouse above the button
		else if (y < 119)
		{
			inside = false;
		}
		//Mouse below the button
		else if (y > 119 + 34)
		{
			inside = false;
		}

		//Mouse is outside button
		if (!inside)
		{
			Button.render(415, 119);
		}
		//Mouse is inside button
		else
		{
			//Set mouse over sprite
			switch (e->type)
			{
			case SDL_MOUSEMOTION:
				ButtonDown.render(415, 119);
				break;

			case SDL_MOUSEBUTTONDOWN:
				ButtonDown.render(415, 119);
				for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) board[i][j] = 0;
				score = 0;
				randomtile(board, 1);
				randomtile(board, 1);
				win = false;
				lose = false;
				Mix_ResumeMusic();
				break;

			case SDL_MOUSEBUTTONUP:
				Button.render(415, 119);
				break;
			}
		}
	}
}

//Is the board full ?
bool fulltile(int** a)
{
	bool check = true;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
			if (a[i][j] == 0)
				check = false;
	}
	return check;
}

// check win 
void checkwin(bool &win, int** a)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (a[i][j] == 2048)
			{
				win = true;
				break;
			}
		}
	}
}

//check lose 
void checklose(bool& lose, int** a)
{
	bool check = false;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 3; j > 0; j--)
		{
			if (a[i][j] == a[i][j-1])
			{
				check = true;
				break;
			}
		}
	}
	for (int j = 0; j < 4; j++)
	{
		
		for (int i = 0; i < 3; i++)
		{
			if (a[i][j] == a[i+1][j])
			{
				check = true;
				break;
			}
		}
	}
	for (int i = 0; i < 4; i++)
	{
		
		for (int j = 0; j < 3; j++)
		{
			if (a[i][j] == a[i][j + 1])
			{
				check = true;
				break;
			}
		}
	}
	for (int j = 0; j < 4; j++)
	{
		for (int i = 3; i > 0; i--)
		{
			if (a[i][j] == a[i-1][j])
			{
				check = true;
				break;
			}
		}
	}
	if (check == false)
	{
		if (fulltile(board) == true)
			lose = true;
	}
}

//Read the file to get the highest score
void savebestscorein(int& best)
{
	std::ifstream file;
	file.open("bestscore.txt");
	int temp;
	file >> temp;
	if (temp > best) best = temp;
	file.close();
}

//save the best score to file
void savebestscoreout(int& bestscore)
{
	std::ifstream file;
	file.open("bestscore.txt");
	int temp;
	file >> temp;
	if (temp < best)
	{
		file.close();
		std::ofstream file;
		file.open("bestscore.txt");
		file << best;
	}
}

//the operation of functions
void play()
{
	//Start up SDL and create window

	if (!init())
	{
		printf("Failed to initialize!\n");
	}
	else
	{
		//Load media

		if (!loadMedia())
		{
			printf("Failed to load media!\n");
		}
		else
		{
			//Main loop flag

			bool quit = false;
			//Event handler

			SDL_Event e;
			board = new int* [4];
			for (int i = 0; i < 4; i++) board[i] = new int[4];
			for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) board[i][j] = 0;
			int** thepreviousboard = new int* [4];
			for (int i = 0; i < 4; i++) thepreviousboard[i] = new int[4];
			for (int i = 0; i < 4; i++) for (int j = 0; j < 4; j++) thepreviousboard[i][j] = 0;
			randomtile(board, 1);
			randomtile(board, 1);
			std::stringstream scoretext, bestscoretext;
			//While application is running
			Mix_PlayMusic(gMusic, -1);
			while (!quit)
			{
				//Handle events on queue

				while (SDL_PollEvent(&e) != 0)
				{

					score += scorepermove;
					scorepermove = 0;
					//User requests quit
					if (e.type == SDL_QUIT)
					{
						quit = true;
					}
					else if (e.type == SDL_KEYDOWN && win == false && lose == false)
					{

						switch (e.key.keysym.sym)
						{
						case SDLK_UP:
							moveleft(board, scorepermove);
							if (checktheresemblance(board, thepreviousboard) == false) randomtile(board, 0);
							break;

						case SDLK_DOWN:
							moveright(board, scorepermove);
							if (checktheresemblance(board, thepreviousboard) == false) randomtile(board, 0);
							break;

						case SDLK_LEFT:
							moveup(board, scorepermove);
							if (checktheresemblance(board, thepreviousboard) == false) randomtile(board, 0);
							break;

						case SDLK_RIGHT:
							movedown(board, scorepermove);
							if (checktheresemblance(board, thepreviousboard) == false) randomtile(board, 0);
							break;

						case SDLK_m:
							if (Mix_PlayingMusic() == 0)
							{
								//Play the music
								Mix_PlayMusic(gMusic, -1);
							}
							//If music is being played
							else
							{
								//If the music is paused
								if (Mix_PausedMusic() == 1)
								{
									//Resume the music
									Mix_ResumeMusic();
								}
								//If the music is playing
								else
								{
									//Pause the music
									Mix_PauseMusic();
								}
							}
						}
						updateboard(board, thepreviousboard);
					}
				}
				scoretext.str("");
				scoretext << score;
				bestscoretext.str("");
				bestscoretext << best;
				//Clear screen

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);
				ScoreTexture.loadFromRenderedText(scoretext.str().c_str(), textColor);
				BestTexture.loadFromRenderedText(bestscoretext.str().c_str(), textColor);
				gBackgroundTexture.render(0, 0);
				Button.render(415, 119);
				handleEvent(&e);
				Best.render(418, 33);
				Score.render(313, 33);
				savebestscorein(best);
				savebestscoreout(best);
				if (score > best) best = score;
				if (score < 10) ScoreTexture.render(358, 55);
				if (score < 100 && score>10) ScoreTexture.render(353, 55);
				if (score < 1000 && score>100) ScoreTexture.render(346, 55);
				if (score < 10000 && score>1000) ScoreTexture.render(340, 55);
				if (score > 10000) ScoreTexture.render(335, 55);
				if (best < 10) BestTexture.render(464, 55);
				if (best < 100 && best>10) BestTexture.render(459, 55);
				if (best < 1000 && best>100) BestTexture.render(452, 55);
				if (best < 10000 && best>1000) BestTexture.render(448, 55);
				if (best > 10000) ScoreTexture.render(437, 55);
				for (int i = 0; i < 4; i++)
				{
					for (int j = 0; j < 4; j++)
					{
						int value = board[i][j];
						renderTile(value, i, j);
					}
				}
				checklose(lose, board);
				checkwin(win, board);
				if (lose == true)
				{
					youlose.render(120, 120);
					Mix_PlayChannel(-1, glose, 0);
					Mix_PauseMusic();
				}
				if (win == true)
				{
					youwin.render(120, 120);
					Mix_PlayChannel(-1, gwin, 0);
					Mix_PauseMusic();
				}
				//Update screen

				SDL_RenderPresent(gRenderer);
			}
		}
	}
	//Free resources and close SDL

	close();

	
}

int main(int argc, char* args[])
{
	play();
	return 0;
}
