#pragma once

#include "DxLib.h"
#include <string>

const int RESOURCE_SIZE = 512;

enum gh{
	dot100,
	dot32,
	rect100,
	rect32,
	image,
};

enum sh{
	chari05,
	cut00,
};

class Resource{
public:

	//シングルトン
	static Resource& get(){
		static Resource inp;
		return inp;
	}

	//グラフィックハンドルを取得
	static int graphic(int num){
		return get().GHandle[num];
	}
	
	//音のハンドルを取得
	static int sound(int num){
		return get().SHandle[num];
	}

	//フォントのハンドルを取得
	static int font(int num){
		return get().FontHandle[num];
	}

	//画像・効果音・フォントをロード
	void load_png_wav(){
		GHandle[dot100] = LoadGraph("Images/dot100.png");
		GHandle[dot32] = LoadGraph("Images/dot32.png");
		GHandle[rect100] = LoadGraph("Images/rect100.png");
		GHandle[rect32] = LoadGraph("Images/rect32.png");
		GHandle[image] = LoadGraph("Images/image.jpg");

		SHandle[chari05] = LoadSoundMem("Sounds/chari05.wav");
		SHandle[cut00] = LoadSoundMem("Sounds/cut00.wav");

		FontHandle[0] = CreateFontToHandle( "Segoe UI" , 20 , 5 ,DX_FONTTYPE_ANTIALIASING_4X4) ;
		FontHandle[1] = CreateFontToHandle( "Segoe UI" , 40 , 6 ,DX_FONTTYPE_ANTIALIASING_4X4) ;
	}
	
private:
	int GHandle[RESOURCE_SIZE];
	int SHandle[RESOURCE_SIZE];
	int FontHandle[RESOURCE_SIZE];

	Resource()
	{
		load_png_wav();
	}

	Resource& operator=(const Resource& a);
	Resource(const Resource& a);
};