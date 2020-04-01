#include <thread>
#include <chrono>
#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <fluxions_vulkan_context.hpp>
#include <fluxions_vulkan_config.hpp>
#include <hatchetfish.hpp>

#pragma comment(lib, "vulkan-1.lib")

class TestApp {
public:
	TestApp() : vconfig(vcontext) {

	}

	~TestApp() {

	}

	bool init() {
		SDL_Init(SDL_INIT_VIDEO);
		vcontext.setName("HelloWorld");
		vcontext.setWindowTitle("Hello, World");
		if (!vcontext.init()) return false;
		if (!vconfig.init()) return false;
		return true;
	}

	void kill() {
		vconfig.kill();
		vcontext.kill();
		SDL_Quit();
	}

	void mainloop() {
		using namespace std::chrono_literals;
		Hf::StopWatch sw;
		bool done = false;
		float t0 = sw.Stop_sf();
		while (!done) {
			float t1 = sw.Stop_sf();
			float dt = t1 - t0;
			t0 = t1;
			SDL_Event e;
			while (SDL_PollEvent(&e)) {
				switch (e.type) {
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE)
						done = true;
					break;
				case SDL_WINDOWEVENT:
					if (e.window.event == SDL_WINDOWEVENT_CLOSE) {
						done = true;
					}
				}
			}

			vcontext.setClearColor({ std::sin(t1), 0.3f, 0.4f, 1.0f });
			vcontext.beginFrame();
			
			vconfig.use(t1 * 30.0f);
			vconfig.restore();

			vcontext.presentFrame();
		}
	}

private:
	Fluxions::VulkanContext vcontext;
	Fluxions::VulkanConfig vconfig;
};

int main(int argc, char** argv) {
	TestApp ta;
	try {
		if (ta.init()) {
			ta.mainloop();
			ta.kill();
		}
	}
	catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}