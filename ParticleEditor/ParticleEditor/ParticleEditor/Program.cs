using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ParticleEditor
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
            
           // Application.Run(new Form1());
            Form1 myForm = new Form1();

            myForm.Show();



            myForm.Initialize();

            // Store the starting time
            int nNow = System.Environment.TickCount;

            while (myForm.Running)
            {
                // Calculate the elapsed time between frames
                int nBefore = nNow;
                nNow = System.Environment.TickCount;
                float fElapsedTime = (nNow - nBefore) / 1000.0f;


                myForm.Update(fElapsedTime);
                myForm.Render();

                Application.DoEvents();
            }
            myForm.Terminate();


            }

        }
    }

