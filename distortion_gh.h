#pragma once
#include <iostream>
#include "DxLib.h"
#include "Input.h"

//点を表すクラス
class dtparts{
public:
	double x;
	double y;
	double bx;
	double by;
	double vx;
	double vy;
};

//歪み可能スクリーン
class distortion_gh{
public:
	static const int MASSH = 40;
	//点の数
	static const int XPTNUM = ScreenWidth / MASSH + 1;
	static const int YPTNUM = ScreenHeight / MASSH + 1;
	static const int ALLPTNUM = XPTNUM * YPTNUM;
	//分割された画像の数
	static const int XNUM = ScreenWidth / MASSH;
	static const int YNUM = ScreenHeight / MASSH;
	static const int ALLNUM = XNUM * YNUM;
	//バネ定数
	const float k;
	//減速率
	const float spdrate;

	dtparts pt[ALLPTNUM];
	int ghandle[ALLNUM];

	distortion_gh() :
		k(0.02),
		spdrate(0.95)
	{
		for (int iy = 0; iy < YPTNUM; iy++){
			for (int ix = 0; ix < XPTNUM; ix++){
				pt[ix + iy * XPTNUM] = { MASSH * ix, MASSH * iy, MASSH * ix, MASSH * iy, 0, 0 };
			}
		}
		loadgh();
	}

	void loadgh(){
		LoadDivGraph("Images/image.jpg", ALLNUM, XNUM, YNUM, MASSH, MASSH, ghandle);
	}

	//座標更新
	void update(){
		for (int i = 0; i < ALLPTNUM; i++){
			Func::addVector(pt[i].x, pt[i].y, Input::mousex, Input::mousey, 0.2, pt[i].vx, pt[i].vy);

			Func::addVector(pt[i].x, pt[i].y, pt[i].bx, pt[i].by,
				k * Func::dist(pt[i].x, pt[i].y, pt[i].bx, pt[i].by),
				pt[i].vx, pt[i].vy);

			pt[i].vx *= spdrate;
			pt[i].vy *= spdrate;
			pt[i].x += pt[i].vx;
			pt[i].y += pt[i].vy;
		}

		//端っこの画像を固定
		for (int i = 0; i<YPTNUM; i++){
			pt[i * XPTNUM].x = 0;
			pt[(i + 1) * XPTNUM - 1].x = ScreenWidth;
		}

		for (int i = 0; i < XPTNUM; i++){
			pt[i].y = 0;
			pt[ALLPTNUM - XPTNUM + i - 1].y = ScreenHeight;
		}
	}

	//描画
	void draw(){
		for (int iy = 0; iy < YNUM; iy++){
			for (int ix = 0; ix < XNUM; ix++){
				DrawModiGraphF(
					pt[ix + iy*XPTNUM].x, pt[ix + iy*XPTNUM].y,
					pt[ix + iy*XPTNUM + 1].x, pt[ix + iy*XPTNUM + 1].y,
					pt[ix + (iy + 1)*XPTNUM + 1].x, pt[ix + (iy + 1)*XPTNUM + 1].y,
					pt[ix + (iy + 1)*XPTNUM].x, pt[ix + (iy + 1)*XPTNUM].y,
					ghandle[ix + iy * XNUM], 1);
			}
		}
	}

};