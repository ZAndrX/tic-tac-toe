
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>
#include <tchar.h>
#include <windowsx.h>
#include <fstream>

using namespace std;

struct st //вспомогательная структура для хранение данных(на случай отстутствия конфигурационного файла, в ней записаные дефолтные значения)
{
    int width=320; //ширина окна
    int height=240; //высота окна
    int N=3; //размерность сетки
    int net_width=width/N; //ширина ячейки сетки 
    int net_height=height/N; //высота ячейки сетки
    int block; //размер, которого бдует рисунок в ячейке
}build;

//глобальные переменные 
DWORD red0 = RGB(255, 0, 0); //начальный цвет сетки
DWORD bg = RGB(117, 187, 253); //начальный цвет фона
int color = 0; //вспомогательная переменная для градиента
vector<vector<short>> matrix(build.N, vector<short>(build.N, 0));
const wchar_t class_name[] = L"Sample Window Class"; //название класса
const TCHAR szTitle[] = L"Крестики-нолики"; //заголовок окна

HPEN COLOR_NET; //кисть для сетки
POINT mouse; //переменная для вычиления нажатия

HBRUSH COLOR_BG = CreateSolidBrush(bg); //создание фона
void saving() {//функция сохранения
    ofstream out("config.txt"); //открываем файл на запись
    if (out.is_open()) //если открылся, заносим в него все данные
    {
        out.clear();
        out << build.N << endl << build.width << endl << build.height << endl << bg << endl << red0;
        out.close();
    }
}
void read() {//функция считывания данныъ
    ifstream in("config.txt"); //пытаемся открыть файл на чтение
    if (in.is_open()) { //если открывается, считываем все данные проверяя, его правильность
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
void up_block() {//вычисления размера рисунка в ячейке
    build.block = min(build.net_width, build.net_height)*0.8;
}
void up_net() {//обновление размеров ячеки сетки
          build.net_width = build.width / build.N;
          build.net_height = build.height / build.N;
          up_block();
}
void up_size(int width,int height) {//обновление размеров окна
    build.width = width;
    build.height = height;
    up_net();
}
void painting(HDC hdc, int x0, int y0, int x1, int y1) {//рисование линии
    MoveToEx(hdc, x0, y0, NULL);
    LineTo(hdc, x1, y1);
}
void cross(HDC hdc, int i, int j) {//рисование крестика
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
void RunNotepad()//запуск блокнота
{
    STARTUPINFO sInfo;
    PROCESS_INFORMATION pInfo;
    ZeroMemory(&sInfo, sizeof(STARTUPINFO));
    CreateProcess(_T("C:\\Windows\\Notepad.exe"),NULL, NULL, NULL, FALSE, 0, NULL, NULL, &sInfo, &pInfo);
}
void create_net(HWND& hwnd,DWORD& red0) {//рисование сетки
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
void smoothness(DWORD& red0) { //планое увеличение цвета
    red0 = RGB(255, color, 0);
    if (color <= 250) color += 5;
}
void smoothness2(DWORD& red0) { //плавное уменьшение цвета
    red0 = RGB(255, color, 0);
    if (color >= 5) color -= 5;

}
LRESULT CALLBACK WindowProc(HWND hwnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps; //структура рисования
    int j, i;//переменные для отслеживания нажатий
    switch (Msg)
    {
    case WM_KEYDOWN:
        switch (wParam) {
        case VK_ESCAPE: //закрытие программы на esc
            saving();
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            break;
        case  VK_RETURN: //изменение цвета фона на enter
            //if (COLOR_BG) DeleteObject(COLOR_BG);
            bg = RGB(rand() % 256, rand() % 256, rand() % 256);
            COLOR_BG = CreateSolidBrush(bg);
            SetClassLongPtr(hwnd, GCL_HBRBACKGROUND, (LONG)COLOR_BG);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
    case WM_MOUSEWHEEL: //плавное изменение цвета сетки 
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
    case WM_CHAR://для комбинаций клавиш
    {
        if (GetKeyState(VK_CONTROL) & 0x8000 && GetKeyState('Q') & 0x8000) //зактытие на ctrl+q
        {
            saving();
            DestroyWindow(hwnd);
            PostQuitMessage(0);
            return 0;
        }
        else
            if (GetKeyState(VK_SHIFT) & 0x8000 && GetKeyState('C') & 0x8000) { //запуск блокнота на shift+c
                RunNotepad();
                break;
            }
        break;
    }
    case WM_RBUTTONDOWN: //записывание нажатий правой клавиши мыши в matrix
        mouse.x = GET_X_LPARAM(lParam);
        mouse.y = GET_Y_LPARAM(lParam);
        i = mouse.x / build.net_width;
        j = mouse.y / build.net_height;
        if (matrix[i][j] == 0 && i < build.N && j < build.N && i >= 0 && j >= 0) {
            matrix[i][j] = 1;
            InvalidateRect(hwnd, NULL, TRUE);
        }
        break;
    case WM_LBUTTONDOWN: //записывание нажатий левой клавиши мыши в matrix
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
    case WM_DESTROY://закрытие окна
        saving();
        PostQuitMessage(0);
        break;
    case WM_SIZE: //изменение размеров окна
        up_size(LOWORD(lParam), HIWORD(lParam));
        break;
    case WM_PAINT://рисоавание фона, сетки, крестиков и ноликов
    {
        HDC hdc = BeginPaint(hwnd, &ps);
        COLOR_NET = CreatePen(PS_SOLID, 4, red0);
        SelectObject(hdc, COLOR_NET);
        for (int i = 1; i < build.N; i++) {// сетка
            painting(hdc, 0, build.net_height * i, build.width, build.net_height * i);
            painting(hdc, build.net_width * i, 0, build.net_width * i, build.height);
        }

        COLOR_NET = CreatePen(PS_SOLID, 4, RGB(255, 0, 0));
        SelectObject(hdc, COLOR_NET);
        for (int i = 0; i < build.N; i++)
            for (int j = 0; j < build.N; j++)
                if (matrix[i][j] == -1)//нолики
                    Ellipse(hdc,
                        i * build.net_width + build.net_width / 2 - build.block / 2,
                        j * build.net_height + build.net_height / 2 - build.block / 2,
                        i * build.net_width + build.net_width / 2 + build.block / 2,
                        j * build.net_height + build.net_height / 2 + build.block / 2);
                else //крестики
                    if (matrix[i][j] == 1)
                        cross(hdc, i, j);
        //очищение объектов
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
    //считывание аргумента командной строки и считывание конфигурационного файла
    LPWSTR* argv;
    int argc;
    argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    read();
    if (argc > 1) {
        build.N = stoi(argv[1]);
        matrix.assign(build.N, vector<short>(build.N, 0));
    }
    //создание класса виндовс и задание его параметров
    WNDCLASS wc = { };
    wc.style = CS_HREDRAW | CS_VREDRAW; //перерисовка при изменении размров окна
    wc.cbClsExtra = 0;//количество дополнительный байт                            
    wc.cbWndExtra = 0;//количество дополнительный байт  
    wc.lpfnWndProc = WindowProc; //Указатель на оконную процедуру
    wc.hInstance = hInstance;//передача оконной процедуры
    wc.lpszClassName = class_name; //передача имени класса
    wc.hbrBackground = COLOR_BG; //фон
    RegisterClass(&wc); //регистрация класса
    // Create the window.
    HWND hwnd = CreateWindowEx(
        0,                              // расширенный стиль класса
        class_name,                     // название класса
        szTitle,                         // передача заголовка
        WS_OVERLAPPEDWINDOW,            // стиль окна
        CW_USEDEFAULT, CW_USEDEFAULT, //позиция окна
        build.width, build.height, //размер окна
        NULL,       //родительское окно
        NULL,       // дачернее окно
        hInstance,  // передача histance 
        NULL        // Дополнительные данные приложения
    );
    if (hwnd == NULL)//если hwnd не задано закрываем приложение
    {
        return 0;
    }
    ShowWindow(hwnd, nCmdShow);
    // Запускаем цикл сообщений.
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    //очищаем память
    //DeleteObject( wc);
    DeleteObject(COLOR_BG);
    DeleteObject(COLOR_NET);
    return 0;
}


