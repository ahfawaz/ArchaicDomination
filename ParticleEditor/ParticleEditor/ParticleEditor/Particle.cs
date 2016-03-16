using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ParticleEditor
{
    class Particle
    {

        //int m_nCurrLife;
        private float m_nCurrLife;

        public float CurrLife
        {
            get { return m_nCurrLife; }
            set { m_nCurrLife = value; }
        }
        //int m_nMaxLife;
        private float m_nMaxLife;

        public float MaxLife
        {
            get { return m_nMaxLife; }
            set { m_nMaxLife = value; }
        }
        //int m_nCurrRotation;
        private float m_nCurrRotation;

        public float CurrRotation
        {
            get { return m_nCurrRotation; }
            set { m_nCurrRotation = value; }
        }
        //int m_nCurrScaleX;
        private float m_nCurrScaleX;

        public float CurrScaleX
        {
            get { return m_nCurrScaleX; }
            set { m_nCurrScaleX = value; }
        }
        //int m_nCurrScaleY;
        private float m_nCurrScaleY;

        public float CurrScaleY
        {
            get { return m_nCurrScaleY; }
            set { m_nCurrScaleY = value; }
        }
        //ParticlePosX & Y
        //private Point location;

        //public Point Location
        //{
        //    get { return location; }
        //    set { location = value; }
        //}
        private float m_fParticlePosX;

        public float ParticlePosX
        {
            get { return m_fParticlePosX; }
            set { m_fParticlePosX = value; }
        }

        private float m_fParticlePosY;

        public float ParticlePosY
        {
            get { return m_fParticlePosY; }
            set { m_fParticlePosY = value; }
        }

        private Color curcolor;

        public Color CurrColor
        {
            get { return curcolor; }
            set { curcolor = value; }
        }

        //float m_fCurrAccelerationX;
        private float m_fCurrAccelerationX;

        public float CurrAccelerationX
        {
            get { return m_fCurrAccelerationX; }
            set { m_fCurrAccelerationX = value; }
        }
        //float m_fCurrAccelerationY;
        private float m_fCurrAccelerationY;

        public float CurrAccelerationY
        {
            get { return m_fCurrAccelerationY; }
            set { m_fCurrAccelerationY = value; }
        }
        //float m_fCurrVelocityX;
        private float m_fCurrVelocityX;

        public float CurrVelocityX
        {
            get { return m_fCurrVelocityX; }
            set { m_fCurrVelocityX = value; }
        }
        //float m_fCurrVelocityY;
        private float m_fCurrVelocityY;

        public float CurrVelocityY
        {
            get { return m_fCurrVelocityY; }
            set { m_fCurrVelocityY = value; }
        }
        //bool m_bAlive;
        private bool m_bAlive;

        public bool Alive
        {
            get { return m_bAlive; }
            set { m_bAlive = value; }
        }

        public Particle()
        {

            //this.location               =  new Point(0,0);
            this.m_fParticlePosX        = 0.0f;
            this.m_fParticlePosY        = 0.0f;
            this.curcolor               = new Color();
            this.m_bAlive               =  false;
            this.m_nCurrLife            =  0;
            this.m_nMaxLife             =  0;
            this.m_nCurrRotation        =  0;
            this.m_nCurrScaleX          =  0;
            this.m_nCurrScaleY          =  0;
            this.m_fCurrAccelerationX   =  0;
            this.m_fCurrAccelerationY   =  0;
            this.m_fCurrVelocityX       =  0;
            this.m_fCurrVelocityY       =  0;
            
            
        }

        public Particle
            (
            float posx, float posy
            , Color color,
            bool alive ,
            float currlife    , float maxlife, 
            float currrotation,
            float curscalex   , float curscaley,
            float curaccx     , float curaccy,
            float curvelx    , float curvely
            )
        {
            //this.location               = location;
            this.m_fParticlePosX        = posx;
            this.m_fParticlePosY        = posy;
            this.curcolor               = color;
            this.m_bAlive               = alive;
            this.m_nCurrLife            = currlife;
            this.m_nMaxLife             = maxlife;
            this.m_nCurrRotation        = currrotation;
            this.m_nCurrScaleX          = curscalex;
            this.m_nCurrScaleY          = curscaley;
            this.m_fCurrAccelerationX   = curaccx;
            this.m_fCurrAccelerationY   = curaccy;
            this.m_fCurrVelocityX       = curvelx;
            this.m_fCurrVelocityY       = curvely;
        }
    }
}
