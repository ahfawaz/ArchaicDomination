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
    class Emitter
    {
        public List<Particle> ParticleList;

        //int m_nImageID;
        private string m_nImageID;

        public string ImageID
        {
            get { return m_nImageID; }
            set { m_nImageID = value; }
        }
        //int m_nMinLife;
        private float m_nMinLife;

        public float MinLife
        {
            get { return m_nMinLife; }
            set { m_nMinLife = value; }
        }
        //int m_nMaxLife;
        private float m_nMaxLife;

        public float MaxLife
        {
            get { return m_nMaxLife; }
            set { m_nMaxLife = value; }
        }
        //int m_nWidth;
        private Size size;

        public Size Size
        {
            get { return size; }
            set { size = value; }
        }
        //int m_nGravityX;
        private float m_nGravityX;

        public float GravityX
        {
            get { return m_nGravityX; }
            set { m_nGravityX = value; }
        }
        //int m_nGravityY;
        private float m_nGravityY;

        public float GravityY
        {
            get { return m_nGravityY; }
            set { m_nGravityY = value; }
        }
        //int m_nCurrRotation;
        private decimal m_nCurrRotation;

        public decimal CurrRotation
        {
            get { return m_nCurrRotation; }
            set { m_nCurrRotation = value; }
        }
        //int m_nStartingRotation;
        private float m_nStartingRotation;

        public float StartingRotation
        {
            get { return m_nStartingRotation; }
            set { m_nStartingRotation = value; }
        }
        //int m_nEndingRotation;
        private float m_nEndingRotation;

        public float EndingRotation
        {
            get { return m_nEndingRotation; }
            set { m_nEndingRotation = value; }
        }
        //int m_nSpreadValue;
        private decimal m_nSpreadValue;

        public decimal SpreadValue
        {
            get { return m_nSpreadValue; }
            set { m_nSpreadValue = value; }
        }
        //int m_nAngularOffset;
        private decimal m_nAngularOffset;

        public decimal AngularOffset
        {
            get { return m_nAngularOffset; }
            set { m_nAngularOffset = value; }
        }

        private Color startingcolor;

        public Color StartingColor
        {
            get { return startingcolor; }
            set { startingcolor = value; }
        }
        private Color endingcolor;

        public Color EndingColor
        {
            get { return endingcolor; }
            set { endingcolor = value; }
        }
       
        //int m_nSpawnRate;
        private float m_nSpawnRate;

        public float SpawnRate
        {
            get { return m_nSpawnRate; }
            set { m_nSpawnRate = value; }
        }
        //int m_nMaxParticle;
        private decimal m_nMaxParticle;

        public decimal MaxParticle
        {
            get { return m_nMaxParticle; }
            set { m_nMaxParticle = value; }
        }
        //int m_nStartingScaleX;
        private float m_nStartingScaleX;

        public float StartingScaleX
        {
            get { return m_nStartingScaleX; }
            set { m_nStartingScaleX = value; }
        }
        //int m_nEndingScaleX;
        private float m_nEndingScaleX;

        public float EndingScaleX
        {
            get { return m_nEndingScaleX; }
            set { m_nEndingScaleX = value; }
        }
        //int m_nStartingScaleY;
        private float m_nStartingScaleY;

        public float StartingScaleY
        {
            get { return m_nStartingScaleY; }
            set { m_nStartingScaleY = value; }
        }
        //int m_nEndingScaleY;
        private float m_nEndingScaleY;

        public float EndingScaleY
        {
            get { return m_nEndingScaleY; }
            set { m_nEndingScaleY = value; }
        }
        //float m_fEmitterPosX & Y;
        //private Point location;

        //public Point Location
        //{
        //    get { return location; }
        //    set { location = value; }
        //} 
        //posx&y
        private float m_fEmitterPosX;

        public float EmitterPosX
        {
            get { return m_fEmitterPosX; }
            set { m_fEmitterPosX = value; }
        }
        private float m_fEmitterPosY;

        public float EmitterPosY
        {
            get { return m_fEmitterPosY; }
            set { m_fEmitterPosY = value; }
        }
        //range x&y
        private float m_fEmitterRangeX;

        public float EmitterRangeX
        {
            get { return m_fEmitterRangeX; }
            set { m_fEmitterRangeX = value; }
        }
        private float m_fEmitterRangeY;

        public float EmitterRangeY
        {
            get { return m_fEmitterRangeY; }
            set { m_fEmitterRangeY = value; }
        }

        //float m_fStartingAccelerationX;
        private float m_fStartingAccelerationX;

        public float StartingAccelerationX
        {
            get { return m_fStartingAccelerationX; }
            set { m_fStartingAccelerationX = value; }
        }
         //float m_fEndingAccelerationX;
        private float m_fEndingAccelerationX;

        public float EndingAccelerationX
        {
            get { return m_fEndingAccelerationX; }
            set { m_fEndingAccelerationX = value; }
        }
      
        
        //float m_fStartingAccelerationY;
        private float m_fStartingAccelerationY;

        public float StartingAccelerationY
        {
            get { return m_fStartingAccelerationY; }
            set { m_fStartingAccelerationY = value; }
        }
        //float m_fEndingAccelerationY;
        private float m_fEndingAccelerationY;

        public float EndingAccelerationY
        {
            get { return m_fEndingAccelerationY; }
            set { m_fEndingAccelerationY = value; }
        }
        //float m_fStartingVelocityX;
        private float m_fStartingVelocityX;

        public float StartingVelocityX
        {
            get { return m_fStartingVelocityX; }
            set { m_fStartingVelocityX = value; }
        }
        //float m_fEndingVelocityX;
        private float m_fEndingVelocityX;

        public float EndingVelocityX
        {
            get { return m_fEndingVelocityX; }
            set { m_fEndingVelocityX = value; }
        }
        //float m_fStartingVelocityY;
        private float m_fStartingVelocityY;

        public float StartingVelocityY
        {
            get { return m_fStartingVelocityY; }
            set { m_fStartingVelocityY = value; }
        }
        //float m_fEndingVelocityY;
        private float m_fEndingVelocityY;

        public float EndingVelocityY
        {
            get { return m_fEndingVelocityY; }
            set { m_fEndingVelocityY = value; }
        }
        //bool m_bLooping;
        private bool looping;

        public bool Looping
        {
            get { return looping; }
            set { looping = value; }
        }

        







         public Emitter()
        {
            ParticleList                        = new List<Particle>();
            this.m_nImageID                     = "";
           // this.location                       = new Point(0,0);
             this.m_fEmitterPosX                = 0.0f;
             this.m_fEmitterPosY                = 0.0f;
             this.EmitterRangeX                 = 0.0f;
             this.EmitterRangeY                 = 0.0f;
            this.size                           = new Size(0,0);
            this.looping                        = false;
            this.startingcolor                  = new Color();
            this.endingcolor                    = new Color();
            this.m_nMinLife                     = 0.0f;
            this.m_nMaxLife                     = 0.0f;
            this.m_nGravityX                    = 0.0f;
            this.m_nGravityY                    = 0.0f;
            this.m_nCurrRotation                = 0;
            this.m_nStartingRotation            = 0.0f;
            this.m_nEndingRotation              = 0.0f;
            this.m_nSpreadValue                 = 0;
            this.m_nAngularOffset               = 0;
            this.m_nSpawnRate                   = 0.0f;
            this.m_nMaxParticle                 = 0;
            this.m_nStartingScaleX              = 0.0f;
            this.m_nEndingScaleX                = 0.0f;
            this.m_nStartingScaleY              = 0.0f;
            this.m_nEndingScaleY                = 0.0f;
            this.m_fStartingAccelerationX       = 0.0f;
            this.m_fEndingAccelerationX         = 0.0f;
            this.m_fStartingAccelerationY       = 0.0f;
            this.m_fEndingAccelerationY         = 0.0f;
            this.m_fStartingVelocityX           = 0.0f;
            this.m_fEndingVelocityX             = 0.0f;
            this.m_fStartingVelocityY           = 0.0f;
            this.m_fEndingVelocityY             = 0.0f;

        }

         public Emitter
             (
             string Image,float posx, float posy,float rangex, float rangey,/* Point location*/ Size size, bool loop, Color startingcolor, Color endingcolor,
             float minlife, float maxlife, float gravityx, float gravityy,
             decimal currotation, float startingrotation, float endingrotation,
             decimal spreadvalue, decimal angularoffset, float spawnrate, decimal maxparticle,
             float startingscalex, float endingscalex, float startingscaley, float endingscaley,
             float startingaccx, float endingaccx, float startingaccy,float endingaccy, 
             float startingvelx, float endingvelx, float startingvely,float endingvely
             )
         {
             this.m_nImageID                    =  Image; 
           //  this.location                      =  location;
             this.m_fEmitterPosX                = posx;
             this.m_fEmitterPosY                = posy;
             this.EmitterRangeX                 = rangex;
             this.EmitterRangeY                 = rangey;
             this.size                          =  size; 
             this.looping                       =  loop;
             this.startingcolor                 = startingcolor;
             this.endingcolor                   = endingcolor;
             this.m_nMinLife                    =  minlife; 
             this.m_nMaxLife                    =  maxlife; 
             this.m_nGravityX                   =  gravityx; 
             this.m_nGravityY                   =  gravityy;
             this.m_nCurrRotation               =  currotation; 
             this.m_nStartingRotation           =  startingrotation;
             this.m_nEndingRotation             =  endingrotation;
             this.m_nSpreadValue                =  spreadvalue;
             this.m_nAngularOffset              =  angularoffset;
             this.m_nSpawnRate                  =  spawnrate;
             this.m_nMaxParticle                =  maxparticle;
             this.m_nStartingScaleX             =  startingscalex; 
             this.m_nEndingScaleX               =  endingscalex;
             this.m_nStartingScaleY             =  startingscaley;
             this.m_nEndingScaleY               =  endingscaley;
             this.m_fStartingAccelerationX      =  startingaccx;
             this.m_fEndingAccelerationX        =  endingaccx;
             this.m_fStartingAccelerationY      =  startingaccy;
             this.m_fEndingAccelerationY        =  endingaccy;
             this.m_fStartingVelocityX          =  startingvelx;
             this.m_fEndingVelocityX            =  endingvelx;
             this.m_fStartingVelocityY          = startingvely;
             this.m_fEndingVelocityY            = endingvely;
         }
         public override string ToString()
         {
             return ImageID;
         }


    }
}
