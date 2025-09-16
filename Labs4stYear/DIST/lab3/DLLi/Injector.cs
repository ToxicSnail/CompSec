using System;
using System.Diagnostics;
using System.Linq;
using System.Windows.Forms;

namespace DllInjectorGUI
{
    public class InjectorForm : Form
    {
        private Button injectButton;
        private Label infoLabel;

        public InjectorForm()
        {
            this.Text = "DLL Injector";
            this.Size = new System.Drawing.Size(400, 200);
            this.StartPosition = FormStartPosition.CenterScreen;

            infoLabel = new Label();
            infoLabel.Text = "Нажмите кнопку, чтобы инжектировать DLL в TargetProcess.exe.";
            infoLabel.AutoSize = true;

            injectButton = new Button();
            injectButton.Text = "Запустить инъекцию";
            injectButton.AutoSize = true;
            injectButton.Click += InjectButton_Click;

            FlowLayoutPanel panel = new FlowLayoutPanel();
            panel.Dock = DockStyle.Fill;
            panel.Controls.Add(infoLabel);
            panel.Controls.Add(injectButton);

            this.Controls.Add(panel);
        }

        private void InjectButton_Click(object sender, EventArgs e)
        {
            Process[] targetProcesses = Process.GetProcessesByName("TargetProcess");
            if (targetProcesses == null || targetProcesses.Length == 0)
            {
                MessageBox.Show("Процесс TargetProcess.exe не найден.", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            string pid = targetProcesses.First().Id.ToString();

            string dllPath = @"D:\localdisc\Programming\GCC\DIST\DIST_lab3\src\DllInjectorAsDll.dll";

            string arguments = $"\"{dllPath}\" HelperFunc {pid}";

            ProcessStartInfo psi = new ProcessStartInfo("rundll32.exe", arguments)
            {
                CreateNoWindow = true,
                UseShellExecute = false
            };

            try
            {
                Process proc = Process.Start(psi);
                proc.WaitForExit();
                MessageBox.Show("Инъекция выполнена успешно!", "Успех", MessageBoxButtons.OK, MessageBoxIcon.Information);
            }
            catch (Exception ex)
            {
                MessageBox.Show($"Ошибка при выполнении инъекции: {ex.Message}", "Ошибка", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        [STAThread]
        public static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.Run(new InjectorForm());
        }
    }
}
