import matplotlib.pyplot as plt
import numpy as np
import math
def create_histogram(filename):
    # 1. Чтение данных из файла
    try:
        with open(filename, 'r') as f:
            data = [int(line.strip()) for line in f]
    except FileNotFoundError:
        print(f"Ошибка: Файл '{filename}' не найден.")
        return
    except ValueError:
        print(f"Ошибка: Некорректные данные в файле '{filename}'. Убедитесь, что в файле только целые числа.")
        return
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
    plt.bar(states, data)  # Создаем столбчатую диаграмму

    # 5. Настройка графика
    plt.xlabel("Состояния", fontsize=12)  # Добавляем подпись к оси X
    plt.ylabel("Количество измерений", fontsize=12)  # Добавляем подпись к оси Y
    plt.title("Гистограмма результатов измерений", fontsize=14)  # Добавляем заголовок
    plt.xticks(rotation=45, ha="right") #Поворачиваем названия состояний для читаемости

    plt.tight_layout()  # Автоматически корректирует параметры размещения, чтобы график помещался в область
    plt.show()  # Отображаем график

# Пример использования
if __name__ == "__main__":
    filename = "results.txt"  # Имя файла с результатами измерений
    create_histogram(filename)


