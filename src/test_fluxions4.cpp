#include <thread>
#include <chrono>
#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <hatchetfish.hpp>
#include <fluxions4_vulkan_context.hpp>
#include <fluxions4_vulkan_pipeline.hpp>
#include <fluxions4_simple_shapes.hpp>
#include <fluxions4_superquadrics.hpp>

#pragma comment(lib, "vulkan-1.lib")

class TestApp {
public:
	TestApp() : vkpipeline(vkcontext) {

	}

	~TestApp() {

	}

	bool init() {
		SDL_Init(SDL_INIT_VIDEO);
		vkcontext.setName("HelloWorld");
		vkcontext.setWindowTitle("Hello, World");
		if (!vkcontext.init()) return false;
		if (!vkpipeline.init()) return false;

		MakeDescriptorSet(vkpipeline, cubeDS);
		MakeDescriptorSet(vkpipeline, sqDS);

		Fluxions::CreateCube(cube);

		return true;
	}

	void kill() {
		cube.kill();
		sq.kill();
		// descriptor sets must be killed before pipeline
		sqDS.kill();
		cubeDS.kill();
		vkpipeline.kill();
		vkcontext.kill();
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

			const uint32_t slices = 50;
			const uint32_t stacks = 50;
			Fluxions::CreateSuperquadric(sq, 2.0f + 2.0f * sin(t1), 2.0f + 2.0f * cos(1.05f * t1), slices, stacks);

			sqDS.useDELETEME(t1 * 30.0f, -2.0f);
			cubeDS.useDELETEME(t1 * 25.0f, 2.0f);

			vkcontext.setClearColor({ std::sin(t1), 0.3f, 0.4f, 1.0f });
			vkcontext.beginFrame();

			vkpipeline.use(cubeDS);
			cube.draw();

			vkpipeline.use(sqDS);
			sq.draw();

			vkpipeline.restore();

			vkcontext.presentFrame();
		}
	}

private:
	Fluxions::VulkanMesh cube;	
	Fluxions::VulkanMesh sq;

	Fluxions::VulkanDescriptorSet cubeDS;
	Fluxions::VulkanDescriptorSet sqDS;

	Fluxions::VulkanPipeline vkpipeline;
	Fluxions::VulkanContext vkcontext;
};

int main(int argc, char** argv) {
	TestApp ta;
	try {
		if (ta.init()) {
			ta.mainloop();
			ta.kill();
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
	}
	return 0;
}
