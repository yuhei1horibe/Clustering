#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>


#define	SAFE_DELETE(p)	if(p){delete p;p=NULL;}


const int	WIDTH	= 128;	//画面の幅と高さの定義
const int	HEIGHT	= 128;	//高さ

//その点のデータ
struct POINT{
	int	x;
	int	y;

	int	Cls;

	int	Sum_dist;

	int	id;			//セントロイドが移動したかどうかを確かめるために利用

	//リスト構造
	struct POINT	*next;
};


//座標データを初期化
int	InitPoints(struct POINT *Ps, const int n)
{
	struct POINT	*pt;	//ワークポインタ
	int				i;

	srand((unsigned)time(NULL));

	//メモリ確保
	if((Ps = new struct POINT) == NULL)
	{
		MassageBox(NULL, "座標メモリ確保エラー。", "Memory Allocation Failure.", MB_OK);
		return -1;
	}
	pt	= Ps;
	pt->x	= rand() % WIDTH;	//ｘ座標
	pt->y	= rand() % HEIGHT;	//ｙ座標

	//ランダムにデータを生成
	for(i = 0; i < n; i++)
	{
		//メモリ確保
		if((pt->next = new struct POINT) == NULL)
		{
			MassageBox(NULL, "座標メモリ確保エラー。", "Memory Allocation Failure.", MB_OK);
			return -1;
		}
		pt	= pt->next;

		pt->x	= rand() % WIDTH;	//x座標
		pt->y	= rand() % HEIGHT;	//y座標
	}
	pt->next	= NULL;

	return 0;
}



//座標データを受け取って、そのデータを分類
int Clustering(struct POINT *Points, struct POINT *Centroids, const int N)
{
	//ループ変数
	int				i;
	int				j;
	int				k;

	struct POINT	*pts;		//ワークポインタ
	struct POINT	*ptd;		//

	struct POINT	*ci;		//代表点データ
	int				dist[512];	//ユークリッド距離を保持する
	int				temp;


	ptd	= Points->next;
	pts	= Centroids->next;

	//分布データについてもっとも距離が近い点でデータを分割
	while(1)
	{
		temp	= 9999999;
		//センとロイドに対して距離を算出して、そのもっとも近いものに分類
		for(k = 0; k < 512; k++)
		{
			dist[k]	= (ptd->x - pts->x) * (ptd->x - pts->x) + (ptd->y - pts->y) * (ptd->y - pts->y);

			if(temp > dist[k])
			{
				ptd->Cls	= k;
				temp		= dist[k];
			}

			if(pts->next == NULL)
				break;

			else
				pts	= pts->next;
		}

		if(ptd->next == NULL)
			break;
		
		else
			ptd	= ptd->next;
	}

	return 0;
}

//セントロイドを更新
int	UpdateCentroid(struct POINT *Points, struct POINT *Centroid)
{
	//ループ制御変数
	int				Min;	//最小となる距離

	struct POINT	*wp1;	//リスト操作のためのワークポインタ
	struct POINT	*wp2;


	wp1	= Points->next;
	wp2	= Points->next;

	if(wp2->next != NULL)
		wp2	= wp2->next;

	//それぞれの点から他の点までの距離の総和を計算
	Min		= 9999999;
	while(1)
	{
		wp1->Sum_dist	= 0;
		while(1)
		{
			if(wp1 != wp2)
				wp1->Sum_dist	+= (wp1->x - wp2->x) * (wp1->x - wp2->x) + (wp1->y - wp2->y) * (wp1->y - wp2->y);

			if(wp2->next == NULL)
				break;

			else
				wp2	= wp2->next;
		}
		if(Min > wp1->Sum_dist)
			Centroid->next	= wp1;

		if(wp1->next == NULL)
			break;

		else
			wp1	= wp1->next;
	}

	return 0;
}

//クラスタリング実行関数
int	Execute(struct POINT *Points, struct POINT *Centroids, const int N)
{
	struct POINT	**Class;	//勢力圏ごとにリストを形成

	struct POINT	*wp;		//リスト操作のためのワークポインタ

	struct POINT	**wpa;		//ワークポインタの配列

	int				Flag;		//セントロイドが一つでも更新されているかを判定
	int				*FingPtr;	//クラスタリングを行う前のセントロイドの情報を保持
	int				i;
	int				temp;


	//セントロイドのフィンガープリント
	if((FingPtr = new int[N]) == NULL)
	{
		MessageBox(NULL, "メモリ確保エラー", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//リストを分割するためのポインタを確保
	Class	= new struct POINT*[N];
	if(Class == NULL)
	{
		MessageBox(NULL, "メモリ確保エラー", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//実体を確保(先頭はダミー)
	for(i = 0; i < N; i++)
	{
		if((Class[i]	= new (struct POINT)) == NULL)
		{
			MessageBox(NULL, "メモリ確保エラー", "Memory Allocaion Falure.", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}
	}

	//ワークポインタの配列を確保
	wpa		= new struct POINT*[N];
	if(wpa == NULL)
	{
		MessageBox(NULL, "メモリ確保エラー", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//ポインタ初期化
	for(i = 0; i < N; i++)
	{
		Class[i]->next	= NULL;
		wpa[i]			= Class[i];
	}

	wp	= Centroids->next;
	//セントロイドのフィンガープリントを作成
	for(i = 0; i < N; i++)
	{
		FingPtr[i]	= wp->id;

		wp	= wp->next;
	}

	//データを勢力圏ごとに分類してフラグ書き込み
	if(Clustering(Points, Centroids, N) == -1)
		return -1;

	wp	= Centroids->next;
	//セントロイドリストを解体
	for(i = 0; i < N; i++)
	{
		wpa[i]->next	= wp;
		wpa[i]			= wpa[i]->next;

		wp				= wp->next;
		wpa[i]->next	= NULL;
	}

	wp	= Points->next;
	while(1)
	{
		temp	= wp->Cls;

		//勢力圏ごとにリスト構造を形成
		wpa[temp]->next	= wp;
		wpa[temp]		= wpa[temp]->next;

		if(wp->next == NULL)
		{
			wpa[temp]->next	= NULL;
			break;
		}

		else
		{
			wp				= wp->next;
			wpa[temp]->next	= NULL;
		}
	}

	wp	= Centroids;
	while(1)
	{
		Flag	= 0;

		//セントロイドの更新
		for(i = 0; i < N; i++)
		{
			UpdateCentroid(Class[i], wp);

			if(wp->next == NULL)
			{
				MessageBox(NULL, "セントロイドの更新に失敗しました", "Falure", MB_OK | MB_ICONEXCLAMATION);
				return -1;
			}

			wp		= wp->next;
			wp->next	= NULL;

			if(wp->id != FingPtr[i])
				Flag |= 1;
		}
	}

	//リストを全て一つにつなぎなおし
	for(i = 0; i < (N - 1); i++)
	{
		wp	= Class[i]->next;
		while(1)
		{
			if(wp->next == NULL)
				break;

			else
				wp	= wp->next;
		}
		wp->next	= Class[i]->next;
	}

	return Flag;
}