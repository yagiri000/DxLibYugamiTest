#pragma once

#include "DxLib.h"
#include "func.h"
#include <functional>

//!!!!!!!!!!!!! 注意 !!!!!!!!!!!!!
//getする前にstaticメンバにアクセスするのは不可能


template<class T>
class property {
public:
	property() = default;
	property(const property& other) { }

	void operator()(const std::function<T()>& getter) {
		if (!get) get = getter;// 一度しかセットできないようにしました
	}

	T operator()() const         { return get(); }

	operator T() const           { return get(); }

	typedef T value_type;

private:
	std::function<T()> get;
};



class Input{
public:

	Input()
	{
		mousex_ = 0;
		mousey_ = 0;
		eframe_ = 0;
		mousex([this](){return mousex_; });
		mousey([this](){return mousey_; });
		eframe([this](){return eframe_; });
	}
	Input& operator=(const Input& a) = delete;
	Input(const Input& a) = delete;

	char buf[256];
	int keystate[256];

	static property<int> mousex;
	static property<int> mousey;
	static property<int> eframe;


	static Input& get(){
		static Input inp;
		return inp;
	}

	static void update(){
		get().do_update();
	}
	
	static int getkey(int i){
		return get().keystate[i];
	}

private:
	int mousex_;
	int mousey_;//マウス座標
	int eframe_;//経過フレーム
	void do_update(){
		GetHitKeyStateAll(buf);
		buf[250] =  GetMouseInput() & MOUSE_INPUT_LEFT;
		buf[251] =  ( (GetMouseInput() & MOUSE_INPUT_RIGHT) == 0) ? 0 : 1;
		for(int i = 0; i< 256; i++){
			if(buf[i] == 1){
				keystate[i]++;
			}else{
				keystate[i] = 0;
			}
		}
		
		//マウス座標更新
		GetMousePoint( &mousex_, &mousey_ ); 
		//経過フレーム計測
		eframe_++;
	}
};


