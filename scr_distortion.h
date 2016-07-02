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

//歪み可能スクリーン　drawstartとflipのみ呼べば良い
class scr_distortion{
public:
	scr_distortion(const scr_distortion& r) = delete;			//コピーコンストラクタ delete 指定
	scr_distortion& operator=(const scr_distortion& r) = delete;	//代入演算子 delete 指定

	static scr_distortion& get(){
		static scr_distortion inst;
		return inst;
	}

	static const int MASSH = 20;
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

	int subscreen;

	dtparts pt[ALLPTNUM];
	int ghandle[ALLNUM];

	//描画開始時に呼ぶ
	void DrawStart(){
		SetDrawScreen(subscreen);
		ClearDrawScreen();
	}

	//描画終了時に呼ぶ
	void DrawEnd_DrawToBack(){
		scrupdate();
		updatepoint();
		SetDrawScreen(DX_SCREEN_BACK);
		//debug_drawpoint();
		drawdistortion();
	}

	//コンストラクタ
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
		//分割用画像作成
		for (int i = 0; i < ALLNUM; i++){
			ghandle[i] = MakeScreen(MASSH, MASSH, 1);
		}
		//サブスクリーン作成
		subscreen = MakeScreen(ScreenWidth, ScreenHeight, 1);
	}


	//座標に作用
	void addforce(){
		if (Input::getkey(250)){
			for (int i = 0; i < ALLPTNUM; i++){
				if (Func::dist(pt[i].x, pt[i].y, Input::mousex, Input::mousey) < 150){
					Func::addVector(pt[i].x, pt[i].y, Input::mousex, Input::mousey, 0.5, pt[i].vx, pt[i].vy);
				}
			}
		}
	}

	//座標更新
	void updatepoint(){
		addforce();
		for (int i = 0; i < ALLPTNUM; i++){
			//bx,byを中心とした単振動
			Func::addVector(pt[i].x, pt[i].y, pt[i].bx, pt[i].by, 
				k * Func::dist(pt[i].x, pt[i].y, pt[i].bx, pt[i].by), 
				pt[i].vx, pt[i].vy);
			//減速
			pt[i].vx *= spdrate;
			pt[i].vy *= spdrate;
			//移動
			pt[i].x += pt[i].vx;
			pt[i].y += pt[i].vy;
			//速度が0に近ければ静止
			if (Func::dist(pt[i].x, pt[i].y, pt[i].bx, pt[i].by) < 0.1 && abs(pt[i].vx) < 0.01 && abs(pt[i].vy) < 0.01){
				pt[i].vx = 0; 
				pt[i].x = pt[i].bx; 			
				pt[i].vy = 0; 
				pt[i].y = pt[i].by; 
			}

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


	//サブスクリーンに描画された画像を小さい画像に分割
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


	//描画
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