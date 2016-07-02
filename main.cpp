#include <DxLib.h>
#include <iostream>
#include <vector>
#include <string>

#include "myglobal.h"

#include "Input.h"
#include "Resource.h"
#include "Func.h"
#include "scr_distortion.h"



int WINAPI WinMain( HINSTANCE hInstance , HINSTANCE hPrevInstance , LPSTR lpCmdLine , int nCmdShow )
{
	ChangeWindowMode( TRUE );
	SetGraphMode( ScreenWidth , ScreenHeight , 32 );
	SetOutApplicationLogValidFlag( FALSE ) ;//logを生成しないように設定
	if(DxLib_Init() == 1){return -1;}//初期化に失敗時にエラーを吐かせて終了
	SetDrawScreen( DX_SCREEN_BACK ) ;

	Input::get();
	Resource::get();

	scr_distortion::get();

	while( ProcessMessage() == 0)
	{
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		Input::update();//キー更新・マウス座標更新・経過フレーム更新

		scr_distortion::get().DrawStart();

		//DrawCircle(Input::mousex, Input::mousey, 16, 0xFFFFFF, 1);
		DrawGraph(0, 0, Resource::graphic(image), 1);
		DrawCircle((Input::eframe*2) % 999, 300, 64, 0xFF0000, 1);
		//ステータス描画
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 255 );	SetDrawBright(255,255,255);
		int stry = 0;
		DrawFormatStringToHandle(20, stry += 20, 0xFFFFFF, Resource::font(0), "MX:%3d MY:%3d", Input::mousex(), Input::mousey());
		DrawFormatStringToHandle(20, stry += 20, 0xFFFFFF, Resource::font(0), "クリック維持で歪ませられる");

		scr_distortion::get().DrawEnd_DrawToBack();//裏画面を表画面にコピー
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}