using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace IC_Animation_Editor
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            //Application.Run(new Form1());

            Form1 aForm = new Form1();
            aForm.Show();

            aForm.Initialize();


            int currTime = System.Environment.TickCount;

            while (aForm.Active)
            {
                int etime = currTime;
                currTime = System.Environment.TickCount;
                float fElapsedTime = (currTime - etime) / 1000.0f;

                aForm.Update(fElapsedTime);
                aForm.Render();

                Application.DoEvents();
            }

            aForm.Terminate();
        }
    }
}
