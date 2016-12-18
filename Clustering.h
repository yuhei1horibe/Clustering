#pragma once

#include "Resource.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<windows.h>

#define	SAFE_DELETE(p)	if(p){delete p;p=NULL;}


//ボタンのハンドルID
#define		ID_CLUSTER	1001
#define		ID_UPDATE	1002
#define		ID_UPDATE1	1003
#define		ID_UPDATE2	1004
#define		ID_INIT		1005
#define		ID_CLEAR	1006


const int	WIDTH	= 512;	//画面の幅と高さの定義
const int	HEIGHT	= 512;	//高さ



//その点のデータ
struct VECTOR2{
	int	x;
	int	y;

	int	Cls;

	double	Sum_dist;

	int	id;			//セントロイドが移動したかどうかを確かめるために利用

	//リスト構造
	struct VECTOR2	*next;
};


//座標データを受け取って、そのデータを分類
int Clustering(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode = 0);

//セントロイドを更新
int	UpdateCentroid(struct VECTOR2 *Points, struct VECTOR2 *Centroid, const int mode = 0);

//クラスタリング実行関数
int	Execute(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode = 0);

//リストクリア
void	ClearList(struct VECTOR2 *List);