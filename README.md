# tic-tac-toe
Лаб. 1 Начальное знакомство с API ОС
Задание
Необходимо разработать приложение, которое при помощи чистого WinAPI
будет создавать окно, обрабатывать клики мыши и нажатия клавиш, а также
сохранять свое состояние и восстанавливать его при последующем запуске.
По умолчанию окно размером 320 на 240 px должно быть закрашено
синим цветом с красной сеткой линий NxN.
Размер поля (переменная N) по умолчанию выбирается на усмотрение
программиста. Однако, пользователь имеет возможность переопределить размер
поля,
указав его в качестве аргумента командной строки при запуске приложения.
Окно существует и обрабатывает события до тех пор, пока пользователь не
закрыл его. При закрытии окна происходит завершение приложения.
При клике левой клавишей мыши на поле должен быть нарисован
круг определенного цвета в той ячейке поля, в которую произошел клик.
При клике правой клавишей мыши на поле должен быть нарисован крестик
определенного цвета в той ячейке поля, в которую произошел клик.
Цвет элементов поля (крестиков и кружков) выбирается на усмотрение
программиста.
Если пользователь нажал на клавиатуре Ctrl+Q или ESC, окно должно закрыться.
Если пользователь нажал на клавиатуре Shift+C, должен запуститься блокнот.
Если пользователь нажал ENTER, фоновый цвет поля должен измениться на
случайный.
При прокрутке пользователем колеса мыши текущий цвет линий сетки должен
плавно изменяться.
Смену цвета фона необходимо организовать при помощи подмены кисти окна.
Использование сообщения WM_PAINT для данной подзадачи не предполагается.
Приложение должно сохранять свое состояние путем реализации чтения/записи
конфигурационного файла программы, в котором будут храниться следующие
настройки:
Размер поля (N)
Размер окна (ширина и высота)
Цвет фона и цвет линий сетки
При первом запуске (когда конфигурационного файла еще нет) программа должна
корректно обработать эту ситуаццию,
выбрав для всех параметров указанные в задании умолчания.
Размер поля, указанный пользователем при запуске приложения в качестве аргумента
командной строки, имеет приоритет
над значением, записанным в конфигурационном файле.
Формат файла (текстовый, бинарный) - на усмотрение программиста.
