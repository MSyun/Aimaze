//	セル
// 2016. 7.11	プログラム作成
// 参考サイト	http://marupeke296.com/COLSmp_3D_No3_Octree.html

#pragma once


#include	<Windows.h>
#include	<vector>
#include	<list>
#include	<d3dx9.h>

#include	"../Convert.h"


/*
	MEMO	:
			CCellクラスのデストラクタでごり押しをしているので要注意
*/



template <class T>
class CCell;


//----- 分木登録オブジェクト
template <class T>
class	OBJECT_FOR_TREE
{
public:
	CCell<T>	*m_pCell;			// 登録空間
	T	*m_pObject;					// 判定対象オブジェクト
	OBJECT_FOR_TREE<T>*	m_pPre;		// 前のOBJECT_FOR_TREE構造体
	OBJECT_FOR_TREE<T>*	m_pNext;	// 次の			〃			
	int	m_id;

public:
	OBJECT_FOR_TREE( int id = 0 ) {
		m_id		= id;
		m_pCell		= NULL;
		m_pObject	= NULL;
		m_pPre		= NULL;
		m_pNext		= NULL;
	}

	virtual ~OBJECT_FOR_TREE() {}

	// 空間を登録
	void RegistCell( CCell<T>* pCell ) {
		m_pCell = pCell;
	}

	// 自らリストから外れる
	bool Remove() {
		// すでに逸脱している時は処理終了
		if( !m_pCell )
			return false;

		// 自分を登録している空間に自信を通知
		if( !m_pCell->OnRemove( this ) )
			return false;

		// 逸脱処理
		// 前後のオブジェクトを結びつける
		if( m_pPre != NULL ) {
			m_pPre->m_pNext = m_pNext;
			m_pPre = NULL;
		}
		if( m_pNext != NULL ) {
			m_pNext->m_pPre = m_pPre;
			m_pNext = NULL;
		}
		m_pCell = NULL;
		return true;
	}

	// 次のオブジェクトへのスマートポインタを取得
	OBJECT_FOR_TREE<T>* GetNextObj() {
		return m_pNext;
	}
};

//----- 線形4分木空間管理クラス
#define	CLINER4TREEMANAGER_MAXLEVEL		9
template <class T>
class	CLiner4TreeManager
{
protected:
	unsigned int	m_uiDim;
	CCell<T>**		ppCellAry;		// 線形空間ポインタ配列
	unsigned int	m_iPow[CLINER4TREEMANAGER_MAXLEVEL + 1];	// べき乗数値配列
	float			m_fW;			// 領域のX軸幅
	float			m_fH;			// 領域のY軸幅
	float			m_fLeft;		// 領域の左側（X軸最小値）
	float			m_fTop;			// 領域の上側（Y軸最小値）
	float			m_fUnit_W;		// 最小レベル空間の幅単位
	float			m_fUnit_H;		// 最小レベル空間の高単位
	DWORD			m_dwCellNum;	// 空間の数
	unsigned int	m_uiLevel;		// 最下位レベル

public:
	// コンストラクタ
	CLiner4TreeManager() {
		m_uiLevel	= 0;
		m_fW		= 0.0f;
		m_fH		= 0.0f;
		m_fLeft		= 0.0f;
		m_fTop		= 0.0f;
		m_fUnit_W	= 0.0f;
		m_fUnit_H	= 0.0f;
		m_dwCellNum	= 0;
		ppCellAry	= NULL;
		m_uiDim		= 0;
	}

	// デストラクタ
	virtual ~CLiner4TreeManager() {
		DWORD i;
		for( i = 0; i < m_dwCellNum; i ++ ) {
			if( ppCellAry[i] != NULL ) {
				delete ppCellAry[i];
				ppCellAry[i] = NULL;
			}
			delete [] ppCellAry;
			ppCellAry = NULL;
		}
	}

	// 線形4分木配列を構築する
	bool Init( unsigned int Level, float left, float top, float right, float bottom ) {
		// 設定最高レベル以上の空間は作れない
		if( Level >= CLINER4TREEMANAGER_MAXLEVEL )
			return false;

		// 各レベルでの空間数を算出
		int i;
		m_iPow[0] = 1;
		for( i = 1; i < CLINER4TREEMANAGER_MAXLEVEL + 1; i ++ )
			m_iPow[i] = m_iPow[i-1]*4:

		// Levelレベル（0基点）の配列作成
		m_dwCellNum = (m_iPow[Level + 1] - 1) / 3;
		ppCellAry = new CCell<T>*[m_dwCellNum];
		ZeroMemory( ppCellAry, sizeof(CCell<T>*)*m_dwCellNum );

		// 領域を登録
		m_fLeft		= left;
		m_fTop		= top;
		m_fW		= right - left;
		m_fH		= bottom - top;
		m_fUnit_W	= m_fW / (1 << Level);
		m_fUnit_H	= m_fH / (1 << Level);

		m_uiLevel	= level;

		return true;
	}

	// オブジェクトを登録する
	bool Regist( float left, float top, float right, float bottom, OBJECT_FOR_TREE<T>* pOFT ) {
		// オブジェクトの境界範囲から登録モートン番号を算出
		DWORD Elem = GetMortonNumber( left, top, right, bottom );
		if( Elem < m_dwCellNum ) {
			// 空間がない場合は新規登録
			if( !ppCellAry[Elem] )
				CreateNewCell(Elem);
			return ppCellAry[Elem]->Push(pOFT);
		}
		return false;	// 登録失敗
	}

	// 衝突判定リスト
	DWORD GetAllCollisionList( std::vector<T*> &ColVect ) {
		// リスト（配列）は必ず初期化します
		ColVect.clear();

		// ルート空間の存在をチェック
		if( ppCellAry[0] == NULL )
			return 0;	// 空間が存在していない

		// ルート空間を処理
		std::list<T*> ColStac;
		GetCollisionList( 0. ColVect, ColStac );

		return (DWORD)ColVect.size();
	}


protected:
	// 空間内で衝突リストを作成する
	bool GetCollisionList( DWORD Elem, std::vector<T*> &ColVect, std::list<T*> &ColStac ) {
		std::list<T*>::iterator it;
		// ①空間内のオブジェクト同士の衝突リスト作成
		OBJECT_FOR_TREE<T>* pOFT1 = ppCellAry[Elem]->GetFirstObj();
		while( pOFT1 ) {
			OBJECT_FOR_TREE<T>* pOFT2 = pOFT1->m_pNext;
			while( pOFT2 ) {
				// 衝突リスト作成
				ColVect.push_back(pOFT1->m_pObject);
				ColVect.push_back(pOFT2->m_pObject);
				pOFT2 = pOFT2->m_pNext;
			}
			// ②衝突スタックとの衝突リスト作成
			for( it = ColStac.begin(); it != ColStac.end(); it ++ ) {
				ColVect.push_back(pOFT1->m_pObject);
				ColVect.push_back(*it);
			}
			pOFT1 = pOFT1->m_pNext;
		}

		bool ChildFlg = false;
		// ③子空間に移動
		DWORD ObjNum = 0;
		DWORD i, NextElem;
		for( i = 0; i < 4; i ++ ) {
			NextElem = Elem * 4 + 1 + i;
			if( NextElem < m_dwCellNum && ppCellAry[Elem * 4 + 1 + i] ) {
				if( !ChildFlg ) {
					// ④登録オブジェクトをスタックに追加
					pOFT1 = ppCellAry[Elem]->GetFirstObj();
					while( pOFT1 ) {
						ColStac.push_back( pOFT1->m_pObject );
						ObjNum++;
						pOFT1 = pOFT1->m_pNext;
					}
				}
				ChildFlg = true;
				GetCollisionList( Elem * 4 + 1 + i, ColVect, ColStac );	// 子空間へ
			}
		}

		// ⑤スタックからオブジェクトを外す
		if( ChildFlg ) {
			for( i = 0; i < ObjNum; i ++ ) {
				ColStac.pop_back();
			}
		}

		return true;
	}

	// 空間を生成
	bool CreateNewCell( DWORD Elem ) {
		// 引数の要素番号
		while( !ppCellAry[Elem] ) {
			// 指定の要素番号に空間を新規塘路ｋ
			ppCellAry[Elem] = new CCell<T>;

			// 親空間にジャンプ
			Elem = (Elem - 1) >> 2;
			if( Elem >= m_dwCellNum ) break;
		}
		return true;
	}

	// 座標から空間番号を算出
	DWORD GetMortonNumber( float left, float top, float right, float bottom ) {
		// 最小レベルにおける各軸位置を算出
		DWORD LT = GetPointElem(left, top);
		DWORD RB = GetPointElem(right, bottom);

		// 空間番号を引き算して
		// 最上位区切りから所属レベルを算出
		DWORD Def = RB ^ LT;
		unsigned int HiLevel = 0;
		unsigned int i;
		for( i = 0; i < m_uiLevel; i ++ ) {
			DWORD Check = (Def >> (i * 2)) & 0x3;
			if( Check != 0 )
				HiLevel = i + 1;
		}
		DWORD SpaceNum = RB >> (HiLevel * 2);
		DWORD AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 3;
		SpaceNum += AddNum;

		if( SpaceNum > m_dwCellNum )
			return 0xffffffff;

		return SpaceNum;
	}

	// ビット分割関数
	DWORD BitSeparate32( DWORD n ) {
		n = (n | (n << 8)) & 0x00ff00ff;	// 0000 0000 abcd efgh
		n = (n | (n << 4)) & 0x0f0f0f0f;	// 0000 abcd 0000 efgh
		n = (n | (n << 2)) & 0x33333333;	// 00ab 00cd 00ef 00gh
		return (n | (n << 1)) & 0x55555555;	// 0a0b 0c0d 0e0f 0g0h
	}

	// 2Dモートン空間番号算出関数
	WORD Get2DMortonNumber( WORD x, WORD y ) {
		return (WORD)(BitSeparate32(x) | (BitSeparate32(y)<<1));
	}

	// 座標→線形4分木要素番号変換関数
	DWORD GetPointElem( float pos_x, float pos_y ) {
		return Get2DMortonNumber(
			(WORD)((pos_x - m_fLeft) / m_fUnit_W),
			(WORD)((pos_y - m_fTop) / m_fUnit_H) );
	}
};


//----- 線形8分木空間管理クラス
#define	CLINER8TREEMANAGER_MAXLEVEL		7
template <class T>
class	CLiner8TreeManager
{
protected:
	unsigned int	m_uiDim;
	CCell<T>**		ppCellAry;		// 線形空間ポインタ配列
	unsigned int	m_iPow[CLINER8TREEMANAGER_MAXLEVEL + 1];	// べき乗数値配列
	Vector3			m_vW;			// 領域の幅
	Vector3			m_vRgnMin;		// 領域の最小値
	Vector3			m_vRgnMax;		// 領域の最大値
	Vector3			m_vUnit;		// 最小領域の辺の長さ
	DWORD			m_dwCellNum;	// 空間の数
	unsigned int	m_uiLevel;		// 最下位レベル

public:
	// コンストラクタ
	CLiner8TreeManager() {
		m_uiDim		= 0;
		ppCellAry	= NULL;
		m_vW.x = m_vW.y = m_vW.z = 1.0f;
		m_vRgnMin.x = m_vRgnMin.y = m_vRgnMin.z = 0.0f;
		m_vRgnMax.x = m_vRgnMax.y = m_vRgnMax.z = 1.0f;
		m_vUnit.x = m_vUnit.y = m_vUnit.z = 1.0f;
		m_dwCellNum	= 0;
		m_uiLevel	= 0;
	}

	// デストラクタ
	virtual ~CLiner8TreeManager() {
		for( DWORD i = 0; i < m_dwCellNum; i ++ ) {
			if( ppCellAry[i] != NULL ) {
				delete ppCellAry[i];
				ppCellAry[i] = NULL;
			}
		}
		delete [] ppCellAry;
		ppCellAry = NULL;
	}

	// 線形4分木配列を構築する
	bool Init( unsigned int Level, Vector3& Min, Vector3& Max ) {
		// 設定最高レベル以上の空間は作れない
		if( Level >= CLINER8TREEMANAGER_MAXLEVEL )
			return false;

		// 各レベルでの空間数を算出
		int i;
		m_iPow[0] = 1;
		for( i = 1; i < CLINER8TREEMANAGER_MAXLEVEL + 1; i ++ )
			m_iPow[i] = m_iPow[i-1]*8;

		// Levelレベル（0基点）の配列作成
		m_dwCellNum = (m_iPow[Level + 1] - 1) / 7;
		ppCellAry = new CCell<T>*[m_dwCellNum];
		ZeroMemory( ppCellAry, sizeof(CCell<T>*)*m_dwCellNum );

		// 領域を登録
		m_vRgnMin	= Min;
		m_vRgnMax	= Max;
		m_vW		= m_vRgnMax - m_vRgnMin;
		m_vUnit		= m_vW / ((float)(1 << Level));

		m_uiLevel	= Level;

		return true;
	}

	// オブジェクトを登録する
	bool Regist(Vector3* Min, Vector3* Max, OBJECT_FOR_TREE<T>* pOFT ) {
		// オブジェクトの境界範囲から登録モートン番号を算出
		DWORD Elem = GetMortonNumber( Min, Max );
		if( Elem < m_dwCellNum ) {
			// 空間がない場合は新規登録
			if( !ppCellAry[Elem] )
				CreateNewCell(Elem);
			return ppCellAry[Elem]->Push(pOFT);
		}
		return false;	// 登録失敗
	}

	// 衝突判定リスト
	DWORD GetAllCollisionList( std::vector<T> &ColVect ) {
		// リスト（配列）は必ず初期化します
		ColVect.clear();

		// ルート空間の存在をチェック
		if( !ppCellAry[0] )
			return 0;	// 空間が存在していない

		// ルート空間を処理
		std::list<T> ColStac;
		GetCollisionList( 0, ColVect, ColStac );

		return (DWORD)ColVect.size();
	}


protected:
	// 空間内で衝突リストを作成する
	bool GetCollisionList( DWORD Elem, std::vector<T> &ColVect, std::list<T> &ColStac ) {
		std::list<T>::iterator it;
		// ①空間内のオブジェクト同士の衝突リスト作成
		OBJECT_FOR_TREE<T>* pOFT1 = ppCellAry[Elem]->GetFirstObj();
		while( pOFT1 ) {
			OBJECT_FOR_TREE<T>* pOFT2 = pOFT1->m_pNext;
			while( pOFT2 ) {
				// 衝突リスト作成
				if(pOFT1->m_pObject != NULL)
					ColVect.push_back(*pOFT1->m_pObject);
				if(pOFT2->m_pObject != NULL)
					ColVect.push_back(*pOFT2->m_pObject);
				pOFT2 = pOFT2->m_pNext;
			}
			// ②衝突スタックとの衝突リスト作成
			for( it = ColStac.begin(); it != ColStac.end(); it ++ ) {
				ColVect.push_back(*pOFT1->m_pObject);
				ColVect.push_back(*it);
			}
			pOFT1 = pOFT1->m_pNext;
		}

		bool ChildFlg = false;
		// ③子空間に移動
		DWORD ObjNum = 0;
		DWORD i, NextElem;
		for( i = 0; i < 8; i ++ ) {
			NextElem = Elem * 8 + 1 + i;
			if( NextElem < m_dwCellNum && ppCellAry[Elem * 8 + 1 + i] ) {
				if( !ChildFlg ) {
					// ④登録オブジェクトをスタックに追加
					pOFT1 = ppCellAry[Elem]->GetFirstObj();
					while( pOFT1 ) {
						ColStac.push_back( *pOFT1->m_pObject );
						ObjNum++;
						pOFT1 = pOFT1->m_pNext;
					}
				}
				ChildFlg = true;
				GetCollisionList( Elem * 8 + 1 + i, ColVect, ColStac );	// 子空間へ
			}
		}

		// ⑤スタックからオブジェクトを外す
		if( ChildFlg ) {
			for( i = 0; i < ObjNum; i ++ ) {
				ColStac.pop_back();
			}
		}

		return true;
	}

	// 空間を生成
	bool CreateNewCell( DWORD Elem ) {
		// 引数の要素番号
		while( !ppCellAry[Elem] ) {
			// 指定の要素番号に空間を新規塘路ｋ
			ppCellAry[Elem] = new CCell<T>;

			// 親空間にジャンプ
			Elem = (Elem - 1) >> 3;
			if( Elem >= m_dwCellNum ) break;
		}
		return true;
	}

	// 座標から空間番号を算出
	DWORD GetMortonNumber(Vector3* Min, Vector3* Max ) {
		// 最小レベルにおける各軸位置を算出
		DWORD LT = GetPointElem(*Min);
		DWORD RB = GetPointElem(*Max);

		// 空間番号を引き算して
		// 最上位区切りから所属レベルを算出
		DWORD Def = RB ^ LT;
		unsigned int HiLevel = 1;
		unsigned int i;
		for( i = 0; i < m_uiLevel; i ++ ) {
			DWORD Check = (Def >> (i * 3)) & 0x7;
			if( Check != 0 )
				HiLevel = i + 1;
		}
		DWORD SpaceNum = RB >> (HiLevel * 3);
		DWORD AddNum = (m_iPow[m_uiLevel - HiLevel] - 1) / 7;
		SpaceNum += AddNum;

		if( SpaceNum > m_dwCellNum )
			return 0xffffffff;

		return SpaceNum;
	}

	// ビット分割関数
	DWORD BitSeparateFor3D( BYTE n ) {
		DWORD s = n;
		s = (s | (s << 8)) & 0x0000f00f;
		s = (s | (s << 4)) & 0x000c30c3;
		s = (s | (s << 2)) & 0x00249249;
		return s;
	}

	// 3Dモートン空間番号算出関数
	DWORD Get3DMortonNumber( BYTE x, BYTE y, BYTE z ) {
		return BitSeparateFor3D(x) | BitSeparateFor3D(y) << 1 | BitSeparateFor3D(z) << 2;
	}

	// 座標→線形8分木要素番号変換関数
	DWORD GetPointElem(Vector3& p) {
		return Get3DMortonNumber(
			(BYTE)((p.x - m_vRgnMin.x) / m_vUnit.x),
			(BYTE)((p.y - m_vRgnMin.y) / m_vUnit.y),
			(BYTE)((p.z - m_vRgnMin.z) / m_vUnit.z));
	}
};


//----- 空間クラス
template <class T>
class	CCell
{
protected:
	OBJECT_FOR_TREE<T>*	m_pLatest;	// 最新OFTへのポインタ

public:
	// コンストラクタ
	CCell() {
		ZeroMemory( &m_pLatest, sizeof(m_pLatest) );
//		m_pLatest = NULL;
	}

	// デストラクタ
	virtual ~CCell() {
		// ここで全部離す
//		if( m_pLatest )
//			ResetLink(m_pLatest);
	}

	// リンクを全てリセット
	void ResetLink( OBJECT_FOR_TREE<T>* pOFT ) {
//		if (pOFT == 0xDDDDDDDD)	return;
		if( pOFT->m_pNext )
			ResetLink(pOFT->m_pNext);
		pOFT->m_pNext = NULL;
	}

	// OFTをプッシュ
	bool Push( OBJECT_FOR_TREE<T>* pOFT ) {
		if( !pOFT )					return false;	// インスタンスが無いよ
		if( pOFT->m_pCell == this )	return false;	// 2重登録チェック
		if( !m_pLatest ) {
			m_pLatest = pOFT;	// 空間に新規登録
		} else {
			// 最新OFTオブジェクトを更新
			pOFT->m_pNext = m_pLatest;
			m_pLatest->m_pPre = pOFT;
			m_pLatest = pOFT;
		}
		pOFT->RegistCell(this);	// 空間を登録
		return true;
	}

	OBJECT_FOR_TREE<T>* GetFirstObj() {
		return m_pLatest;
	}

	// 削除されるオブジェクトをチェック
	bool OnRemove( OBJECT_FOR_TREE<T>* pRemoveObj ) {
		// 一番前が同じオブジェクトか確認
		if( m_pLatest == pRemoveObj ) {
			// 次のオブジェクトに挿げ替え
			if( m_pLatest )
				m_pLatest = m_pLatest->GetNextObj();
		}
		return true;
	}
};