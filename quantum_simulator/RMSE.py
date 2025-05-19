import matplotlib.pyplot as plt
from matplotlib import font_manager
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
    plt.plot(x1, y1, marker='o', linestyle='-', color='r', label='Без коррекции')  # 'o' добавляет маркеры на точки
    plt.plot(x2, y2, marker='o', linestyle='-', color='b', label='С коррекцией')  # 'o' добавляет маркеры на точки
    plt.xlabel('Среднее квадратичное отклонение', fontweight='bold', labelpad=5, fontsize=25)
    plt.ylabel('RMSE', fontweight='bold', labelpad=5, fontsize=25)
    plt.title('Зависимость RMSE от среднего квадратичного отклонения', fontweight='bold', fontsize=30)

    plt.xticks(fontsize=23, fontweight='bold')
    plt.yticks(fontsize=23, fontweight='bold')

    plt.grid(True)  # Добавляет сетку
    font_prop = font_manager.FontProperties(weight='bold', size=25)
    plt.legend(loc='upper left', bbox_to_anchor=(0, -0.05), prop=font_prop)

    plt.savefig('RMSE.png') # Сохраняет график в файл

    plt.subplots_adjust(bottom=0.2)

    plt.show()  # Показывает график
else:
    print("Невозможно построить график: данные отсутствуют.")