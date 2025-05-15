import matplotlib.pyplot as plt

def read_data(filename):
  """Читает данные из файла с парами чисел (x, y).

  Args:
    filename: Имя файла.

  Returns:
    Два списка: список координат x и список координат y.
    Возвращает пустые списки в случае ошибки.
  """
  x = []
  y = []
  try:
    with open(filename, 'r') as f:
      for line in f:
        try:
          xi, yi = map(float, line.strip().split())  # Преобразуем в float
          x.append(xi)
          y.append(yi)
        except ValueError:
          print(f"Ошибка при чтении строки: {line.strip()}.  Пропускаю строку.")
  except FileNotFoundError:
    print(f"Файл не найден: {filename}")
    return [], []
  return x, y


# Пример использования:
filename1 = 'coord.txt'
filename2 = 'coord_correction.txt'
x1, y1 = read_data(filename1)
x2, y2 = read_data(filename2)
# if not x or not y:
#     print("Данные не были прочитаны. Проверьте файл.")
# else:
#     print("Данные успешно прочитаны.")



if x1 and y1:  # Убедимся, что x и y не пустые
    plt.plot(x1, y1, marker='o', linestyle='-', color='b')  # 'o' добавляет маркеры на точки
    plt.plot(x2, y2, marker='o', linestyle='-', color='r')  # 'o' добавляет маркеры на точки
    plt.xlabel('Дисперсия')
    plt.ylabel('RMSE')
    plt.title('Зависимость RMSE от дисперсии')
    plt.grid(True)  # Добавляет сетку
    plt.savefig('without_correction.png') # Сохраняет график в файл
    plt.show()  # Показывает график
else:
    print("Невозможно построить график: данные отсутствуют.")