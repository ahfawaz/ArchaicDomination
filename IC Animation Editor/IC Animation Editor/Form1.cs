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

namespace IC_Animation_Editor
{
    public partial class Form1 : Form
    {
        #region Globals
        SGP.CSGP_Direct3D pD3D = null;
        SGP.CSGP_TextureManager pTM = null;

        string fileName = null;
        string associatedFilePath = null;
        string associatedFileName = null;

        string SaveAsFile;
        string SpriteSheet;
        int SpriteSheetID;
        Rectangle currImageView;
        Rectangle imageView;

        Point OffSetMain = new Point(0, 0);
        Point OffSetList1 = new Point(0, 0);
        Point OffSetList2 = new Point(0, 0);
        int FrameSelected = -1;
        int AnimeSelected = -1;
        bool RenderChanged = false;
        bool ActiveChanged = false;
        bool CollisionChanged = false;
        bool PreviewPlay = false;

        enum UserMode { SelectMode, RenderMode, ActiveMode, CollisionMode, AnchorMode, };
        UserMode currMode = UserMode.SelectMode;

        Point beginRECT = new Point();
        Point endRECT = new Point();
        bool sizing = false;
        float fTimer = 0.0f;

        private bool active;
        public bool Active
        {
            get { return active; }
            set { active = value; }
        }

        List<Animations> AnimeList;
        Animations currAnime;
        Frame currFrame;
        #endregion

        public Form1()
        {
            InitializeComponent();
            this.Focus();

            currImageView = imageView = new Rectangle(0, 0, ImagePanel.Size.Width, ImagePanel.Size.Height);

        }

        public void Initialize()
        {
            pD3D = SGP.CSGP_Direct3D.GetInstance();
            pTM = SGP.CSGP_TextureManager.GetInstance();

            pD3D.Initialize(ImagePanel, false);
            pD3D.AddRenderTarget(PreviewPanel);
            pD3D.AddRenderTarget(AnimationListBox);
            pD3D.AddRenderTarget(FrameListBox);

            pTM.Initialize(pD3D.Device, pD3D.Sprite);

            SpriteSheetID = -1;

            currAnime = new Animations();
            currFrame = new Frame();
            AnimeList = new List<Animations>();

            currMode = UserMode.RenderMode;

            active = true;
        }

        public void Terminate()
        {
            if (SpriteSheetID != -1)
            {
                pTM.UnloadTexture(SpriteSheetID);
                SpriteSheetID = -1;
            }

            pTM.Terminate();
            pD3D.Terminate();
        }

        public void Update(float fElapsedTime)
        {
            fTimer += fElapsedTime;

            //will do voodoo magic here for the preview window else this is all i need;
            if (PreviewPlay)
            {
                fTimer += fElapsedTime;

                if (fTimer >= currFrame.Duration)
                {
                    if (FrameSelected + 1 > currAnime.NumOfFrames - 1 && isLoopingCheckBox.Checked)
                    {
                        FrameSelected = 0;
                        SelectedFrame();
                        fTimer = 0.0f;
                    }
                    else if (FrameSelected + 1 > currAnime.NumOfFrames - 1 && !isLoopingCheckBox.Checked)
                    {
                        PreviewPlay = false;
                        fTimer = 0.0f;
                    }
                    else
                    {
                        ++FrameSelected;
                        SelectedFrame();
                        fTimer = 0.0f;
                    }
                }
                return;
            }


            if (ImageScrollX.Value != OffSetMain.X || ImageScrollY.Value != OffSetMain.Y)
            {
                OffSetMain.X = ImageScrollX.Value;
                OffSetMain.Y = ImageScrollY.Value;
                currImageView = new Rectangle(imageView.Left + OffSetMain.X, imageView.Top + OffSetMain.Y, imageView.Right + OffSetMain.X, imageView.Bottom + OffSetMain.Y);
            }

            if (AnimeListScroll.Value != OffSetList1.Y)
            {
                OffSetList1.Y = AnimeListScroll.Value;
            }
            if (FrameListScroll.Value != OffSetList2.Y)
            {
                OffSetList2.Y = FrameListScroll.Value;
            }

            Point tempAnchor = new Point((int)AnchorXUpDown.Value, (int)AnchorYUpDown.Value);
            currFrame.SetAnchor(tempAnchor);

            if (RenderChanged)
            {
                Rectangle tempRender = new Rectangle();
                tempRender.X = (int)RenderLeft.Value;
                tempRender.Y = (int)RenderTop.Value;
                tempRender.Width = (int)(RenderRight.Value - RenderLeft.Value);
                tempRender.Height = (int)(RenderBottom.Value - RenderTop.Value);
                currFrame.SetFrameRender(tempRender);

                RenderChanged = false;
            }

            if (ActiveChanged)
            {
                Rectangle tempActive = new Rectangle();
                tempActive.X = (int)ActiveLeft.Value;
                tempActive.Y = (int)ActiveTop.Value;
                tempActive.Width = (int)(ActiveRight.Value - ActiveLeft.Value);
                tempActive.Height = (int)(ActiveBottom.Value - ActiveTop.Value);
                currFrame.SetFrameActive(tempActive);

                ActiveChanged = false;
            }

            if (CollisionChanged)
            {
                Rectangle tempCollision = new Rectangle();
                tempCollision.X = (int)CollisionLeft.Value;
                tempCollision.Y = (int)CollisionTop.Value;
                tempCollision.Width = (int)(CollisionRight.Value - CollisionLeft.Value);
                tempCollision.Height = (int)(CollisionBottom.Value - CollisionTop.Value);
                currFrame.SetFrameCollision(tempCollision);

                CollisionChanged = false;
            }

            if (FrameSelected != FrameNumberUpDown.Value && currAnime.Framelist.Count > 0)
            {
                if (PreviewPlay == false)
                {
                    if (FrameSelected < 0)
                        FrameSelected = 0;
                    if (FrameSelected > currAnime.NumOfFrames - 1)
                        FrameSelected = currAnime.NumOfFrames - 1;
                }

                FrameSelected = (int)FrameNumberUpDown.Value;
                SelectedFrame();
            }
        }

        void SelectedFrame()
        {
            if (currAnime.NumOfFrames > 0)
            {
                currFrame = currAnime.Framelist[FrameSelected];
                EventNameBox.Text = currFrame.EventName;
                Duration.Value = (decimal)currFrame.Duration;

                RenderLeft.Value = currFrame.RenderSource.Left;
                RenderTop.Value = currFrame.RenderSource.Top;
                RenderRight.Value = currFrame.RenderSource.Right;
                RenderBottom.Value = currFrame.RenderSource.Bottom;

                ActiveLeft.Value = currFrame.ActiveSource.Left;
                ActiveTop.Value = currFrame.ActiveSource.Top;
                ActiveRight.Value = currFrame.ActiveSource.Right;
                ActiveBottom.Value = currFrame.ActiveSource.Bottom;

                CollisionLeft.Value = currFrame.CollisionSource.Left;
                CollisionTop.Value = currFrame.CollisionSource.Top;
                CollisionRight.Value = currFrame.CollisionSource.Right;
                CollisionBottom.Value = currFrame.CollisionSource.Bottom;

                AnchorXUpDown.Value = currFrame.AnchorPoint.X;
                AnchorYUpDown.Value = currFrame.AnchorPoint.Y; 
            }
        }

        #region Rendering
        public void Render()
        {
            RenderMain();
            RenderPreview();
            RenderLists();
        }

        public void RenderMain()
        {
            pD3D.Clear(ImagePanel, Color.Magenta);

            pD3D.DeviceBegin();
            pD3D.SpriteBegin();

            //Begin Rendering stuff

            if (SpriteSheetID != -1)
            {
                pTM.Draw(SpriteSheetID, 0, 0, 1.0f, 1.0f, currImageView);
            }

            for (int index = 0; index < AnimeList.Count; index++)
            {
                for (int i = 0; i < AnimeList[index].NumOfFrames; i++)
                {
                    Rectangle temp = new Rectangle(AnimeList[index].Framelist[i].RenderSource.Left - OffSetMain.X, AnimeList[index].Framelist[i].RenderSource.Top - OffSetMain.Y, AnimeList[index].Framelist[i].RenderSource.Width, AnimeList[index].Framelist[i].RenderSource.Height);
                    pD3D.DrawHollowRect(temp, Color.Violet, 3);
                    pD3D.DrawRect(temp, Color.FromArgb(50, Color.Violet));


                    temp = new Rectangle(AnimeList[index].Framelist[i].ActiveSource.Left - OffSetMain.X, AnimeList[index].Framelist[i].ActiveSource.Top - OffSetMain.Y, AnimeList[index].Framelist[i].ActiveSource.Width, AnimeList[index].Framelist[i].ActiveSource.Height);
                    pD3D.DrawHollowRect(temp, Color.Yellow, 3);
                    pD3D.DrawRect(temp, Color.FromArgb(50, Color.Yellow));

                    temp = new Rectangle(AnimeList[index].Framelist[i].CollisionSource.Left - OffSetMain.X, AnimeList[index].Framelist[i].CollisionSource.Top - OffSetMain.Y, AnimeList[index].Framelist[i].CollisionSource.Width, AnimeList[index].Framelist[i].CollisionSource.Height);
                    pD3D.DrawHollowRect(temp, Color.Olive, 3);
                    pD3D.DrawRect(temp, Color.FromArgb(50, Color.Olive));

                    pD3D.DrawRect(new Rectangle(AnimeList[index].Framelist[i].AnchorPoint.X - OffSetMain.X, AnimeList[index].Framelist[i].AnchorPoint.Y - OffSetMain.Y, 5, 5), Color.Brown);
                }
            }

            for (int i = 0; i < currAnime.Framelist.Count; i++)
            {
                Rectangle temp = new Rectangle(currAnime.Framelist[i].RenderSource.Left - OffSetMain.X, currAnime.Framelist[i].RenderSource.Top - OffSetMain.Y, currAnime.Framelist[i].RenderSource.Width, currAnime.Framelist[i].RenderSource.Height);
                pD3D.DrawHollowRect(temp, Color.Cyan, 3);
                pD3D.DrawRect(temp, Color.FromArgb(50, Color.Cyan));


                temp = new Rectangle(currAnime.Framelist[i].ActiveSource.Left - OffSetMain.X, currAnime.Framelist[i].ActiveSource.Top - OffSetMain.Y, currAnime.Framelist[i].ActiveSource.Width, currAnime.Framelist[i].ActiveSource.Height);
                pD3D.DrawHollowRect(temp, Color.Orange, 3);
                pD3D.DrawRect(temp, Color.FromArgb(50, Color.Orange));

                temp = new Rectangle(currAnime.Framelist[i].CollisionSource.Left - OffSetMain.X, currAnime.Framelist[i].CollisionSource.Top - OffSetMain.Y, currAnime.Framelist[i].CollisionSource.Width, currAnime.Framelist[i].CollisionSource.Height);
                pD3D.DrawHollowRect(temp, Color.Lime, 3);
                pD3D.DrawRect(temp, Color.FromArgb(50, Color.Lime));

                pD3D.DrawRect(new Rectangle(currAnime.Framelist[i].AnchorPoint.X - OffSetMain.X, currAnime.Framelist[i].AnchorPoint.Y - OffSetMain.Y, 5, 5), Color.Gray);
            }

            Rectangle currTemp = new Rectangle(currFrame.RenderSource.Left - OffSetMain.X, currFrame.RenderSource.Top - OffSetMain.Y, currFrame.RenderSource.Width, currFrame.RenderSource.Height);
            pD3D.DrawHollowRect(currTemp, Color.Blue, 3);
            pD3D.DrawRect(currTemp, Color.FromArgb(50, Color.Blue));


            currTemp = new Rectangle(currFrame.ActiveSource.Left - OffSetMain.X, currFrame.ActiveSource.Top - OffSetMain.Y, currFrame.ActiveSource.Width, currFrame.ActiveSource.Height);
            pD3D.DrawHollowRect(currTemp, Color.Red, 3);
            pD3D.DrawRect(currTemp, Color.FromArgb(50, Color.Red));

            currTemp = new Rectangle(currFrame.CollisionSource.Left - OffSetMain.X, currFrame.CollisionSource.Top - OffSetMain.Y, currFrame.CollisionSource.Width, currFrame.CollisionSource.Height);
            pD3D.DrawHollowRect(currTemp, Color.Green, 3);
            pD3D.DrawRect(currTemp, Color.FromArgb(50, Color.Green));

            pD3D.DrawRect(new Rectangle(currFrame.AnchorPoint.X - OffSetMain.X, currFrame.AnchorPoint.Y - OffSetMain.Y, 5, 5), Color.Black);

            //End Rendering stuff

            pD3D.SpriteEnd();
            pD3D.DeviceEnd();
            pD3D.Present();
        }

        public void RenderLists()
        {
            pD3D.Clear(AnimationListBox, Color.White);

            pD3D.DeviceBegin();
            pD3D.SpriteBegin();
            //Begin Rendering stuff
            for (int i = 0; i < AnimeList.Count; i++)
            {
                if (i == AnimeSelected)
                {
                    pD3D.DrawRect(new Rectangle(0, i * 18 - OffSetList1.Y, AnimationListBox.Size.Width, 18), Color.LightBlue);
                    pD3D.DrawText(AnimeList[i].AnimationName, 0, i * 18 - OffSetList1.Y, Color.Black);
                }
                else
                    pD3D.DrawText(AnimeList[i].AnimationName, 0, i * 18 - OffSetList1.Y, Color.Black);
            }
            //End Rendering stuff

            pD3D.SpriteEnd();
            pD3D.DeviceEnd();
            pD3D.Present();

            pD3D.Clear(FrameListBox, Color.White);

            pD3D.DeviceBegin();
            pD3D.SpriteBegin();

            //Begin Rendering stuff

            for (int i = 0; i < currAnime.Framelist.Count; i++)
            {
                if (i == FrameSelected)
                {
                    pD3D.DrawRect(new Rectangle(0, i * 18 - OffSetList2.Y, FrameListBox.Size.Width, 18), Color.LightBlue);
                    string temp = "Frame " + i;
                    pD3D.DrawText(temp, 0, i * 18 - OffSetList2.Y, Color.Black);
                }
                else
                {
                    string temp = "Frame " + i;
                    pD3D.DrawText(temp, 0, i * 18 - OffSetList2.Y, Color.Black);
                }
            }

            //End Rendering stuff
            pD3D.SpriteEnd();
            pD3D.DeviceEnd();
            pD3D.Present();
        }

        public void RenderPreview()
        {
            pD3D.Clear(PreviewPanel, Color.Magenta);

            pD3D.DeviceBegin();
            pD3D.SpriteBegin();

            //Begin Rendering stuff
            if (SpriteSheetID != -1 && !currFrame.RenderSource.IsEmpty)
            {
                int anchorX = PreviewPanel.Size.Width / 2;
                int anchorY = PreviewPanel.Size.Height / 2;
                int imageX = currFrame.AnchorPoint.X - currFrame.RenderSource.Left;
                int imageY = currFrame.AnchorPoint.Y - currFrame.RenderSource.Top;
                pTM.Draw(SpriteSheetID, anchorX - imageX, anchorY - imageY, 1.0f, 1.0f, currFrame.RenderSource);

            }

            //End Rendering stuff

            pD3D.SpriteEnd();
            pD3D.DeviceEnd();
            pD3D.Present();
        } 
        #endregion

        #region FormClosing
        private void Form1_FormClosed(object sender, FormClosedEventArgs e)
        {
            ClosingOut();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ClosingOut();
            Close();
        }

        private void ClosingOut()
        {
            active = false;
        }
        #endregion

        private void ImagePanel_MouseDown(object sender, MouseEventArgs e)
        {
            //beginRECT = new Point(0, 0);
            //endRECT = new Point(0, 0);

            switch (currMode)
            {
                case UserMode.RenderMode:
                    {
                        //Get the mouse coord

                        beginRECT.X = e.X;
                        beginRECT.Y = e.Y;
                        sizing = true;

                    }
                    break;
                case UserMode.ActiveMode:
                    {
                        //Get the mouse coord

                        beginRECT.X = e.X;
                        beginRECT.Y = e.Y;
                        sizing = true;

                    }
                    break;
                case UserMode.CollisionMode:
                    {
                        //Get the mouse coord

                        beginRECT.X = e.X;
                        beginRECT.Y = e.Y;
                        sizing = true;

                    }
                    break;
                case UserMode.AnchorMode:
                    {
                        //Get the mouse coord

                        beginRECT.X = e.X + OffSetMain.X;
                        beginRECT.Y = e.Y + OffSetMain.Y;
                        sizing = true;

                    }
                    break;
            }
        }

        private void ImagePanel_MouseUp(object sender, MouseEventArgs e)
        {
            switch (currMode)
            {
                case UserMode.RenderMode:
                    {
                        //Get the mouse coord

                        int x = Math.Min(beginRECT.X, endRECT.X) + OffSetMain.X;
                        int y = Math.Min(beginRECT.Y, endRECT.Y) + OffSetMain.Y;
                        int width = Math.Max(beginRECT.X, endRECT.X) + OffSetMain.X - x;
                        int height = Math.Max(beginRECT.Y, endRECT.Y) + OffSetMain.Y - y;

                        currFrame.SetFrameRender(new Rectangle(x, y, width, height));
                        RenderLeft.Value = currFrame.RenderSource.Left;
                        RenderTop.Value = currFrame.RenderSource.Top;
                        RenderRight.Value = currFrame.RenderSource.Right;
                        RenderBottom.Value = currFrame.RenderSource.Bottom;

                        sizing = false;
                        RenderChanged = false;
                    }
                    break;
                case UserMode.ActiveMode:
                    {
                        //Get the mouse coord

                        int x = Math.Min(beginRECT.X, endRECT.X) + OffSetMain.X;
                        int y = Math.Min(beginRECT.Y, endRECT.Y) + OffSetMain.Y;
                        int width = Math.Max(beginRECT.X, endRECT.X) + OffSetMain.X - x;
                        int height = Math.Max(beginRECT.Y, endRECT.Y) + OffSetMain.Y - y;

                        currFrame.SetFrameActive(new Rectangle(x, y, width, height));
                        ActiveLeft.Value = currFrame.ActiveSource.Left;
                        ActiveTop.Value = currFrame.ActiveSource.Top;
                        ActiveRight.Value = currFrame.ActiveSource.Right;
                        ActiveBottom.Value = currFrame.ActiveSource.Bottom;

                        sizing = false;
                        ActiveChanged = false;
                    }
                    break;
                case UserMode.CollisionMode:
                    {
                        //Get the mouse coord

                        int x = Math.Min(beginRECT.X, endRECT.X) + OffSetMain.X;
                        int y = Math.Min(beginRECT.Y, endRECT.Y) + OffSetMain.Y;
                        int width = Math.Max(beginRECT.X, endRECT.X) + OffSetMain.X - x;
                        int height = Math.Max(beginRECT.Y, endRECT.Y) + OffSetMain.Y - y;

                        currFrame.SetFrameCollision(new Rectangle(x, y, width, height));
                        CollisionLeft.Value = currFrame.CollisionSource.Left;
                        CollisionTop.Value = currFrame.CollisionSource.Top;
                        CollisionRight.Value = currFrame.CollisionSource.Right;
                        CollisionBottom.Value = currFrame.CollisionSource.Bottom;

                        sizing = false;
                        CollisionChanged = false;
                    }
                    break;
                case UserMode.AnchorMode:
                    currFrame.SetAnchor(new Point((Size)beginRECT));
                    AnchorXUpDown.Value = currFrame.AnchorPoint.X;
                    AnchorYUpDown.Value = currFrame.AnchorPoint.Y;
                    break;
            }
        }

        private void ImagePanel_MouseMove(object sender, MouseEventArgs e)
        {
            string coord = "Cursor Position: " + (e.X + OffSetMain.X);
            coord += ", " + (e.Y + OffSetMain.Y);
            CursorCoord.Text = coord;
            switch (currMode)
            {
                case UserMode.RenderMode:
                    {
                        //Get the mouse coord
                        if (sizing == false)
                            return;

                        endRECT.X = e.X;
                        endRECT.Y = e.Y;

                        if (beginRECT.X < 0)
                            beginRECT.X = 0;
                        if (beginRECT.X > imageView.Width - 1)
                            beginRECT.X = imageView.Width - 1;
                        if (beginRECT.Y < 0)
                            beginRECT.Y = 0;
                        if (beginRECT.Y > imageView.Height - 1)
                            beginRECT.Y = imageView.Height - 1;

                        if (endRECT.X < 0)
                            endRECT.X = 0;
                        if (endRECT.X > imageView.Width - 1)
                            beginRECT.X = imageView.Width - 1;
                        if (endRECT.Y < 0)
                            endRECT.Y = 0;
                        if (endRECT.Y > imageView.Height - 1)
                            endRECT.Y = imageView.Height - 1;

                        int x = Math.Min(beginRECT.X, endRECT.X) + OffSetMain.X;
                        int y = Math.Min(beginRECT.Y, endRECT.Y) + OffSetMain.Y;
                        int width = Math.Max(beginRECT.X, endRECT.X) - Math.Min(beginRECT.X, endRECT.X);
                        int height = Math.Max(beginRECT.Y, endRECT.Y) - Math.Min(beginRECT.Y, endRECT.Y);

                        currFrame.SetFrameRender(new Rectangle(x, y, width, height));

                    }
                    break;
                case UserMode.ActiveMode:
                    {
                        //Get the mouse coord
                        if (sizing == false)
                            return;

                        endRECT.X = e.X;
                        endRECT.Y = e.Y;

                        if (beginRECT.X < 0)
                            beginRECT.X = 0;
                        if (beginRECT.X > imageView.Width - 1)
                            beginRECT.X = imageView.Width - 1;
                        if (beginRECT.Y < 0)
                            beginRECT.Y = 0;
                        if (beginRECT.Y > imageView.Height - 1)
                            beginRECT.Y = imageView.Height - 1;

                        if (endRECT.X < 0)
                            endRECT.X = 0;
                        if (endRECT.X > imageView.Width - 1)
                            beginRECT.X = imageView.Width - 1;
                        if (endRECT.Y < 0)
                            endRECT.Y = 0;
                        if (endRECT.Y > imageView.Height - 1)
                            endRECT.Y = imageView.Height - 1;

                        int x = Math.Min(beginRECT.X, endRECT.X) + OffSetMain.X;
                        int y = Math.Min(beginRECT.Y, endRECT.Y) + OffSetMain.Y;
                        int width = Math.Max(beginRECT.X, endRECT.X) - Math.Min(beginRECT.X, endRECT.X);
                        int height = Math.Max(beginRECT.Y, endRECT.Y) - Math.Min(beginRECT.Y, endRECT.Y);

                        currFrame.SetFrameActive(new Rectangle(x, y, width, height));

                    }
                    break;
                case UserMode.CollisionMode:
                    {
                        //Get the mouse coord
                        if (sizing == false)
                            return;

                        endRECT.X = e.X;
                        endRECT.Y = e.Y;

                        if (beginRECT.X < 0)
                            beginRECT.X = 0;
                        if (beginRECT.X > imageView.Width - 1)
                            beginRECT.X = imageView.Width - 1;
                        if (beginRECT.Y < 0)
                            beginRECT.Y = 0;
                        if (beginRECT.Y > imageView.Height - 1)
                            beginRECT.Y = imageView.Height - 1;

                        if (endRECT.X < 0)
                            endRECT.X = 0;
                        if (endRECT.X > imageView.Width - 1)
                            beginRECT.X = imageView.Width - 1;
                        if (endRECT.Y < 0)
                            endRECT.Y = 0;
                        if (endRECT.Y > imageView.Height - 1)
                            endRECT.Y = imageView.Height - 1;

                        int x = Math.Min(beginRECT.X, endRECT.X) + OffSetMain.X;
                        int y = Math.Min(beginRECT.Y, endRECT.Y) + OffSetMain.Y;
                        int width = Math.Max(beginRECT.X, endRECT.X) - Math.Min(beginRECT.X, endRECT.X);
                        int height = Math.Max(beginRECT.Y, endRECT.Y) - Math.Min(beginRECT.Y, endRECT.Y);

                        currFrame.SetFrameCollision(new Rectangle(x, y, width, height));

                    }
                    break;
                case UserMode.AnchorMode:
                    {
                        beginRECT.X = e.X + OffSetMain.X;
                        beginRECT.Y = e.Y + OffSetMain.Y;

                        if (beginRECT.X < 0)
                            beginRECT.X = 0;
                        if (beginRECT.X > imageView.Width - 1)
                            beginRECT.X = imageView.Width - 1;
                        if (beginRECT.Y < 0)
                            beginRECT.Y = 0;
                        if (beginRECT.Y > imageView.Height - 1)
                            beginRECT.Y = imageView.Height - 1;

                    }
                    break;
            }
        }

        private void activeModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            currMode = UserMode.ActiveMode;

        }

        private void renderModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            currMode = UserMode.RenderMode;
        }

        private void collisionModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            currMode = UserMode.CollisionMode;
        }

        private void anchorModeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            currMode = UserMode.AnchorMode;
        }

        private void checkBox1_CheckedChanged(object sender, EventArgs e)
        {
            if (isLoopingCheckBox.Checked)
                currAnime.IsLooping = true;
            else
                currAnime.IsLooping = false;
        }

        #region AnimationList Buttons
        private void newAnimeButton_Click(object sender, EventArgs e)
        {
            if (PreviewPlay)
                return;

            if (newAnimeButton.Text == "Add New Animation")
            {
                currAnime.AnimationName = AnimationNameBox.Text;
                currAnime.NumOfFrames = currAnime.Framelist.Count;
                currAnime.ImageFile = SpriteSheet;

                AnimeList.Add(new Animations(currAnime));
                currAnime = new Animations();
                currFrame = new Frame();
                EventNameBox.Text = "New Event";
                AnimationNameBox.Text = "New Animation";

                AnimeListScroll.Minimum = 0;
                AnimeListScroll.SmallChange = (AnimeList.Count * 20) / 20;
                AnimeListScroll.LargeChange = (AnimeList.Count * 20) / 10;

                AnimeListScroll.Maximum = (AnimeList.Count * 20) -  AnimationListBox.Size.Height;
                //AnimeListScroll.Maximum += ImageScrollY.Width;
                AnimeListScroll.Maximum += AnimeListScroll.LargeChange;
            }

            else if (newAnimeButton.Text == "Update Animation")
            {
                currAnime.AnimationName = AnimationNameBox.Text;
                currAnime.NumOfFrames = currAnime.Framelist.Count;
                currAnime.ImageFile = SpriteSheet;
                AnimeList[AnimeSelected] = currAnime;
                currAnime = new Animations();

                newAnimeButton.Text = "Add New Animation";
            }
        }

        private void AnimationListBox_MouseDown(object sender, MouseEventArgs e)
        {
            if (AnimeList.Count > 0)
            {
                int y = (e.Y + OffSetList1.Y) / 18;
                if (y >= AnimeList.Count || y < 0)
                    return;

                AnimeSelected = y;
                currAnime = AnimeList[y];
                currFrame = new Frame();


                FrameListScroll.Minimum = 0;
                FrameListScroll.SmallChange = (currAnime.Framelist.Count * 20) / 20;
                FrameListScroll.LargeChange = (currAnime.Framelist.Count * 20) / 10;

                FrameListScroll.Maximum = (currAnime.NumOfFrames * 20) - FrameListBox.Size.Height;
                //AnimeListScroll.Maximum += ImageScrollY.Width;
                FrameListScroll.Maximum += FrameListScroll.LargeChange;

                FrameNumberUpDown.Maximum = currAnime.NumOfFrames - 1;

                EventNameBox.Text = "New Event";
                AnimationNameBox.Text = currAnime.AnimationName;
                addNewFrameButton.Text = "Add New Frame";
                newAnimeButton.Text = "Update Animation";
            }
        }

        private void removeAnimeButton_Click(object sender, EventArgs e)
        {
            if (AnimeSelected >= 0)
            {
                AnimeList.Remove(AnimeList[AnimeSelected]);
            }
            AnimeSelected = -1;
            addNewFrameButton.Text = "Add New Frame";
        }

        private void removeAllAnimeButton_Click(object sender, EventArgs e)
        {
            AnimeList.Clear();
            AnimeSelected = -1;
            currFrame = new Frame();
            EventNameBox.Text = "New Event";
            AnimationNameBox.Text = "New Animation";
            addNewFrameButton.Text = "Add New Frame";
        }
        #endregion

        #region FrameList Commands
        private void addNewFrameButton_Click(object sender, EventArgs e)
        {
            if (addNewFrameButton.Text == "Add New Frame")
            {
                currFrame.EventName = EventNameBox.Text;
                currFrame.Duration = (float)Duration.Value;
                currAnime.Framelist.Add(new Frame(currFrame));
                currAnime.NumOfFrames++;
                FrameNumberUpDown.Maximum = currAnime.NumOfFrames - 1;
                currFrame = new Frame();
                EventNameBox.Text = "New Event";

                FrameListScroll.Minimum = 0;
                FrameListScroll.SmallChange = (currAnime.Framelist.Count * 20) / 20;
                FrameListScroll.LargeChange = (currAnime.Framelist.Count * 20) / 10;

                FrameListScroll.Maximum = (currAnime.NumOfFrames * 20) - FrameListBox.Size.Height;
                //AnimeListScroll.Maximum += ImageScrollY.Width;
                FrameListScroll.Maximum += FrameListScroll.LargeChange;
            }

            else if (addNewFrameButton.Text == "Update Frame")
            {
                currFrame.EventName = EventNameBox.Text;
                currFrame.Duration = (float)Duration.Value;
                currAnime.Framelist[FrameSelected] = currFrame;
                currFrame = new Frame();

                addNewFrameButton.Text = "Add New Frame";
            }
        }

        private void FrameListBox_MouseDown(object sender, MouseEventArgs e)
        {
            if (currAnime.Framelist.Count > 0)
            {
                int y = (e.Y + OffSetList2.Y) / 18;
                if (y >= currAnime.Framelist.Count || y < 0)
                    return;

                FrameSelected = y;
                SelectedFrame();
                FrameNumberUpDown.Value = y;

                addNewFrameButton.Text = "Update Frame";
            }
        }

        private void removeFrameButton_Click(object sender, EventArgs e)
        {
            if (FrameSelected >= 0)
            {
                currAnime.Framelist.Remove(currAnime.Framelist[FrameSelected]);
                currAnime.NumOfFrames--;
                FrameNumberUpDown.Maximum = currAnime.NumOfFrames - 1;
            }
            FrameSelected = -1;
            currFrame = new Frame();
            addNewFrameButton.Text = "Add New Frame";
        }

        private void removeAllFramesButton_Click(object sender, EventArgs e)
        {
            currAnime.Framelist.Clear();
            currAnime.NumOfFrames = 0;
            FrameNumberUpDown.Maximum = currAnime.NumOfFrames - 1;
            FrameSelected = -1;
            currFrame = new Frame();
            addNewFrameButton.Text = "Add New Frame";
            EventNameBox.Text = "New Event";
        }

        private void MoveFrameUpButton_Click(object sender, EventArgs e)
        {
            if (FrameSelected == 0 || FrameSelected == -1)
                return;

            Frame temp1 = new Frame(currAnime.Framelist[FrameSelected]);
            Frame temp2 = new Frame(currAnime.Framelist[FrameSelected - 1]);

            currAnime.Framelist[FrameSelected] = temp2;
            currAnime.Framelist[FrameSelected - 1] = temp1;
            FrameSelected--;

        }

        private void moveFrameDownButton_Click(object sender, EventArgs e)
        {
            if (FrameSelected >= currAnime.Framelist.Count - 1 || FrameSelected == -1)
                return;

            Frame temp1 = new Frame(currAnime.Framelist[FrameSelected]);
            Frame temp2 = new Frame(currAnime.Framelist[FrameSelected + 1]);

            currAnime.Framelist[FrameSelected] = temp2;
            currAnime.Framelist[FrameSelected + 1] = temp1;
            FrameSelected++;
        }
        #endregion

        #region Frame Adjustment Commands
        private void RenderLeft_ValueChanged(object sender, EventArgs e)
        {
            if (RenderChanged)
                RenderChanged = false;
            else
                RenderChanged = true;
        }

        private void ActiveLeft_ValueChanged(object sender, EventArgs e)
        {
            if (ActiveChanged)
                ActiveChanged = false;
            else
                ActiveChanged = true;
        }

        private void CollisionLeft_ValueChanged(object sender, EventArgs e)
        {
            if (CollisionChanged)
                CollisionChanged = false;
            else
                CollisionChanged = true;
        }
        #endregion

        #region Preview Commands
        private void button9_Click(object sender, EventArgs e)
        {
            if (FrameSelected + 1 > currAnime.NumOfFrames - 1 && !isLoopingCheckBox.Checked)
                return;
            else if (FrameSelected + 1 > currAnime.NumOfFrames - 1 && isLoopingCheckBox.Checked)
            {
                FrameSelected = 0;
                FrameNumberUpDown.Value = FrameSelected;
                currFrame = currAnime.Framelist[FrameSelected];
                return;
            }
            FrameSelected++;
            FrameNumberUpDown.Value = FrameSelected;
            SelectedFrame();


        }

        private void StepBackButton_Click(object sender, EventArgs e)
        {
            if (FrameSelected - 1 < 0 && !isLoopingCheckBox.Checked)
                return;
            else if (FrameSelected - 1 < 0 && isLoopingCheckBox.Checked)
            {
                FrameSelected = currAnime.NumOfFrames - 1;
                FrameNumberUpDown.Value = FrameSelected;
                currFrame = currAnime.Framelist[FrameSelected];
                return;
            }
            FrameSelected--;
            FrameNumberUpDown.Value = FrameSelected;
            SelectedFrame();

        }

        private void PlayButton_Click(object sender, EventArgs e)
        {
            if (currAnime.NumOfFrames == 0)
                return;

            FrameSelected = 0;
            SelectedFrame();
            PreviewPlay = true;
        }


        private void StopButton_Click(object sender, EventArgs e)
        {
            if (currAnime.NumOfFrames == 0)
                return;

            PreviewPlay = !PreviewPlay;

            SelectedFrame();
        }

        private void ResetButton_Click(object sender, EventArgs e)
        {
            if (currAnime.NumOfFrames == 0)
                return;

            FrameSelected = 0;
            SelectedFrame();
        }
        #endregion

        #region File Commands
        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveFileDialog dlg = new SaveFileDialog();
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement xRoot = new XElement("Animations");

                for (int i = 0; i < AnimeList.Count; i++)
                {
                    XElement xAnime = new XElement("Animation");
                    xRoot.Add(xAnime);

                    XAttribute xAnimationInfo = new XAttribute("ImageID", "Resources/Graphics/" + AnimeList[i].ImageFile);
                    xAnime.Add(xAnimationInfo);

                    xAnimationInfo = new XAttribute("AnimationName", AnimeList[i].AnimationName);
                    xAnime.Add(xAnimationInfo);

                    xAnimationInfo = new XAttribute("numFrames", AnimeList[i].NumOfFrames);
                    xAnime.Add(xAnimationInfo);

                    xAnimationInfo = new XAttribute("isLooping", AnimeList[i].IsLooping);
                    xAnime.Add(xAnimationInfo);

                    for (int index = 0; index < AnimeList[i].NumOfFrames; index++)
                    {
                        XElement xFrame = new XElement("Frame");
                        xAnime.Add(xFrame);

                        XAttribute xFrameInfo = new XAttribute("AnchorPointX", AnimeList[i].Framelist[index].AnchorPoint.X);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("AnchorPointY", AnimeList[i].Framelist[index].AnchorPoint.Y);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("Duration", AnimeList[i].Framelist[index].Duration);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("Event", AnimeList[i].Framelist[index].EventName);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("RenderRectLeft", AnimeList[i].Framelist[index].RenderSource.Left);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("RenderRectTop", AnimeList[i].Framelist[index].RenderSource.Top);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("RenderRectRight", AnimeList[i].Framelist[index].RenderSource.Right);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("RenderRectBottom", AnimeList[i].Framelist[index].RenderSource.Bottom);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("ActiveRectLeft", AnimeList[i].Framelist[index].ActiveSource.Left);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("ActiveRectTop", AnimeList[i].Framelist[index].ActiveSource.Top);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("ActiveRectRight", AnimeList[i].Framelist[index].ActiveSource.Right);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("ActiveRectBottom", AnimeList[i].Framelist[index].ActiveSource.Bottom);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("CollisionRectLeft", AnimeList[i].Framelist[index].CollisionSource.Left);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("CollisionRectTop", AnimeList[i].Framelist[index].CollisionSource.Top);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("CollisionRectRight", AnimeList[i].Framelist[index].CollisionSource.Right);
                        xFrame.Add(xFrameInfo);

                        xFrameInfo = new XAttribute("CollisionRectBottom", AnimeList[i].Framelist[index].CollisionSource.Bottom);
                        xFrame.Add(xFrameInfo);
                    }
                }
                xRoot.Save(dlg.FileName);
                SaveAsFile = dlg.FileName;
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();
            dlg.Filter = "All Files|*.*|Xml Files|*.xml";
            dlg.FilterIndex = 2;
            dlg.DefaultExt = "xml";

            if (DialogResult.OK == dlg.ShowDialog())
            {
                XElement xRoot = XElement.Load(dlg.FileName);
                SaveAsFile = dlg.FileName;

                IEnumerable<XElement> xAnimations = xRoot.Elements("Animation");
                foreach (XElement xAnime in xAnimations)
                {
                    Animations animeXml = new Animations();
                    XAttribute xImage = xAnime.Attribute("ImageID");
                    string loadImage = xImage.Value.ToString();
                    animeXml.ImageFile = loadImage;

                    if (SpriteSheet != loadImage)
                    {
                        SpriteSheet = loadImage;
                        SpriteSheetID = pTM.LoadTexture(SpriteSheet);

                        Size imageSize = new Size(pTM.GetTextureWidth(SpriteSheetID), pTM.GetTextureHeight(SpriteSheetID));

                        // Sets scrolling offset
                        ImageScrollX.Minimum = 0;
                        ImageScrollX.SmallChange = imageSize.Width / 20;
                        ImageScrollX.LargeChange = imageSize.Width / 10;

                        ImageScrollX.Maximum = imageSize.Width - ImagePanel.Size.Width;
                        ImageScrollX.Maximum += ImageScrollY.Width;
                        ImageScrollX.Maximum += ImageScrollX.LargeChange;

                        ImageScrollY.Minimum = 0;
                        ImageScrollY.SmallChange = imageSize.Height / 20;
                        ImageScrollY.LargeChange = imageSize.Height / 10;

                        ImageScrollY.Maximum = imageSize.Height - ImagePanel.Size.Height;
                        ImageScrollY.Maximum += ImageScrollX.Height;
                        ImageScrollY.Maximum += ImageScrollY.LargeChange;
                    }

                    XAttribute xAnimeName = xAnime.Attribute("AnimationName");
                    string animationName = xAnimeName.Value.ToString();
                    animeXml.AnimationName = animationName;
                    AnimationNameBox.Text = animationName;

                    XAttribute xFrameNumber = xAnime.Attribute("numFrames");
                    int numFrames = int.Parse(xFrameNumber.Value);
                    animeXml.NumOfFrames = numFrames;

                    XAttribute xLoop = xAnime.Attribute("isLooping");
                    bool looping = bool.Parse(xLoop.Value);
                    animeXml.IsLooping = looping;
                    isLoopingCheckBox.Checked = looping;

                    IEnumerable<XElement> xFrames = xAnime.Elements("Frame");
                    foreach (XElement xFrame in xFrames)
                    {
                        Frame frameXML = new Frame();

                        Point anchorPoint = new Point();
                        XAttribute xAnchor = xFrame.Attribute("AnchorPointX");
                        anchorPoint.X = int.Parse(xAnchor.Value);

                        xAnchor = xFrame.Attribute("AnchorPointY");
                        anchorPoint.Y = int.Parse(xAnchor.Value);
                        frameXML.SetAnchor(anchorPoint);

                        XAttribute xDuration = xFrame.Attribute("Duration");
                        float timeOfFrame = float.Parse(xDuration.Value);
                        frameXML.Duration = timeOfFrame;

                        XAttribute xEvent = xFrame.Attribute("Event");
                        string eventName = xEvent.Value.ToString();
                        frameXML.EventName = eventName;

                        XAttribute xSourceRect = xFrame.Attribute("RenderRectLeft");
                        int sourceLeft = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("RenderRectTop");
                        int sourceTop = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("RenderRectRight");
                        int sourceRight = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("RenderRectBottom");
                        int sourceBottom = int.Parse(xSourceRect.Value);

                        Rectangle RenderSource = new Rectangle(sourceLeft, sourceTop, sourceRight - sourceLeft, sourceBottom - sourceTop);
                        frameXML.SetFrameRender(RenderSource);
                        RenderLeft.Value = sourceLeft;
                        RenderTop.Value = sourceTop;
                        RenderRight.Value = sourceRight;
                        RenderBottom.Value = sourceBottom;

                        xSourceRect = xFrame.Attribute("ActiveRectLeft");
                        sourceLeft = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("ActiveRectTop");
                        sourceTop = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("ActiveRectRight");
                        sourceRight = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("ActiveRectBottom");
                        sourceBottom = int.Parse(xSourceRect.Value);

                        Rectangle ActiveSource = new Rectangle(sourceLeft, sourceTop, sourceRight - sourceLeft, sourceBottom - sourceTop);
                        frameXML.SetFrameActive(ActiveSource);
                        ActiveLeft.Value = sourceLeft;
                        ActiveTop.Value = sourceTop;
                        ActiveRight.Value = sourceRight;
                        ActiveBottom.Value = sourceBottom;

                        xSourceRect = xFrame.Attribute("CollisionRectLeft");
                        sourceLeft = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("CollisionRectTop");
                        sourceTop = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("CollisionRectRight");
                        sourceRight = int.Parse(xSourceRect.Value);

                        xSourceRect = xFrame.Attribute("CollisionRectBottom");
                        sourceBottom = int.Parse(xSourceRect.Value);

                        Rectangle CollisionSource = new Rectangle(sourceLeft, sourceTop, sourceRight - sourceLeft, sourceBottom - sourceTop);
                        frameXML.SetFrameCollision(CollisionSource);
                        CollisionLeft.Value = sourceLeft;
                        CollisionTop.Value = sourceTop;
                        CollisionRight.Value = sourceRight;
                        CollisionBottom.Value = sourceBottom;

                        animeXml.AddFrame(frameXML);
                    }
                    AnimeList.Add(animeXml);
                }

                AnimeListScroll.Minimum = 0;
                AnimeListScroll.SmallChange = (AnimeList.Count * 20) / 20;
                AnimeListScroll.LargeChange = (AnimeList.Count * 20) / 10;

                AnimeListScroll.Maximum = (AnimeList.Count * 20) - AnimationListBox.Size.Height;
                //AnimeListScroll.Maximum += ImageScrollY.Width;
                AnimeListScroll.Maximum += AnimeListScroll.LargeChange;
            }
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            OpenFileDialog dlg = new OpenFileDialog();

            if (associatedFilePath != "")
                dlg.InitialDirectory = associatedFilePath;

            dlg.Filter = "All Image Files ( JPEG,BMP,PNG )|*.jpg;*.jpeg;*.bmp;*.png;|JPEG Files ( *.jpg;*.jpeg )|*.jpg;*.jpeg|BMP Files ( *.bmp )|*.bmp|PNG Files ( *.png )|*.png| All Files (*.*)|*.*";
            dlg.FilterIndex = 0;


            if (DialogResult.OK == dlg.ShowDialog())
            {
                associatedFilePath = dlg.FileName.Replace(dlg.SafeFileName, "");
                associatedFileName = dlg.SafeFileName;

                SpriteSheet = associatedFileName;
                SpriteSheetID = pTM.LoadTexture(associatedFilePath + SpriteSheet);

                Size imageSize = new Size(pTM.GetTextureWidth(SpriteSheetID), pTM.GetTextureHeight(SpriteSheetID));

                // Sets scrolling offset
                ImageScrollX.Minimum = 0;
                ImageScrollX.SmallChange = imageSize.Width / 20;
                ImageScrollX.LargeChange = imageSize.Width / 10;

                ImageScrollX.Maximum = imageSize.Width - ImagePanel.Size.Width;
                ImageScrollX.Maximum += ImageScrollY.Width;
                ImageScrollX.Maximum += ImageScrollX.LargeChange;

                ImageScrollY.Minimum = 0;
                ImageScrollY.SmallChange = imageSize.Height / 20;
                ImageScrollY.LargeChange = imageSize.Height / 10;

                ImageScrollY.Maximum = imageSize.Height - ImagePanel.Size.Height;
                ImageScrollY.Maximum += ImageScrollX.Height;
                ImageScrollY.Maximum += ImageScrollY.LargeChange;
            }
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (SaveAsFile == null)
            {
                saveAsToolStripMenuItem_Click(sender, e);
                return;
            }

            XElement xRoot = new XElement("Animations");

            for (int i = 0; i < AnimeList.Count; i++)
            {
                XElement xAnime = new XElement("Animation");
                xRoot.Add(xAnime);

                XAttribute xAnimationInfo = new XAttribute("ImageID", AnimeList[i].ImageFile);
                xAnime.Add(xAnimationInfo);

                xAnimationInfo = new XAttribute("AnimationName", AnimeList[i].AnimationName);
                xAnime.Add(xAnimationInfo);

                xAnimationInfo = new XAttribute("numFrames", AnimeList[i].NumOfFrames);
                xAnime.Add(xAnimationInfo);

                xAnimationInfo = new XAttribute("isLooping", AnimeList[i].IsLooping);
                xAnime.Add(xAnimationInfo);

                for (int index = 0; index < AnimeList[i].NumOfFrames; index++)
                {
                    XElement xFrame = new XElement("Frame");
                    xAnime.Add(xFrame);

                    XAttribute xFrameInfo = new XAttribute("AnchorPointX", AnimeList[i].Framelist[index].AnchorPoint.X);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("AnchorPointY", AnimeList[i].Framelist[index].AnchorPoint.Y);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("Duration", AnimeList[i].Framelist[index].Duration);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("Event", AnimeList[i].Framelist[index].EventName);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("RenderRectLeft", AnimeList[i].Framelist[index].RenderSource.Left);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("RenderRectTop", AnimeList[i].Framelist[index].RenderSource.Top);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("RenderRectRight", AnimeList[i].Framelist[index].RenderSource.Right);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("RenderRectBottom", AnimeList[i].Framelist[index].RenderSource.Bottom);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("ActiveRectLeft", AnimeList[i].Framelist[index].ActiveSource.Left);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("ActiveRectTop", AnimeList[i].Framelist[index].ActiveSource.Top);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("ActiveRectRight", AnimeList[i].Framelist[index].ActiveSource.Right);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("ActiveRectBottom", AnimeList[i].Framelist[index].ActiveSource.Bottom);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("CollisionRectLeft", AnimeList[i].Framelist[index].CollisionSource.Left);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("CollisionRectTop", AnimeList[i].Framelist[index].CollisionSource.Top);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("CollisionRectRight", AnimeList[i].Framelist[index].CollisionSource.Right);
                    xFrame.Add(xFrameInfo);

                    xFrameInfo = new XAttribute("CollisionRectBottom", AnimeList[i].Framelist[index].CollisionSource.Bottom);
                    xFrame.Add(xFrameInfo);
                }
            }
            xRoot.Save(SaveAsFile);
        }

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            AnimeList.Clear();
            currFrame = new Frame();
            currAnime = new Animations();
            SpriteSheet = null;
            SaveAsFile = null;

            if (SpriteSheetID != -1)
                pTM.UnloadTexture(SpriteSheetID);
            SpriteSheetID = -1;
            isLoopingCheckBox.Checked = false;

            RenderLeft.Value = 0;
            RenderTop.Value = 0;
            RenderRight.Value = 0;
            RenderBottom.Value = 0;

            ActiveLeft.Value = 0;
            ActiveTop.Value = 0;
            ActiveRight.Value = 0;
            ActiveBottom.Value = 0;

            CollisionLeft.Value = 0;
            CollisionTop.Value = 0;
            CollisionRight.Value = 0;
            CollisionBottom.Value = 0;

            AnchorXUpDown.Value = 0;
            AnchorYUpDown.Value = 0;

            Duration.Value = 1;

            FrameNumberUpDown.Maximum = 0;
            FrameNumberUpDown.Value = 0;

            AnimationNameBox.Text = "New Animation";
            EventNameBox.Text = "New Event";

            if (addNewFrameButton.Text == "Update Frame")
                addNewFrameButton.Text = "Add New Frame";
            if (newAnimeButton.Text == "Update Animation")
                newAnimeButton.Text = "Add New Animation";
        } 
        #endregion
    }
}
