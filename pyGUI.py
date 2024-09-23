import serial
import time
from tkinter import *

# Создаем окно приложения
window = Tk()
window.title("Мониторинг данных с датчиков")
window.geometry("600x400")

# Текстовые поля для отображения данных
label1 = Label(window, text="ТДС: 0.00 ppm", font=("Arial", 18))
label1.pack(anchor='n', pady=5)

label2 = Label(window, text="Температура: 0.00 °C", font=("Arial", 18))
label2.pack(anchor='n', pady=5)

label3 = Label(window, text="Эхолот: 0.00 см", font=("Arial", 18))
label3.pack(anchor='n', pady=5)

data_label = Label(window, text="Массив данных: [0.00, 0.00, 0.00]", font=("Arial", 18))
data_label.pack(anchor='center', pady=20)

# Настройки порта
port = 'COM7'  # Убедитесь, что это правильный порт
baudrate = 9600
timeout = 1

# Открываем последовательный порт
try:
    ser = serial.Serial(port, baudrate, timeout=timeout)
    time.sleep(2)  # Даем время на установку соединения
except serial.SerialException:
    print(f"Не удалось подключиться к {port}. Проверьте порт и настройки.")
    ser = None

# Функция для обновления данных с последовательного порта
def update_data():
    if ser and ser.in_waiting > 0:
        try:
            # Чтение данных с порта
            line = ser.readline().decode('utf-8').rstrip()
            # Разделение строки на значения для каждого датчика
            values = line.split(',')

            # Обновляем значения с датчиков
            if len(values) >= 3:
                label1.config(text=f"ТДС: {values[0]} ppm")
                label2.config(text=f"Температура: {values[1]} °C")
                label3.config(text=f"Эхолот: {values[2]} см")
                data_label.config(text=f"Массив данных: {values}")
        except Exception as e:
            print(f"Ошибка при чтении данных: {e}")

    # Перезапуск функции через 1 секунду
    window.after(1000, update_data)

# Запускаем цикл обновления данных
if ser:
    window.after(1000, update_data)

# Запуск основного цикла интерфейса
window.mainloop()

# Закрываем порт при завершении программы
if ser:
    ser.close()
