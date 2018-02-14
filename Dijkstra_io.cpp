
#include "stdafx.h"
#include "dijkstra.h"

struct IntPair
{
	int i1;
	int i2;

	IntPair(int _i1, int _i2 )
	{
		i1 = _i1;
		i2 = _i2;
	}
	bool ChkSame( IntPair* chk )
	{
		if(i1 == chk->i1 && i2 == chk->i2 ) {return true;}
		if(i1 == chk->i2 && i2 == chk->i1 ) {return true;}

		return false;
	}
};

//ファイル読み込み
bool Dijkstra::Inport(char* filePath)
{
	bool ret = false;

	Clear();

	FILE* fp = NULL;

	try {

        fp =  fopen( filePath, "r" );

		if ( !fp ) { throw 0; }

		char s[256];
		if ( !fgets( s, 256, fp ) ) {
			throw 0;
		}
		if ( strcmp(s, "//NODE POINTS\n") != 0 ) {
			throw 0;
		}

		while( fgets( s, 256, fp ) ) {
			if ( strcmp(s, "//START NODE\n") == 0 ) {
				break;
			}
			int x,y;
			char *token, *nxtToken;
			token = strtok_s( s, ",", &nxtToken );
			x = atoi(token);
			token = strtok_s( NULL, ",", &nxtToken );
			if ( !token ) {
				throw 0;
			}
			y = atoi(token);

			AddNode(x,y);
		}
		
		//スタートノード
		{
			if ( !fgets( s, 256, fp ) ) {
				throw 0;
			}
			Node* n = QueryFromIdx( atoi(s));
			if ( n ) { m_start = n; }
		}

		//ゴールノード
		{
			if ( !fgets( s, 256, fp ) ) {
				throw 0;
			}
			if ( strcmp(s, "//GOAL NODE\n") != 0 ) {
				throw 0;
			}

			if ( !fgets( s, 256, fp ) ) {
				throw 0;
			}

			Node* n = QueryFromIdx( atoi(s));
			if ( n ) { m_goal = n; }
		}

		//接続情報読み込み
		if ( !fgets( s, 256, fp ) ) {
			throw 0;
		}
		if ( strcmp(s, "//CONNECT INFO\n") != 0 ) {
			throw 0;
		}
		while( fgets( s, 256, fp ) ) {
			int i1,i2;
			char *token, *nxtToken;
			token = strtok_s( s, ",", &nxtToken );
			i1 = atoi(token);
			token = strtok_s( NULL, ",", &nxtToken );
			if ( !token ) {
				throw 0;
			}
			i2 = atoi(token);

			Node* n1 = QueryFromIdx(i1);
			Node* n2 = QueryFromIdx(i2);
			if ( !n1 || !n2 || ( n1 == n2 ) ) {
				throw 0;
			}
			
			ConnectNode( n1, n2 );
		}
		
		ret = true;

	} catch(...) {
		Clear();
	}
	
	if ( fp ) {
		fclose(fp);
	}

	return ret;

}

//ファイル書き込み
bool Dijkstra::Export( LPCWSTR filePath )
{
	FILE* fp;
	_wfopen_s( &fp, filePath, (wchar_t*)("w") );

	if ( fp ) {

		//ノード点
		fputws( (wchar_t*)("//NODE POINTS\n"),fp);
		TCHAR text[1000];
		ZeroMemory(text, sizeof(text));

		std::list<Node*>::iterator it = m_node.begin();
		for ( ; it != m_node.end(); it++ ) {		
			wsprintf(text, ("%d,%d\n"), (*it)->x, (*it)->y );
			fputws( (wchar_t*)text,fp);
		}

		//スタートノード インデックス
		fputws( (wchar_t*)("//START NODE\n"),fp);
		wsprintf(text, ("%d\n"), QueryIdx(m_start) );
		fputws( (wchar_t*)text,fp);

		//ゴールノード インデックス
		fputws( (wchar_t*)("//GOAL NODE\n"),fp);
		wsprintf(text, _T("%d\n"), QueryIdx(m_goal) );
		fputws( (wchar_t*)text,fp);

		//接続情報
		fputws( (wchar_t*)("//CONNECT INFO\n"),fp);
		
		//接続ノードの組み合わせを重複除外して列挙
		std::list<IntPair> addPair;
		it = m_node.begin();
		int currIdx = 0;
		for ( ; it != m_node.end(); it++, currIdx++ ) {

			std::list<NodeConnect>::iterator it_cnct = (*it)->m_connectNode.begin();
			for ( ; it_cnct != (*it)->m_connectNode.end(); it_cnct++ ) {
				int sideNodeIdx = QueryIdx(it_cnct->node);
				
				IntPair add(currIdx, sideNodeIdx);

				//重複なければ登録
				bool find = false;
				std::list<IntPair>::iterator it_pair = addPair.begin();
				for ( ; it_pair != addPair.end(); it_pair++ ) {
					if ( add.ChkSame(&( *it_pair) ) ) {
						find = true;
						break;
					}
				}
				if ( find ) {
					continue;
				}
				addPair.push_back(add);

				wsprintf(text, _T("%d,%d\n"), currIdx, sideNodeIdx );
				fputws( (wchar_t*)text,fp);
			}
		}

		fclose(fp);
	} else {
		return false;
	}

	return true;
}