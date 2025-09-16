# DIST_lab4

**DIST_lab4** — консольное приложение для рекурсивного шифрования и дешифрования всех файлов в указанной директории на базе алгоритма AES-256.

## Возможности

* Шифрование (`encrypt`) и дешифрование (`decrypt`) всех файлов в папке и её подпапках.
* Режим работы задаётся первым параметром команды.
* Обход директорий выполняется через API (`dirent.h`).
* Алгоритм AES-256 CBC реализован через OpenSSL (`<openssl/aes.h>`).
* Использует паттерн **Singleton** для единой точки доступа к классу `Encryptor`.

## Требования

* Linux
* CMake ≥ 3.5
* Компилятор с поддержкой C++17 (g++ 7+)
* OpenSSL (dev-пакет)

## Установка зависимостей

```bash
sudo apt update
sudo apt install -y build-essential cmake libssl-dev
```

## Сборка

```bash
git clone <repo>
cd DIST_lab4
mkdir build && cd build
cmake ..
make
```

## Использование

```bash
# Шифрование:
./encryptor encrypt <путь_к_папке> <ключ>

# Дешифрование:
./encryptor decrypt <путь_к_папке> <ключ>
```

* `<путь_к_папке>` — абсолютный или относительный путь к целевой директории.
* `<ключ>` — пароль (до 32 байт), используемый для AES-256.

## Примеры

```bash
# Зашифровать все файлы в ~/secret_folder
./encryptor encrypt ~/secret_folder SuperSecretKey123

# Расшифровать обратно
./encryptor decrypt ~/secret_folder SuperSecretKey123
```

## Структура проекта

```
Lab4Encryptor/
├─ CMakeLists.txt
├─ main.cpp         # Точка входа, парсит аргументы и вызывает Encryptor
├─ Encryptor.h/.cpp # Singleton-класс: обход директорий и вызов AES
├─ DirWalker.h/.cpp # Рекурсивный сбор файлов через dirent.h
└─ Aes256.h/.cpp    # Обёртка над OpenSSL AES под CBC
```
