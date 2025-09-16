import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class DllInjectorLauncher {

    public static void main(String[] args) {
        String targetProcessName = "TargetProcess.exe";
        
        String pid = getProcessId(targetProcessName);
        if (pid == null) {
            System.out.println("Процесс " + targetProcessName + " не найден.");
            return;
        }
        System.out.println("Найден PID процесса: " + pid);
        
<<<<<<< HEAD
        String dllPath = "D:\\localdisc\\Programming\\GCC\\DIST\\DIST_lab3\\src\\DllInjectorAsDll.dll";
=======
        String dllPath = "D:\\localdisc\\Programming\\GCC\\DIST\\DIST_lab3\\DLLi\\DllInjectorAsDll.dll";
>>>>>>> dc20c7340a56374ab0e3ff29a3ac1122f581687d
        String command = "rundll32.exe \"" + dllPath + "\" HelperFunc " + pid;
        System.out.println("Выполняется команда: " + command);
        
        try {
            Process process = Runtime.getRuntime().exec(command);
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
            process.waitFor();
            System.out.println("Команда выполнена.");
        } catch (IOException | InterruptedException e) {
            System.err.println("Ошибка при выполнении команды: " + e.getMessage());
        }
    }

    private static String getProcessId(String processName) {
        try {
            Process process = Runtime.getRuntime().exec("tasklist");
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                if (line.contains(processName)) {
                    String[] tokens = line.split("\\s+");
                    if (tokens.length >= 2) {
                        return tokens[1];
                    }
                }
            }
        } catch (IOException e) {
            System.err.println("Ошибка при выполнении tasklist: " + e.getMessage());
        }
        return null;
    }
<<<<<<< HEAD

}
=======
}
>>>>>>> dc20c7340a56374ab0e3ff29a3ac1122f581687d
