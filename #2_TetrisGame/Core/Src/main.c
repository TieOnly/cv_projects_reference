#include "sys.h"

int main(void)
{
	user_t user;
	user_inst(&user, "tieonly");
	user.init(&user);
	while (1) { user.loop(&user); }
}