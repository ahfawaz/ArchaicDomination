using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Xml;
using System.Xml.Serialization;
using System.Xml.Linq;

namespace ParticleEditor
{
    public partial class Form1 : Form
    {
        //Initializing Direct3D Wrappers
        SGP.CSGP_Direct3D m_D3D = null;
        SGP.CSGP_TextureManager m_TM = null;

        //FilePath
        string filename;
        string associatedfilepath;
        string associatedfilename;

        // Game time
        float m_fTimer = 0;
        float m_fTimerBucket = 0;

        // FPS
        int m_nFPS = 60;
        int m_nFrames = 0;
        float m_fFPSTimer = 0.0f;

        // Loading the Image variable
        int m_nParticleID = -1;
        
        //Mouse Hold
        bool isDown = false;
        int initialX;
        int initialY;
        int ImageWidth;
        int ImageHeight;
        //Location of Emitter
        Point Location = new Point(362, 285);

        //Objects
        Emitter emit = new Emitter();

        //Game Running Bool
        public bool Running
        {
            get;
            set;
        }

        public Form1()
        {
            Running = true;
            filename = "";
            associatedfilepath = "";
            associatedfilename = "";

            //ImageHeight = (int)numericUpDownHeight.Value;
           // ImageWidth = (int)numericUpDownWidth.Value;

            InitializeComponent();
            

            //Initializing Emitter 
            Random rand = new Random();

            emit.MaxParticle = numericUpDownMaxParticles.Value;

            emit.CurrRotation = 360;
            emit.EmitterPosX = Location.X;
            emit.EmitterPosY = Location.Y;
            emit.EmitterRangeX = (float)numericUpDownEmitterLineX.Value;
            emit.EmitterRangeY = (float)numericUpDownEmitterLineY.Value;
            emit.StartingColor = StartingColorbutton.BackColor;
            emit.EndingColor = EndingColorbutton.BackColor;
            emit.Size = new Size((int)numericUpDownWidth.Value, (int)numericUpDownHeight.Value);
            emit.MinLife = (float)numericUpDownLifeMin.Value;
            emit.MaxLife = (float)numericUpDownLifeMax.Value;
            emit.GravityX = (float)numericUpDownGravityX.Value;
            emit.GravityY = (float)numericUpDownGravityY.Value;
            emit.StartingRotation = (float)numericUpDownRotStarting.Value;
            emit.EndingRotation = (float)numericUpDownRotEnding.Value;
            emit.SpreadValue = numericUpDownSpreadValue.Value;
            emit.AngularOffset = numericUpDownAngularOffset.Value;
            emit.SpawnRate = (float)numericUpDownSpawnRate.Value;
            emit.StartingScaleX = (float)numericUpDownScaleXStarting.Value;
            emit.EndingScaleX = (float)numericUpDownScaleXEnding.Value;
            emit.StartingScaleY = (float)numericUpDownScaleYStarting.Value;
            emit.EndingScaleY = (float)numericUpDownScaleYEnding.Value;
            emit.StartingAccelerationX = (float)numericUpDownAccXStarting.Value;
            emit.EndingAccelerationX = (float)numericUpDownAccXEnding.Value;
            emit.StartingAccelerationY = (float)numericUpDownAccYStarting.Value;
            emit.EndingAccelerationY = (float)numericUpDownAccYEnding.Value;
            emit.StartingVelocityX = (float)numericUpDownVelXStarting.Value;
            emit.EndingVelocityX = (float)numericUpDownVelXEnding.Value;
            emit.StartingVelocityY = (float)numericUpDownVelYStarting.Value;
            emit.EndingVelocityY = (float)numericUpDownVelYEnding.Value;
            emit.Looping = checkBoxLooping.Checked;


            //Initializing Particles in Emitter
            for (int i = 0; i < emit.MaxParticle; ++i)
            {
                Particle particle = new Particle();

                //Initializing IsAlive Bool
                particle.Alive = false;

              
                particle.CurrLife = 0;

                float lifediff = emit.MaxLife - emit.MinLife;
                double n = rand.NextDouble();
                float curr = lifediff * (float)n;
                float random = curr + emit.MinLife;

                particle.MaxLife = random;
                //particle.MaxLife = rand.Next((int)emit.MinLife, (int)emit.MaxLife);
                //Initializing Emitter RangeX
                emit.EmitterRangeX = rand.Next(0, (int)numericUpDownEmitterLineX.Value);
                //Initializing Emitter RangeY
                emit.EmitterRangeY = rand.Next(0, (int)numericUpDownEmitterLineY.Value);

                //scale
                particle.CurrScaleX = emit.StartingScaleX;
                particle.CurrScaleY = emit.StartingScaleY;
                
                //Initializing CurrRotation
                 particle.CurrRotation = emit.StartingRotation;
                //particle.CurrRotation = rand.Next((int)emit.StartingRotation, (int)emit.EndingRotation);

                //Initializing CurrAccelerationX
                float accxdiff = emit.EndingAccelerationX - emit.StartingAccelerationX;
                double acx = rand.NextDouble();
                float curracx = accxdiff * (float)acx;
                float randomacx = curracx + emit.StartingAccelerationX;
                particle.CurrAccelerationX = randomacx;

                //Initializing CurrAccelerationY
                float accydiff = emit.EndingAccelerationY - emit.StartingAccelerationY;
                double acy = rand.NextDouble();
                float curracy = accydiff * (float)acy;
                float randomacy = curracy + emit.StartingAccelerationY;
                particle.CurrAccelerationY = randomacy;

                //Initializing CurrVelocityX
                float velxdiff = emit.EndingVelocityX - emit.StartingVelocityX;
                double velx = rand.NextDouble();
                float currvelx = velxdiff * (float)velx;
                float randomvelx = currvelx + emit.StartingVelocityX;
                particle.CurrVelocityX = randomvelx;

                //Initializing CurrVelocityY
                float velydiff = emit.EndingVelocityY - emit.StartingVelocityY;
                double vely = rand.NextDouble();
                float currvely = velydiff * (float)vely;
                float randomvely = currvely + emit.StartingVelocityY;
                particle.CurrVelocityY = randomvely;

                //Initializing ParticlePosX
                particle.ParticlePosX = emit.EmitterPosX;/*rand.Next( (int)emit.EmitterRangeX,(int)emit.EmitterPosX);*/ //emit.EmitterPosX;
                //Initializing ParticlePosY
                particle.ParticlePosY = emit.EmitterPosY;/*rand.Next((int)emit.EmitterRangeY,(int)emit.EmitterPosY );*/ //emit.EmitterPosY;

                //Initializing CurrColor            
                particle.CurrColor = emit.StartingColor;

                //Adding the Particle information into the Emitter's Particle List
                emit.ParticleList.Add(particle);
            }

            this.Focus();
        }


        public void Initialize()
        {
            // Access the SGP Wrapper singletons
            m_D3D = SGP.CSGP_Direct3D.GetInstance();
            m_TM = SGP.CSGP_TextureManager.GetInstance();
            m_D3D.Initialize(ParticlePanel, false);

            m_TM.Initialize(m_D3D.Device, m_D3D.Sprite);

            // success.
            Running = true;
        }




        public void Update(float fElapsedTime)
        {
            // Update global timer
            if (m_nParticleID != -1)
            {
                m_fTimer += fElapsedTime;
                m_fTimerBucket += fElapsedTime;
            }


            Random rand = new Random();
            for (int i = 0; i < emit.MaxParticle; ++i)
            {
                if (emit.Looping == true)
                {
                    if (m_fTimerBucket >= emit.SpawnRate)
                    {
                       
                        if (emit.ParticleList[i].Alive == false)            //if looping is true, and timer bucket is greater than spawn rate,                                                                        
                        {                                                   //will loop until the first particle found in the list is dead, then 
                            emit.ParticleList[i].Alive = true;              //It will turn the particle Alive and reset the Timer Bucket to 0, and break
                            m_fTimerBucket = 0;                             //As Soon as it turns the Particle Alive.
                            continue;
                        }

                    }
                }

                emit.ParticleList[i].CurrLife = emit.ParticleList[i].CurrLife + fElapsedTime;
 
                if (emit.ParticleList[i].CurrLife >=emit.ParticleList[i].MaxLife)
                {                                                            //if the partcile's CurrLife is greater than the max life, 
                    emit.ParticleList[i].CurrLife = emit.ParticleList[i].MaxLife;            //The Current Particle's Life will be set to it's Max Life and 
                    emit.ParticleList[i].Alive = false;                      //will be considered Dead.
                }
               
               
                if (emit.ParticleList[i].Alive == true)                                             //If Particle is Alive, Adjust the Current Variables over time.
                {
                    //cur = (end - start)*(curlife/maxlife)+start;  Linear Interpolation

                    //V = V + A * dt;
                    //alter velocity
                    emit.ParticleList[i].CurrVelocityX = emit.ParticleList[i].CurrVelocityX + emit.ParticleList[i].CurrAccelerationX * fElapsedTime;
                    emit.ParticleList[i].CurrVelocityY = emit.ParticleList[i].CurrVelocityY + emit.ParticleList[i].CurrAccelerationY * fElapsedTime;

                    //Point offset = new Point(0,0);
                    //offset.X -= (int)emit.EmitterRangeX;
                    //offset.Y -= (int)emit.EmitterRangeY;

                    //P = P + V * dt;
                    //alter position
                    emit.ParticleList[i].ParticlePosX = emit.ParticleList[i].ParticlePosX + (emit.ParticleList[i].CurrVelocityX + emit.GravityX) * fElapsedTime;
                    emit.ParticleList[i].ParticlePosY = emit.ParticleList[i].ParticlePosY + (emit.ParticleList[i].CurrVelocityY - emit.GravityY) * fElapsedTime;


                    //alter rotation
                    emit.ParticleList[i].CurrRotation = (emit.EndingRotation - emit.StartingRotation) * (emit.ParticleList[i].CurrLife / emit.ParticleList[i].MaxLife) + emit.StartingRotation;
                   
                    //alter scale
                    emit.ParticleList[i].CurrScaleX = (emit.EndingScaleX - emit.StartingScaleX) * (emit.ParticleList[i].CurrLife / emit.ParticleList[i].MaxLife) + emit.StartingScaleX;
                    emit.ParticleList[i].CurrScaleY = (emit.EndingScaleY - emit.StartingScaleY) * (emit.ParticleList[i].CurrLife / emit.ParticleList[i].MaxLife) + emit.StartingScaleY;
                    
                    //alter Color
                    float r, g, b;
                    r = (emit.EndingColor.R - emit.StartingColor.R) * (emit.ParticleList[i].CurrLife / emit.ParticleList[i].MaxLife) + emit.StartingColor.R;
                    g = (emit.EndingColor.G - emit.StartingColor.G) * (emit.ParticleList[i].CurrLife / emit.ParticleList[i].MaxLife) + emit.StartingColor.G;
                    b = (emit.EndingColor.B - emit.StartingColor.B) * (emit.ParticleList[i].CurrLife / emit.ParticleList[i].MaxLife) + emit.StartingColor.B;
                    if (r <= 0) r = 0;
                    if (g <= 0) g = 0;
                    if (b <= 0) b = 0;
                    if (r >= 255) r = 255;
                    if (g >= 255) g = 255;
                    if (b >= 255) b = 255;

                    emit.ParticleList[i].CurrColor = Color.FromArgb((int)r, (int)g, (int)b);


                }
                else
                {                                                                                   //Other wise adjust the Dead Particle's Variables
                    emit.ParticleList[i].Alive = false;

                    //adjust life random
                        emit.ParticleList[i].CurrLife = 0;
                    
                      float lifediff = emit.MaxLife - emit.MinLife;
                      double n = rand.NextDouble();
                      float curr = lifediff * (float)n;
                      float random = curr + emit.MinLife;
                      emit.ParticleList[i].MaxLife = random;

                      //scale
                      emit.ParticleList[i].CurrScaleX = emit.StartingScaleX;
                      emit.ParticleList[i].CurrScaleY = emit.StartingScaleY;

                    //adjust Rotation
                      if (emit.StartingRotation > emit.EndingRotation)
                      {
                          float rotdiff = emit.StartingRotation - emit.EndingRotation;
                          double r = rand.NextDouble();
                          float currrot = rotdiff * (float)r;
                          float randomr = currrot + emit.EndingRotation;
                          emit.ParticleList[i].CurrRotation = randomr;

                          //emit.ParticleList[i].CurrRotation = (int)rand.Next((int)emit.EndingRotation, (int)emit.StartingRotation);
                      }
                      else
                      {
                          float rotdiff = emit.EndingRotation - emit.StartingRotation;
                          double r = rand.NextDouble();
                          float currrot = rotdiff * (float)r;
                          float randomr = currrot + emit.StartingRotation;
                          emit.ParticleList[i].CurrRotation = randomr;

                          //emit.ParticleList[i].CurrRotation = (int)rand.Next((int)emit.StartingRotation, (int)emit.EndingRotation);
                      }

                    ////adjust scaleX
                    //if ((int)emit.StartingScaleX > (int)emit.EndingScaleX)
                    //{
                    //    emit.ParticleList[i].CurrScaleX = (int)rand.Next((int)emit.EndingScaleX, (int)emit.StartingScaleX);
                    //}
                    //else
                    //{
                    //    emit.ParticleList[i].CurrScaleX = (int)rand.Next((int)emit.StartingScaleX, (int)emit.EndingScaleX);
                    //}
                    // //adjust scaleY
                    //if ((int)emit.StartingScaleY > (int)emit.EndingScaleY)
                    //{
                    //    emit.ParticleList[i].CurrScaleY = (int)rand.Next((int)emit.EndingScaleY, (int)emit.StartingScaleY);
                    //}
                    //else
                    //{
                    //    emit.ParticleList[i].CurrScaleY = (int)rand.Next((int)emit.StartingScaleY, (int)emit.EndingScaleY);
                    //}

                    //adjust accelerationX
                    if (emit.StartingAccelerationX > emit.EndingAccelerationX)
                    {
                        float accxdiff = emit.StartingAccelerationX - emit.EndingAccelerationX;
                        double acx = rand.NextDouble();
                        float curracx = accxdiff * (float)acx;
                        float randomacx = curracx + emit.StartingAccelerationX;
                        emit.ParticleList[i].CurrAccelerationX = randomacx;
                    }
                    else
                    {
                        float accxdiff = emit.EndingAccelerationX - emit.StartingAccelerationX ;
                        double acx = rand.NextDouble();
                        float curracx = accxdiff * (float)acx;
                        float randomacx = curracx + emit.StartingAccelerationX;
                        emit.ParticleList[i].CurrAccelerationX = randomacx;
                    }

                    //adjust accelerationY
                    if (emit.StartingAccelerationY > emit.EndingAccelerationY)
                    {
                        float accydiff = emit.StartingAccelerationY - emit.EndingAccelerationY;
                        double acy = rand.NextDouble();
                        float curracy = accydiff * (float)acy;
                        float randomacy = curracy + emit.StartingAccelerationY;

                        emit.ParticleList[i].CurrAccelerationY = randomacy;
                    }
                    else
                    {
                        float accydiff = emit.EndingAccelerationY - emit.StartingAccelerationY;
                        double acy = rand.NextDouble();
                        float curracy = accydiff * (float)acy;
                        float randomacy = curracy + emit.StartingAccelerationY;
                        emit.ParticleList[i].CurrAccelerationY = randomacy;
                    }
                        
                    //adjust VelX
                    if (emit.StartingVelocityX > emit.EndingVelocityX)
                    {
                        float velxdiff = emit.StartingVelocityX - emit.EndingVelocityX;
                        double velx = rand.NextDouble();
                        float currvelx = velxdiff * (float)velx;
                        float randomvelx = currvelx + emit.StartingVelocityX;
                        emit.ParticleList[i].CurrVelocityX = randomvelx;
                    }
                    else
                    {
                        float velxdiff = emit.EndingVelocityX - emit.StartingVelocityX;
                        double velx = rand.NextDouble();
                        float currvelx = velxdiff * (float)velx;
                        float randomvelx = currvelx + emit.StartingVelocityX;
                        emit.ParticleList[i].CurrVelocityX = randomvelx;
                    }
                    //adjust VelY
                    if (emit.StartingVelocityY > emit.EndingVelocityY)
                    {
                        float velydiff = emit.StartingVelocityY - emit.EndingVelocityY;
                        double vely = rand.NextDouble();
                        float currvely = velydiff * (float)vely;
                        float randomvely = currvely + emit.StartingVelocityY;
                        emit.ParticleList[i].CurrVelocityY = randomvely;
                    }
                    else
                    {
                        float velydiff = emit.EndingVelocityY - emit.StartingVelocityY ;
                        double vely = rand.NextDouble();
                        float currvely = velydiff * (float)vely;
                        float randomvely = currvely + emit.StartingVelocityY;
                        emit.ParticleList[i].CurrVelocityY = randomvely;
                    }



                    if (emit.EmitterPosY <= 0)
                        emit.EmitterPosY = 0;
                    if (emit.EmitterPosX <= 0)
                        emit.EmitterPosX = 0;


                    float posxdiff = (emit.EmitterPosX + emit.EmitterRangeX) - emit.EmitterPosX;
                    double posx = rand.NextDouble();
                    float currposx = posxdiff * (float)posx;
                    float randomposx = currposx + emit.EmitterPosX;
                    //adjust posX
                    emit.ParticleList[i].ParticlePosX = randomposx;


                   // if ((int)emit.EmitterRangeX > (int)emit.EmitterPosX)
                   //     emit.ParticleList[i].ParticlePosX = rand.Next((int)emit.EmitterPosX, (int)emit.EmitterRangeX);
                  //  else
                   //     emit.ParticleList[i].ParticlePosX = rand.Next((int)emit.EmitterRangeX, (int)emit.EmitterPosX);


                    float posydiff = (emit.EmitterPosY + emit.EmitterRangeY) - emit.EmitterPosY;
                    double posy = rand.NextDouble();
                    float currposy = posydiff * (float)posy;
                    float randomposy = currposy + emit.EmitterPosY;

                    //adjust posY
                    emit.ParticleList[i].ParticlePosY = randomposy;


                   





                    //adjust Color
                    emit.ParticleList[i].CurrColor = emit.StartingColor;

                    //Random rand = new Random();
                    //if ((int)emit.EmitterRangeX > (int)emit.EmitterPosX)
                    //    emit.EmitterPosX = rand.Next((int)emit.EmitterPosX, (int)emit.EmitterRangeX);
                    //else
                    //    emit.EmitterPosX = rand.Next((int)emit.EmitterRangeX, (int)emit.EmitterPosX);
                }

            }

            // Update FPS
            if (m_nParticleID != -1)
            {
                ++m_nFrames;
                m_fFPSTimer += fElapsedTime;
                if (m_fFPSTimer > 1.0f)	// refresh every second
                {
                    m_nFPS = m_nFrames;
                    m_nFrames = 0;
                    m_fFPSTimer = 0.0f;

                   // labelparticle.Text = emit.MaxParticle.ToString();

                }
            }


        }









        public void Render()
        {
            // Clear the left render target
            m_D3D.Clear(ParticlePanel, Color.Black);

            // Begin rendering
            m_D3D.DeviceBegin();
            m_D3D.SpriteBegin();
            {

                for (int i = 0; i < emit.MaxParticle; ++i)
                {
                    if (emit.ParticleList[i].Alive == true)
                    {


                        if (m_nParticleID != -1) // If ParticleID is Holding on to a Image.
                        {
                            Rectangle rect = new Rectangle(0, 0, emit.Size.Width, emit.Size.Height);

                            m_TM.Draw
                                (
                                m_nParticleID,                                                                                                  //Image
                                (int)(emit.ParticleList[i].ParticlePosX - emit.Size.Width / 2.0f * emit.ParticleList[i].CurrScaleX),       //PositionX
                                (int)(emit.ParticleList[i].ParticlePosY - emit.Size.Height / 2.0f * emit.ParticleList[i].CurrScaleY),      //PositionY
                                emit.ParticleList[i].CurrScaleX,                                                                                //ScaleX
                                emit.ParticleList[i].CurrScaleY,                                                                                //Scaley
                                rect,                                                                                                           //rect -> Size
                                emit.Size.Width / 2 * emit.ParticleList[i].CurrScaleX,                                                          //RotationCenterX
                                emit.Size.Height / 2 * emit.ParticleList[i].CurrScaleY,                                                         //RotationCenterY
                                emit.ParticleList[i].CurrRotation,                                                                              //Rotation
                                emit.ParticleList[i].CurrColor                                                                                  //Color
                                );                                                                               
                        }
                    }
                }


                // Draw the application time (in milliseconds with 2 precision) in the text box
                txtTime.Text = ((int)(m_fTimer * 100) / 100.0f).ToString();
                // Draw the fps in the text box
                txtFPS.Text = m_nFPS.ToString();
            }
            // End rendering
            m_D3D.SpriteEnd();
            m_D3D.DeviceEnd();
            m_D3D.Present();
        }

        public void Terminate()
        {
            if (m_nParticleID != -1)
            {
                m_TM.UnloadTexture(m_nParticleID);
                m_nParticleID = -1;
            }

            // Shut down the wrappers
            m_TM.Terminate();
            m_D3D.Terminate();
        }

        private void Form1_FormClosing(object sender, FormClosingEventArgs e)
        {
            Running = false;
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            this.Close();
        }

        private void StartingColorbutton_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            dlg.Color = StartingColorbutton.BackColor;
            if (DialogResult.OK == dlg.ShowDialog())
            {
                StartingColorbutton.BackColor = dlg.Color;
                emit.StartingColor = StartingColorbutton.BackColor;
            }
        }

        private void EndingColorbutton_Click(object sender, EventArgs e)
        {
            ColorDialog dlg = new ColorDialog();
            dlg.Color = EndingColorbutton.BackColor;
            if (DialogResult.OK == dlg.ShowDialog())
            {
                EndingColorbutton.BackColor = dlg.Color;
                emit.EndingColor = EndingColorbutton.BackColor;
            }
        }
        private void loadImage_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (DialogResult.OK == dlg.ShowDialog())
            {
                // putting the directory into the programms settings and saving it so it can be loaded the next time the programm runs.
                associatedfilepath = dlg.FileName.Replace(dlg.SafeFileName, "");
                // Properties.Settings.Default.AssociatedFileDirectory = associatedfilepath;
                Properties.Settings.Default.Save();

                //gets the filename without the directory
                associatedfilename = dlg.SafeFileName;

                if (m_nParticleID == -1)
                {
                    m_nParticleID = m_TM.LoadTexture(dlg.FileName);
                   
                }
                else
                {
                    m_TM.UnloadTexture(m_nParticleID);
                   
                    m_nParticleID = m_TM.LoadTexture(dlg.FileName);
                }
               
                
                emit.ImageID = "Resources/Graphics/"+ associatedfilename;
                //emit.ImageID = associatedfilename;


            }
        }
        private void loadEmitterToolStripMenuItem_Click(object sender, EventArgs e)
        {
             System.Windows.Forms.OpenFileDialog dlg = new OpenFileDialog();

             dlg.Filter = "All Files|*.*|Xml Files(*.xml)|*.xml";
             dlg.FilterIndex = 2;

             if (DialogResult.OK == dlg.ShowDialog())
             {
                 XElement xRoot = XElement.Load(dlg.FileName);
                 XElement xEmitter = xRoot.Element("Emitter");



                 XAttribute xImageID = xEmitter.Attribute("ImageID");
                 if (m_nParticleID == -1)
                 {
                     emit.ImageID = Convert.ToString(xImageID.Value);
                     m_nParticleID = m_TM.LoadTexture(emit.ImageID);
                 }
                 else
                 {
                     m_TM.UnloadTexture(m_nParticleID);
                     emit.ImageID = Convert.ToString(xImageID.Value);
                     m_nParticleID = m_TM.LoadTexture(emit.ImageID);
                 }

                 //Position
                 XAttribute xEmitterPosX = xEmitter.Attribute("EmitterPosX");
                 double emitterposx = Convert.ToDouble(xEmitterPosX.Value);
                 emit.EmitterPosX = (float)emitterposx;

                 XAttribute xEmitterPosY = xEmitter.Attribute("EmitterPosY");
                 double emitterposy = Convert.ToDouble(xEmitterPosY.Value);
                 emit.EmitterPosY = (float)emitterposy;

                 //Size
                 XAttribute xWidth = xEmitter.Attribute("Width");
                 int width = Convert.ToInt32(xWidth.Value);
                 //emit.Size.Width = (int)width;
                 numericUpDownWidth.Value = width;

                 XAttribute xHeight = xEmitter.Attribute("Height");
                 int height = Convert.ToInt32(xHeight.Value);
                 //emit.Size.Height = (int)height;
                 emit.Size = new Size(width, height);
                 numericUpDownHeight.Value = height;

                 //Life
                 XAttribute xMinLife = xEmitter.Attribute("MinLife");
                 double minlife = Convert.ToDouble(xMinLife.Value);
                 emit.MinLife = (float)minlife;
                 numericUpDownLifeMin.Value = (decimal)emit.MinLife;

                 XAttribute xMaxLife = xEmitter.Attribute("MaxLife");
                 double maxlife = Convert.ToDouble(xMaxLife.Value);
                 emit.MaxLife = (float)maxlife;
                 numericUpDownLifeMax.Value = (decimal)emit.MaxLife;

                 //Gravity
                 XAttribute xGravityX = xEmitter.Attribute("GravityX");
                 double gravityx = Convert.ToDouble(xGravityX.Value);
                 emit.GravityX = (float)gravityx;
                 trackBarGravityX.Value = (int)gravityx;
                 numericUpDownGravityX.Value = trackBarGravityX.Value;

                 XAttribute xGravityY = xEmitter.Attribute("GravityY");
                 double gravityy = Convert.ToDouble(xGravityY.Value);
                 emit.GravityY = (float)gravityy;
                 trackBarGravityY.Value = (int)gravityy;
                 numericUpDownGravityY.Value = trackBarGravityY.Value;

                 //Rotation - doubting
                 XAttribute xCurrRotation = xEmitter.Attribute("CurrRotation");
                 double currrotation = Convert.ToDouble(xCurrRotation.Value);
                 emit.CurrRotation = (decimal)currrotation;


                 //StartingRotation
                 XAttribute xStartingRotation = xEmitter.Attribute("StartingRotation");
                 double startingrotation = Convert.ToDouble(xStartingRotation.Value);
                 emit.StartingRotation = (float)startingrotation;
                 numericUpDownRotStarting.Value = (decimal)emit.StartingRotation;

                 //EndingRotation
                 XAttribute xEndingRotation = xEmitter.Attribute("EndingRotation");
                 double endingrotation = Convert.ToDouble(xEndingRotation.Value);
                 emit.EndingRotation = (float)endingrotation;
                 numericUpDownRotEnding.Value = (decimal)emit.EndingRotation;

                 //SpreadValue - doubting
                 XAttribute xSpreadValue = xEmitter.Attribute("SpreadValue");
                 double spreadvalue = Convert.ToDouble(xSpreadValue.Value);
                 emit.SpreadValue = (decimal)spreadvalue;
                 numericUpDownSpreadValue.Value = (decimal)emit.SpreadValue;

                 //AngularOffset
                 XAttribute xAngularOffset = xEmitter.Attribute("AngularOffset");
                 double angularoffset = Convert.ToDouble(xAngularOffset.Value);
                 emit.AngularOffset = (decimal)angularoffset;
                 numericUpDownAngularOffset.Value = (decimal)emit.AngularOffset;

                 //StartingAlpha
                 XAttribute xStartingAlpha = xEmitter.Attribute("StartingAlpha");
                 double startingalpha = Convert.ToDouble(xStartingAlpha.Value);
                 //emit.StartingColor.A = (byte)startingalpha;

                 //EndingAlpha
                 XAttribute xEndingAlpha = xEmitter.Attribute("EndingAlpha");
                 double endingalpha = Convert.ToDouble(xEndingAlpha.Value);
                 //emit.EndingColor.A = (byte)endingalpha;

                 //StartingColorR
                 XAttribute xStartingColorR = xEmitter.Attribute("StartingColorR");
                 double startingcolorR = Convert.ToDouble(xStartingColorR.Value);
                 //emit.StartingColor.R = (byte)startingcolorR;

                 //EndingColorR
                 XAttribute xEndingColorR = xEmitter.Attribute("EndingColorR");
                 double endingcolorR = Convert.ToDouble(xEndingColorR.Value);
                 //emit.EndingColor.R = (byte)endingcolorR;

                 //StartingColorG
                 XAttribute xStartingColorG = xEmitter.Attribute("StartingColorG");
                 double startingcolorG = Convert.ToDouble(xStartingColorG.Value);
                 //emit.StartingColor.G = (byte)startingcolorG;

                 //EndingColorG
                 XAttribute xEndingColorG = xEmitter.Attribute("EndingColorG");
                 double endingcolorG = Convert.ToDouble(xEndingColorG.Value);
                 //emit.EndingColor.G = (byte)endingcolorG;

                 //StartingColorB
                 XAttribute xStartingColorB = xEmitter.Attribute("StartingColorB");
                 double startingcolorB = Convert.ToDouble(xStartingColorB.Value);
                 //emit.StartingColor.B = (byte)startingcolorG;

                 //EndingColorB
                 XAttribute xEndingColorB = xEmitter.Attribute("EndingColorB");
                 double endingcolorB = Convert.ToDouble(xEndingColorB.Value);
                 //emit.EndingColor.B = (byte)endingcolorB;

                 emit.StartingColor = Color.FromArgb((int)startingalpha, (int)startingcolorR, (int)startingcolorG, (int)startingcolorB);
                 emit.EndingColor = Color.FromArgb((int)endingalpha, (int)endingcolorR, (int)endingcolorG, (int)endingcolorB);

                 StartingColorbutton.BackColor = Color.FromArgb((int)startingalpha, (int)startingcolorR, (int)startingcolorG, (int)startingcolorB);
                 EndingColorbutton.BackColor = Color.FromArgb((int)endingalpha, (int)endingcolorR, (int)endingcolorG, (int)endingcolorB);

                 //Looping
                 XAttribute xLooping = xEmitter.Attribute("Looping");
                 bool looping = Convert.ToBoolean(xLooping.Value);
                 emit.Looping = looping;
                 checkBoxLooping.Checked = looping;

                 //SpawnRate
                 XAttribute xSpawnRate = xEmitter.Attribute("SpawnRate");
                 double spawnrate = Convert.ToDouble(xSpawnRate.Value);
                 emit.SpawnRate = (float)spawnrate;
                 numericUpDownSpawnRate.Value = (decimal)spawnrate;

                 //MaxParticle
                 XAttribute xMaxParticle = xEmitter.Attribute("MaxParticle");
                 int maxparticle = Convert.ToInt32(xMaxParticle.Value);
                 emit.MaxParticle = (decimal)maxparticle;
                 numericUpDownMaxParticles.Value = emit.MaxParticle;

                 //StartingAccelerationX
                 XAttribute xStartingAccelerationX = xEmitter.Attribute("StartingAccelerationX");
                 double startingaccelerationX = Convert.ToDouble(xStartingAccelerationX.Value);
                 emit.StartingAccelerationX = (float)startingaccelerationX;
                 numericUpDownAccXStarting.Value = (decimal)emit.StartingAccelerationX;
                 trackBarStartingAccX.Value = (int)startingaccelerationX;

                 //EndingAccelerationX
                 XAttribute xEndingAccelerationX = xEmitter.Attribute("EndingAccelerationX");
                 double endingaccelerationX = Convert.ToDouble(xEndingAccelerationX.Value);
                 emit.EndingAccelerationX = (float)endingaccelerationX;
                 numericUpDownAccXEnding.Value = (decimal)emit.EndingAccelerationX;
                 trackBarEndingAccX.Value = (int)endingaccelerationX;

                 //StartingAccelerationY
                 XAttribute xStartingAccelerationY = xEmitter.Attribute("StartingAccelerationY");
                 double startingaccelerationY = Convert.ToDouble(xStartingAccelerationY.Value);
                 emit.StartingAccelerationY = (float)startingaccelerationY;
                 numericUpDownAccYStarting.Value = (decimal)emit.StartingAccelerationY;
                 trackBarStartingAccY.Value = (int)startingaccelerationY;

                 //EndingAccelerationY
                 XAttribute xEndingAccelerationY = xEmitter.Attribute("EndingAccelerationX");
                 double endingaccelerationY = Convert.ToDouble(xEndingAccelerationY.Value);
                 emit.EndingAccelerationY = (float)endingaccelerationY;
                 numericUpDownAccYEnding.Value = (decimal)emit.EndingAccelerationY;
                 trackBarEndingAccY.Value = (int)endingaccelerationY;

                 //StartingVelocityX
                 XAttribute xStartingVelocityX = xEmitter.Attribute("StartingVelocityX");
                 double startingvelocityX = Convert.ToDouble(xStartingVelocityX.Value);
                 emit.StartingVelocityX = (float)startingvelocityX;
                 numericUpDownVelXStarting.Value = (decimal)emit.StartingVelocityX;
                 trackBarStartingVelX.Value = (int)startingvelocityX;

                 //EndingVelocityX
                 XAttribute xEndingVelocityX = xEmitter.Attribute("EndingVelocityX");
                 double endingvelocityX = Convert.ToDouble(xEndingVelocityX.Value);
                 emit.EndingVelocityX = (float)endingvelocityX;
                 numericUpDownVelXEnding.Value = (decimal)emit.EndingVelocityX;
                 trackBarEndingVelX.Value = (int)endingvelocityX;

                 //StartingVelocityY
                 XAttribute xStartingVelocityY = xEmitter.Attribute("StartingVelocityY");
                 double startingvelocityY = Convert.ToDouble(xStartingVelocityY.Value);
                 emit.StartingVelocityY = (float)startingvelocityY;
                 numericUpDownVelYStarting.Value = (decimal)emit.StartingVelocityY;

                 //EndingVelocityY
                 XAttribute xEndingVelocityY = xEmitter.Attribute("EndingVelocityY");
                 double endingvelocityY = Convert.ToDouble(xEndingVelocityY.Value);
                 emit.EndingVelocityY = (float)endingvelocityY;
                 numericUpDownVelYEnding.Value = (decimal)emit.EndingVelocityY;
                 trackBarEndingVelY.Value = (int)endingvelocityY;

                 //StartingScaleX
                 XAttribute xStartingScaleX = xEmitter.Attribute("StartingScaleX");
                 double startingscaleX = Convert.ToDouble(xStartingScaleX.Value);
                 emit.StartingScaleX = (float)startingscaleX;
                 numericUpDownScaleXStarting.Value = (decimal)emit.StartingScaleX;

                 //EndingScaleX
                 XAttribute xEndingScaleX = xEmitter.Attribute("EndingScaleX");
                 double endingscaleX = Convert.ToDouble(xEndingScaleX.Value);
                 emit.EndingScaleX = (float)endingscaleX;
                 numericUpDownScaleXEnding.Value = (decimal)emit.EndingScaleX;

                 //StartingScaleY
                 XAttribute xStartingScaleY = xEmitter.Attribute("StartingScaleY");
                 double startingscaleY = Convert.ToDouble(xStartingScaleY.Value);
                 emit.StartingScaleY = (float)startingscaleY;
                 numericUpDownScaleYStarting.Value = (decimal)emit.StartingScaleY;

                 //EndingScaleY
                 XAttribute xEndingScaleY = xEmitter.Attribute("EndingScaleY");
                 double endingscaleY = Convert.ToDouble(xEndingScaleY.Value);
                 emit.EndingScaleY = (float)endingscaleY;
                 numericUpDownScaleYEnding.Value = (decimal)emit.EndingScaleY;

                 //EmitterRangeX
                 XAttribute xEmitterRangeX = xEmitter.Attribute("EmitterRangeX");
                 double emitterrangex = Convert.ToDouble(xEmitterRangeX.Value);
                 emit.EmitterRangeX = (float)emitterrangex;
                 numericUpDownEmitterLineX.Value = (decimal)emit.EmitterRangeX;
                 trackBarLineX.Value = (int)emitterrangex;

                 //EmitterRangeY
                 XAttribute xEmitterRangeY = xEmitter.Attribute("EmitterRangeY");
                 double emitterrangey = Convert.ToDouble(xEmitterRangeY.Value);
                 emit.EmitterRangeY = (float)emitterrangey;
                 numericUpDownEmitterLineY.Value = (decimal)emit.EmitterRangeY;
                 trackBarLineY.Value = (int)emitterrangey;


             }



        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            System.Windows.Forms.SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "XML Files | *.xml;*.XML";
            dlg.Title = "Save an example file";
            dlg.DefaultExt = "xml";
            dlg.AddExtension = true;

            if (dlg.ShowDialog() == DialogResult.OK)
            {
                filename = dlg.FileName;


                 SaveFile(filename);
            }
        }
        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (filename == "")
            {
                saveAsToolStripMenuItem_Click(sender, e);
            }
            else
            {
                SaveFile(filename);




            }
        }

        private void SaveFile(string _filename)
        {
            XmlTextWriter file = new XmlTextWriter(_filename, null);

            XElement xRoot = new XElement("Effect");

            XElement xEmitter = new XElement("Emitter");
            xRoot.Add(xEmitter);
            //ImageID
            XAttribute xImageID = new XAttribute("ImageID", emit.ImageID);
            xEmitter.Add(xImageID);
            //Position
            XAttribute xEmitterPosX = new XAttribute("EmitterPosX", emit.EmitterPosX);
            xEmitter.Add(xEmitterPosX);

            XAttribute xEmitterPosY = new XAttribute("EmitterPosY", emit.EmitterPosY);
            xEmitter.Add(xEmitterPosY);

            //Size
            XAttribute xWidth = new XAttribute("Width", emit.Size.Width);
            xEmitter.Add(xWidth);

            XAttribute xHeight = new XAttribute("Height", emit.Size.Height);
            xEmitter.Add(xHeight);

            //Life
            XAttribute xMinLife = new XAttribute("MinLife", emit.MinLife);
            xEmitter.Add(xMinLife);

            XAttribute xMaxLife = new XAttribute("MaxLife", emit.MaxLife);
            xEmitter.Add(xMaxLife);
            //Gravity
            XAttribute xGravityX = new XAttribute("GravityX", emit.GravityX);
            xEmitter.Add(xGravityX);

            XAttribute xGravityY = new XAttribute("GravityY", emit.GravityY);
            xEmitter.Add(xGravityY);

            //doubting CurrRotation
            XAttribute xCurrRotation = new XAttribute("CurrRotation", emit.CurrRotation);
            xEmitter.Add(xCurrRotation);

            //Rotation
            XAttribute xStartingRotation = new XAttribute("StartingRotation", emit.StartingRotation);
            xEmitter.Add(xStartingRotation);

            XAttribute xEndingRotation = new XAttribute("EndingRotation", emit.EndingRotation);
            xEmitter.Add(xEndingRotation);

            //doubting Spread Value
            XAttribute xSpreadValue = new XAttribute("SpreadValue", emit.SpreadValue);
            xEmitter.Add(xSpreadValue);

            //doubting AngularOffset
            XAttribute xAngularOffset = new XAttribute("AngularOffset", emit.AngularOffset);
            xEmitter.Add(xAngularOffset);

            //ColorAlpha
            XAttribute xStartingAlpha = new XAttribute("StartingAlpha", emit.StartingColor.A);
            xEmitter.Add(xStartingAlpha);

            XAttribute xEndingAlpha = new XAttribute("EndingAlpha", emit.EndingColor.A);
            xEmitter.Add(xEndingAlpha);

            //ColorR
            XAttribute xStartingColorR = new XAttribute("StartingColorR", emit.StartingColor.R);
            xEmitter.Add(xStartingColorR);

            XAttribute xEndingColorR = new XAttribute("EndingColorR", emit.EndingColor.R);
            xEmitter.Add(xEndingColorR);

            //ColorG
            XAttribute xStartingColorG = new XAttribute("StartingColorG", emit.StartingColor.G);
            xEmitter.Add(xStartingColorG);

            XAttribute xEndingColorG = new XAttribute("EndingColorG", emit.EndingColor.G);
            xEmitter.Add(xEndingColorG);

            //ColorB
            XAttribute xStartingColorB = new XAttribute("StartingColorB", emit.StartingColor.B);
            xEmitter.Add(xStartingColorB);

            XAttribute xEndingColorB = new XAttribute("EndingColorB", emit.EndingColor.B);
            xEmitter.Add(xEndingColorB);

            //Looping
            XAttribute xLooping = new XAttribute("Looping", emit.Looping);
            xEmitter.Add(xLooping);

            //SpawnRate
            XAttribute xSpawnRate = new XAttribute("SpawnRate", emit.SpawnRate);
            xEmitter.Add(xSpawnRate);

            //MaxParticle
            XAttribute xMaxParticle = new XAttribute("MaxParticle", emit.MaxParticle);
            xEmitter.Add(xMaxParticle);

            //AccelerationX
            XAttribute xStartingAccelerationX = new XAttribute("StartingAccelerationX", emit.StartingAccelerationX);
            xEmitter.Add(xStartingAccelerationX);

            XAttribute xEndingAccelerationX = new XAttribute("EndingAccelerationX", emit.EndingAccelerationX);
            xEmitter.Add(xEndingAccelerationX);

            //AccelerationY
            XAttribute xStartingAccelerationY = new XAttribute("StartingAccelerationY", emit.StartingAccelerationY);
            xEmitter.Add(xStartingAccelerationY);

            XAttribute xEndingAccelerationY = new XAttribute("EndingAccelerationY", emit.EndingAccelerationY);
            xEmitter.Add(xEndingAccelerationY);

            //VelocityX
            XAttribute xStartingVelocityX = new XAttribute("StartingVelocityX", emit.StartingVelocityX);
            xEmitter.Add(xStartingVelocityX);

            XAttribute xEndingVelocityX = new XAttribute("EndingVelocityX", emit.EndingVelocityX);
            xEmitter.Add(xEndingVelocityX);

            //VelocityY
            XAttribute xStartingVelocityY = new XAttribute("StartingVelocityY", emit.StartingVelocityY);
            xEmitter.Add(xStartingVelocityY);

            XAttribute xEndingVelocityY = new XAttribute("EndingVelocityY", emit.EndingVelocityY);
            xEmitter.Add(xEndingVelocityY);

            //ScaleX
            XAttribute xStartingScaleX = new XAttribute("StartingScaleX", emit.StartingScaleX);
            xEmitter.Add(xStartingScaleX);

            XAttribute xEndingScaleX = new XAttribute("EndingScaleX", emit.EndingScaleX);
            xEmitter.Add(xEndingScaleX);

            //ScaleY
            XAttribute xStartingScaleY = new XAttribute("StartingScaleY", emit.StartingScaleY);
            xEmitter.Add(xStartingScaleY);

            XAttribute xEndingScaleY = new XAttribute("EndingScaleY", emit.EndingScaleY);
            xEmitter.Add(xEndingScaleY);

            //LineX&Y
            XAttribute xEmitterRangeX = new XAttribute("EmitterRangeX", emit.EmitterRangeX);
            xEmitter.Add(xEmitterRangeX);

            XAttribute xEmitterRangeY = new XAttribute("EmitterRangeY", emit.EmitterRangeY);
            xEmitter.Add(xEmitterRangeY);



            //Square Size





            //Circle Radius











            //xRoot.Save(_filename);
            file.Close();
            xRoot.Save(_filename);
        }

      
        private void ParticlePanel_MouseDown(object sender, MouseEventArgs e)
        {
            isDown = true;
            initialX = e.X;
            initialY = e.Y;
        }

        private void ParticlePanel_MouseMove(object sender, MouseEventArgs e)
        {
            toolStripStatusLabelX.Text = e.Location.X.ToString();
            toolStripStatusLabelY.Text = e.Location.Y.ToString();

            //toolStripStatusLabelX.Text = initialX.ToString();
            // toolStripStatusLabelY.Text = initialY.ToString();
            if (isDown == true)
            {

                
                emit.EmitterPosX = e.Location.X - ImageWidth / 2 ;
                emit.EmitterPosY = e.Location.Y - ImageHeight / 2;


            }

        }

        private void ParticlePanel_MouseUp(object sender, MouseEventArgs e)
        {
            isDown = false;

        }


        private void buttonReset_Click(object sender, EventArgs e)
        {
            //Position
            emit.EmitterPosX = 362;
            emit.EmitterPosY = 285;
            //EmitterRange
            emit.EmitterRangeX = 0;
            numericUpDownEmitterLineX.Value = 0;
            emit.EmitterRangeY = 0;
            numericUpDownEmitterLineY.Value = 0;

            //Emitter Size
            emit.Size = new Size(32,32);
            numericUpDownWidth.Value = 32;
            numericUpDownHeight.Value = 32;

            //Life
            emit.MinLife = 2;
            numericUpDownLifeMin.Value = 2;
            emit.MaxLife = 6;
            numericUpDownLifeMax.Value = 6;

            //Gravity
            emit.GravityX = 0;
            trackBarGravityX.Value = 0;
            numericUpDownGravityX.Value = 0;
            emit.GravityY = 0;
            trackBarGravityY.Value = 0;
            numericUpDownGravityY.Value = 0;
            //Rotation
            emit.StartingRotation = 1;
            numericUpDownRotStarting.Value = 1;
            emit.EndingRotation = 5;
            numericUpDownRotEnding.Value = 5;

            //SpreadValue
            emit.SpreadValue = 360;
            numericUpDownSpreadValue.Value = 360;

            //AngularOffset
            emit.AngularOffset = 0;
            numericUpDownAngularOffset.Value = 0;

            //SpawnRate
            emit.SpawnRate = 0.010f;
            numericUpDownSpawnRate.Value = (decimal)0.010f;

            //Max Particles
            emit.MaxParticle = 50;
            numericUpDownMaxParticles.Value = 50;

            //ScaleX
            emit.StartingScaleX = 1;
            numericUpDownScaleXStarting.Value = 1;
            emit.EndingScaleX = 3;
            numericUpDownScaleXEnding.Value = 3;

            //ScaleY
            emit.StartingScaleY = 1;
            numericUpDownScaleYStarting.Value = 1;
            emit.EndingScaleY = 3;
            numericUpDownScaleYEnding.Value = 3;

            //AccelerationX
            emit.StartingAccelerationX = -65;
            numericUpDownAccXStarting.Value = -65;
            emit.EndingAccelerationX = 65;
            numericUpDownAccXEnding.Value = 65;

            //AccelerationY
            emit.StartingAccelerationY = -65;
            numericUpDownAccYStarting.Value = -65;
            emit.EndingAccelerationY = 65;
            numericUpDownAccYEnding.Value = 65;

            //VelocityX
            emit.StartingVelocityX = -65;
            numericUpDownVelXStarting.Value = -65;
            emit.EndingVelocityX = 65;
            numericUpDownVelXEnding.Value = 65;

            //VelocityY
            emit.StartingVelocityY = -65;
            numericUpDownVelYStarting.Value = -65;
            emit.EndingVelocityY = 65;
            numericUpDownVelYEnding.Value = 65;

            //Looping
            emit.Looping = true;
            checkBoxLooping.Checked = true;

            //Color

            emit.StartingColor = Color.Yellow;
            emit.EndingColor = Color.Red;

            StartingColorbutton.BackColor = Color.Yellow;
            EndingColorbutton.BackColor = Color.Red;

            //TrackBars
            trackBarEndingAccX.Value = 65;
            trackBarStartingAccX.Value = -65;
            trackBarStartingAccY.Value = -65;
            trackBarEndingAccY.Value = 65;
            trackBarEndingVelX.Value = 65;
            trackBarEndingVelY.Value = 65;
            trackBarStartingVelX.Value = -65;
            trackBarStartingVelY.Value = -65;
            trackBarGravityX.Value = 0;
            trackBarGravityY.Value = 0;

        }














































































        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //numericUpDown and TrackBars

        private void numericUpDownEmitterLineX_ValueChanged(object sender, EventArgs e)
        {
            emit.EmitterRangeX = (float)numericUpDownEmitterLineX.Value;
        }
        private void numericUpDownEmitterLineY_ValueChanged(object sender, EventArgs e)
        {
            emit.EmitterRangeY = (float)numericUpDownEmitterLineY.Value;
        }
        private void numericUpDownEmitterSquareSize_ValueChanged(object sender, EventArgs e)
        {
           
        }
        private void numericUpDownEmitterCircleRadius_ValueChanged(object sender, EventArgs e)
        {

        }
        private void numericUpDownRotMin_ValueChanged(object sender, EventArgs e)
        {
            emit.StartingRotation = (float)numericUpDownRotStarting.Value;
        }
        private void numericUpDownRotMax_ValueChanged(object sender, EventArgs e)
        {
            emit.EndingRotation = (float)numericUpDownRotEnding.Value;
        }
        private void numericUpDownAccMinX_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAccXStarting.Value > numericUpDownAccXEnding.Value - (decimal)0.001f)
            {
                numericUpDownAccXStarting.Value = numericUpDownAccXEnding.Value - (decimal)0.001f;

            }
            trackBarStartingAccX.Value = (int)numericUpDownAccXStarting.Value;
            emit.StartingAccelerationX = (float)numericUpDownAccXStarting.Value;
        }
        private void numericUpDownAccMaxX_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAccXEnding.Value < numericUpDownAccXStarting.Value + (decimal)0.001f)
            {
                numericUpDownAccXEnding.Value = numericUpDownAccXStarting.Value + (decimal)0.001f;

            }


            trackBarEndingAccX.Value = (int)numericUpDownAccXEnding.Value;
            emit.EndingAccelerationX = (float)numericUpDownAccXEnding.Value;
        }
        private void numericUpDownAccMinY_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAccYStarting.Value > numericUpDownAccYEnding.Value - (decimal)0.001f)
            {
                numericUpDownAccYStarting.Value = numericUpDownAccYEnding.Value - (decimal)0.001f;

            }

            trackBarStartingAccY.Value = (int)numericUpDownAccYStarting.Value;
            emit.StartingAccelerationY = (float)numericUpDownAccYStarting.Value;
        }
        private void numericUpDownAccMaxY_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownAccYEnding.Value < numericUpDownAccYStarting.Value + (decimal)0.001f)
            {
                numericUpDownAccYEnding.Value = numericUpDownAccYStarting.Value + (decimal)0.001f;

            }

            trackBarEndingAccY.Value = (int)numericUpDownAccYEnding.Value;
            emit.EndingAccelerationY = (float)numericUpDownAccYEnding.Value;
        }
        private void numericUpDownVelXMin_ValueChanged(object sender, EventArgs e)
        {
            trackBarStartingVelX.Value = (int)numericUpDownVelXStarting.Value;
            emit.StartingVelocityX = (float)numericUpDownVelXStarting.Value;
        }
        private void numericUpDownVelXMax_ValueChanged(object sender, EventArgs e)
        {
            trackBarEndingVelX.Value = (int)numericUpDownVelXEnding.Value;
            emit.EndingVelocityX = (float)numericUpDownVelXEnding.Value;
        }
        private void numericUpDownVelYMin_ValueChanged(object sender, EventArgs e)
        {
            trackBarStartingVelY.Value = (int)numericUpDownVelYStarting.Value;
            emit.StartingVelocityY = (float)numericUpDownVelYStarting.Value;
        }

        private void numericUpDownVelYMax_ValueChanged(object sender, EventArgs e)
        {
            trackBarEndingVelY.Value = (int)numericUpDownVelYEnding.Value;
            emit.EndingVelocityY = (float)numericUpDownVelYEnding.Value;
        }
        private void numericUpDownScaleXMin_ValueChanged(object sender, EventArgs e)
        {
            emit.StartingScaleX = (float)numericUpDownScaleXStarting.Value;
        }
        private void numericUpDownScaleXMax_ValueChanged(object sender, EventArgs e)
        {
            emit.EndingScaleX = (float)numericUpDownScaleXEnding.Value;
        }
        private void numericUpDownScaleYMin_ValueChanged(object sender, EventArgs e)
        {
            emit.StartingScaleY = (float)numericUpDownScaleYStarting.Value;
        }
        private void numericUpDownScaleYMax_ValueChanged(object sender, EventArgs e)
        {
            emit.EndingScaleY = (float)numericUpDownScaleYEnding.Value;
        }

        private void checkBoxLooping_CheckedChanged(object sender, EventArgs e)
        {
            emit.Looping = checkBoxLooping.Checked;
        }
        private void numericUpDownSpawnRate_ValueChanged(object sender, EventArgs e)
        {
            emit.SpawnRate = (float)numericUpDownSpawnRate.Value;
        }
        private void numericUpDownWidth_ValueChanged(object sender, EventArgs e)
        {
            emit.Size = new Size((int)numericUpDownWidth.Value, (int)numericUpDownHeight.Value);
            ImageWidth = (int)numericUpDownWidth.Value;
        }

        private void numericUpDownHeight_ValueChanged(object sender, EventArgs e)
        {
            emit.Size = new Size((int)numericUpDownWidth.Value, (int)numericUpDownHeight.Value);
            ImageHeight = (int)numericUpDownHeight.Value;
        }

        private void numericUpDownLifeMin_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownLifeMin.Value > numericUpDownLifeMax.Value-(decimal)0.001f)
            {
                numericUpDownLifeMin.Value = numericUpDownLifeMax.Value - (decimal)0.001f;
           
            }
                emit.MinLife = (float)numericUpDownLifeMin.Value;
        }

        private void numericUpDownLifeMax_ValueChanged(object sender, EventArgs e)
        {
            if (numericUpDownLifeMax.Value < numericUpDownLifeMin.Value + (decimal)0.001f)
            {
                numericUpDownLifeMax.Value = numericUpDownLifeMin.Value + (decimal)0.001f;
               
            }

            emit.MaxLife = (float)numericUpDownLifeMax.Value;
        }

        private void trackBarStartingVelX_Scroll(object sender, EventArgs e)
        {

            numericUpDownVelXStarting.Value = trackBarStartingVelX.Value;
        }

        private void trackBarEndingVelX_Scroll(object sender, EventArgs e)
        {
            numericUpDownVelXEnding.Value = trackBarEndingVelX.Value;
        }

        private void trackBarStartingVelY_Scroll(object sender, EventArgs e)
        {
            numericUpDownVelYStarting.Value = trackBarStartingVelY.Value;

        }


        private void trackBarEndingVelY_Scroll(object sender, EventArgs e)
        {
            numericUpDownVelYEnding.Value = trackBarEndingVelY.Value;
        }

        private void trackBarStartingAccX_Scroll(object sender, EventArgs e)
        {
            numericUpDownAccXStarting.Value = trackBarStartingAccX.Value;
        }

        private void trackBarEndingAccX_Scroll(object sender, EventArgs e)
        {
            numericUpDownAccXEnding.Value = trackBarEndingAccX.Value;
        }

        private void trackBarStartingAccY_Scroll(object sender, EventArgs e)
        {
            numericUpDownAccYStarting.Value = trackBarStartingAccY.Value;
        }

        private void trackBarEndingAccY_Scroll(object sender, EventArgs e)
        {
            numericUpDownAccYEnding.Value = trackBarEndingAccY.Value;
        }

        private void trackBarLineX_Scroll(object sender, EventArgs e)
        {
            numericUpDownEmitterLineX.Value = trackBarLineX.Value;

        }

        private void trackBarLineY_Scroll(object sender, EventArgs e)
        {
            numericUpDownEmitterLineY.Value = trackBarLineY.Value;
           
        }

        private void trackBarEmitterSize_Scroll(object sender, EventArgs e)
        {
            numericUpDownEmitterSquareSize.Value = trackBarEmitterSize.Value;
        }

        private void trackBarRadius_Scroll(object sender, EventArgs e)
        {
            numericUpDownEmitterCircleRadius.Value = trackBarRadius.Value;
        }

        private void trackBarGravityX_Scroll(object sender, EventArgs e)
        {
            emit.GravityX = trackBarGravityX.Value;
            numericUpDownGravityX.Value = trackBarGravityX.Value;
        }

        private void trackBarGravityY_Scroll(object sender, EventArgs e)
        {
            emit.GravityY = trackBarGravityY.Value;
            numericUpDownGravityY.Value = trackBarGravityY.Value;
        }
        private void numericUpDownGravityX_ValueChanged(object sender, EventArgs e)
        {
            trackBarGravityX.Value = (int)numericUpDownGravityX.Value;
            emit.GravityX = (float)numericUpDownGravityX.Value;
        }
        private void numericUpDownGravityY_ValueChanged(object sender, EventArgs e)
        {
            trackBarGravityY.Value = (int)numericUpDownGravityY.Value;
            emit.GravityY = (float)numericUpDownGravityY.Value;
        }
        private void numericUpDownMaxParticles_ValueChanged(object sender, EventArgs e)
        {
            emit.MaxParticle = numericUpDownMaxParticles.Value;
            emit.ParticleList.Clear();
            //Initializing Emitter 
            Random rand = new Random();
            //Initializing Particles in Emitter
            for (int i = 0; i < emit.MaxParticle; ++i)
            {
                Particle particle = new Particle();

                //Initializing IsAlive Bool
                particle.Alive = false;

                particle.CurrLife = 0;
                //int minlife =  emit.MinLife * 100;
                float lifediff = emit.MaxLife - emit.MinLife;
                double n = rand.NextDouble();
                float curr = lifediff * (float)n;
                float random = curr + emit.MinLife;

                particle.MaxLife = random;
                //particle.MaxLife = rand.Next((int)emit.MinLife, (int)emit.MaxLife);

                particle.CurrScaleX = emit.StartingScaleX;
                particle.CurrScaleY = emit.StartingScaleY;

                //Initializing Emitter RangeX
                emit.EmitterRangeX = rand.Next(0, (int)numericUpDownEmitterLineX.Value);
                //Initializing Emitter RangeY
                emit.EmitterRangeY = rand.Next(0, (int)numericUpDownEmitterLineY.Value);

                //Initializing CurrRotation
                //float rotdiff = emit.EndingRotation - emit.StartingRotation;
                //double r = rand.NextDouble();
                //float currrot = rotdiff * (float)r;
                //float randomr = currrot + emit.StartingRotation;
                particle.CurrRotation = emit.StartingRotation;
                //particle.CurrRotation = rand.Next((int)emit.StartingRotation, (int)emit.EndingRotation);

                //Initializing CurrAccelerationX
                float accxdiff = emit.EndingAccelerationX - emit.StartingAccelerationX;
                double acx = rand.NextDouble();
                float curracx = accxdiff * (float)acx;
                float randomacx = curracx + emit.StartingAccelerationX;
                particle.CurrAccelerationX = randomacx;

                //Initializing CurrAccelerationY
                float accydiff = emit.EndingAccelerationY - emit.StartingAccelerationY;
                double acy = rand.NextDouble();
                float curracy = accydiff * (float)acy;
                float randomacy = curracy + emit.StartingAccelerationY;
                particle.CurrAccelerationY = randomacy;

                //Initializing CurrVelocityX
                float velxdiff = emit.EndingVelocityX - emit.StartingVelocityX;
                double velx = rand.NextDouble();
                float currvelx = velxdiff * (float)velx;
                float randomvelx = currvelx + emit.StartingVelocityX;
                particle.CurrVelocityX = randomvelx;

                //Initializing CurrVelocityY
                float velydiff = emit.EndingVelocityY - emit.StartingVelocityY;
                double vely = rand.NextDouble();
                float currvely = velydiff * (float)vely;
                float randomvely = currvely + emit.StartingVelocityY;
                particle.CurrVelocityY = randomvely;

                //Initializing ParticlePosX
                particle.ParticlePosX = emit.EmitterPosX;/*rand.Next( (int)emit.EmitterRangeX,(int)emit.EmitterPosX);*/ //emit.EmitterPosX;
                //Initializing ParticlePosY
                particle.ParticlePosY = emit.EmitterPosY;/*rand.Next((int)emit.EmitterRangeY,(int)emit.EmitterPosY );*/ //emit.EmitterPosY;

                //Initializing CurrColor            
                particle.CurrColor = emit.StartingColor;

                //Adding the Particle information into the Emitter's Particle List
                emit.ParticleList.Add(particle);
            }
        }

        private void setResourceDirectoryToolStripMenuItem_Click(object sender, EventArgs e)
        {

            //System.Windows.Forms.FolderBrowserDialog dlg = new FolderBrowserDialog();

            //if (Properties.Settings.Default.AssociatedFileDirectory != null)
            //    dlg.SelectedPath = Properties.Settings.Default.AssociatedFileDirectory;

            //if (dlg.ShowDialog() == DialogResult.OK)
            //{
            //    // putting the directory into the programms settings and saving it so it can be loaded the next time the programm runs.
            //    Properties.Settings.Default.AssociatedFileDirectory = dlg.SelectedPath;
            //    Properties.Settings.Default.Save();
            //    associatedfilepath = dlg.SelectedPath;
            //    FileDirectoryTextBox.Text = dlg.SelectedPath;
            //}
        }

        private void aboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            About.Show
                (
                "Team Name\nIron Clad\nCreated By:\nRoberto Crespo\nEmail:\nCresic95@fullsail.edu\nDate Created:\n10/18/2013",
                "About",
                "OK",
                "Cancel"
                );
        }

        private void instructionsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Instructions.Show
                (
                "Moving Particles -> Hold Down the Mouse Left Click to Move the Emitter Around.\n\nAdjusting Variables -> As you will notice, playing around with Scroll Bars and numbers will effect how the Particles\nwill spawn.",
                "Instructions",
                "OK",
                "Cancel"
                );
        }

       
       

       

       
    }
}
