#include "robot.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <fstream>
#include <initializer_list>

#undef main

namespace robot {

	namespace {
		constexpr int SCREEN_WIDTH = 640;
		constexpr int SCREEN_HEIGHT = 480;
		constexpr int CELL_SIZE = 32;
		constexpr int FIELD_WIDTH = SCREEN_WIDTH / CELL_SIZE;
		constexpr int FIELD_HEIGHT = SCREEN_HEIGHT / CELL_SIZE;
		constexpr int CHAR_WIDTH = 28;
		constexpr int CHAR_HEIGHT = 36;
		constexpr int CHARS_PER_ROW = 16;

		struct Sdl {
			Sdl() {
				if (SDL_Init(SDL_INIT_VIDEO) < 0) {
					fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
					exit(-1);
				}
				IMG_Init(IMG_INIT_PNG);
				window = SDL_CreateWindow(
					"Robot",
					SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
					SCREEN_WIDTH, SCREEN_HEIGHT,
					SDL_WINDOW_SHOWN
				);
				if (!window) {
					fprintf(stderr, "could not create window: %s\n", SDL_GetError());
					exit(-1);
				}
				rend = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
				if (!rend) {
					fprintf(stderr, "could not create renderer: %s\n", SDL_GetError());
					exit(-1);
				}
			}
			~Sdl() {
				SDL_DestroyWindow(window);
				SDL_DestroyRenderer(rend);
				IMG_Quit();
				SDL_Quit();
			}
			SDL_Window* window = nullptr;
			SDL_Renderer* rend = nullptr;
		};

		Sdl sdl;

		struct Image {
			SDL_Texture* tex;
			Image(const char* name) {
				SDL_Surface* surf = IMG_Load(name);
				if (!surf) {
					fprintf(stderr, "could not load image: %s, %s\n", name, SDL_GetError());
					exit(-1);
				}
				tex = SDL_CreateTextureFromSurface(sdl.rend, surf);
				if (!tex) {
					fprintf(stderr, "could not convert image: %s, %s\n", name, SDL_GetError());
					exit(-1);
				}
				SDL_FreeSurface(surf);
			}

			void blt(int sx, int sy, int dx, int dy, int w, int h) {
				SDL_Rect src{ sx, sy, w, h };
				SDL_Rect dst{ dx, dy, w, h };
				SDL_RenderCopy(sdl.rend, tex, &src, &dst);
			}
			void bltRotate(int sx, int sy, int dx, int dy, int w, int h, double angle) {
				SDL_Rect src{ sx, sy, w, h };
				SDL_Rect dst{ dx, dy, w, h };
				SDL_RenderCopyEx(sdl.rend, tex, &src, &dst, angle, nullptr, SDL_FLIP_NONE);
			}

			~Image() {
				SDL_DestroyTexture(tex);
			}
		};

		struct Scene {
			Image font;
			Image sprites;
			int x = FIELD_WIDTH / 2, y = FIELD_HEIGHT / 2, dir = 3;
			int hide_counter = 0;
			int pxX = 0, pxY = 0;

			enum {
				F_EMPTY = 0x100,
				F_CROSS = 0x200,
				F_MARK = 0x400,
				F_ARROWS = 0x800,
				F_ARROWS_SHIFT = 12,
			};
			int field[FIELD_WIDTH][FIELD_HEIGHT] = { 0 }; // 0-wall, 1-empty, 2-cross, 3-mark, 4..7 - arrows, ' '..7f - char, chars 0..9 also numbers

			Scene()
				: font("font.png")
				, sprites("sprites.png") {
				for (int x = 1; x < FIELD_WIDTH - 1; x++) {
					for (int y = 1; y < FIELD_HEIGHT - 1; y++)
						field[x][y] = F_EMPTY;
				}
				pxX = x * CELL_SIZE;
				pxY = y * CELL_SIZE;
				redraw();
			}
			void drawChar(unsigned int c, int x, int y) {
				if (c < ' ' || c > 0x7f)
					return;
				c -= ' ';
				font.blt((c % CHARS_PER_ROW) * CHAR_WIDTH, (c / CHARS_PER_ROW) * CHAR_HEIGHT, x, y, CHAR_WIDTH, CHAR_HEIGHT);
			}
			void redraw() {
				SDL_Event event;
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_QUIT) {
						exit(1);
					}
				}
				for (int x = 0; x < FIELD_WIDTH; x++) {
					for (int y = 0; y < FIELD_HEIGHT; y++) {
						auto f = field[x][y];
						sprites.blt(CELL_SIZE, f & F_EMPTY ? 0 : CELL_SIZE, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);
						if (f & F_CROSS)
							sprites.blt(0, CELL_SIZE / 2, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE / 2, CELL_SIZE / 2);
						if (f & F_MARK)
							sprites.blt(CELL_SIZE / 2, 0, x * CELL_SIZE + CELL_SIZE / 2, y * CELL_SIZE, CELL_SIZE / 2, CELL_SIZE / 2);
						if (f & F_ARROWS)
							sprites.bltRotate(CELL_SIZE / 2, CELL_SIZE / 2, x * CELL_SIZE, y * CELL_SIZE + CELL_SIZE / 2, CELL_SIZE / 2, CELL_SIZE / 2, ((f >> F_ARROWS_SHIFT) & 3) * 90);
						drawChar(f & 0xff, x * CELL_SIZE + (CELL_SIZE - CHAR_WIDTH), y * CELL_SIZE);
					}
				}
				if (!hide_counter)
					sprites.bltRotate(0, CELL_SIZE, pxX, pxY, CELL_SIZE, CELL_SIZE, dir * 90);
				SDL_RenderPresent(sdl.rend);
				if (!hide_counter)
					SDL_Delay(1000 / 60);
			}
			void animateTo(int x, int y) {
				this->x = x;
				this->y = y;
				x *= CELL_SIZE;
				y *= CELL_SIZE;
				if (hide_counter) {
					pxX = x;
					pxY = y;
				}
				do {
					if (pxX < x)
						pxX += 4;
					else if (pxX > x)
						pxX -= 4;
					if (pxY < y)
						pxY += 4;
					else if (pxY > y)
						pxY -= 4;
					redraw();
				} while (pxX != x || pxY != y);
			}
			void error(std::string message) {
				std::cout << "Error: ";
				quitWithMessage(message);
			}
			void quitWithMessage(std::string message) {
				std::cout << message << std::endl;
				for (;;) {
					redraw();
					SDL_Delay(1000 / 5);
				}
			}
			static constexpr int dx[4] = { 1, 0, -1,  0 };
			static constexpr int dy[4] = { 0, 1,  0, -1 };
		};

		Scene scene;

	} // namespace

	void hide() {
		scene.hide_counter++;
	}
	void show() {
		if (--scene.hide_counter == 0)
			scene.redraw();
	}
	void step() {
		int x = scene.x + scene.dx[scene.dir];
		int y = scene.y + scene.dy[scene.dir];
		if (x < 0 || y < 0 || x >= FIELD_WIDTH || y >= FIELD_HEIGHT)
			scene.error("Robot went out of field");
		if ((scene.field[x][y] & Scene::F_EMPTY) == 0)
			scene.error("Robot hit the wall");
		scene.animateTo(x, y);
	}
	void turnCw() {
		scene.dir = (scene.dir + 1) & 3;
		scene.redraw();
	}
	bool isAtWall() {
		int x = scene.x + scene.dx[scene.dir];
		int y = scene.y + scene.dy[scene.dir];
		return x < 0 || y < 0 ||
			x >= FIELD_WIDTH ||
			y >= FIELD_HEIGHT ||
			(scene.field[x][y] & Scene::F_EMPTY) == 0;
	}
	bool isOnCross() {
		return (scene.field[scene.x][scene.y] & Scene::F_CROSS) != 0;
	}
	bool isOnMark() {
		return (scene.field[scene.x][scene.y] & Scene::F_MARK) != 0;
	}
	void setMark() {
		if (isOnMark())
			scene.error("Robot is already on mark");
		scene.field[scene.x][scene.y] |= Scene::F_MARK;
		scene.redraw();
	}
	void clearMark() {
		if (!isOnMark())
			scene.error("Robot is not on mark");
		scene.field[scene.x][scene.y] &= ~Scene::F_MARK;
		scene.redraw();
	}
	bool isOnArrow() {
		return (scene.field[scene.x][scene.y] & Scene::F_ARROWS) != 0;
	}
	bool isSameArrowDirection() {
		if (!isOnArrow())
			scene.error("Robot is not on arrow");
		return ((scene.field[scene.x][scene.y] >> Scene::F_ARROWS_SHIFT) & 3) == scene.dir;
	}
	void setArrow() {
		auto& v = scene.field[scene.x][scene.y];
		v = (v & ~(3 << Scene::F_ARROWS_SHIFT)) | Scene::F_ARROWS | (scene.dir << Scene::F_ARROWS_SHIFT);
		scene.redraw();
	}
	void clearArrow() {
		if (!isOnArrow())
			scene.error("Robot is not on arrow");
		scene.field[scene.x][scene.y] &= ~Scene::F_ARROWS;
		scene.redraw();
	}
	bool isOnChar() {
		return (scene.field[scene.x][scene.y] & 0xff) != 0;
	}
	void setChar(char c) {
		auto& v = scene.field[scene.x][scene.y];
		v = (v & ~0xff) | (c & 0xff);
		scene.redraw();
	}
	char getChar() {
		if (!isOnChar())
			scene.error("Robot is not on character");
		return scene.field[scene.x][scene.y] & 0xff;
	}
	void clearChar() {
		if (!isOnChar())
			scene.error("Robot is not on character");
		scene.field[scene.x][scene.y] &= ~0xff;
		scene.redraw();
	}
	void saveField(const char* file_name) {
		std::ofstream f(file_name, std::ofstream::out | std::ofstream::binary);
		auto putb = [&](char c) { f.write(&c, 1); };
		putb(FIELD_WIDTH);
		putb(FIELD_HEIGHT);
		putb((char)scene.x);
		putb((char)scene.y);
		putb((char)scene.dir);
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			for (int x = 0; x < FIELD_WIDTH; x++) {
				putb((char)scene.field[x][y]);
				putb((char)(scene.field[x][y] >> 8));
			}
		}
	}
	void loadField(const char* file_name) {
		std::ifstream f(file_name, std::ofstream::in | std::ofstream::binary);
		auto getb = [&]() {
			char c;
			f.read(&c, 1);
			return c & 0xff;
			};
		if ((getb() != FIELD_WIDTH) || (getb() != FIELD_HEIGHT))
			scene.error(file_name + std::string(" has bad format"));
		scene.x = getb();
		scene.y = getb();
		scene.dir = getb();
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			for (int x = 0; x < FIELD_WIDTH; x++) {
				scene.field[x][y] = getb();
				scene.field[x][y] |= getb() << 8;
			}
		}
		scene.pxX = scene.x * CELL_SIZE;
		scene.pxY = scene.y * CELL_SIZE;
	}
	void makeField(int dir, std::initializer_list<char*> field) {
		memset(scene.field, 0, sizeof(scene.field));
		scene.dir = dir;
		int y = 0;
		for (char* r : field) {
			if (++y == FIELD_HEIGHT)
				break;
			for (int x = 1; x < FIELD_WIDTH && *r; x++, r++) {
				switch (*r) {
				case '@':
					scene.x = x;
					scene.y = y;
					scene.pxX = x * CELL_SIZE;
					scene.pxY = y * CELL_SIZE;
					[[fallthrough]];
				case ' ': scene.field[x][y] = Scene::F_EMPTY; break;
				case '#': scene.field[x][y] = 0; break;
				case '*': scene.field[x][y] = Scene::F_EMPTY| Scene::F_CROSS; break;
				case '.': scene.field[x][y] = Scene::F_EMPTY | Scene::F_MARK; break;
				case '<': scene.field[x][y] = Scene::F_EMPTY | Scene::F_ARROWS | (2 << Scene::F_ARROWS_SHIFT); break;
				case '>': scene.field[x][y] = Scene::F_EMPTY | Scene::F_ARROWS; break;
				case '^': scene.field[x][y] = Scene::F_EMPTY | Scene::F_ARROWS | (3 << Scene::F_ARROWS_SHIFT); break;
				case '_': scene.field[x][y] = Scene::F_EMPTY | Scene::F_ARROWS | (1 << Scene::F_ARROWS_SHIFT); break;
				default:
					scene.field[x][y] = Scene::F_EMPTY | *r;
					break;
				}
			}
		}
	}

	void robotMain();

} // namespace robot

using namespace robot;

int main(int argc, char* argv[]) {
	robotMain();
	saveField("last.field");
	scene.quitWithMessage("Done");
	return 0;
}
