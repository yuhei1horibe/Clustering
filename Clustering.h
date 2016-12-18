#pragma once

#include "Resource.h"
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<math.h>
#include<windows.h>

#define	SAFE_DELETE(p)	if(p){delete p;p=NULL;}


//�{�^���̃n���h��ID
#define		ID_CLUSTER	1001
#define		ID_UPDATE	1002
#define		ID_UPDATE1	1003
#define		ID_UPDATE2	1004
#define		ID_INIT		1005
#define		ID_CLEAR	1006


const int	WIDTH	= 512;	//��ʂ̕��ƍ����̒�`
const int	HEIGHT	= 512;	//����



//���̓_�̃f�[�^
struct VECTOR2{
	int	x;
	int	y;

	int	Cls;

	double	Sum_dist;

	int	id;			//�Z���g���C�h���ړ��������ǂ������m���߂邽�߂ɗ��p

	//���X�g�\��
	struct VECTOR2	*next;
};


//���W�f�[�^���󂯎���āA���̃f�[�^�𕪗�
int Clustering(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode = 0);

//�Z���g���C�h���X�V
int	UpdateCentroid(struct VECTOR2 *Points, struct VECTOR2 *Centroid, const int mode = 0);

//�N���X�^�����O���s�֐�
int	Execute(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode = 0);

//���X�g�N���A
void	ClearList(struct VECTOR2 *List);