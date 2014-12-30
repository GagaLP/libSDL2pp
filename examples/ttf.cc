/*
  libSDL2pp - C++11 bindings/wrapper for SDL2
  Copyright (C) 2013-2014 Dmitry Marakasov <amdmi3@amdmi3.ru>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

#include <iostream>

//#include <SDL2/SDL.h>

#include <SDL2pp/SDL.hh>
#include <SDL2pp/SDLTTF.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Window.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Exception.hh>

using namespace SDL2pp;

int Run() {
	SDL sdl(SDL_INIT_VIDEO);
	SDLTTF ttf;
	Window window("libSDL2pp demo: font", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_RESIZABLE);
	Renderer render(window, -1, SDL_RENDERER_ACCELERATED);

	Font font(TESTDATA_DIR "/Vera.ttf", 20);

	Surface solid = font.RenderText_Solid("Hello, world! (solid mode)", SDL_Color({255, 255, 255, 255}));
	Surface shaded = font.RenderText_Shaded("Hello, world! (shaded mode)", SDL_Color({255, 255, 255, 255}), SDL_Color({127, 127, 127, 255}));
	Surface blended = font.RenderText_Blended("Hello, world! (blended mode)", SDL_Color({255, 255, 255, 255}));

	Texture solid_tex(render, solid);
	Texture shaded_tex(render, shaded);
	Texture blended_tex(render, blended);

	while (1) {
		// Process input
		SDL_Event event;
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && (event.key.keysym.sym == SDLK_ESCAPE || event.key.keysym.sym == SDLK_q)))
				return 0;

		// Clear screen
		render.SetDrawColor(0, 63, 63);
		render.Clear();

		// Render 3 strings
		int h = 0;
		render.Copy(solid_tex, NullOpt, Rect(0, h, solid.Get()->w, solid.Get()->h));
		h += solid.Get()->h;

		render.Copy(shaded_tex, NullOpt, Rect(0, h, shaded.Get()->w, shaded.Get()->h));
		h += shaded.Get()->h;

		render.Copy(blended_tex, NullOpt, Rect(0, h, blended.Get()->w, blended.Get()->h));

		render.Present();

		// Frame limiter
		SDL_Delay(1);
	}

	return 0;
}

int main() {
	try {
		return Run();
	} catch (Exception& e) {
		std::cerr << "Error: " << e.what() << " (" << e.GetSDLError() << ")" << std::endl;
	} catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}

	return -1;
}