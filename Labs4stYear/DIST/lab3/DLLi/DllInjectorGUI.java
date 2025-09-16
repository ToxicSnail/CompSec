import javax.swing.*;
import java.awt.*;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class DllInjectorGUI {
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

    private static void injectDll(String pid) {
        String dllPath = "D:\\localdisc\\Programming\\GCC\\DIST\\DIST_lab3\\src\\DllInjectorAsDll.dll";
        String command = "rundll32.exe \"" + dllPath + "\" HelperFunc " + pid;

        try {
            Process process = Runtime.getRuntime().exec(command);
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                System.out.println(line);
            }
            process.waitFor();
            JOptionPane.showMessageDialog(null, "Инъекция выполнена успешно!", "Успех", JOptionPane.INFORMATION_MESSAGE);
        } catch (IOException | InterruptedException e) {
            JOptionPane.showMessageDialog(null, "Ошибка при выполнении инъекции: " + e.getMessage(), "Ошибка", JOptionPane.ERROR_MESSAGE);
        }
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("DLL Injector");
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setSize(400, 200);
        frame.setLocationRelativeTo(null);

        JPanel panel = new JPanel();
        frame.add(panel);
        panel.setLayout(new FlowLayout());

        JLabel label = new JLabel("Нажмите кнопку, чтобы инжектировать DLL в TargetProcess.exe.");
        panel.add(label);

        JButton injectButton = new JButton("Запустить инъекцию");
        panel.add(injectButton);

        injectButton.addActionListener(e -> {
            String pid = getProcessId("TargetProcess.exe");
            if (pid == null) {
                JOptionPane.showMessageDialog(null, "Процесс TargetProcess.exe не найден.", "Ошибка", JOptionPane.ERROR_MESSAGE);
            } else {
                injectDll(pid);
            }
        });
        
        frame.setVisible(true);
    }
}
