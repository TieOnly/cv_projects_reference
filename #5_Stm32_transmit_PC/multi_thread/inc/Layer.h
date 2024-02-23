#pragma once

class Layer
{
public:
	enum class IDLayer
	{
		Main,
		COM_Config, COM_Select,
		
		Transmit, 
		TX_Text, 
		TX_Image, TX_Image_Load, 
		TX_Video, TX_Video_Load, TX_Video_Speed,
		
		Recieve, Game,
		
		ChangeBaud,
		
		Quit
	};
public:
	static void SetIdLayer( const IDLayer& _id )
    { Get().id = _id; }
    static void SetIdLayerPre( const IDLayer& _id )
    { Get().id_pre = _id; }
    static const IDLayer& GetIdLayer()
    { return Get().id; }
    static const IDLayer& GetIdLayerPre()
    { return Get().id_pre; }
private:
	static Layer& Get()
    {
        static Layer layer;
        return layer;
    }
	IDLayer id = IDLayer::Main;
	IDLayer id_pre = IDLayer::Main;
};

