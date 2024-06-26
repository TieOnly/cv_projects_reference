#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include "tie_st7735.h"
#include "utils.h"

int main()
{
	tft_init();
	tft_clear(0xffff);
	tft_display();
	printf("Done init tft\n");
	return 0;
}
