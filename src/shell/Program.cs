using System;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    internal static class Program
    {
        [STAThread]
        static int Main(string[] args)
        {
            Application.EnableVisualStyles(); Application.SetCompatibleTextRenderingDefault(false);
            string root=Path.GetFullPath(Path.Combine(AppDomain.CurrentDomain.BaseDirectory,".."));
            Application.ThreadException+=delegate(object sender,System.Threading.ThreadExceptionEventArgs e) { MessageBox.Show(e.Exception.Message,"OpenMSE",MessageBoxButtons.OK,MessageBoxIcon.Error); };
            try {
                if(args.Length==2 && args[0]=="--smoke-test") return GuiSmoke.Run(root,args[1]);
                using(var form=new MainForm(root)) {
                    foreach(string path in args) if(File.Exists(path)) form.OpenFile(path);
                    Application.Run(form);
                }
                return 0;
            } catch(Exception ex) {
                if(args.Length>0 && args[0]=="--smoke-test") { File.WriteAllText(Path.Combine(args[1],"failure.txt"),ex.ToString(),Encoding.UTF8); return 1; }
                MessageBox.Show(ex.Message,"OpenMSE",MessageBoxButtons.OK,MessageBoxIcon.Error); return 1;
            }
        }
    }
}
