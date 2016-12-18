#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<windows.h>


#define	SAFE_DELETE(p)	if(p){delete p;p=NULL;}


const int	WIDTH	= 128;	//��ʂ̕��ƍ����̒�`
const int	HEIGHT	= 128;	//����

//���̓_�̃f�[�^
struct POINT{
	int	x;
	int	y;

	int	Cls;

	int	Sum_dist;

	int	id;			//�Z���g���C�h���ړ��������ǂ������m���߂邽�߂ɗ��p

	//���X�g�\��
	struct POINT	*next;
};


//���W�f�[�^��������
int	InitPoints(struct POINT *Ps, const int n)
{
	struct POINT	*pt;	//���[�N�|�C���^
	int				i;

	srand((unsigned)time(NULL));

	//�������m��
	if((Ps = new struct POINT) == NULL)
	{
		MassageBox(NULL, "���W�������m�ۃG���[�B", "Memory Allocation Failure.", MB_OK);
		return -1;
	}
	pt	= Ps;
	pt->x	= rand() % WIDTH;	//�����W
	pt->y	= rand() % HEIGHT;	//�����W

	//�����_���Ƀf�[�^�𐶐�
	for(i = 0; i < n; i++)
	{
		//�������m��
		if((pt->next = new struct POINT) == NULL)
		{
			MassageBox(NULL, "���W�������m�ۃG���[�B", "Memory Allocation Failure.", MB_OK);
			return -1;
		}
		pt	= pt->next;

		pt->x	= rand() % WIDTH;	//x���W
		pt->y	= rand() % HEIGHT;	//y���W
	}
	pt->next	= NULL;

	return 0;
}



//���W�f�[�^���󂯎���āA���̃f�[�^�𕪗�
int Clustering(struct POINT *Points, struct POINT *Centroids, const int N)
{
	//���[�v�ϐ�
	int				i;
	int				j;
	int				k;

	struct POINT	*pts;		//���[�N�|�C���^
	struct POINT	*ptd;		//

	struct POINT	*ci;		//��\�_�f�[�^
	int				dist[512];	//���[�N���b�h������ێ�����
	int				temp;


	ptd	= Points->next;
	pts	= Centroids->next;

	//���z�f�[�^�ɂ��Ă����Ƃ��������߂��_�Ńf�[�^�𕪊�
	while(1)
	{
		temp	= 9999999;
		//�Z���ƃ��C�h�ɑ΂��ċ������Z�o���āA���̂����Ƃ��߂����̂ɕ���
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

//�Z���g���C�h���X�V
int	UpdateCentroid(struct POINT *Points, struct POINT *Centroid)
{
	//���[�v����ϐ�
	int				Min;	//�ŏ��ƂȂ鋗��

	struct POINT	*wp1;	//���X�g����̂��߂̃��[�N�|�C���^
	struct POINT	*wp2;


	wp1	= Points->next;
	wp2	= Points->next;

	if(wp2->next != NULL)
		wp2	= wp2->next;

	//���ꂼ��̓_���瑼�̓_�܂ł̋����̑��a���v�Z
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

//�N���X�^�����O���s�֐�
int	Execute(struct POINT *Points, struct POINT *Centroids, const int N)
{
	struct POINT	**Class;	//���͌����ƂɃ��X�g���`��

	struct POINT	*wp;		//���X�g����̂��߂̃��[�N�|�C���^

	struct POINT	**wpa;		//���[�N�|�C���^�̔z��

	int				Flag;		//�Z���g���C�h����ł��X�V����Ă��邩�𔻒�
	int				*FingPtr;	//�N���X�^�����O���s���O�̃Z���g���C�h�̏���ێ�
	int				i;
	int				temp;


	//�Z���g���C�h�̃t�B���K�[�v�����g
	if((FingPtr = new int[N]) == NULL)
	{
		MessageBox(NULL, "�������m�ۃG���[", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//���X�g�𕪊����邽�߂̃|�C���^���m��
	Class	= new struct POINT*[N];
	if(Class == NULL)
	{
		MessageBox(NULL, "�������m�ۃG���[", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//���̂��m��(�擪�̓_�~�[)
	for(i = 0; i < N; i++)
	{
		if((Class[i]	= new (struct POINT)) == NULL)
		{
			MessageBox(NULL, "�������m�ۃG���[", "Memory Allocaion Falure.", MB_OK | MB_ICONEXCLAMATION);
			return -1;
		}
	}

	//���[�N�|�C���^�̔z����m��
	wpa		= new struct POINT*[N];
	if(wpa == NULL)
	{
		MessageBox(NULL, "�������m�ۃG���[", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		return -1;
	}

	//�|�C���^������
	for(i = 0; i < N; i++)
	{
		Class[i]->next	= NULL;
		wpa[i]			= Class[i];
	}

	wp	= Centroids->next;
	//�Z���g���C�h�̃t�B���K�[�v�����g���쐬
	for(i = 0; i < N; i++)
	{
		FingPtr[i]	= wp->id;

		wp	= wp->next;
	}

	//�f�[�^�𐨗͌����Ƃɕ��ނ��ăt���O��������
	if(Clustering(Points, Centroids, N) == -1)
		return -1;

	wp	= Centroids->next;
	//�Z���g���C�h���X�g�����
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

		//���͌����ƂɃ��X�g�\�����`��
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

		//�Z���g���C�h�̍X�V
		for(i = 0; i < N; i++)
		{
			UpdateCentroid(Class[i], wp);

			if(wp->next == NULL)
			{
				MessageBox(NULL, "�Z���g���C�h�̍X�V�Ɏ��s���܂���", "Falure", MB_OK | MB_ICONEXCLAMATION);
				return -1;
			}

			wp		= wp->next;
			wp->next	= NULL;

			if(wp->id != FingPtr[i])
				Flag |= 1;
		}
	}

	//���X�g��S�Ĉ�ɂȂ��Ȃ���
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