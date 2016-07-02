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
	SetOutApplicationLogValidFlag( FALSE ) ;//log�𐶐����Ȃ��悤�ɐݒ�
	if(DxLib_Init() == 1){return -1;}//�������Ɏ��s���ɃG���[��f�����ďI��
	SetDrawScreen( DX_SCREEN_BACK ) ;

	Input::get();
	Resource::get();

	scr_distortion::get();

	while( ProcessMessage() == 0)
	{
		SetDrawScreen(DX_SCREEN_BACK);
		ClearDrawScreen();
		Input::update();//�L�[�X�V�E�}�E�X���W�X�V�E�o�߃t���[���X�V

		scr_distortion::get().DrawStart();

		//DrawCircle(Input::mousex, Input::mousey, 16, 0xFFFFFF, 1);
		DrawGraph(0, 0, Resource::graphic(image), 1);
		DrawCircle((Input::eframe*2) % 999, 300, 64, 0xFF0000, 1);
		//�X�e�[�^�X�`��
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , 255 );	SetDrawBright(255,255,255);
		int stry = 0;
		DrawFormatStringToHandle(20, stry += 20, 0xFFFFFF, Resource::font(0), "MX:%3d MY:%3d", Input::mousex(), Input::mousey());
		DrawFormatStringToHandle(20, stry += 20, 0xFFFFFF, Resource::font(0), "�N���b�N�ێ��Řc�܂�����");

		scr_distortion::get().DrawEnd_DrawToBack();//����ʂ�\��ʂɃR�s�[
		ScreenFlip();
	}

	DxLib_End();
	return 0;
}