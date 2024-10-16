import threading
import time

event = threading.Event()    # Создаем объект события

def producer():    # Функция потока-поставщика
    while True:
        print("Поток-поставщик:инициализированно условное событие")
        event.set()    # Устанавливаем событие
        time.sleep(1)

def consumer():    # ункция потока-потребителя
    while True:
        print("Поток-потребитель: ожидание условного события")
        event.wait()    # Ожидаем событие
        event.clear()    # Сбрасываем наше событие
        print("Поток-потребитель: условное событие получено")

# Создаем и запускаем потоки
producer_thread = threading.Thread(target=producer)
consumer_thread = threading.Thread(target=consumer)

producer_thread.start()
consumer_thread.start()

# Чтобы завершить программу, можно использовать KeyboardInterrupt (Ctrl+C)
try:
    producer_thread.join()
    consumer_thread.join()
except KeyboardInterrupt:
    print("Программа завершена!")
