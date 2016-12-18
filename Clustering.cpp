// Clustering.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//
#include "stdafx.h"
#include "Clustering.h"
#define MAX_LOADSTRING 100



// �O���[�o���ϐ� :
int				cnt	= 0;				//id����U��̂��߂̕ϐ�
int				N;						//�Z���g���C�h�̌����J�E���g

struct VECTOR2	List;					//���W���X�g
struct VECTOR2	*AtEnd	= &List;		//���X�g�̖���

struct VECTOR2	Centroids;				//�Z���g���C�h�̃��X�g
struct VECTOR2	*CAtEnd	= &Centroids;	//�Z���g���C�h���X�g�̖���

COLORREF		Color[10];				//�u���V�ƃy���̐F���P�O�F��



HINSTANCE hInst;								// ���݂̃C���^�[�t�F�C�X
TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��

// ���̃R�[�h ���W���[���Ɋ܂܂��֐��̐錾��]�����܂� :
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: �����ɃR�[�h��}�����Ă��������B
	MSG msg;
	HACCEL hAccelTable;

	// �O���[�o������������������Ă��܂��B
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLUSTERING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//���X�g��������
	List.next		= NULL;
	Centroids.next	= NULL;

	//�����̎��������
	srand((unsigned)time(NULL));

	//�F���P�O�F����
	Color[0]	= RGB(255, 0, 0);	//��
	Color[1]	= RGB(0, 0, 255);	//��
	Color[2]	= RGB(0, 255, 0);	//��
	Color[3]	= RGB(255, 200, 0);	//���F
	Color[4]	= RGB(0, 170, 170);	//��
	Color[5]	= RGB(255, 0, 150);	//�Ԏ�
	Color[6]	= RGB(170, 0, 255);	//��
	Color[7]	= RGB(170, 50, 50);	//���F
	Color[8]	= RGB(70, 150, 70);	//������
	Color[9]	= RGB(255, 130, 0);	//�I�����W




	// �A�v���P�[�V�����̏����������s���܂��B:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_CLUSTERING);

	// ���C�� ���b�Z�[�W ���[�v :
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  �֐� : MyRegisterClass()
//
//  �ړI : �E�B���h�E �N���X��o�^���܂��B
//
//  �R�����g :
//
//    ���̊֐�����юg�����́A'RegisterClassEx' �֐����ǉ����ꂽ
//     Windows 95 ���O�� Win32 �V�X�e���ƌ݊�������ꍇ�ɂ̂ݕK�v�ł��B
//    �A�v���P�[�V�������A�֘A�t����ꂽ 
//    �������`���̏������A�C�R�����擾�ł���悤�ɂ���ɂ́A
//    ���̊֐����Ăяo���Ă��������B
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_CLUSTERING);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCTSTR)IDC_CLUSTERING;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   �֐� : InitInstance(HANDLE, int)
//
//   �ړI : �C���X�^���X �n���h����ۑ����āA���C�� �E�B���h�E���쐬���܂��B
//
//   �R�����g :
//
//        ���̊֐��ŁA�O���[�o���ϐ��ŃC���X�^���X �n���h����ۑ����A
//        ���C�� �v���O���� �E�B���h�E���쐬����ѕ\�����܂��B
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // �O���[�o���ϐ��ɃC���X�^���X�������i�[���܂��B

   hWnd = CreateWindow(szWindowClass, "�N���X�^�����O", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
      CW_USEDEFAULT, 0, WIDTH + 170, HEIGHT + 100, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  �֐� : WndProc(HWND, unsigned, WORD, LONG)
//
//  �ړI :  ���C�� �E�B���h�E�̃��b�Z�[�W���������܂��B
//
//  WM_COMMAND	- �A�v���P�[�V���� ���j���[�̏���
//  WM_PAINT	- ���C�� �E�B���h�E�̕`��
//  WM_DESTROY	- ���~���b�Z�[�W��\�����Ė߂�
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int						wmId, wmEvent;
	PAINTSTRUCT				ps;
	HDC						hdc;

	HPEN					hBlackPen;
	HPEN					hPen[10];

	HBRUSH					hBlackBrush;
	HBRUSH					hBrush[10];

	//�{�^��
	HWND					hButton1;
	HWND					hButton2;
	HWND					hButton3;
	HWND					hButton4;
	HWND					hButton5;
	HWND					hButton6;

	static POINTS			pt;
	struct VECTOR2			*wp;	//���[�N�|�C���^
	struct VECTOR2			*wp2;
	static struct VECTOR2	*wpd;

	int						Count	= 0;
	static char				Str[512];

	int						dist;	//����
	int						Min;	//�ŏ��l��ێ�

	int						mode	= 0;

	switch (message) 
	{
	case WM_CREATE:
		//�{�^�����쐬
		hButton1 = CreateWindow(
                "BUTTON", "�N���X�킯", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 50, 100, 25, hWnd, (HMENU)ID_CLUSTER, hInst ,NULL);

		hButton2 = CreateWindow(
                "BUTTON", "�X�V", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 90, 100, 25, hWnd, (HMENU)ID_UPDATE, hInst ,NULL);

		hButton3 = CreateWindow(
                "BUTTON", "������", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 130, 100, 25, hWnd, (HMENU)ID_INIT, hInst ,NULL);

		hButton4 = CreateWindow(
                "BUTTON", "�N���A", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 170, 100, 25, hWnd, (HMENU)ID_CLEAR, hInst ,NULL);

		hButton5 = CreateWindow(
                "BUTTON", "���UX > Y", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 230, 100, 25, hWnd, (HMENU)ID_UPDATE1, hInst ,NULL);

		hButton6 = CreateWindow(
                "BUTTON", "���UX < Y", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 280, 100, 25, hWnd, (HMENU)ID_UPDATE2, hInst ,NULL);

wpd	= &List;
		break;

	case WM_LBUTTONDOWN:
		//��`�̒��ł��邩�ǂ����̔���
		pt	= MAKEPOINTS(lParam);
		if((pt.x > 10) && (pt.x < WIDTH) && (pt.y > 10) && (pt.y < HEIGHT))
		{
			if((AtEnd->next = new struct VECTOR2) == NULL)
			{
				MessageBox(hWnd, "���͂��ꂽ�_�̍��W��ێ����郁�����m�ۂɎ��s���܂����B", "Error", MB_OK | MB_ICONEXCLAMATION);
			}

			else
			{
				AtEnd			= AtEnd->next;
				AtEnd->x		= pt.x;
				AtEnd->y		= pt.y;
				AtEnd->id		= cnt++;
				AtEnd->Cls		= -1;
				AtEnd->Sum_dist	= 0;
				AtEnd->next	= NULL;
			}
			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

	case WM_RBUTTONDOWN:
		if(List.next != NULL)
		{
			//�_��`��\�ȋ�`�̒����ǂ����̔���
			pt	= MAKEPOINTS(lParam);
			if((pt.x > 10) && (pt.x < WIDTH) && (pt.y > 10) && (pt.y < HEIGHT))
			{
				if(N < 10)
				{
					wp		= &List;
					Min		= 9999999;

					//�N���b�N���ꂽ�_�����ԋ߂��_��I��
					while(1)
					{
						dist	= ((wp->next->x - pt.x) * (wp->next->x - pt.x) + (wp->next->y - pt.y) * (wp->next->y - pt.y));

						if(Min > dist)
						{
							Min	= dist;
							wp2	= wp;
						}
						wp	= wp->next;

						if(wp->next == NULL)
							break;
					}

					//�|�C���^�̂Ȃ�����
					CAtEnd->next	= wp2->next;

					if(wp2->next->next != NULL)
						wp2->next	= wp2->next->next;

					else
						wp2->next	= NULL;

					CAtEnd			= CAtEnd->next;
					CAtEnd->next	= NULL;

					//�p�����[�^�ɒl����
					CAtEnd->Cls		= N++;
					CAtEnd->Sum_dist= 0;
					InvalidateRect(hWnd, NULL, TRUE);
				}

				else
					MessageBox(hWnd, "�Z���g���C�h�͂P�O�ȏ���܂���", "Worning", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 

		//�{�^������
		switch(wmId)
		{
		case ID_CLUSTER:
			if((List.next != NULL) && (Centroids.next != NULL))
			{
				Clustering(&List, &Centroids, N);
				InvalidateRect(hWnd, NULL, TRUE);

				AtEnd	= &List;
				//���X�g�̖����̍X�V
				while(1)
				{
					if(AtEnd->next == NULL)
						break;

					else
						AtEnd	= AtEnd->next;
				}

				CAtEnd	= &List;
				//�Z���g���C�h���X�g�̖����̍X�V
				while(1)
				{
					if(CAtEnd->next == NULL)
						break;

					else
						CAtEnd	= CAtEnd->next;
				}
			}

			if(List.next == NULL)
					MessageBox(hWnd, "�_��ł��Ă�������", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			if(Centroids.next == NULL)
				MessageBox(hWnd, "�Z���g���C�h�����߂Ă�������", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			break;

		case ID_UPDATE2:
			mode++;

		case ID_UPDATE1:
			mode++;

		case ID_UPDATE:
			if((List.next != NULL) && (Centroids.next != NULL))
			{
				if(Execute(&List, &Centroids, N, mode) == 0)
					MessageBox(hWnd, "�v�Z����", "OK", MB_OK);

				AtEnd	= &List;
				//���X�g�̖����̍X�V
				while(1)
				{
					if(AtEnd->next == NULL)
						break;

					else
						AtEnd	= AtEnd->next;
				}

				CAtEnd	= &Centroids;
				//�Z���g���C�h���X�g�̖����̍X�V
				while(1)
				{
					if(CAtEnd->next == NULL)
						break;

					else
						CAtEnd	= CAtEnd->next;
				}
			}

			if(List.next == NULL)
				MessageBox(hWnd, "�_��ł��Ă�������", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			if(Centroids.next == NULL)
				MessageBox(hWnd, "�Z���g���C�h�����߂Ă�������", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ID_INIT:
			//�Z���g���C�h���X�g���N���A���Ēʏ탊�X�g�ɕt��
			AtEnd->next		= Centroids.next;
			Centroids.next	= NULL;
			N				= 0;

			//���X�g�̖����̍X�V
			while(1)
			{
				if(AtEnd->next == NULL)
					break;

				else
					AtEnd	= AtEnd->next;
			}
			//�Z���g���C�h���X�g
			CAtEnd	= &Centroids;

			wp	= &List;
			//�_�̃N���X�킯���N���A
			while(1)
			{
				if(wp->next == NULL)
					break;

				else
				{
					wp		= wp->next;
					wp->Cls	= -1;
				}
			}

			InvalidateRect(hWnd,NULL, TRUE);
			break;

		case ID_CLEAR:
			ClearList(&List);
			ClearList(&Centroids);
			List.next		= NULL;
			Centroids.next	= NULL;
			AtEnd			= &List;
			CAtEnd			= &Centroids;

			N				= 0;
			cnt				= 0;

			InvalidateRect(hWnd, NULL, TRUE);
			break;
		}

		// �I�����ꂽ���j���[�̉�� :
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc		= BeginPaint(hWnd, &ps);
		// TODO: �`��R�[�h�������ɒǉ����Ă�������...

		//�y���A�u���V�̍쐬
		hBlackPen	= CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
		hBlackBrush	= CreateSolidBrush(RGB(0, 0, 0));
		for(int i = 0; i < 10; i++)
		{
			hPen[i]		= CreatePen(PS_SOLID, 0, Color[i]);
			hBrush[i]	= CreateSolidBrush(Color[i]);
		}

		//�_��`��ł����`�̈��`��
		SelectObject(hdc, hBlackPen);
		Rectangle(hdc, 10, 10, 10 + WIDTH, 10 + HEIGHT);

		//����
		strcpy(Str, "���N���b�N�F�_��ǉ�");
		TextOut(hdc, 70, HEIGHT + 20, Str, (int)strlen(Str));

		strcpy(Str, "�E�N���b�N�F�Z���g���C�h��I��");
		TextOut(hdc, 270, HEIGHT + 20, Str, (int)strlen(Str));


		//�ʏ�̓_��`��
		if(List.next != NULL)
		{
			wp	= List.next;
			while(1)
			{
				if(wp->Cls == -1)
				{
					SelectObject(hdc, hBlackPen);
					SelectObject(hdc, hBlackBrush);
				}

				else
				{
					SelectObject(hdc, hPen[wp->Cls]);
					SelectObject(hdc, hBrush[wp->Cls]);
				}

				Ellipse(hdc, wp->x, wp->y, wp->x + 5, wp->y + 5);

				if(wp->next ==NULL)
					break;

				else
					wp	= wp->next;
			}
		}
		
		//�Z���g���C�h�ɑI�����ꂽ�_��`��
		if(Centroids.next != NULL)
		{
			wp	= Centroids.next;
			while(1)
			{
				SelectObject(hdc, hPen[Count]);
				SelectObject(hdc, hBrush[Count++]);
				Ellipse(hdc, wp->x, wp->y, wp->x + 10, wp->y + 10);

				if(wp->next ==NULL)
					break;

				else
					wp	= wp->next;
			}
		}
		for(int i = 0; i < 10; i++)
		{
			DeleteObject(hPen[i]);
			DeleteObject(hBrush[i]);
		}
		DeleteObject(hBlackPen);
		DeleteObject(hBlackBrush);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		ClearList(&List);
		ClearList(&Centroids);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

