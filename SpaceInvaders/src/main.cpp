#include <Game.h>

int main(int argc, char** argv)
{
	auto g = Game{};
	g.init();
	g.setup();
	g.tick();

	return 0;
}
