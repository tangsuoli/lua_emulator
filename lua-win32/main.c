#if defined(UNICODE) && !defined(_UNICODE)
    #define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
    #define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <shlobj.h>
#include <stdbool.h>



/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");


#define IDB_BASE 3300

HWND g_hwnd = NULL;
HWND g_hEdit = NULL;
HWND g_hRunBtn = NULL;
int g_key_value = -1;
char g_selected_dir[MAX_PATH] = {0};
char *g_stringFromFile = NULL;
char *g_stringFromEditor = NULL;

int WINAPI WinMain (HINSTANCE hThisInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpszArgument,
                     int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           _T("Code::Blocks Template Windows App"),       /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           1024,                 /* The programs width */
           768,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );
    g_hwnd = hwnd;

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        if (messages.message == 953)printf("WinMain GetMessage message = %d\n", messages.message);
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}

// 此回调函数为全局函数或静态函数;
static int CALLBACK BrowseCallbackProc( HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData )
{
    switch(uMsg)
    {
    case BFFM_INITIALIZED:
        {
            SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
        }
        break;
    default:
        break;
    }
    return 0;
}

static bool SelectFilePath(char *srcFilePath)
{
    char* szPathName[MAX_PATH] = {0};
    BROWSEINFO bInfo = {0};
    char* strDefaultPath = ("D:\\"); // 注意路径中不要带'\..\'或'\.\'符号，否则设置默认路径失败;
    bInfo.hwndOwner = GetForegroundWindow(); // 父窗口;
    bInfo.lpszTitle = _T("选择目录");
    bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI /*包含一个编辑框 用户可以手动填写路径 对话框可以调整大小之类的..;*/
        | BIF_UAHINT /*带TIPS提示*/ /*| BIF_NONEWFOLDERBUTTON 不带新建文件夹按钮*/;
    // 关于更多的 ulFlags 参考 http://msdn.microsoft.com/en-us/library/bb773205(v=vs.85).aspx;
    bInfo.lpfn        = (BrowseCallbackProc);
    bInfo.lParam    = (LPARAM)(LPCTSTR)(strDefaultPath);

    LPITEMIDLIST lpDlist;
    lpDlist = SHBrowseForFolder(&bInfo);
    if ( NULL == lpDlist ) // 单击了确定按钮;
    {
        return false;
    }
    SHGetPathFromIDList(lpDlist, szPathName);
    strcpy(srcFilePath, szPathName);
    return true;
}

void create25Button(HWND hwnd) {
    int l =35;
    int t = 10;
    int lg = 70;
    int tg = 70;
    int w = 60;
    int h = 60;

    int n = 0;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            char id[16];
            n++;
            CreateWindow(TEXT("Button"), TEXT(itoa(n,id,10)), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
                l+lg*i, t+tg*j, w, h, hwnd, (HMENU)(IDB_BASE+n), (HINSTANCE)hwnd, NULL);
        }
    }
}

void createOtherButtons(HWND hwnd) {
    int l =145;
    int t = 650;
    int w = 120;
    int h = 50;
    CreateWindow(TEXT("Button"), TEXT("选择卡片目录"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
          l, t, w, h, hwnd, (HMENU)(IDB_BASE+100), (HINSTANCE)hwnd, NULL);

    l = 850;
    t = 650;
    g_hRunBtn = CreateWindow(TEXT("Button"), TEXT("保存并运行脚本"), WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
          l, t, w, h, hwnd, (HMENU)(IDB_BASE+101), (HINSTANCE)hwnd, NULL);

    l = 450;
    t = 10;
    w = 1024-35-l;
    h = 600;
    g_hEdit = CreateWindow(TEXT("EDIT"),TEXT("选择卡片脚本（card.lua）的目录"),WS_CHILD | WS_VISIBLE |
        WS_VSCROLL | WS_HSCROLL | ES_AUTOHSCROLL | ES_MULTILINE,
        l,t,w,h,hwnd,(HMENU)(IDB_BASE+102), (HINSTANCE)hwnd,NULL);
}


static void OnCreate(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {

    create25Button(hwnd);
    createOtherButtons(hwnd);
}


static void OnSave() {
    char path[MAX_PATH] = {0};
    sprintf(path, "%s/card.lua", g_selected_dir);
    FILE* pFile = fopen(path,"w");
    int len = strlen(g_stringFromEditor);
    char* tempString = (char *)malloc(len + 1);
    memset(tempString,0,len + 1);
    int i = 0, j = 0;

    printf("OnSave 1111 %d, %s\n", len, g_stringFromEditor);
     printf("OnSave 2222 %d, %d, %d\n", g_stringFromEditor[0], g_stringFromEditor[1], g_stringFromEditor[2]);

    while (i < len){
        if (g_stringFromEditor[i] != '\r') {
             tempString[j++] = g_stringFromEditor[i++];
        } else {
            i++;
        }
    }

    fwrite(tempString,strlen(tempString),1,pFile);
    fclose(pFile);
}

static void get_editor_string() {
    LRESULT nTextLen = SendMessage(g_hEdit,WM_GETTEXTLENGTH,0,0);
    if (g_stringFromEditor == NULL) {
        free(g_stringFromEditor);
        g_stringFromEditor = NULL;
    }
    g_stringFromEditor = (char *)malloc(nTextLen + 1);
    memset(g_stringFromEditor,0,nTextLen + 1);

    SendMessage(g_hEdit,WM_GETTEXT,nTextLen + 1,(LPARAM)g_stringFromEditor);
    printf("get_editor_string %s\n", g_stringFromEditor);
}

static void OnOpen()
{
    //打开文件读取数据
    char path[MAX_PATH] = {0};
    sprintf(path, "%s/card.lua", g_selected_dir);

	int line_len = 0; // 文件每行的长度
	char buf[1024] = {0}; // 行数据缓存

	FILE* fp = fopen(path, "r");
	if (NULL == fp) {
		printf("open %s failed.\n", path);
		return -1;
	}

    //获取文件长度
    fseek(fp,0,SEEK_END);
    long nFilen = ftell(fp);
    if (g_stringFromFile == NULL) {
        free(g_stringFromFile);
        g_stringFromFile = NULL;
    }
    g_stringFromFile = (char *)malloc(nFilen + 1);
    memset(g_stringFromFile,0,nFilen + 1);

    fseek(fp,0,SEEK_SET);
	while(fgets(buf, 1024, fp)) {
        int offset = 0;
		line_len = strlen(buf);
		if ('\n' == buf[line_len - 1]) {// 排除换行符
			buf[line_len - 1] = '\0';

			line_len--;
			if (0 == line_len) {
                strncat(g_stringFromFile, "\r\n", 2);
				continue;	//空行
			}
		}

		// windos文本排除回车符
		if ('\r' == buf[line_len - 1]) {
			buf[line_len - 1] = '\0';
			line_len--;
			if (0 == line_len) {
                strncat(g_stringFromFile, "\r\n", 2);
				continue;	//空行
			}
		}
		strncat(g_stringFromFile, buf, line_len);
		strncat(g_stringFromFile, "\r\n", 2);
	}

	if (0 == feof) {
		// 未读到文件末尾
		printf("fgets error\n");
		return -1;
	}
	fclose(fp);
    SetWindowTextA(g_hEdit, g_stringFromFile);
}

static void OnCommand(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    int nEventID = LOWORD(wParam);
    printf("OnCommand: nEventID =[%d]\n", nEventID);

    if (nEventID < IDB_BASE+100) {
        if (is_playing()) {
            g_key_value = -1;
            printf("OnCommand: is playing !g_key_value invalid\n");
        } else {
            g_key_value = nEventID-IDB_BASE;
            printf("OnCommand: g_key_value =[%d]\n", g_key_value);
        }
        return;
    }
    switch (nEventID) {
        case IDB_BASE+100: // select dir
            SelectFilePath(g_selected_dir);
            printf("g_selected_dir = %s\n", g_selected_dir);
            OnOpen();
            break;
        case IDB_BASE+101: // run
            get_editor_string();
            OnSave();
            lua_test_main_string(g_stringFromEditor);
            break;
    }
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //printf("WindowProcedure message = %d\n", message);
    switch (message)                  /* handle the messages */
    {
        case WM_CREATE:
            OnCreate(hwnd, message, wParam, lParam);
            break;
        case WM_COMMAND:
            OnCommand(hwnd, message, wParam, lParam);
            break;
        case WM_SIZE:
           // OnSize(hwnd, message, wParam, lParam);
            break;
        case MM_MCINOTIFY:
            stop_play_sound();
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;

        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
