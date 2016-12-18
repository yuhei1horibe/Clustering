// Clustering.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//
#include "stdafx.h"
#include "Clustering.h"
#define MAX_LOADSTRING 100



// グローバル変数 :
int				cnt	= 0;				//id割り振りのための変数
int				N;						//セントロイドの個数をカウント

struct VECTOR2	List;					//座標リスト
struct VECTOR2	*AtEnd	= &List;		//リストの末尾

struct VECTOR2	Centroids;				//セントロイドのリスト
struct VECTOR2	*CAtEnd	= &Centroids;	//セントロイドリストの末尾

COLORREF		Color[10];				//ブラシとペンの色を１０色分



HINSTANCE hInst;								// 現在のインターフェイス
TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名

// このコード モジュールに含まれる関数の宣言を転送します :
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);




int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: ここにコードを挿入してください。
	MSG msg;
	HACCEL hAccelTable;

	// グローバル文字列を初期化しています。
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLUSTERING, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	//リストを初期化
	List.next		= NULL;
	Centroids.next	= NULL;

	//乱数の種を初期化
	srand((unsigned)time(NULL));

	//色を１０色生成
	Color[0]	= RGB(255, 0, 0);	//赤
	Color[1]	= RGB(0, 0, 255);	//青
	Color[2]	= RGB(0, 255, 0);	//緑
	Color[3]	= RGB(255, 200, 0);	//黄色
	Color[4]	= RGB(0, 170, 170);	//青緑
	Color[5]	= RGB(255, 0, 150);	//赤紫
	Color[6]	= RGB(170, 0, 255);	//青紫
	Color[7]	= RGB(170, 50, 50);	//茶色
	Color[8]	= RGB(70, 150, 70);	//薄い緑
	Color[9]	= RGB(255, 130, 0);	//オレンジ




	// アプリケーションの初期化を実行します。:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_CLUSTERING);

	// メイン メッセージ ループ :
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
//  関数 : MyRegisterClass()
//
//  目的 : ウィンドウ クラスを登録します。
//
//  コメント :
//
//    この関数および使い方は、'RegisterClassEx' 関数が追加された
//     Windows 95 より前の Win32 システムと互換させる場合にのみ必要です。
//    アプリケーションが、関連付けられた 
//    正しい形式の小さいアイコンを取得できるようにするには、
//    この関数を呼び出してください。
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
//   関数 : InitInstance(HANDLE, int)
//
//   目的 : インスタンス ハンドルを保存して、メイン ウィンドウを作成します。
//
//   コメント :
//
//        この関数で、グローバル変数でインスタンス ハンドルを保存し、
//        メイン プログラム ウィンドウを作成および表示します。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // グローバル変数にインスタンス処理を格納します。

   hWnd = CreateWindow(szWindowClass, "クラスタリング", WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
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
//  関数 : WndProc(HWND, unsigned, WORD, LONG)
//
//  目的 :  メイン ウィンドウのメッセージを処理します。
//
//  WM_COMMAND	- アプリケーション メニューの処理
//  WM_PAINT	- メイン ウィンドウの描画
//  WM_DESTROY	- 中止メッセージを表示して戻る
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

	//ボタン
	HWND					hButton1;
	HWND					hButton2;
	HWND					hButton3;
	HWND					hButton4;
	HWND					hButton5;
	HWND					hButton6;

	static POINTS			pt;
	struct VECTOR2			*wp;	//ワークポインタ
	struct VECTOR2			*wp2;
	static struct VECTOR2	*wpd;

	int						Count	= 0;
	static char				Str[512];

	int						dist;	//距離
	int						Min;	//最小値を保持

	int						mode	= 0;

	switch (message) 
	{
	case WM_CREATE:
		//ボタンを作成
		hButton1 = CreateWindow(
                "BUTTON", "クラスわけ", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 50, 100, 25, hWnd, (HMENU)ID_CLUSTER, hInst ,NULL);

		hButton2 = CreateWindow(
                "BUTTON", "更新", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 90, 100, 25, hWnd, (HMENU)ID_UPDATE, hInst ,NULL);

		hButton3 = CreateWindow(
                "BUTTON", "初期化", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 130, 100, 25, hWnd, (HMENU)ID_INIT, hInst ,NULL);

		hButton4 = CreateWindow(
                "BUTTON", "クリア", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 170, 100, 25, hWnd, (HMENU)ID_CLEAR, hInst ,NULL);

		hButton5 = CreateWindow(
                "BUTTON", "分散X > Y", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 230, 100, 25, hWnd, (HMENU)ID_UPDATE1, hInst ,NULL);

		hButton6 = CreateWindow(
                "BUTTON", "分散X < Y", 
                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
                WIDTH + 30, 280, 100, 25, hWnd, (HMENU)ID_UPDATE2, hInst ,NULL);

wpd	= &List;
		break;

	case WM_LBUTTONDOWN:
		//矩形の中であるかどうかの判定
		pt	= MAKEPOINTS(lParam);
		if((pt.x > 10) && (pt.x < WIDTH) && (pt.y > 10) && (pt.y < HEIGHT))
		{
			if((AtEnd->next = new struct VECTOR2) == NULL)
			{
				MessageBox(hWnd, "入力された点の座標を保持するメモリ確保に失敗しました。", "Error", MB_OK | MB_ICONEXCLAMATION);
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
			//点を描画可能な矩形の中かどうかの判定
			pt	= MAKEPOINTS(lParam);
			if((pt.x > 10) && (pt.x < WIDTH) && (pt.y > 10) && (pt.y < HEIGHT))
			{
				if(N < 10)
				{
					wp		= &List;
					Min		= 9999999;

					//クリックされた点から一番近い点を選択
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

					//ポインタのつなぎかえ
					CAtEnd->next	= wp2->next;

					if(wp2->next->next != NULL)
						wp2->next	= wp2->next->next;

					else
						wp2->next	= NULL;

					CAtEnd			= CAtEnd->next;
					CAtEnd->next	= NULL;

					//パラメータに値を代入
					CAtEnd->Cls		= N++;
					CAtEnd->Sum_dist= 0;
					InvalidateRect(hWnd, NULL, TRUE);
				}

				else
					MessageBox(hWnd, "セントロイドは１０個以上作れません", "Worning", MB_OK | MB_ICONEXCLAMATION);
			}
		}
		break;

	case WM_COMMAND:
		wmId    = LOWORD(wParam); 
		wmEvent = HIWORD(wParam); 

		//ボタン操作
		switch(wmId)
		{
		case ID_CLUSTER:
			if((List.next != NULL) && (Centroids.next != NULL))
			{
				Clustering(&List, &Centroids, N);
				InvalidateRect(hWnd, NULL, TRUE);

				AtEnd	= &List;
				//リストの末尾の更新
				while(1)
				{
					if(AtEnd->next == NULL)
						break;

					else
						AtEnd	= AtEnd->next;
				}

				CAtEnd	= &List;
				//セントロイドリストの末尾の更新
				while(1)
				{
					if(CAtEnd->next == NULL)
						break;

					else
						CAtEnd	= CAtEnd->next;
				}
			}

			if(List.next == NULL)
					MessageBox(hWnd, "点を打ってください", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			if(Centroids.next == NULL)
				MessageBox(hWnd, "セントロイドを決めてください", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			break;

		case ID_UPDATE2:
			mode++;

		case ID_UPDATE1:
			mode++;

		case ID_UPDATE:
			if((List.next != NULL) && (Centroids.next != NULL))
			{
				if(Execute(&List, &Centroids, N, mode) == 0)
					MessageBox(hWnd, "計算完了", "OK", MB_OK);

				AtEnd	= &List;
				//リストの末尾の更新
				while(1)
				{
					if(AtEnd->next == NULL)
						break;

					else
						AtEnd	= AtEnd->next;
				}

				CAtEnd	= &Centroids;
				//セントロイドリストの末尾の更新
				while(1)
				{
					if(CAtEnd->next == NULL)
						break;

					else
						CAtEnd	= CAtEnd->next;
				}
			}

			if(List.next == NULL)
				MessageBox(hWnd, "点を打ってください", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			if(Centroids.next == NULL)
				MessageBox(hWnd, "セントロイドを決めてください", "Caution!", MB_OK | MB_ICONEXCLAMATION);

			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case ID_INIT:
			//セントロイドリストをクリアして通常リストに付加
			AtEnd->next		= Centroids.next;
			Centroids.next	= NULL;
			N				= 0;

			//リストの末尾の更新
			while(1)
			{
				if(AtEnd->next == NULL)
					break;

				else
					AtEnd	= AtEnd->next;
			}
			//セントロイドリスト
			CAtEnd	= &Centroids;

			wp	= &List;
			//点のクラスわけをクリア
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

		// 選択されたメニューの解析 :
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
		// TODO: 描画コードをここに追加してください...

		//ペン、ブラシの作成
		hBlackPen	= CreatePen(PS_SOLID, 0, RGB(0, 0, 0));
		hBlackBrush	= CreateSolidBrush(RGB(0, 0, 0));
		for(int i = 0; i < 10; i++)
		{
			hPen[i]		= CreatePen(PS_SOLID, 0, Color[i]);
			hBrush[i]	= CreateSolidBrush(Color[i]);
		}

		//点を描画できる矩形領域を描画
		SelectObject(hdc, hBlackPen);
		Rectangle(hdc, 10, 10, 10 + WIDTH, 10 + HEIGHT);

		//説明
		strcpy(Str, "左クリック：点を追加");
		TextOut(hdc, 70, HEIGHT + 20, Str, (int)strlen(Str));

		strcpy(Str, "右クリック：セントロイドを選択");
		TextOut(hdc, 270, HEIGHT + 20, Str, (int)strlen(Str));


		//通常の点を描画
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
		
		//セントロイドに選択された点を描画
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

// バージョン情報ボックスのメッセージ ハンドラです。
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

