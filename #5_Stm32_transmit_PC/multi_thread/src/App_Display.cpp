#include "App.h"

void App::Init_Display()
{
	switch (Layer::GetIdLayer())
	{
	case Layer::IDLayer::Main: Init_Main(); break;
	case Layer::IDLayer::COM_Config: Init_COMconfig(); break;
	case Layer::IDLayer::COM_Select: Init_COMselect(); break;
	case Layer::IDLayer::Transmit: Init_Transmit(); break;
	case Layer::IDLayer::TX_Image: Init_TXimage(); break;
	case Layer::IDLayer::TX_Text: Init_TXtext(); break;
	case Layer::IDLayer::TX_Image_Load: Init_TXimage_load(); break;
	case Layer::IDLayer::TX_Video: Init_TXvideo(); break;
	case Layer::IDLayer::TX_Video_Load: Init_TXvideo_load(); break;
	case Layer::IDLayer::TX_Video_Speed: Init_TXvideo_speed(); break;
	case Layer::IDLayer::ChangeBaud: Init_ChangeBaud(); break;
	case Layer::IDLayer::Recieve: Init_Recieve(); break;
	case Layer::IDLayer::Game: Init_Game(); break;
	case Layer::IDLayer::Quit: Init_Quit(); break;
	default: break;
	}
	if( vSeqLayer.size() > 1 && bBackable ) cmd_title.push_back("Quay lai");
}

void App::Display()
{
	system("cls");
	std::cout << "==================================UART STM32==================================" << "\n\n";
	PT::printText(">> " + label + " <<", PT::Alignment::Center); std::cout << std::endl;
	std::cout << "==============================================================================" << std::endl;
	PT::printText(body, PT::Alignment::Center); std::cout << std::endl;
	for( int i = 0; i < (int)cmd_title.size(); ++i )
	{
		std::string cmd_line = std::to_string(i+1) + ". " + cmd_title[i];
		PT::printText( cmd_line, PT::Alignment::Center ); std::cout << std::endl;
	}
	PT::printText( "Enter command: ", PT::Alignment::Center );
}

void App::ProcessCMD()
{
	std::getline(std::cin, cmd_input);
	if( type_input == TypeInput::Char )
	{
		if( cmd_input[0] < '0' || cmd_input[0] > '9' ) cmd = 0;
		else cmd = cmd_input[0] - '0';
	}
	else if( type_input == TypeInput::Number )
	{
		if( cmd_input[0] < '0' || cmd_input[0] > '9' ) cmd = 0;
		else cmd = std::stoi(cmd_input);
	}
	else cmd = 0;

	switch (Layer::GetIdLayer())
	{
	case Layer::IDLayer::Main: ProcessCMD_Main(); break;
	case Layer::IDLayer::COM_Config: ProcessCMD_COMconfig(); break;
	case Layer::IDLayer::COM_Select: ProcessCMD_COMselect(); break;
	case Layer::IDLayer::Transmit: ProcessCMD_Transmit(); break;
	case Layer::IDLayer::TX_Text: ProcessCMD_TXtext(); break;
	case Layer::IDLayer::TX_Image: ProcessCMD_TXimage(); break;
	case Layer::IDLayer::TX_Image_Load: ProcessCMD_TXimage_load(); break;
	case Layer::IDLayer::TX_Video: ProcessCMD_TXvideo(); break;
	case Layer::IDLayer::TX_Video_Load: ProcessCMD_TXvideo_load(); break;
	case Layer::IDLayer::TX_Video_Speed: ProcessCMD_TXvideo_speed(); break;
	case Layer::IDLayer::ChangeBaud: ProcessCMD_ChangeBaud(); break;
	case Layer::IDLayer::Recieve: ProcessCMD_Recieve(); break;
	case Layer::IDLayer::Game: ProcessCMD_Game(); break;
	case Layer::IDLayer::Quit: ProcessCMD_Quit(); break;
	default: break;
	}

	if( vSeqLayer.size() > 1 && (cmd == (int)cmd_title.size()) )
	{
		vSeqLayer.pop_back();
		Layer::SetIdLayer( vSeqLayer.back() );
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_Main()
{
	label = "MENU";
	body = "Danh sach chuc nang";
	cmd_title.clear();
	cmd_title.push_back("COM");
	cmd_title.push_back("Truyen");
	cmd_title.push_back("Nhan");
	cmd_title.push_back("Baudrate");
	cmd_title.push_back("Thoat");
}
void App::ProcessCMD_Main()
{
	switch (cmd)
	{
	case 1: Layer::SetIdLayer( Layer::IDLayer::COM_Config ); break;
	case 2: Layer::SetIdLayer( Layer::IDLayer::Transmit ); break;
	case 3: Layer::SetIdLayer( Layer::IDLayer::Recieve ); break;
	case 4: Layer::SetIdLayer( Layer::IDLayer::ChangeBaud ); break;
	case 5: Layer::SetIdLayer( Layer::IDLayer::Quit ); break;
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_COMconfig()
{
	label = "CAU HINH CONG COM";
	body = "Danh sach cong COM kha dung: \n";
	body += App::ScanPorts();
	cmd_title.clear();
	cmd_title.push_back("Reload");
	cmd_title.push_back("Chon cong COM");
}
void App::ProcessCMD_COMconfig()
{
	switch (cmd)
	{
	case 1:
	{
		body = "Danh sach cong COM kha dung: \n";
		body += App::ScanPorts();
	} break;
	case 2:
	{
		Layer::SetIdLayer( Layer::IDLayer::COM_Select ); break;
	} break;
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_COMselect()
{
	label = "CHON CONG COM";
	body = "Danh sach cong COM kha dung:";
	body += App::ScanPorts();
	cmd_title.clear();
	const std::vector<std::string> listcom = App::GetComAvailable(); 
	for( int i = 0; i < (int)listcom.size(); i++ )
	{
		cmd_title.push_back(listcom[i]);
	}
}
void App::ProcessCMD_COMselect()
{
	if( cmd >= (int)cmd_title.size() ) return;
	com_name = cmd_title[cmd-1];
	body = "Da chon cong: " + com_name;
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_Transmit()
{
	label = "TRUYEN";
	cmd_title.clear();
	if( com_name == "" || !App::CheckCOM( com_name ) )
	{
		body = "Hay chon cong COM kha dung";
		return;
	}
	else body = "Truyen du lieu cho cong " + com_name;
	cmd_title.push_back("Van Ban");
	cmd_title.push_back("Hinh Anh");
	cmd_title.push_back("Video");
}
void App::ProcessCMD_Transmit()
{
	if( com_name == "" ) return;
	switch (cmd)
	{
	case 1: Layer::SetIdLayer( Layer::IDLayer::TX_Text ); break;
	case 2: Layer::SetIdLayer( Layer::IDLayer::TX_Image ); break;
	case 3: Layer::SetIdLayer( Layer::IDLayer::TX_Video ); break;
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_TXtext()
{
	label = "Truyen van ban";
	if( !App::Valid_InitCOM() ) return;
	body = "Nhap doan van ban can truyen";
	bBackable = false;
	type_input = TypeInput::Str;
}
void App::ProcessCMD_TXtext()
{
	if( App::CheckBreakCOM() ) return;
	//Exit mode command input type string 
	if( bBackable )
	{
		//New Transmit data
		if( cmd == 1 ) App::Init_TXtext();
		return;
	}
	bBackable = true;
	type_input = TypeInput::Char;
	cmd_title.clear();
	cmd_title.push_back("Gui moi");
	cmd_title.push_back("Quay lai");

	serial::Serial myS(com_name, baudrate);
	if( !myS.isOpen() ) myS.open();

	cmd_input += "\n";
	if( myS.isOpen() )
	{
		myS.write(cmd_input);
	}
	myS.close();
	body = cmd_input;
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_TXimage()
{
	label = "Truyen Hinh Anh";
	if( !App::Valid_InitCOM() ) return;
	body = "Truyen anh qua cong " + com_name;

	TXimage_sc.Reset();
	tie::thread::AddThread(
		tie::thread::MakeLoopThread<
			std::string&, uint32_t, 
			SerialControl&, std::vector<uint8_t>&
		>, 1000,
		AppThread::Loop_TXimage, 
		std::ref(com_name), baudrate, std::ref(TXimage_sc), std::ref(si.buff)
	);
	while( !TXimage_sc.bDoneInit ){}

	if( TXimage_sc.bInit )
	{
		cmd_title.push_back("Load anh");
		cmd_title.push_back("Truyen");
	}
	else body = "Ket noi voi cong " + com_name + " that bai";
}
void App::ProcessCMD_TXimage()
{
	switch (cmd)
	{
	case 1: 
	{
		TXimage_sc.bExit = 1;
		Layer::SetIdLayer( Layer::IDLayer::TX_Image_Load ); 
	} break;
	case 2: 
	{
		body = "Dang truyen du lieu";
		TXimage_sc.bPause = 0; 
	} break;
	case 3:
	{
		TXimage_sc.bExit = 1;
		Layer::SetIdLayer( Layer::IDLayer::Transmit );
	} break;
	
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_TXimage_load()
{
	if( !App::Valid_InitCOM() ) return;
	si.Reset();
	system("subexe\\sub_bmp.exe"); 
	
	FileIO data;
	data.NewBuff();
	if( data.LoadFromFile("./export/sub_bmp.txt") )
	{
		si.bLoadSuccess = true;
		std::stringstream buff( data.GetBuff() );
		std::string token;
		while (std::getline(buff, token))
		{
			if( token == "[Data]" )
			{
				// si.buff.clear();
				while (std::getline( buff, token, ',' ) && (token != "#"))
				{
					si.buff.push_back( (uint8_t)std::stoi(token) );						
				}
			}
			else if( token == "[Name]" )
			{
				std::getline( buff, token );
				si.filename = token;
			}
			else if( token == "[Load Failed]" )
			{
				si.bLoadSuccess = false;
				body = "Load anh that bai";
				break;
			}
		}
		if( si.bLoadSuccess ) body = "Load anh " + si.filename + " thanh cong";
	}
	else 
	{
		si.bLoadSuccess = false;
		body = "Load anh that bai";
	}
}
void App::ProcessCMD_TXimage_load()
{}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_TXvideo()
{
	label = "Truyen Video";
	cmd_title.clear();	
	if( !App::Valid_InitCOM() ) return;

	TXvideo_sc.Reset();
	tie::thread::AddThread(
		tie::thread::MakeLoopThread<
			std::string&, 
			uint32_t, 
			SerialControl&, 
			std::vector<std::vector<uint8_t>>&
		>, 10000,
		AppThread::Loop_TXvideo, 
		std::ref(com_name), baudrate, std::ref(TXvideo_sc), std::ref(sg.buff)
	);
	while( !TXvideo_sc.bDoneInit ){}
	
	if( TXvideo_sc.bInit )
	{
		body = "Toc do hien tai: " + std::to_string(TXvideo_sc.restframe) + "ms/frame. ";
		cmd_title.push_back("Truyen");
		cmd_title.push_back("Tam dung");
		cmd_title.push_back("Toc do frame");
		cmd_title.push_back("Load video");
	}
	else
		body = "Ket noi voi cong " + com_name + " that bai";
}
void App::ProcessCMD_TXvideo()
{
	if( App::CheckBreakCOM() )
	{
		TXvideo_sc.Reset();
		return;
	}
	switch (cmd)
	{
	case 1: 
	{
		body = "Dang truyen du lieu";
		TXvideo_sc.bPause = 0; 
	} break;
	case 2: 
	{
		body = "Tam dung truyen du lieu";
		TXvideo_sc.bPause = 1; 
	} break;
	case 3: 
	{
		TXvideo_sc.bExit = 1; 
		Layer::SetIdLayer( Layer::IDLayer::TX_Video_Speed );
	} break;
	case 4: 
	{
		TXvideo_sc.bExit = 1; 
		Layer::SetIdLayer( Layer::IDLayer::TX_Video_Load );
	} break;
	case 5: 
	{
		TXvideo_sc.bExit  = 1; 
	} break;
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_TXvideo_load()
{
	if( !App::Valid_InitCOM() ) return;
	body = "Nhap file co trong export (khong bao gom duoi mo rong): ";
	type_input = TypeInput::Str;
	bBackable = false;
}
void App::ProcessCMD_TXvideo_load()
{
	if( App::CheckBreakCOM() ) return;
	//Ensure data be loaded one time
	if( bBackable ) return;
	bBackable = true;
	type_input = TypeInput::Char;
	cmd_title.push_back("Quay lai");

	//Load data from .u8gif file
	FileIO data;
	data.NewBuff();
	const std::string filename = "./export/" + cmd_input + ".8bgif";
	if( data.LoadFromFile( filename ) )
	{
		sg.Reset();
		sg.bLoadSuccess = true;
		std::stringstream ss_0( data.GetBuff() );
		std::string tk_0;
		while ( std::getline( ss_0, tk_0 ) )
		{
			if( tk_0 == "[Data]" )
			{
				while( std::getline(ss_0, tk_0) && tk_0 != "#" )
				{
					sg.buff.push_back({});
					std::stringstream ss_1(tk_0);
					std::string tk_1;
					while( std::getline( ss_1, tk_1, ',' ) && tk_1 != "" )
					{
						sg.buff.back().push_back( (uint8_t)std::stoi(tk_1) );
					}
				}
			}
		}
		body = "Load video " + filename + " thanh cong";
	}
	else 
	{
		sg.bLoadSuccess = false;
		body = "Load video " + filename + " that bai";
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_TXvideo_speed()
{
	bBackable = false;
	body = "Toc do hien tai: " + std::to_string(TXvideo_sc.restframe) + "ms/frame. "; 
	body += "\nNhap toc do truyen khung hinh (ms/frame): ";
	type_input = TypeInput::Number;
	cmd_title.clear();
}
void App::ProcessCMD_TXvideo_speed()
{
	if( cmd < 0 || cmd > 1000000 )
		cmd = 100;
	type_input = TypeInput::Char;
	TXvideo_sc.restframe = cmd;
	bBackable = true;
	cmd = (int)cmd_title.size();
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_ChangeBaud()
{
	body = "Baudrate hien tai: " + std::to_string(baudrate);
	body += ", Chon toc do baud truyen du lieu";
	cmd_title.clear();
	cmd_title.push_back("9600");
	cmd_title.push_back("115200");
	cmd_title.push_back("1000000");
}
void App::ProcessCMD_ChangeBaud()
{
	switch (cmd)
	{
	case 1: 
	{
		baudrate = 9600;
		body = "Toc do baud da chon: 9600"; 
	} break;
	case 2: 
	{
		baudrate = 115200;
		body = "Toc do baud da chon: 115200"; 
	} break;
	case 3: 
	{
		baudrate = 1000000;
		body = "Toc do baud da chon: 1000000"; 
	} break;
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_Recieve()
{
	label = "NHAN";
	cmd_title.clear();
	if( com_name == "" || !App::CheckCOM( com_name ) )
	{
		body = "Hay chon cong COM kha dung";
		return;
	}
	else body = "Nhan du lieu tu cong " + com_name;
	cmd_title.push_back("Game");
}
void App::ProcessCMD_Recieve()
{
	if( com_name == "" ) return;
	switch (cmd)
	{
	case 1: Layer::SetIdLayer( Layer::IDLayer::Game ); break;
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_Quit()
{
	label = "QUIT";
	body = "Chac chan muon thoat?";
	cmd_title.clear();
	cmd_title.push_back("Dung vay");
}
void App::ProcessCMD_Quit()
{
	switch (cmd)
	{
	case 1: bQuit = 1; break;
	default: break;
	}
}

////////////////////////////////////////////////////////////////////////////////////////
void App::Init_Game()
{
	label = "Dieu Khien Game";
	if( !App::Valid_InitCOM() ) return;

	RXcontrol_sc.Reset();
	tie::thread::AddThread(
		tie::thread::MakeLoopThread<std::string&, uint32_t, App::SerialControl&>, 10000,
		AppThread::Loop_RXcontrol, 
		std::ref(com_name), RXcontrol_baudrate, std::ref(RXcontrol_sc)
	);
	while( !RXcontrol_sc.bDoneInit ) {}
 
	if( RXcontrol_sc.bInit )
	{
		body = "Mac dinh baudrate 115200";
		cmd_title.push_back("Snek");
	}
	else
		body = "Ket noi voi cong " + com_name + " that bai";
}
void App::ProcessCMD_Game()
{
	// if( !RXcontrol_sc.bInit ) return;
	if( App::CheckBreakCOM() ) return;
	switch (cmd)
	{
	case 1: 
	{
		RXcontrol_sc.bPause = 0;
		system("game\\Snek.exe"); 
	} break;
	case 2: 
	{
		RXcontrol_sc.bExit = 1;
	} break;
	default: break;
	}
}