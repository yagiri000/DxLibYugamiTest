#pragma once

#include <DxLib.h>
#include <iostream>
#include <vector>
#include <string>
#include <random>

#include "myglobal.h"

/*
//シングルトンRandomクラス
------------------以下、当たり判定---------------------
------------------以下、極座標←→XY座標------------------
------------------以下、回転---------------------
------------------以下、ベクトル---------------------
------------------以下、イージング曲線---------------
------------------以下、便利関数------------------


*/

//シングルトンRandomクラス
class Random
{
	std::mt19937 mt;
	//コンストラクタ　SeedSetUseRDでMTの初期値を設定
	Random(){
		SeedSetUseRD();
	}
	Random(const Random& r) = delete;
	Random& operator=(const Random& r) = delete;
public:
	static Random& get()
	{
		static Random r;
		return r;
	}
	//MTの初期値をRandom_deviceを使用して設定
	static void SeedSetUseRD(){
		std::random_device r;
		get().mt.seed(r());
	}
	//MTの初期値設定
	static void Initialize(int seed)
	{
		get().mt.seed(seed);
	}
	//mtエンジン本体を取得
	static std::mt19937& getMT(){ return get().mt; }
	//完全乱数
	static unsigned int RamdomDeviceValue()
	{
		std::random_device r;
		return r();
	}

	//通常のrand取得
	static int Rand(){
		return get().mt();
	}
	//0~max指定取得(int)
	static int Rand(int max)
	{
		std::uniform_int_distribution<int> uid(0, max);
		return uid(get().mt);
	}
	//min~maxを返す(int)
	static int Rand(int min, int max)
	{
		std::uniform_int_distribution<int> uid(min, max);
		return uid(get().mt);
	}
	//1か-1を返す関数
	static int RandPM1(){
		std::uniform_int_distribution<int> uid(0, 1);
		return uid(get().mt) * 2 - 1;
	}
	//0~1を返す(double)
	static double Value()
	{
		std::uniform_real_distribution<double> urd(0, 1);
		return urd(get().mt);
	}
	//0~maxを返す(double)
	static double FRand(double max)
	{
		std::uniform_real_distribution<double> urd(0, max);
		return urd(get().mt);
	}
	//min~maxを返す(double)
	static double FRand(double min, double max)
	{
		std::uniform_real_distribution<double> urd(min, max);
		return urd(get().mt);
	}
	//正規分布
	static double FRandND(double mean, double sigma)
	{
		std::normal_distribution<double> nd(mean, sigma);
		return nd(get().mt);
	}
	static double FRandND(double mean, double sigma, double limit)
	{
		std::normal_distribution<double> nd(mean, sigma);
		double r = nd(get().mt);
		if (r < mean - limit)
			r = mean - limit;
		if (r > mean + limit)
			r = mean + limit;
		return r;
	}
	//ラジアンで取得
	static double FRadRand()
	{
		return FRand(-PI, PI);
	}
	static double FRadRand(double deg_min, double deg_max)
	{
		return FRand(deg_min * arcrad, deg_max * arcrad);
	}
	static double FRadRandND(double deg_mean, double deg_sigma = 0)
	{
		return FRandND(deg_mean * arcrad, deg_sigma * arcrad);
	}
	//半径rの円上の点を返す
	static void RandOnCircle(double r, double &x, double &y){
		double randang = FRadRand();
		x = r * cos(randang);
		y = r * sin(randang);
	}
	//半径rの円上の点を返す　原点からの角度も返す
	static void RandOnCircle(double r, double &x, double &y, double &ang){
		double randang = FRadRand();
		x = r * cos(randang);
		y = r * sin(randang);
		ang = randang;
	}
	//半径rの円内部の点を返す
	static void RandInCircle(double r, double &x, double &y){
		double randang = FRadRand();
		double rr = r * Value();
		x = rr * cos(randang);
		y = rr * sin(randang);
	}
	//半径rの円内部の点を返す　原点からの角度も返す
	static void RandInCircle(double r, double &x, double &y, double &ang){
		double randang = FRadRand();
		double rr = r * Value();
		x = rr * cos(randang);
		y = rr * sin(randang);
		ang = randang;
	}
};

class Func{
private:
	Func(){}
	Func& operator=(const Func& a);
	Func(const Func& a);

public:
	//------------------以下、当たり判定-------------------- -

	//2乗
	template<typename T> static T Sqr(T x)
	{
		return x*x;
	}
	//内積計算
	template<typename T> static T Dot2D(T Ax, T Ay, T Bx, T By)
	{
		return Ax*Bx + Ay*By;
	}
	//外積計算
	template<typename T> static T Cross2D(T Ax, T Ay, T Bx, T By)
	{
		return Ax*By - Bx*Ay;
	}

	static bool Circles(double x1, double y1, double r1, double x2, double y2, double r2)
	{
		return Sqr(x2 - x1) + Sqr(y2 - y1) < Sqr(r2 + r1);
	}
	//矩形同士
	static bool Boxes(double a1x, double a1y, double a2x, double a2y, double b1x, double b1y, double b2x, double b2y)
	{
		return a1x < b2x && a1y < b2y && b1x < a2x && b1y < a2y;
	}
	//線分同士(0ならあたってない、0以上はあたってる
	static double Lines(double a_x, double a_y, double b_x, double b_y, double c_x, double c_y, double d_x, double d_y)
	{
		double c0;
		c0 = Cross2D(b_x - a_x, b_y - a_y, d_x - c_x, d_y - c_y);
		if (c0 == 0.0f)
			return 0.0f;
		double r0, r1;
		r0 = Cross2D(c_x - a_x, c_y - a_y, b_x - a_x, b_y - a_y) / c0;
		r1 = Cross2D(c_x - a_x, c_y - a_y, d_x - c_x, d_y - c_y) / c0;
		if (r0 >= 0.0f && r0 <= 1.0f && r1 >= 0.0f && r1 <= 1.0f)
			return r1;
		else
			return 0.0f;
	}
	//円と線分
	static bool CircleAndLine(double cx, double cy, double r, double ax, double ay, double bx, double by)
	{
		double d;
		//また、円の半径はr,OP間の距離をDとする。

		// d = | AB×AO | / | AB |
		d = abs((bx - ax)*(cy - ay) - (by - ay)*(cx - ax)) / sqrt(Sqr(by - ay) + Sqr(bx - ax));

		//dが半径以上だったらあたってない
		if (d > r)
			return false;
		//AB・AO及びAB・BOが両方とも鈍角(cosが負)なら線分外に円があるのであたってない
		if (((bx - ax)*(by - ay) + (cx - ax)*(cy - ay))*((bx - ax)*(by - ay) + (cx - bx)*(cy - by)) > 0)
			return false;

		return true;
	}

	//円(半径cr,(cx,cy)) と　長方形(rx1,ry1)左上, (rx2, ry2)右下の当たり判定
	static bool CircleAndRect(double cx, double cy, double cr, double rx1, double ry1, double rx2, double ry2){
		double rectx = (rx1 + rx2) / 2.0;
		double recty = (ry1 + ry2) / 2.0;
		return	cx > rx1 - cr && cy > ry1 - cr &&
				cx < rx2 + cr && cy < ry2 + cr;
	}



	

	//------------------以下、回転---------------------

	//原点を中心にang分回転
	static void rotateonzero(double &x,  double &y, double ang){
		double sx = x - 0;
		double sy = y - 0;
		x = sx * cos(ang) - sy * sin(ang) + 0;
		y = sx * sin(ang) + sy * cos(ang) + 0;
	}
	
	//ある点(centerx,centery)を中心にang分回転
	static void rotateonpoint(double &x,  double &y,  double centerx, double centery, double ang){
		double sx = x - centerx;
		double sy = y - centery;
		x = sx * cos(ang) - sy * sin(ang) + centerx;
		y = sx * sin(ang) + sy * cos(ang) + centery;
	}
	

	//------------------以下、ベクトル---------------------

	//(原点から(sx,sy)への単位ベクトルを求める
	static void normalizedVector(double &vx, double &vy, double sx, double sy){
		double rd = atan2(sy, sx);
		vx = cos(rd);
		vy = sin(rd);
	}

	//(sx,sy)から(ex,ey)への単位ベクトルを求める
	static void normalizedVector(double &vx, double &vy, double sx, double sy, double ex, double ey){
		double rd = atan2(ey - sy, ex - sx);
		vx = cos(rd);
		vy = sin(rd);
	}


	//(dx,dy)方向、大きさdistのベクトルを求め、結果をresultに代入
	static void calVector(double sx, double sy, double ex, double ey, double dist, double &resultx, double &resulty){
		double rd = atan2(ey - sy, ex - sx);
		resultx = dist * cos(rd);
		resulty = dist * sin(rd);
	}

	//(dx,dy)方向、大きさdistのベクトルを求め、結果をresultに加算
	static void addVector(double sx, double sy, double ex, double ey, double dist, double &resultx, double &resulty){
		double rd = atan2(ey - sy, ex - sx);
		resultx += dist * cos(rd);
		resulty += dist * sin(rd);
	}

	//極座標表現からxy表現に直す
	static void polartoxy(double &x, double &y, double r, double ang){
		x = r * cos(ang);
		y = r * sin(ang);
	}
	

	/*
	startx : 開始の値(開始時の座標やスケールなど)
	enddif : 開始と終了の値の差分
	time_now : 現在の時間
	time_end : Tweenの合計時間
	*/

	//／＼　←こういうグラフ
	static double linef(double startx, double dif, double time_now, double time_end){
		return startx + (dif - startx) * (1.0 - abs(time_end/2 - time_now) / (time_end/2) );
	}

	//／＼　←こういう2次曲線
	static double quadraticf(double startx, double dif, double time_now, double time_end){
		double x = time_now / time_end;
		return startx + (dif - startx) * -4 * (x * x - x);
	}



	//---------------以下、イージング曲線---------------

	/*
	t : 現在の時間
	b : 開始の値(開始時の座標やスケールなど)
	c : 開始と終了の値の差分
	d : Tweenの合計時間
	*/

	//イージング曲線 Cubic
	static double ease_cubic_in(double t, double b, double c, double d){
		t /= d;
		return c*t*t*t + b;
	}

	static double ease_cubic_out(double t, double b, double c, double d){
		t /= d;
		t = t - 1;
		return c*(t*t*t + 1) + b;
	}

	static double ease_cubic_inout(double t, double b, double c, double d){
		t /= d/2.0;
		if (t < 1){
			return c/2.0*t*t*t + b;
		}
		else{
			t = t - 2;
			return c/2.0 * (t*t*t + 2) + b;
		}
	}
	
	
	//イージング曲線 Sinusoidal
	static double ease_sin_in(double t, double b, double c, double d){
		return -c * cos(t/d * (PI/2.0)) + c + b;
	}

	static double ease_sin_out(double t, double b, double c, double d){
		return c * sin(t/d * (PI/2.0)) + b;
	}

	static double ease_sin_inout(double t, double b, double c, double d){
		return -c/2.0 * (cos(PI*t/d) - 1) + b;
	}
	
	//イージング曲線 Exponential
	static double ease_exp_in(double t, double b, double c, double d){
		return c * pow(2.0, (10*(t/d - 1))) + b;
	}

	static double ease_exp_out(double t, double b, double c, double d){
		return c * (-(pow(2.0,(-10.0 * t/d))) + 1) + b;
	}

	static double ease_exp_inout(double t, double b, double c, double d){
		t /= d/2.0;
		if(t < 1)
			return c/2.0 * pow(2.0,(10.0 * (t-1))) + b;
		t = t - 1;
		return c/2.0 * (-(pow(2.0,(-10*t))) + 2 ) + b;
	}

	//イージング曲線 Circular
	static double ease_sqrt_in(double t, double b, double c, double d){
		t /= d;
		return -c * (sqrt(1 - t*t) - 1) + b;
	}

	static double ease_sqrt_out(double t, double b, double c, double d){
		t /= d;
		t = t - 1;
		return c * sqrt(1 - t*t) + b;
	}

	static double ease_sqrt_inout(double t, double b, double c, double d){
		t /= d/2.0;
		if(t < 1)
			return -c/2.0 * (sqrt(1 - t*t) + 1);
		t = t - 2;
		return c/2.0 * (sqrt(1 - t*t) + 1) + b;
	}
	


	//ベジェ曲線  p1->p2->p3  0<t<1
	static double bz_curve(double p1, double p2, double p3, double t){
		double tp = 1 - t;
		return t*t*p3 + 2*t*tp*p2 + tp*tp*p1;
	}

	//ベジェ曲線  p1->p2->p3  nowtime, endtime
	static double bz_curve(double p1, double p2, double p3, double nowt, double endt){
		double t = nowt / endt;
		double tp = 1 - t;
		return t*t*p3 + 2*t*tp*p2 + tp*tp*p1;
	}

	//----------------以下、便利関数------------------

	//HSVをRGBに換算
	static unsigned long HSVtoRGB(int h, int s, int v){
		float f;
		int i, p, q, t;
		int rgb[3];

		i = ((int)floor(h / 60.0f)) % 6;
		f = (float)(h / 60.0f) - (float)floor(h / 60.0f);
		p = (int)(v * (1.0f - (s / 255.0f)));
		q = (int)(v * (1.0f - (s / 255.0f) * f));
		t = (int)(v * (1.0f - (s / 255.0f) * (1.0f - f)));

		switch(i){
		case 0 : rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
		case 1 : rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
		case 2 : rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
		case 3 : rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
		case 4 : rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
		case 5 : rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
		}

		return GetColor(rgb[0], rgb[1], rgb[2]);
	}
	
	//HSVでsetdrawbright
	static void SetDrawBrightHSV(int h, int s, int v){
		float f;
		int i, p, q, t;
		int rgb[3];

		i = ((int)floor(h / 60.0f)) % 6;
		f = (float)(h / 60.0f) - (float)floor(h / 60.0f);
		p = (int)(v * (1.0f - (s / 255.0f)));
		q = (int)(v * (1.0f - (s / 255.0f) * f));
		t = (int)(v * (1.0f - (s / 255.0f) * (1.0f - f)));

		switch(i){
		case 0 : rgb[0] = v; rgb[1] = t; rgb[2] = p; break;
		case 1 : rgb[0] = q; rgb[1] = v; rgb[2] = p; break;
		case 2 : rgb[0] = p; rgb[1] = v; rgb[2] = t; break;
		case 3 : rgb[0] = p; rgb[1] = q; rgb[2] = v; break;
		case 4 : rgb[0] = t; rgb[1] = p; rgb[2] = v; break;
		case 5 : rgb[0] = v; rgb[1] = p; rgb[2] = q; break;
		}

		SetDrawBright(rgb[0], rgb[1], rgb[2]);
	}

	//自分の角度,相手の角度 ２角の関係を返す　時計回り側に動いたほうが近ければ1 そうでなければ-1
	static int abs_angle(double angle1, double angle2){
		if(sin(angle2 - angle1)>0){
			return 1;
		}else{
			return -1;
		}
	}
	
	
	//原点との距離を返す
	static double dist(double x, double y){
		return sqrt(x*x + y*y);
	}

	//2点の距離を返す
	static double dist(double x1, double y1, double x2, double y2){
		return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
	}

	//2点の距離の2乗を返す
	static double distSqr(double x1, double y1, double x2, double y2){
		return (x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2);
	}
};