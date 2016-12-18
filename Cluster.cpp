
#include"Clustering.h"



//座標データを受け取って、そのデータを分類
int Clustering(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode)
{
	//ループ変数
	int				k;

	struct VECTOR2	*pts;		//ワークポインタ
	struct VECTOR2	*ptd;		//

	double			dist;		//ユークリッド距離を保持する
	double			temp;

	double			SigmaX;
	double			SigmaY;

	//分散を考慮
	if(mode == 0)
	{
		SigmaX	= 1;
		SigmaY	= 1;
	}

	else if(mode == 1)
	{
		SigmaX	= 1;
		SigmaY	= 0.001;
	}

	else
	{
		SigmaX	= 0.001;
		SigmaY	= 1;
	}

	ptd	= Points->next;

	//分布データについてもっとも距離が近い点でデータを分割
	while(1)
	{
		pts		= Centroids->next;
		temp	= 9999999.0;
		k		= 0;
		//セントロイドに対して距離を算出して、そのもっとも近いものに分類
		while(1)
		{
			dist	= sqrt((double)(((ptd->x - pts->x) * (ptd->x - pts->x)) / SigmaX) + (((ptd->y - pts->y) * (ptd->y - pts->y)) / SigmaY));
			if(temp > dist)
			{
				ptd->Cls	= k;
				temp		= dist;
			}

			if(pts->next == NULL)
				break;

			else
				pts	= pts->next;

			k++;
		}
		if(ptd->next == NULL)
			break;
		
		else
			ptd	= ptd->next;
	}

	return 0;
}

//セントロイドを更新
int	UpdateCentroid(struct VECTOR2 *Points, struct VECTOR2 *Centroid, const int mode)
{
	//ループ制御変数
	double			Min;	//最小となる距離

	struct VECTOR2	*wp1;	//リスト操作のためのワークポインタ
	struct VECTOR2	*wp2;

	double			SigmaX;
	double			SigmaY;


	//分散を考慮
	if(mode == 0)
	{
		SigmaX	= 1;
		SigmaY	= 1;
	}

	else if(mode == 1)
	{
		SigmaX	= 1;
		SigmaY	= 0.001;
	}

	else
	{
		SigmaX	= 0.001;
		SigmaY	= 1;
	}


	wp1	= Points->next;
	wp2	= Points->next;

	Min				= 999999999.0;

	if(Points->next == NULL)
		return 1;

	//それぞれの点から他の点までの距離の総和を計算
	while(1)
	{
		wp1->Sum_dist	= 0;
		wp2				= Points->next;

		while(1)
		{
			if(wp1 != wp2)
				wp1->Sum_dist	+= sqrt((double)((((wp1->x - wp2->x) * (wp1->x - wp2->x)) / SigmaX) + (((wp1->y - wp2->y) * (wp1->y - wp2->y)) / SigmaY)));

			if(wp2->next == NULL)
				break;

			else
				wp2	= wp2->next;
		}


		if(Min > wp1->Sum_dist)
		{
			Min				= wp1->Sum_dist;
			Centroid->next	= wp1;
		}

		if(wp1->next == NULL)
			break;

		else
			wp1	= wp1->next;
	}

	wp1	= Points->next;
	wp2	= Points->next;
	//セントロイドの切り離し
	if(wp1 == Centroid->next)
	{
		Points->next	= wp1->next;
		wp1->next		= NULL;
	}

	else
	{
		while(1)
		{
			if(wp1->next == Centroid->next)
				break;

			else
				wp1	= wp1->next;
		}
		wp2				= wp1;
		wp1				= Centroid->next;
		wp2->next		= wp1->next;
	}
	wp1->next			= NULL;
	return 0;
}

//クラスタリング実行関数
int	Execute(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode)
{
	struct VECTOR2	**Class;	//勢力圏ごとにリストを形成

	struct VECTOR2	*wp;		//リスト操作のためのワークポインタ

	struct VECTOR2	**wpa;		//ワークポインタの配列

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
	Class	= new struct VECTOR2*[N];
	if(Class == NULL)
	{
		MessageBox(NULL, "メモリ確保エラー", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		delete[] FingPtr;
		return -1;
	}

	//実体を確保(先頭はダミー)
	for(i = 0; i < N; i++)
	{
		if((Class[i]	= new (struct VECTOR2)) == NULL)
		{
			MessageBox(NULL, "メモリ確保エラー", "Memory Allocaion Falure.", MB_OK | MB_ICONEXCLAMATION);
			delete[] FingPtr;
			for(int a = 0; a < i; a++)
				delete Class[a];

			delete[] Class;

			return -1;
		}
	}

	//ワークポインタの配列を確保
	wpa		= new struct VECTOR2*[N];
	if(wpa == NULL)
	{
		MessageBox(NULL, "メモリ確保エラー", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		delete[] FingPtr;
			for(int i = 0; i < N; i++)
				delete Class[i];

		delete[] Class;
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
	if(Clustering(Points, Centroids, N, mode) == -1)
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
	Flag	= 0;
	//セントロイドの更新
	for(i = 0; i < N; i++)
	{
		if(UpdateCentroid(Class[i], wp, mode) == 0)
		{
			if(wp->next == NULL)
			{
				MessageBox(NULL, "セントロイドの更新に失敗しました", "Falure", MB_OK | MB_ICONEXCLAMATION);
				return -1;
			}
			wp			= wp->next;
			wp->next	= NULL;

			if(wp->id != FingPtr[i])
				Flag |= 1;
		}
	}

	Points->next	= Class[0]->next;
	wp				= Class[0]->next;
	//リストを全て一つにつなぎなおし
	for(i = 0; i < (N - 1); i++)
	{
		while(1)
		{
			if(wp->next == NULL)
				break;

			else
				wp	= wp->next;
		}
		while(Class[i + 1]->next == NULL)
			i++;

		wp->next	= Class[i + 1]->next;
	}
/*
	{
	char temp[512];
	struct VECTOR2 *Pt;
	Pt	= Centroids->next;
	while(1)
	{
		sprintf(temp, "id%d", Pt->id);
		MessageBox(NULL, temp, "大丈夫？", MB_OK);

		if(Pt->next == NULL)
			break;

		else
			Pt	= Pt->next;
	}
	}
*/

	delete[] FingPtr;
	for(int i = 0; i < N; i++)
		delete Class[i];

	delete[] Class;
	delete[] wpa;

	return Flag;
}

//リストのメモリ開放
void	ClearList(struct VECTOR2 *List)
{
	struct VECTOR2	*wp;	//ワークポインタ

	if(List->next == NULL)
		return;
	
	else
		List	= List->next;

	while(1)
	{
		if(List->next != NULL)
		{
			wp	= List->next;
			delete List;

			List	= wp;
		}

		else
		{
			delete	List;
			break;
		}
	}
}

