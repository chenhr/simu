#include "reactor.h"
#include "global.h"
using namespace simu_global;

int main(int argc, char **argv)
{
	Reactor reactor;
	reactor.run(kStartRou, kEndRou, kStepRou);
	return 0;
}