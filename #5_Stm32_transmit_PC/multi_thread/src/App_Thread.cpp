#include "App.h"
#include "tiemem.h"

int AppThread::Loop()
{	
	static FrameTime ft;
    static float dura = 0.0f;
    // std::cout << "Dtime: "<< ft.Mark() << std::endl;
	dura += ft.Mark();
	if( dura >= 1.0f )
	{
		dura -= 1.0f;
		std::cout << "TickTX(" << 1 << "): " << dura << std::endl;
	}
    return 0;
}
int AppThread::Loop_Count( float time )
{
	static FrameTime ft;
    static float dura = 0.0f;
    // std::cout << "Dtime: "<< ft.Mark() << std::endl;

    dura += ft.Mark();
    if( dura >= time )
    {
		if( Layer::GetIdLayer() == Layer::IDLayer::Game )
		{
			std::cout << "Tick(" << time << "): " << dura << std::endl;
			// PressKey('w');
		}
        dura -= time;
    }
	return 0;
}
int AppThread::Loop_TXvideo( 
	std::string& com, uint32_t baudrate, 
	App::SerialControl& sc, 
	std::vector<std::vector<uint8_t>>& buff 
)
{
	static int idx = 0;
	static int cnt_frame = buff.size();
	static serial::Serial mys(com, baudrate);
	static FrameTime ft;
    static float dura = 0.000f;
    static float dura_thres = 0.000f;
    static float dtime = 0.000f;
	dtime = ft.Mark();

	if( !sc.bInit )
	{
		mys.setPort(com);
		mys.setBaudrate(baudrate);
		if( !mys.isOpen() ) mys.open();
		idx = 0;
		cnt_frame = buff.size();
		dura = 0.000f;
		dura_thres = (float)sc.restframe / 1000.000f;
	}
	
	if( mys.isOpen() ) sc.bInit = 1;
	else sc.bInit = 0;
	sc.bDoneInit = 1;

	if( !sc.bPause ) dura += dtime;
	else dura = 0.000f;
	if( dura >= dura_thres && mys.isOpen() && !sc.bPause && !sc.bExit )
	{
		// std::this_thread::sleep_until(std::chrono::steady_clock::now() + millis{sc.restframe});
		// std::cout << "Frame: " << dura << " Thread: " << dura_thres << std::endl;
		dura -= dura_thres;
		if( cnt_frame > 0 )
		{
			mys.write(buff[idx]);
			++idx %= cnt_frame;
		}
	}
	if( sc.bExit || !mys.isOpen() )
	{
		mys.close();
		return 1;
	}
	return 0;
}
int AppThread::Loop_TXimage( 
	std::string& com, uint32_t baudrate, 
	App::SerialControl& sc, 
	std::vector<uint8_t>& buff 
)
{
	static serial::Serial mys(com, baudrate);
	static FrameTime ft;
    static float dura = 0.000f;
    static float dura_thres = 0.000f;
    static float dtime = 0.000f;
	dtime = ft.Mark();

	if( !sc.bInit )
	{
		mys.setPort(com);
		mys.setBaudrate(baudrate);
		if( !mys.isOpen() ) mys.open();
		sc.restframe = 1000;
		dura = 0.000f;
		dura_thres = (float)sc.restframe / 1000.000f;
	}
	
	if( mys.isOpen() ) sc.bInit = 1;
	else sc.bInit = 0;
	sc.bDoneInit = 1;

	if( !sc.bPause ) dura += dtime;
	else dura = 0.000f;
	if( dura >= dura_thres && mys.isOpen() && !sc.bPause && !sc.bExit )
	{
		dura -= dura_thres;
		mys.write(buff);
	}

	if( sc.bExit || !mys.isOpen() )
	{
		mys.close();
		return 1;
	}
	return 0;
}
int AppThread::Loop_RXcontrol( std::string& com, uint32_t baudrate, App::SerialControl& sc )
{
	static serial::Serial mys(com, baudrate);

	if( !sc.bInit )
	{
		mys.setPort(com);
		mys.setBaudrate(baudrate);
		if( !mys.isOpen() ) mys.open();
	}
	
	if( mys.isOpen() ) sc.bInit = 1;
	else sc.bInit = 0;
	sc.bDoneInit = 1;

	if( mys.isOpen() && !sc.bPause && !sc.bExit )
	{
		if( mys.available() )
		{
			std::string recv = mys.read();
			if( recv[0] == 'a' || recv[0] == 'd' || recv[0] == 'w' || recv[0] == 's' )
			{
				PressKey(recv[0]);	
				std::cout << "Nhan du lieu (" << recv.size()*sizeof(byte) << "): " << recv[0] << std::endl;
			}
			mys.flushInput();
		}
	}
	if( sc.bExit || !mys.isOpen() )
	{
		mys.close();
		return 1;
	}
	return 0;
}
int AppThread::Loop_RXcontrol_( std::string& com, uint32_t baudrate, App::SerialControl& sc )
{
	static serial::Serial mys(com, baudrate);

	if( !sc.bInit )
	{
		mys.setPort(com);
		mys.setBaudrate(baudrate);
		if( !mys.isOpen() ) mys.open();
	}
	
	if( mys.isOpen() ) sc.bInit = 1;
	else sc.bInit = 0;
	sc.bDoneInit = 1;

	if( mys.isOpen() && !sc.bPause && !sc.bExit )
	{
		if( mys.available() )
		{
			std::string recv = mys.read();
			if( recv[0] == 'a' || recv[0] == 'd' || recv[0] == 'w' || recv[0] == 's' )
			{
				PressKey(recv[0]);	
				std::cout << "Nhan du lieu (" << recv.size()*sizeof(byte) << "): " << recv[0] << std::endl;
			}
			mys.flushInput();
		}
	}
	if( sc.bExit || !mys.isOpen() )
	{
		mys.close();
		return 1;
	}
	return 0;
}

void AppThread::PressKey( CHAR keycode )
{
	SHORT key;
	UINT mapkeypad;
	INPUT in = {0};
	key = VkKeyScan(keycode);
	mapkeypad = MapVirtualKey( LOBYTE(key), 0 );
	in.type = INPUT_KEYBOARD;
	in.ki.dwFlags = KEYEVENTF_SCANCODE;
	in.ki.wScan = mapkeypad;
	SendInput(1, &in, sizeof(in));
	std::this_thread::sleep_for(std::chrono::milliseconds(20));
	in.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;
	SendInput(1, &in, sizeof(in));
}
