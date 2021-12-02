
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <fstream>

using namespace std;

struct st //��������������� ��������� ��� �������� ������(�� ������ ����������� ����������������� �����, � ��� ��������� ��������� ��������)
{
    int width=320; //������ ����
    int height=240; //������ ����
    int N=3; //����������� �����
    int net_width=width/N; //������ ������ ����� 
    int net_height=height/N; //������ ������ �����
    int block; //������, �������� ����� ������� � ������
}build;

//���������� ���������� 
DWORD red0 = RGB(255, 0, 0); //��������� ���� �����
DWORD bg = RGB(117, 187, 253); //��������� ���� ����
int color = 0; //��������������� ���������� ��� ���������
vector<vector<short>> matrix(build.N, vector<short>(build.N, 0));
const wchar_t class_name[] = L"Sample Window Class"; //�������� ������
const TCHAR szTitle[] = L"��������-������"; //��������� ����

HPEN COLOR_NET; //����� ��� �����
POINT mouse; //���������� ��� ��������� �������

HBRUSH COLOR_BG = CreateSolidBrush(bg); //�������� ����
void saving() {//������� ����������
    ofstream out("config.txt"); //��������� ���� �� ������
    if (out.is_open()) //���� ��������, ������� � ���� ��� ������
    {
        out.clear();
        out << build.N << endl << build.width << endl << build.height << endl << bg << endl << red0;
        out.close();
    }
}
void read() {//������� ���������� ������
    ifstream in("config.txt"); //�������� ������� ���� �� ������
    if (in.is_open()) { //���� �����������, ��������� ��� ������ ��������, ��� ������������
        string str;
        if(!getline(in, str)) return;
        build.N = stoi(str);
        if (!getline(in, str)) return;
        build.width = stoi(str);
        if (!getline(in, str)) return;
        build.height = stoi(str);
        if (!getline(in, str)) return;
        bg= stoi(str);
        COLOR_BG = CreateSolidBrush(bg);
        if (!getline(in, str)) return;
        red0 = stoi(str);
        in.close();
    }

}
void up_block() {//���������� ������� ������� � ������
    build.block = min(build.net_width, build.net_height)*0.8;
}
void up_net() {//���������� �������� ����� �����
          build.net_width = build.width / build.N;
          build.net_height = build.height / build.N;
          up_block();
}
void up_size(int width,int height) {//���������� �������� ����
    build.width = width;
    build.height = height;
    up_net();
}
void painting(HDC hdc, int x0, int y0, int x1, int y1) {//��������� �����
    MoveToEx(hdc, x0, y0, NULL);
    LineTo(hdc, x1, y1);
}
void cross(HDC hdc, int i, int j) {//��������� ��������
    painting(hdc,
        i * build.net_width + build.net_width / 2 - build.block / 2,
        j * build.net_height + build.net_height / 2 - build.block / 2,
        i * build.net_width + build.net_width / 2 + build.block / 2,
        j * build.net_height + build.net_height / 2 + build.block / 2);
    painting(hdc,
        i * build.net_width + build.net_width / 2 + build.block / 2,
        j * build.net_height + build.net_height / 2 - build.block / 2,
        i * build.net_width + build.net_width / 2 - build.block / 2,
        j * build.net_height + build.net_height / 2 + build.block / 2);
};
void RunNotepad()//������ ��������
{
    STARTUPINFO sInfo;
    PROCESS_INFORMATION pInfo;
    ZeroMemory(&sInfo, sizeof(STARTUPINFO));
    CreateProcess(_T("C:\\Windows\\Notepad.exe"),NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}
void create_net(HWND& hwnd,DWORD& red0) {//��������� �����
    PAINTSTRUCT pss;
    HDC HDC = BeginPaint(hwnd, &pss);
    COLOR_NET = CreatePen(PS_SOLID, 4, red0);
    SelectObject(HDC, COLOR_NET);
    for (int i = 1; i < build.N; i++) {
        painting(HDC, 0, build.net_height * i, build.width, build.net_height * i);
        painting(HDC, build.net_width * i, 0, build.net_width * i, build.height);
    }
    DeleteObject(COLOR_NET);
    DeleteObject(HDC);
};
void smoothness(DWORD& red0) { //������ ���������� �����
    red0 = RGB(255, color, 0);
    if (color <= 250) color += 5;
}
void smoothness2(DWORD& red0) { //������� ���������� �����
    red0 = RGB(255, color, 0);
    if (color >= 5) color -= 5;

}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps; //��������� ���������
    int j, i;//���������� ��� ������������ �������
    switch (Msg)
    {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE: //�������� ��������� �� esc
            saving();
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            break;
        case  VK_RETURN: //��������� ����� ���� �� enter
            //if (COLOR_BG) DeleteObject(COLOR_BG);
            bg = RGB(rand() % 256, rand() % 256, rand() % 256);
            COLOR_BG = CreateSolidBrush(bg);
            SetClassLongPtr(hwnd, GCL_HBRBACKGROUND, (LONG)COLOR_BG);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
    case WM_MOUSEWHEEL: //������� ��������� ����� ����� 
        if (GET_WHEEL_DELTA_WPARAM(wParam) > 0) {
            for (int i = 0; i < GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; i++, smoothness(red0));
            InvalidateRect(hwnd, NULL, false);
            create_net(hwnd, red0);
        }

        else
            if (GET_WHEEL_DELTA_WPARAM(wParam) < 0) {
                for (int i = 0; i > GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA; i--, smoothness2(red0));
                InvalidateRect(hwnd, NULL, false);
                create_net(hwnd, red0);
            }
        break;
    case WM_CHAR://��� ���������� ������
    {
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('Q') & 0x8000) //�������� �� ctrl+q
        {
            saving();
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            return 0;
        }
        else
            if (GetKeyState(VK_SHIFT) & 0x8000 && GetKeyState('C') & 0x8000) { //������ �������� �� shift+c
                RunNotepad();
                break;
            }
        break;
    }
    case WM_RBUTTONDOWN: //����������� ������� ������ ������� ���� � matrix
        mouse.x = GET_X_LPARAM(lParam);
        mouse.y = GET_Y_LPARAM(lParam);
        i = mouse.x / build.net_width;
        j = mouse.y / build.net_height;
        if (matrix[i][j] == 0 && i < build.N && j < build.N && i >= 0 && j >= 0) {
            matrix[i][j] = 1;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    case WM_LBUTTONDOWN: //����������� ������� ����� ������� ���� � matrix
        mouse.x = GET_X_LPARAM(lParam);
        mouse.y = GET_Y_LPARAM(lParam);
        i = mouse.x / build.net_width;
        j = mouse.y / build.net_height;
        if (matrix[i][j] == 0 && i < build.N && j < build.N && i >= 0 && j >= 0) {
            matrix[i][j] = -1;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    case WM_CLOSE:
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY://�������� ����
        saving();
        PostQuitMessage(0);
        break;
    case WM_SIZE: //��������� �������� ����
        up_size(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_PAINT://���������� ����, �����, ��������� � �������
    {
        HDC hdc = BeginPaint(hwnd, &ps);
        COLOR_NET = CreatePen(PS_SOLID, 4, red0);
        SelectObject(hdc, COLOR_NET);
        for (int i = 1; i < build.N; i++) {// �����
            painting(hdc, 0, build.net_height * i, build.width, build.net_height * i);
            painting(hdc, build.net_width * i, 0, build.net_width * i, build.height);
        }

        COLOR_NET = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
        SelectObject(hdc, COLOR_NET);
        for (int i = 0; i < build.N; i++)
            for (int j = 0; j < build.N; j++)
                if (matrix[i][j] == -1)//������
                    Ellipse(hdc,
                        i * build.net_width + build.net_width / 2 - build.block / 2,
                        j * build.net_height + build.net_height / 2 - build.block / 2,
                        i * build.net_width + build.net_width / 2 + build.block / 2,
                        j * build.net_height + build.net_height / 2 + build.block / 2);
                else //��������
                    if (matrix[i][j] == 1)
                        cross(hdc, i, j);
        //�������� ��������
        EndPaint(hwnd, &ps);
        DeleteObject(COLOR_NET);
        DeleteObject(hdc);
        break;
    }
    default:
        return DefWindowProc(hwnd, Msg, wParam, lParam);
        break;
    }
    return 0;

}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    //���������� ��������� ��������� ������ � ���������� ����������������� �����
    LPWSTR* argv;
    int argc;
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    read();
    if (argc > 1) {
        build.N = stoi(argv[1]);
        matrix.assign(build.N, vector<short>(build.N, 0));
    }
    //�������� ������ ������� � ������� ��� ����������
    WNDCLASS wc = { };
    wc.style = CS_HREDRAW | CS_VREDRAW; //����������� ��� ��������� ������� ����
    wc.cbClsExtra = 0;//���������� �������������� ����                            
    wc.cbWndExtra = 0;//���������� �������������� ����  
    wc.lpfnWndProc = WindowProc; //��������� �� ������� ���������
    wc.hInstance = hInstance;//�������� ������� ���������
    wc.lpszClassName = class_name; //�������� ����� ������
    wc.hbrBackground = COLOR_BG; //���
    RegisterClass(&wc); //����������� ������
    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // ����������� ����� ������
        class_name,                     // �������� ������
        szTitle,                         // �������� ���������
        WS_OVERLAPPEDWINDOW,            // ����� ����
        CW_USEDEFAULT, CW_USEDEFAULT, //������� ����
        build.width, build.height, //������ ����
        NULL,       //������������ ����
        NULL,       // �������� ����
        hInstance,  // �������� histance 
        NULL        // �������������� ������ ����������
    );
    if (hwnd == NULL)//���� hwnd �� ������ ��������� ����������
    {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    // ��������� ���� ���������.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //������� ������
    //DeleteObject( wc);
    DeleteObject(COLOR_BG);
    DeleteObject(COLOR_NET);
    return 0;
}


