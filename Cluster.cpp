
#include"Clustering.h"



//���W�f�[�^���󂯎���āA���̃f�[�^�𕪗�
int Clustering(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode)
{
	//���[�v�ϐ�
	int				k;

	struct VECTOR2	*pts;		//���[�N�|�C���^
	struct VECTOR2	*ptd;		//

	double			dist;		//���[�N���b�h������ێ�����
	double			temp;

	double			SigmaX;
	double			SigmaY;

	//���U���l��
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

	//���z�f�[�^�ɂ��Ă����Ƃ��������߂��_�Ńf�[�^�𕪊�
	while(1)
	{
		pts		= Centroids->next;
		temp	= 9999999.0;
		k		= 0;
		//�Z���g���C�h�ɑ΂��ċ������Z�o���āA���̂����Ƃ��߂����̂ɕ���
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

//�Z���g���C�h���X�V
int	UpdateCentroid(struct VECTOR2 *Points, struct VECTOR2 *Centroid, const int mode)
{
	//���[�v����ϐ�
	double			Min;	//�ŏ��ƂȂ鋗��

	struct VECTOR2	*wp1;	//���X�g����̂��߂̃��[�N�|�C���^
	struct VECTOR2	*wp2;

	double			SigmaX;
	double			SigmaY;


	//���U���l��
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

	//���ꂼ��̓_���瑼�̓_�܂ł̋����̑��a���v�Z
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
	//�Z���g���C�h�̐؂藣��
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

//�N���X�^�����O���s�֐�
int	Execute(struct VECTOR2 *Points, struct VECTOR2 *Centroids, const int N, const int mode)
{
	struct VECTOR2	**Class;	//���͌����ƂɃ��X�g���`��

	struct VECTOR2	*wp;		//���X�g����̂��߂̃��[�N�|�C���^

	struct VECTOR2	**wpa;		//���[�N�|�C���^�̔z��

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
	Class	= new struct VECTOR2*[N];
	if(Class == NULL)
	{
		MessageBox(NULL, "�������m�ۃG���[", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		delete[] FingPtr;
		return -1;
	}

	//���̂��m��(�擪�̓_�~�[)
	for(i = 0; i < N; i++)
	{
		if((Class[i]	= new (struct VECTOR2)) == NULL)
		{
			MessageBox(NULL, "�������m�ۃG���[", "Memory Allocaion Falure.", MB_OK | MB_ICONEXCLAMATION);
			delete[] FingPtr;
			for(int a = 0; a < i; a++)
				delete Class[a];

			delete[] Class;

			return -1;
		}
	}

	//���[�N�|�C���^�̔z����m��
	wpa		= new struct VECTOR2*[N];
	if(wpa == NULL)
	{
		MessageBox(NULL, "�������m�ۃG���[", "Memory Allocation Falure.", MB_OK | MB_ICONEXCLAMATION);
		delete[] FingPtr;
			for(int i = 0; i < N; i++)
				delete Class[i];

		delete[] Class;
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
	if(Clustering(Points, Centroids, N, mode) == -1)
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
	Flag	= 0;
	//�Z���g���C�h�̍X�V
	for(i = 0; i < N; i++)
	{
		if(UpdateCentroid(Class[i], wp, mode) == 0)
		{
			if(wp->next == NULL)
			{
				MessageBox(NULL, "�Z���g���C�h�̍X�V�Ɏ��s���܂���", "Falure", MB_OK | MB_ICONEXCLAMATION);
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
	//���X�g��S�Ĉ�ɂȂ��Ȃ���
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
		MessageBox(NULL, temp, "���v�H", MB_OK);

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

//���X�g�̃������J��
void	ClearList(struct VECTOR2 *List)
{
	struct VECTOR2	*wp;	//���[�N�|�C���^

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

