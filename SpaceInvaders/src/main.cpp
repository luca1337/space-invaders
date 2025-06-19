#include <game.h>

int main(int argc, char** argv)
{
	auto g = game{};
	g.init();
	g.setup();
	g.tick();

	return 0;
}
