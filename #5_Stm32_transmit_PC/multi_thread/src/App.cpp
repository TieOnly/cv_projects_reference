#include "App.h"

App::App() {
	HWND console = GetConsoleWindow();
    GetWindowRect(console, &win_size);

    MoveWindow(console, win_size.left, win_size.top, 600, 400, TRUE);
	Init_Display();
	vSeqLayer.push_back( Layer::IDLayer::Main );
}
App::~App() { bQuit = 1; }
void App::InitSubThread( App& _app )
{
	//Add sub thread
	// tie::thread::AddThread(
	// 	tie::thread::MakeLoopThread<float>, 60, AppThread::Loop_Count, 2.0f
	// );
}
void App::Active( App& _app )
{
	//Main thread
	tie::thread::AddThreadPause(
		tie::thread::MakeLoopThread<App&>, 60, App::Tick, std::ref(_app)
	);
}

void App::Update()
{
	dTime = ft.Mark();
	if( Layer::GetIdLayer() != Layer::GetIdLayerPre() )
	{
		const Layer::IDLayer id_update = Layer::GetIdLayer();
		Layer::SetIdLayerPre( id_update );
		if( vSeqLayer.back() != id_update ) vSeqLayer.push_back( id_update );
		Init_Display();
	}
	Display();
	ProcessCMD();
}
void App::Print()
{}

void App::KeyEvent()
{
	// if (GetForegroundWindow() == GetConsoleWindow())
	// {
	// 	key[0] = GetAsyncKeyState('1') & 0x8000;
	// 	key[1] = GetAsyncKeyState('5') & 0x8000;
	// 	key[2] = GetAsyncKeyState('3') & 0x8000;
	// }
	// if (key[0] && !old_key[0]) std::cout << 1 << std::endl;
	// if (key[1] && !old_key[1]) std::cout << "That's it" << std::endl;
	// if (key[2] && !old_key[2]) bQuit = true;
	// for (int i = 0; i < 3; i++) old_key[i] = key[i];
}
