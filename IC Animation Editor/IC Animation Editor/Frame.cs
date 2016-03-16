using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Drawing;
using System.Threading.Tasks;

namespace IC_Animation_Editor
{
    class Frame
    {
        private string eventName;
        private float duration;
        private Rectangle renderSource;
        private Rectangle activeSource;
        private Rectangle collisionSource;
        private Point anchorPoint;

        public Frame()
        {
        }

        public Frame(Frame cpy)
        {
            this.eventName = cpy.EventName;
            this.duration = cpy.Duration;
            this.renderSource = cpy.RenderSource;
            this.activeSource = cpy.ActiveSource;
            this.collisionSource = cpy.CollisionSource;
            this.anchorPoint = cpy.AnchorPoint;
        }

        public string EventName
        {
            get { return eventName; }
            set { eventName = value; }
        }

        public Rectangle RenderSource
        {
            get { return renderSource; }
            set { renderSource = value; }
        }

        public Rectangle ActiveSource
        {
            get { return activeSource; }
            set { activeSource = value; }
        }

        public Rectangle CollisionSource
        {
            get { return collisionSource; }
            set { collisionSource = value; }
        }

        public Point AnchorPoint
        {
            get { return anchorPoint; }
            set { anchorPoint = value; }
        }

        public float Duration
        {
            get { return duration; }
            set { duration = value; }
        }
        
        public void SetFrameRender(Rectangle stuff)
        {
            RenderSource = stuff;
        }

        public void SetFrameActive(Rectangle stuff)
        {
            ActiveSource = stuff;
        }

        public void SetFrameCollision(Rectangle stuff)
        {
            CollisionSource = stuff;
        }

        public void SetAnchor(Point stuff)
        {
            anchorPoint = stuff;
        }
    }
}
