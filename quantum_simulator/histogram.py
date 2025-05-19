import matplotlib.pyplot as plt
import numpy as np
import math
from matplotlib import font_manager
def create_histogram(filename):
    # 1. Чтение данных из файла
    n = None
    correction = None
    disp = None
    data = []

    try:
        with open(filename, 'r') as f:
            lines = f.readlines()  # Читаем все строки сразу

            if len(lines) >= 1:
                try:
                    n = int(lines[0].strip()) # Читаем первое число из первой строки
                except ValueError:
                    print("Ошибка: Некорректное первое число в файле.")
                    return None, None, None, []

            if len(lines) >= 2:
                try:
                    correction = int(lines[1].strip()) # Читаем второе число из второй строки
                except ValueError:
                    print("Ошибка: Некорректное второе число в файле.")
                    return None, None, None, []

            if len(lines) >= 3:
                try:
                    disp = float(lines[2].strip()) # Читаем третье число из третьей строки
                except ValueError:
                    print("Ошибка: Некорректное третье число в файле.")
                    return None, None, None, []

            # Читаем оставшиеся числа, начиная с 4-й строки
            for line in lines[3:]:  # Начинаем с индекса 3 (4-я строка)
                try:
                    num = int(line.strip())  # Читаем остальные строки
                    data.append(num)
                except ValueError:
                    print(f"Ошибка: Некорректные данные в строке '{line.strip()}' файла. Пропускаю строку.")
                    # можно было бы return None, None, None, [] здесь, в зависимости от требований

    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
        return None, None, None, []
    num_qubits = int(math.log2(len(data)))
    # 2. Создание списка возможных состояний
    num_states = 2**num_qubits  # 2^num_qubits - общее количество возможных состояний
    states = [bin(i)[2:].zfill(num_qubits) for i in range(num_states)] #Генерация бинарных строк

    # 3. Проверка, что количество результатов соответствует ожидаемому
    if len(data) != num_states:
        print(f"Ошибка: Ожидалось {num_states} результатов измерений, но в файле {len(data)}.")
        return

    # 4. Построение гистограммы
    plt.figure(figsize=(10, 6))  # Устанавливаем размер графика
    bars = plt.bar(states, data)  # Создаем столбчатую диаграмму

    for bar in bars:
        yval = bar.get_height() #Получаем высоту столбца
        plt.text(bar.get_x() + bar.get_width()/2, yval, round(yval, 2), ha='center', va='bottom', fontweight='bold', fontsize=25)

    # 5. Настройка графика
    plt.xlabel("Состояния", fontweight='bold', labelpad=5, fontsize=25)  # Добавляем подпись к оси X
    plt.ylabel("Количество измерений", fontweight='bold', labelpad=15, fontsize=25)  # Добавляем подпись к оси Y
    string = "Гистограмма результатов " + str(n) + " измерений\n"
    if 3.1 <= disp <= 3.2:
        string += ("(Среднее квадратичное отклонение = " + "π" + ",")
    else:
        string += ("(Среднее квадратичное отклонение = " + str(disp) + ",")
    if correction == 0:
        string += " без коррекции)"
    else:
        string += " c коррекцией)"
    plt.title(string, fontweight='bold', fontsize=30)  # Добавляем заголовок
    plt.xticks(rotation=45, ha="right", fontsize=23, fontweight='bold') #Поворачиваем названия состояний для читаемости
    plt.yticks(fontsize=23, fontweight='bold')
    plt.tight_layout()  # Автоматически корректирует параметры размещения, чтобы график помещался в область
    plt.show()  # Отображаем график

# Пример использования
if __name__ == "__main__":
    filename = "results.txt"  # Имя файла с результатами измерений
    create_histogram(filename)


