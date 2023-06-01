#include "framework.h"
#include "Notepad.h"
#include <fstream>
#include <Commdlg.h>
#include <Richedit.h>
#include <shellapi.h>
#include <CommCtrl.h>

    #define MAX_LOADSTRING 100

    // Global Variables:
    HINSTANCE hInst;                                // current instance
    WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
    WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
    bool isPrinting = false;
    WCHAR szPrintText[MAX_LOADSTRING] = L"Print Text";
    HWND hwndEdit;
   


    // Forward declarations of functions included in this code module:
    ATOM                MyRegisterClass(HINSTANCE hInstance);
    BOOL                InitInstance(HINSTANCE, int);
    LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
    INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
    /*Print Function Declaration */
    BOOL DoPrint(HWND hWnd);
    BOOL DoPrintPreview(HWND hWnd);

    int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPWSTR    lpCmdLine,
        _In_ int       nCmdShow)
    {
        UNREFERENCED_PARAMETER(hPrevInstance);
        UNREFERENCED_PARAMETER(lpCmdLine);

        // Initialize global strings
        LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
        LoadStringW(hInstance, IDC_NOTEPAD, szWindowClass, MAX_LOADSTRING);
        MyRegisterClass(hInstance);

        // Perform application initialization:
        if (!InitInstance(hInstance, nCmdShow))
        {
            return FALSE;
        }

        HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_NOTEPAD));

        MSG msg;

        // Main message loop:
        while (GetMessage(&msg, nullptr, 0, 0))
        {
            if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }

        return (int)msg.wParam;
    }

    ATOM MyRegisterClass(HINSTANCE hInstance)
    {
        WNDCLASSEXW wcex;

        wcex.cbSize = sizeof(WNDCLASSEX);

        wcex.style = CS_HREDRAW | CS_VREDRAW;
        wcex.lpfnWndProc = WndProc;
        wcex.cbClsExtra = 0;
        wcex.cbWndExtra = 0;
        wcex.hInstance = hInstance;
        wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_NOTEPAD));
        wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NOTEPAD);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
        /*Wrap Text*/
        wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_NOTEPAD);
        wcex.lpszClassName = szWindowClass;
        wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

        return RegisterClassExW(&wcex);
    }

    BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
    {
        hInst = hInstance; // Store instance handle in our global variable

        HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);


        if (!hWnd)
        {
            return FALSE;
        }
        HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));
        SetMenu(hWnd, hMenu);  // Attach the menu to the window

        ShowWindow(hWnd, nCmdShow);
        UpdateWindow(hWnd);

        return TRUE;
    }

    LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        static HWND hwndEdit;
        static HFONT hFont = NULL;
        static bool ctrlPressed = false;


        switch (message)
        {
            /*WM_Create */
        case WM_CREATE:
        {
            RegisterHotKey(hWnd, 1, MOD_CONTROL, 0x5A); // Ctrl + Z (Undo)
            RegisterHotKey(hWnd, 2, MOD_CONTROL, 0x59); // Ctrl + Y (Redo)
             RegisterHotKey(hWnd, 3, MOD_CONTROL, 0x4F); // Ctrl + O (Open)
            RegisterHotKey(hWnd, 4, MOD_CONTROL, 0x53); // Ctrl + S (Save)

            RECT rcClient;
            GetClientRect(hWnd, &rcClient);

            hwndEdit = CreateWindowEx(
                0,
                L"EDIT",
                nullptr,
                WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL,
                0, 0,
                rcClient.right - rcClient.left,
                rcClient.bottom - rcClient.top,
                hWnd,
                nullptr,
                hInst,
                nullptr
            );
            hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
            SendMessage(hwndEdit, WM_SETFONT, (WPARAM)hFont, TRUE);
        }
        return 0;

        /*Short cot key of undo and redo */


        case WM_HOTKEY:
        {
            switch (LOWORD(wParam))
            {
            case 1: // Ctrl + Z (Undo)
                if (ctrlPressed)
                    SendMessage(hwndEdit, WM_UNDO, 0, 0);
                break;

            case 2: // Ctrl + Y (Redo)
                if (ctrlPressed)
                    SendMessage(hwndEdit, EM_REDO, 0, 0);
                break;
            case 3: // Ctrl + O (Open)
                if (ctrlPressed)
                    SendMessage(hWnd, WM_COMMAND, IDM_FILE_OPEN, 0);
                break;

            case 4: // Ctrl + S (Save)
                if (ctrlPressed)
                    SendMessage(hWnd, WM_COMMAND, IDM_FILE_SAVE, 0);
                break;
            }
        }
        return 0;

        case WM_KEYDOWN:
        {
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                ctrlPressed = true;
            if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
                ctrlPressed = true;
        }
        return 0;

        case WM_KEYUP:
        {
            if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000))
                ctrlPressed = false;
            if (!(GetAsyncKeyState(VK_CONTROL) & 0x8000))
                ctrlPressed = false;
        }
        return 0;



        case WM_SIZE:
        {
            RECT rcClient;
            GetClientRect(hWnd, &rcClient);  
            MoveWindow(hwndEdit, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, TRUE);
        }
        return 0;

        /*Wm_Command */

        case WM_COMMAND:
        {
            switch (LOWORD(wParam))
            {

            case IDM_FILE_PRINT:
                DoPrint(hWnd);
                return 0;

            case IDM_FILE_PRINT_PREVIEW:
                DoPrintPreview(hWnd);
                return 0;


            case IDM_HELP_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                return 0;
    
            case IDM_FORMAT_WRAPTEXT:
            {
                // Toggle wrap text functionality
                DWORD style = GetWindowLong(hwndEdit, GWL_STYLE);
                style ^= ES_AUTOHSCROLL;  // Toggle ES_AUTOHSCROLL flag
                SetWindowLong(hwndEdit, GWL_STYLE, style);
                InvalidateRect(hwndEdit, NULL, TRUE);
                UpdateWindow(hwndEdit);
            }
            return 0;

            case IDM_FILE_OPEN:
            {
                OPENFILENAME ofn;
                wchar_t szFileName[MAX_PATH] = L"";

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

                if (GetOpenFileName(&ofn))
                {
                    // Read the contents of the selected file and display them in the edit control
                    std::wifstream file(ofn.lpstrFile);
                    if (file.is_open())
                    {
                        std::wstring content((std::istreambuf_iterator<wchar_t>(file)), std::istreambuf_iterator<wchar_t>());
                        SetWindowText(hwndEdit, content.c_str());
                        file.close();
                        MessageBox(hWnd, L"File opened successfully!", L"Notepad", MB_OK);
                    }
                    else
                    {
                        MessageBox(hWnd, L"Failed to open the file.", L"Notepad", MB_OK | MB_ICONERROR);
                    }
                }
            }
            return 0;


            case IDM_FILE_SAVEAS:
            {
                OPENFILENAME ofn;
                wchar_t szFileName[MAX_PATH] = L"";

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_OVERWRITEPROMPT;

                if (GetSaveFileName(&ofn))
                {
                    // Save the contents of the edit control to the selected file
                    int length = GetWindowTextLength(hwndEdit);
                    wchar_t* buffer = new wchar_t[length + 1];
                    GetWindowText(hwndEdit, buffer, length + 1);

                    std::wofstream file(ofn.lpstrFile);
                    if (file.is_open())
                    {
                        file << buffer;
                        file.close();
                        MessageBox(hWnd, L"File saved successfully!", L"Notepad", MB_OK);
                    }
                    else
                    {
                        MessageBox(hWnd, L"Failed to save the file.", L"Notepad", MB_OK | MB_ICONERROR);
                    }

                    delete[] buffer;
                }
            }
            return 0;

            case IDM_FILE_SAVE:
            {
                OPENFILENAME ofn;
                wchar_t szFileName[MAX_PATH] = L"";

                ZeroMemory(&ofn, sizeof(ofn));
                ofn.lStructSize = sizeof(ofn);
                ofn.hwndOwner = hWnd;
                ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
                ofn.lpstrFile = szFileName;
                ofn.nMaxFile = MAX_PATH;
                ofn.Flags = OFN_OVERWRITEPROMPT;

                if (GetSaveFileName(&ofn))
                {
                    // Save the contents of the edit control to the selected file
                    int length = GetWindowTextLength(hwndEdit);
                    wchar_t* buffer = new wchar_t[length + 1];
                    GetWindowText(hwndEdit, buffer, length + 1);

                    std::wofstream file(ofn.lpstrFile);
                    if (file.is_open())
                    {
                        file << buffer;
                        file.close();
                        MessageBox(hWnd, L"File saved successfully!", L"Notepad", MB_OK);
                    }
                    else
                    {
                        MessageBox(hWnd, L"Failed to save the file.", L"Notepad", MB_OK | MB_ICONERROR);
                    }

                    delete[] buffer;
                }
            }
                return 0;
             case IDM_EDIT_COPY:
                // Handle the "Cut" menu item
                SendMessage(hwndEdit, WM_CUT, 0, 0);
                return 0;

            case IDM_EDIT_CUT:
                // Handle the "Copy" menu item
                SendMessage(hwndEdit, WM_COPY, 0, 0);
                return 0;

            case IDM_EDIT_PASTE:
                // Handle the "Paste" menu item
                SendMessage(hwndEdit, WM_PASTE, 0, 0);
                return 0;

            case IDM_EXIT:
                // Handle the "Exit" menu item
                DestroyWindow(hWnd);
                return 0;

            case IDM_EDIT_UNDO:
                // Handle the "Undo" menu item
                SendMessage(hwndEdit, WM_UNDO, 0, 0);
                return 0;

            case IDM_EDIT_REDO:
                // Handle the "Redo" menu item
                SendMessage(hwndEdit, EM_REDO, 0, 0);
                return 0;
            }
        }
        return 0;

        case WM_DESTROY:
            UnregisterHotKey(hWnd, 1); // Unregister Ctrl + Z hotkey
            UnregisterHotKey(hWnd, 2); // Unregister Ctrl + Y hotkey
            PostQuitMessage(0);
            PostQuitMessage(0);
            break;
            
        default:
            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        return DefWindowProc(hWnd, message, wParam, lParam);
        return 0;
    }
    INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
        UNREFERENCED_PARAMETER(lParam);
        switch (message)
        {
        case WM_INITDIALOG:
            return (INT_PTR)TRUE;

        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
            {
                EndDialog(hDlg, LOWORD(wParam));
                return (INT_PTR)TRUE;
            }
            break;
        }
        return (INT_PTR)FALSE;
    }
    /*Print Functioln */
    BOOL DoPrint(HWND hWnd)
    {
        PRINTDLG pd = { sizeof(PRINTDLG) };
        pd.Flags = PD_RETURNDC;

        if (!PrintDlg(&pd))
            return FALSE;

        if (pd.hDC != NULL)
        {
            DOCINFO di = { sizeof(DOCINFO) };
            di.lpszDocName = szPrintText;
            di.lpszOutput = NULL;

            if (StartDoc(pd.hDC, &di) > 0)
            {
                if (StartPage(pd.hDC) > 0)
                {
                    RECT rcPrint;
                    rcPrint.left = rcPrint.top = 0;
                    rcPrint.right = GetDeviceCaps(pd.hDC, HORZRES);
                    rcPrint.bottom = GetDeviceCaps(pd.hDC, VERTRES);

                    RECT rcClient;
                    GetClientRect(hWnd, &rcClient);

                    // Calculate the scaling factor for the content
                    double scaleX = static_cast<double>(rcPrint.right) / (rcClient.right - rcClient.left);
                    double scaleY = static_cast<double>(rcPrint.bottom) / (rcClient.bottom - rcClient.top);

                    // Create a memory device context for double buffering
                    HDC hMemDC = CreateCompatibleDC(pd.hDC);
                    HBITMAP hMemBmp = CreateCompatibleBitmap(pd.hDC, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);
                    HBITMAP hMemBmpOld = (HBITMAP)SelectObject(hMemDC, hMemBmp);

                    // Paint the content onto the memory device context
                    SendMessage(hwndEdit, WM_PRINTCLIENT, (WPARAM)hMemDC, PRF_CLIENT);

                    // Stretch the content to fit the print area
                    StretchBlt(pd.hDC, 0, 0, rcPrint.right, rcPrint.bottom,
                        hMemDC, 0, 0, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, SRCCOPY);

                    // Clean up the memory device context
                    SelectObject(hMemDC, hMemBmpOld);
                    DeleteObject(hMemBmp);
                    DeleteDC(hMemDC);

                    // End the page and document
                    EndPage(pd.hDC);
                    EndDoc(pd.hDC);
                }
            }

            DeleteDC(pd.hDC);
        }

        return TRUE;
    }

    // Function to handle print preview
    BOOL DoPrintPreview(HWND hWnd)
    {
        if (isPrinting)
            return FALSE;

        PRINTDLG pd = { sizeof(PRINTDLG) };
        pd.Flags = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
        pd.hwndOwner = hWnd;

        if (!PrintDlg(&pd))
            return FALSE;

        isPrinting = true;

        HDC hdc = pd.hDC;

        // TODO: Implement print preview functionality

        DeleteDC(hdc);

        isPrinting = false;

        return TRUE;
    }