#include "DrawVulkan.h"
#include "destroy.h"

initVK init;

int main()
{
	createWindow();
	createCursor();
	init.InitVulkan();
	loop();
	destroy();
}