//////////////////////////////////////////////////////////////
// CSaveObjBase クラス
// 2016. 9. 24	プログラム作成
// 2016. 9. 27	コメント追加
// author		SyunMizuno
//////////////////////////////////////////////////////////////

#include	"SaveManagerBase.h"
#include	"../Factory/SaveFactory.h"


/*
	memo	:
			リファクタリング必須
*/


/*									//
//		セーブオブジェクトを登録	//
//									*/
void SaveManagerBase::AddSaveData(ISaveObjBase* pobj) {
	// 実態チェック
	if (!pobj)	return;

	// 登録
	m_ObjList.push_back(pobj);
}


/*									//
//				セーブ				//
//									*/
int SaveManagerBase::Save(const string filename) {
	// セーブファイルオープン
	m_fs.open(filename.c_str(), ios_base::out | ios_base::binary);
	if (!m_fs.is_open())
		return 0;

	// データの書き込み
	for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it ++)
		(*it)->Save(this);

	m_fs.close();

	return SAVE_OK;	// セーブ完了
}


/*									//
//				ロード				//
//									*/
int SaveManagerBase::Load(const string filename ) {
	// セーブファイルオープン
	m_fs.open( filename.c_str(), ios_base::in | ios_base::binary );
	if( !m_fs.is_open() )
		return NO_SAVEFILE;

	while( !m_fs.eof() ) {
		// データの読み込み
		// クラスIDの取得
		int ClassID;
		m_fs.read( (char*)&ClassID, sizeof(int) );

		// 読み込み時にエラーが起きた場合はロードを終了する
		if( m_fs.fail() != 0 )	break;

		// 指定クラスIDのオブジェクトを生成して保持
		ISaveObjBase* pobj = CreateObj( ClassID );
		AddSaveData( pobj );

		// オブジェクトが無い場合はデータを飛ばす
		if( !pobj ) { LoadSkip(); continue; }

		// オブジェクトをロード
		pobj->Load( this );
	}

	for( auto it = m_ObjList.begin(); it != m_ObjList.end(); it ++ ) {
		// オブジェクトのポインタをつなげる
		if( *it )
			(*it)->ConnectPtr( this );
	}

	m_fs.close();

	return LOAD_OK;	// ロード完了
}


/*									//
//			読み込みデータを		//
//		1オブジェクト分スキップ		*/
int SaveManagerBase::LoadSkip() {
	int type, size;
	while (!m_fs.eof()) {
		m_fs.read((char*)&type, sizeof(char));	// データタイプ
		if (type == TYPE_END)
			break;
		m_fs.read((char*)&size, sizeof(int));		// データサイズ
		m_fs.ignore(size);							// サイズ分データを飛ばす
	}

	return 1;
}


/*									//
//			ファクトリ関数			//
//		（ここは継承すること前提）	*/
ISaveObjBase* SaveManagerBase::CreateObj(int classID) {
	return SaveFactory::Create(classID);	// 指定のクラスがない
}


/*									//
//			データ書き込み			//
//									*/
int SaveManagerBase::Write(DATARECORD* list, ISaveObjBase* pobj) {
	// クラスIDの書き込み
	int ID = pobj->GetClassID();
	m_fs.write((char*)&ID, sizeof(int));

	while (list->type != TYPE_END) {
		// データの位置を算出
//		char* pos = (char*)pobj + list->offset;
		char* pos;
		int Elem;
		void* p;
		char* sizepos;
		int size;
		char* pmem;

		// データタイプとサイズの書き込み
		m_fs.write((char*)&list->type, sizeof(char));	// データタイプ
//		m_fs.write( (char*)&list->size, sizeof(int) );	// データサイズ

		switch (list->type) {
			// 通常変数
		case TYPE_LOCAL:
			pos = (char*)pobj + list->offset;
			m_fs.write((char*)&list->size, sizeof(int));	// データサイズ
			m_fs.write((char*)pos, list->size);			// データ本体
			break;

			// ポインタ変数
		case TYPE_PTR:
			pos = (char*)pobj + list->offset;
			m_fs.write((char*)&list->size, sizeof(int));	// データサイズ
			p = (void*)*(__int64*)pos;
			Elem = ElemFromPtr(p);		// ポインタを要素番号に変換
			m_fs.write((char*)&Elem, list->size);	// データ本体
			break;

		case TYPE_MEM:
			pos = (char*)pobj + list->offset;
			sizepos = (char*)pobj + list->size;
			size = *(int*)(sizepos);
			m_fs.write((char*)&size, sizeof(int));	// データサイズ
			pmem = (char*)(*(int*)(pos));
			m_fs.write(pmem, size);
			break;

		case TYPE_BASE:
			Write((DATARECORD*)(list->offset), pobj);
			break;
		};

		// 次のリストへ
		list++;
	}

	// セパレータを1つはさむ
	char sep = TYPE_END;
	m_fs.write((char*)&sep, sizeof(char));

	return SAVE_OK;
}


/*									//
//			データ読み込み			//
//									*/
int SaveManagerBase::Read(DATARECORD* list, ISaveObjBase* pobj) {
	while (list->type != TYPE_END) {
		// 変数宣言
		char *pos, *tmp;

		// データタイプを取得
		int type = 0, size = 0;
		m_fs.read((char*)&type, sizeof(char));

		switch (type) {
			// 通常変数
			// ポインタ変数
		case TYPE_LOCAL:
		case TYPE_PTR:
			pos = (char*)pobj + list->offset;		// オブジェクト内メンバ変数の保存位置を算出
			m_fs.read((char*)&size, sizeof(int));	// データサイズ取得
			m_fs.read(pos, list->size);			// データ本体
			break;

			// メモリブロック
		case TYPE_MEM:
			pos = (char*)pobj + list->offset;		// オブジェクト内メンバ変数の保存位置を算出
			m_fs.read((char*)&size, sizeof(int));	// データサイズ取得
			// サイズ分のメモリを確保
			// posの先は任意のポインタ変数なのでtmpポインタと置き
			// tmpがさす先にデータを流し込む
			// pos->tmp->[Data]
			tmp = new char[size];
			m_fs.read(tmp, size);
			*(int*)(pos) = (int)tmp;
			break;

			// 親クラスのメンバ変数
		case TYPE_BASE:
			Read((DATARECORD*)(list->offset), pobj);
			break;
		};

		// 次のリストへ
		list++;
	}

	// セパレートを飛ばす
	m_fs.ignore(sizeof(char));

	return LOAD_OK;
}


/*									//
//		ポインタを要素番号に変換	//
//									*/
int SaveManagerBase::ElemFromPtr(void* pos) {
	int elem = 0;
	for (auto it = m_ObjList.begin(); it != m_ObjList.end(); it ++) {
		void* objptr = (void*)(*it);

		// インスタンス検索
		if (objptr == pos)
			return elem;

		elem ++;
	}

	return NO_ELEM;
}


/*									//
//	オブジェクトのポインタを結合	//
//									*/
int SaveManagerBase::ConnectPtr(DATARECORD* list, ISaveObjBase* pobj) {
	// データタイプがポインタだった時には指定されている
	// 配列要素のオブジェクトポインタを変数に流し込む
	while (list->type != TYPE_END) {
		if (list->type == TYPE_PTR) {
			// 変数のアドレスを算出
			int* pos = (int*)((char*)pobj + list->offset);

			// 変数に格納されている要素番号をポインタに変換
			int ObjID = *pos;
			if (ObjID != NO_ELEM) {
				*pos = (int)(m_ObjList[ObjID]);
			} else {
				*pos = NULL;
			}
		}

		list++;
	}

	return CONNECT_OK;
}