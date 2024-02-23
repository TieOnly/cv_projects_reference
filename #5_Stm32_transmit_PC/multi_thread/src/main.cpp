#include "App.h"

int main()
{
	App app;
	app.InitSubThread( app );
	app.Active( app );
    return 0;
}