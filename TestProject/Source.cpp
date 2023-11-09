#include "SFML/Graphics.hpp"
#include "time.h"
#include <sstream>;
using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point
{int x, y;} a[4], b[4];

int figures[7][4] =
{
	1,3,5,7, // I
	2,4,5,6, // Z
	3,5,4,6, // S
	3,5,4,7, // T
	2,3,5,7, // L
	3,5,7,6, // J
	2,3,4,5, // O
};

bool check()
{
	for (int i = 0;i < 4;i++)
		if (a[i].x < 0 || a[i].x >= N || a[i].y >= M) return 0;
		else if (field[a[i].y][a[i].x]) return 0;

	return 1;
};

int main()
{
	srand(time(0));

	RenderWindow window(VideoMode(640, 960), "Tetris!");

	Texture t, background;
	t.loadFromFile("resources/images/Tetris_SFML_TileSet_Test.png");
	background.loadFromFile("resources/images/tetris_bg.png");

	Sprite s(t);
	Sprite s2(background);

	s.setTextureRect(IntRect(0, 0, 32, 32));
	s2.setTextureRect(IntRect(0, 0, 640, 960));

	Font f;
	f.loadFromFile("resources/fonts/Digital.ttf");

	int score1 = 0;

	std::ostringstream ssScore;
	ssScore << "Score: " << score1;

	Text score;
	score.setFont(f);
	score.setCharacterSize(32);
	score.setPosition({ 180,140 });
	score.setString(ssScore.str());

	int dx = 0; bool rotate = 0; int colorNum = 1;
	float timer = 0,delay = 0.3;

	Clock clock;

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer += time;

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)
				window.close();

			if (e.type == Event::KeyPressed)
				if (e.key.code == Keyboard::Up) rotate = true;
				else if (e.key.code == Keyboard::Left) dx = -1;
				else if (e.key.code == Keyboard::Right) dx = 1;
		}

		if (Keyboard::isKeyPressed(Keyboard::Down)) delay = 0.05;

		/// <- Move -> ///
		for (int i = 0;i < 4;i++) { b[i] = a[i]; a[i].x += dx; }
		if (!check()) for (int i = 0;i < 4;i++) a[i] = b[i];


		/// Rotate ///
		if (rotate)
		{
			Point p = a[1]; //center of rotation
			for (int i = 0;i < 4;i++)
			{
				int x = a[i].y - p.y;
				int y = a[i].x - p.x;
				a[i].x = p.x - x;
				a[i].y = p.y + y;
			}
			if (!check()) for (int i = 0;i < 4;i++) a[i] = b[i];
		}

		///Tick///
		if (timer > delay)
		{
			for (int i = 0;i < 4;i++) { b[i] = a[i]; a[i].y += 1; }

			if (!check())
			{
				for (int i = 0;i < 4;i++) field[b[i].y][b[i].x] = colorNum;

				colorNum = 1 + rand() % 6;
				int n = rand() % 7;
				for (int i = 0;i < 4;i++)
				{
					a[i].x = figures[n][i] % 2;
					a[i].y = figures[n][i] / 2;
				}
			}

			timer = 0;
		}


		//////check lines//////
		int k = M - 1;
		for (int i = M-1;i > 0;i--)
		{
			int count = 0;
			for (int j = 0;j < N;j++)
			{
				if (field[i][j]) count++;
				field[k][j] = field[i][j];
			}
			if (count < N) k--;
		}

		ssScore.str("");
		ssScore << "Score: " << score1;
		score.setString(ssScore.str());

		dx = 0; rotate = 0; delay = 0.3;


	//////draw//////
	window.clear(Color::White);
	window.draw(s2);

	for (int i=0; i < M; i++)
		for (int j=0;j < N;j++)
		{
			if (field[i][j] == 0) continue;
			s.setTextureRect(IntRect(field[i][j] * 32, 0, 32, 32));
			s.setPosition(j * 32, i * 32);
			window.draw(s);
		}

	for (int i = 0;i < 4;i++)
	{
		s.setTextureRect(IntRect(colorNum * 32, 0, 32, 32));
		s.setPosition(a[i].x * 32, a[i].y * 32);
		window.draw(s);
	}
	
	window.draw(s);
	window.draw(score);
	window.display();
	}

	return 0;
}
//Watch this #2
