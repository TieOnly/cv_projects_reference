#pragma once
#include <windows.h>
#include <tie_thread.h>
#include <FrameTime.h>

#include <iostream>
#include <vector>
#include <string>

#include "FileIO.h"
#include "PrintText.h"

#include "Layer.h"
#include "serial.h"

class App
{
public:
	App();
    ~App() noexcept;
    App (const App& another) = delete;
    App& operator = (const App& another) = delete;
	
	void InitSubThread( App& app );
	void Active( App& app );
	static int Tick( App& app )
	{
		app.Update();
		app.Print();
		return app.bQuit;
	}
public:
	struct SerialControl
	{
		bool bInit = 0;
		bool bDoneInit = 0;
		bool bPause = 1;
		bool bExit = 0;
		int restframe = 100;
		void Reset() { bInit = bDoneInit = bExit = 0; bPause = 1; }
	};
	struct SerialImage
	{
		std::string filename = "";
		// int width, height;
		// int constr = 120;
		// std::vector<unsigned char> image;
		std::vector<uint8_t> buff;
		bool bLoadSuccess = false;
		void Reset()
		{
			filename = "";
			// width = height = 0;
			// constr = 120;
			// image.clear();
			bLoadSuccess = false;
			buff.clear();
		}
	};
	struct SerialGif
	{
		std::string filename = "";
		std::vector<std::vector<uint8_t>> buff;
		bool bLoadSuccess = false;
		void Reset()
		{
			filename = "";
			bLoadSuccess = false;
			buff.clear();
		}
	};
	enum class TypeInput
	{
		Char, Number, Str
	};
private:	
	RECT win_size;
	int bQuit = 0;
	void Update();
	void Print();

	//Timer
	FrameTime ft;
	float dTime;

	//Event
	void KeyEvent();
	bool key[3] = { false, false, false };
	bool old_key[3] = { false, false, false };

	//Display
	std::vector<Layer::IDLayer> vSeqLayer;
	std::string label;
	std::string body;
	
	std::string cmd_input;
	TypeInput type_input = TypeInput::Char;
	int cmd;
	std::vector<std::string> cmd_title;
	bool bBackable = true;
private:	//Menu config
	//init display
	void Init_Display();
	void Init_Main();

	void Init_COMconfig();
	void Init_COMselect();
	
	void Init_Transmit();
	void Init_TXtext();
	void Init_TXimage();
	void Init_TXimage_load();
	void Init_TXvideo();
	void Init_TXvideo_load();
	void Init_TXvideo_speed();

	void Init_Recieve();
	void Init_Game();
	
	void Init_ChangeBaud();
	void Init_Quit();
	
	void Display();

	//process command
	void ProcessCMD();
	void ProcessCMD_Main();
	void ProcessCMD_COMconfig();
	void ProcessCMD_COMselect();
	void ProcessCMD_Transmit();
	void ProcessCMD_TXtext();
	void ProcessCMD_TXimage();
	void ProcessCMD_TXimage_load();
	void ProcessCMD_TXvideo();
	void ProcessCMD_TXvideo_load();
	void ProcessCMD_TXvideo_speed();
	void ProcessCMD_ChangeBaud();
	void ProcessCMD_Recieve();
	void ProcessCMD_Game();
	void ProcessCMD_Quit();
private:
	std::string com_name = "";
	uint32_t baudrate = 1000000;
	const std::string ScanPorts();
	const std::vector<std::string> GetComAvailable();
	const bool CheckCOM( const std::string& com_name );
	const bool Valid_InitCOM();
	const bool CheckBreakCOM();
	SerialControl TXvideo_sc;
	SerialGif sg;
private:
	bool Load_image(std::vector<unsigned char>& image, const std::string& filename, int& x, int&y);
	SerialImage si;
	SerialControl TXimage_sc;
private:
	SerialControl RXcontrol_sc;
	uint32_t RXcontrol_baudrate = 115200;
};

namespace AppThread
{
	int Loop();
	int Loop_Count( float time );

	using millis = std::chrono::duration<std::chrono::steady_clock::rep, std::ratio<1, 1000>>;
	// int Loop_TXvideo( std::string& com, uint32_t baudrate, bool& pause, bool& exit );
	// int Loop_TXvideo( std::string& com, uint32_t baudrate, App::SerialControl& sc );
	int Loop_TXvideo( 
		std::string& com, uint32_t baudrate, 
		App::SerialControl& sc, 
		std::vector<std::vector<uint8_t>>& buff 
	);
	int Loop_TXimage( 
		std::string& com, uint32_t baudrate, 
		App::SerialControl& sc, 
		std::vector<uint8_t>& buff 
	);
	int Loop_RXcontrol( std::string& com, uint32_t baudrate, App::SerialControl& sc );
	int Loop_RXcontrol_( std::string& com, uint32_t baudrate, App::SerialControl& sc );
	void PressKey( CHAR keycode );
};
