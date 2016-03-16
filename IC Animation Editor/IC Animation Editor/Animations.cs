using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace IC_Animation_Editor
{

    class Animations
    {
        private List<Frame> framelist;
        private int numOfFrames;
        private string animationName;
        private string imageFile;
        private bool isLooping;

        public bool IsLooping
        {
            get { return isLooping; }
            set { isLooping = value; }
        }

        public string ImageFile
        {
            get { return imageFile; }
            set { imageFile = value; }
        }

        public string AnimationName
        {
            get { return animationName; }
            set { animationName = value; }
        }

        public int NumOfFrames
        {
            get { return numOfFrames; }
            set { numOfFrames = value; }
        }

        public List<Frame> Framelist
        {
            get { return framelist; }
            set { framelist = value; }
        }

        public void AddFrame( Frame newFrame )
        {
            if (framelist == null)
                framelist = new List<Frame>();

            framelist.Add(newFrame);
        }

        public Animations()
        {
            framelist = new List<Frame>();
            isLooping = false;
        }

        public Animations(Animations cpy)
        {
            this.animationName = cpy.animationName;
            this.framelist = cpy.Framelist;
            this.imageFile = cpy.ImageFile;
            this.isLooping = cpy.IsLooping;
            this.numOfFrames = cpy.NumOfFrames;
        }

    }
}
