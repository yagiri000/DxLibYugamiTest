#pragma once
#include <iostream>
#include "DxLib.h"
#include "Input.h"

//�_��\���N���X
class dtparts{
public:
	double x;
	double y;
	double bx;
	double by;
	double vx;
	double vy;
};

//�c�݉\�X�N���[���@drawstart��flip�̂݌ĂׂΗǂ�
class scr_distortion{
public:
	scr_distortion(const scr_distortion& r) = delete;			//�R�s�[�R���X�g���N�^ delete �w��
	scr_distortion& operator=(const scr_distortion& r) = delete;	//������Z�q delete �w��

	static scr_distortion& get(){
		static scr_distortion inst;
		return inst;
	}

	static const int MASSH = 20;
	//�_�̐�
	static const int XPTNUM = ScreenWidth / MASSH + 1;
	static const int YPTNUM = ScreenHeight / MASSH + 1;
	static const int ALLPTNUM = XPTNUM * YPTNUM;
	//�������ꂽ�摜�̐�
	static const int XNUM = ScreenWidth / MASSH;
	static const int YNUM = ScreenHeight / MASSH;
	static const int ALLNUM = XNUM * YNUM;
	//�o�l�萔
	const float k;
	//������
	const float spdrate;

	int subscreen;

	dtparts pt[ALLPTNUM];
	int ghandle[ALLNUM];

	//�`��J�n���ɌĂ�
	void DrawStart(){
		SetDrawScreen(subscreen);
		ClearDrawScreen();
	}

	//�`��I�����ɌĂ�
	void DrawEnd_DrawToBack(){
		scrupdate();
		updatepoint();
		SetDrawScreen(DX_SCREEN_BACK);
		//debug_drawpoint();
		drawdistortion();
	}

	//�R���X�g���N�^
	scr_distortion() :
		k(0.02),
		spdrate(0.95)
	{
		for (int iy = 0; iy < YPTNUM; iy++){
			for (int ix = 0; ix < XPTNUM; ix++){
				pt[ix + iy * XPTNUM] = { 
					(double)MASSH * ix, 
					(double)MASSH * iy, 
					(double)MASSH * ix,
					(double)MASSH * iy,
					(double)0,
					(double)0 
				};
			}
		}
		//�����p�摜�쐬
		for (int i = 0; i < ALLNUM; i++){
			ghandle[i] = MakeScreen(MASSH, MASSH, 1);
		}
		//�T�u�X�N���[���쐬
		subscreen = MakeScreen(ScreenWidth, ScreenHeight, 1);
	}


	//���W�ɍ�p
	void addforce(){
		if (Input::getkey(250)){
			for (int i = 0; i < ALLPTNUM; i++){
				if (Func::dist(pt[i].x, pt[i].y, Input::mousex, Input::mousey) < 150){
					Func::addVector(pt[i].x, pt[i].y, Input::mousex, Input::mousey, 0.5, pt[i].vx, pt[i].vy);
				}
			}
		}
	}

	//���W�X�V
	void updatepoint(){
		addforce();
		for (int i = 0; i < ALLPTNUM; i++){
			//bx,by�𒆐S�Ƃ����P�U��
			Func::addVector(pt[i].x, pt[i].y, pt[i].bx, pt[i].by, 
				k * Func::dist(pt[i].x, pt[i].y, pt[i].bx, pt[i].by), 
				pt[i].vx, pt[i].vy);
			//����
			pt[i].vx *= spdrate;
			pt[i].vy *= spdrate;
			//�ړ�
			pt[i].x += pt[i].vx;
			pt[i].y += pt[i].vy;
			//���x��0�ɋ߂���ΐÎ~
			if (Func::dist(pt[i].x, pt[i].y, pt[i].bx, pt[i].by) < 0.1 && abs(pt[i].vx) < 0.01 && abs(pt[i].vy) < 0.01){
				pt[i].vx = 0; 
				pt[i].x = pt[i].bx; 			
				pt[i].vy = 0; 
				pt[i].y = pt[i].by; 
			}

		}

		//�[�����̉摜���Œ�
		for (int i = 0; i<YPTNUM; i++){
			pt[i * XPTNUM].x = 0;
			pt[(i + 1) * XPTNUM - 1].x = ScreenWidth;
		}

		for (int i = 0; i < XPTNUM; i++){
			pt[i].y = 0;
			pt[ALLPTNUM - XPTNUM + i - 1].y = ScreenHeight;
		}
	}


	//�T�u�X�N���[���ɕ`�悳�ꂽ�摜���������摜�ɕ���
	void scrupdate(){
		SetDrawScreen(subscreen);

		for (int iy = 0; iy < YNUM; iy++){
			for (int ix = 0; ix < XNUM; ix++){
				GetDrawScreenGraph(
					pt[ix + iy*XPTNUM].bx, pt[ix + iy*XPTNUM].by,
					pt[ix + iy*XPTNUM].bx + MASSH, pt[ix + iy*XPTNUM].by + MASSH,
					ghandle[ix + iy*XNUM]);
			}
		}

	}


	//�`��
	void drawdistortion(){
		for (int iy = 0; iy < YNUM ; iy++){
			for (int ix = 0; ix < XNUM ; ix++){
				DrawModiGraph(
					pt[ix + iy*XPTNUM].x, pt[ix + iy*XPTNUM].y,
					pt[ix + iy*XPTNUM + 1].x, pt[ix + iy*XPTNUM + 1].y,
					pt[ix + (iy + 1)*XPTNUM + 1].x, pt[ix + (iy + 1)*XPTNUM + 1].y,
					pt[ix + (iy + 1)*XPTNUM].x, pt[ix + (iy + 1)*XPTNUM].y,
					ghandle[ix + iy * XNUM],1);
			}
		}
	}
	void debug_drawpoint(){
		for (int iy = 0; iy < YPTNUM; iy++){
			for (int ix = 0; ix < XPTNUM; ix++){
				DrawCircle(pt[ix + iy*XPTNUM].x, pt[ix + iy*XPTNUM].y, 4, 0x888888);
			}
		}
	}

};