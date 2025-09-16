#!/usr/bin/env node

import { QMainWindow, QWidget, QLabel, QPushButton, FlexLayout, QApplication, QMessageBox } from "@nodegui/nodegui";
import { exec } from "child_process";
import { promisify } from "util";

const execAsync = promisify(exec);

/**
 * Получает PID процесса по имени с помощью команды tasklist.
 * @param processName Имя процесса, например, "TargetProcess.exe"
 * @returns PID процесса в виде строки или null, если не найден.
 */

async function getProcessId(processName: string): Promise<string | null> {
  try {
    const { stdout } = await execAsync("tasklist");
    const lines = stdout.split("\n");
    for (const line of lines) {
      if (line.includes(processName)) {
        const tokens = line.trim().split(/\s+/);
        if (tokens.length >= 2) {
          return tokens[1];
        }
      }
    }
  } catch (error) {
    console.error("Ошибка при выполнении tasklist:", error);
  }
  return null;
}

/**
 * Запускает инъекцию DLL через rundll32.exe.
 * @param pid PID процесса, в который производится инъекция.
 */

async function injectDll(pid: string): Promise<void> {
  const dllPath = "D:\\localdisc\\Programming\\GCC\\DIST\\DIST_lab3\\src\\DllInjectorAsDll.dll";
  const command = `rundll32.exe "${dllPath}" HelperFunc ${pid}`;
  try {
    const { stdout } = await execAsync(command);
    console.log(stdout);
    QMessageBox.information(null, "Инъекция", "Инъекция выполнена успешно!");
  } catch (error) {
    QMessageBox.critical(null, "Ошибка", "Ошибка при выполнении инъекции: " + error);
  }
}

/**
 * Обработчик нажатия на кнопку «Запустить инъекцию».
 */

async function onInjectButtonClick() {
  const pid = await getProcessId("TargetProcess.exe");
  if (!pid) {
    QMessageBox.critical(null, "Ошибка", "Процесс TargetProcess.exe не найден.");
  } else {
    console.log(`Найден процесс с PID: ${pid}`);
    await injectDll(pid);
  }
}

const app = new QApplication();

const win = new QMainWindow();
win.setWindowTitle("DLL Injector");

const centralWidget = new QWidget();
const layout = new FlexLayout();
centralWidget.setLayout(layout);

const label = new QLabel();
label.setText("Нажмите кнопку, чтобы инжектировать DLL в TargetProcess.exe");
layout.addWidget(label);

const button = new QPushButton();
button.setText("Запустить инъекцию");
layout.addWidget(button);

button.addEventListener("clicked", onInjectButtonClick);

win.setCentralWidget(centralWidget);
win.show();

(global as any).win = win;

app.exec();
