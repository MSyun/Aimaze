///////////////////////////////////////////////////////////////
// 製作者		SyunMizuno
// 2015.12.07	プログラム作成
///////////////////////////////////////////////////////////////

#pragma once

//参考:GameProgrammingGems

/*						//
//		Singleton		//
//						*/
template<typename T> class Singleton {
protected:
	static T* ms_pInstance;	// インスタンス

public:
	Singleton() {
//		assert( !ms_pInstance );
		int offset = (int)(T*)1-(int)(Singleton<T>*)(T*)1;
		ms_pInstance = (T*)(((int)this)+offset);
	}

	~Singleton() {
//		assert( ms_pInstance );
		ms_pInstance = 0;
	}

	static T* Instance(void) {
		return ms_pInstance;
	}
};

// 初期化
template<typename T> T*Singleton<T>::ms_pInstance = 0;