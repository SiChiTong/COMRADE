#ifndef VIEW3D
#define VIEW3D

class object3d
{
	public:
		object3d(unsigned short int x_res, unsigned short int y_res, unsigned short int z_res, unsigned char*** map);
		~object3d();
		void draw(void);
		void draw_smooth(float azimuth, float elevation);
	private:
		unsigned short int x, y, z;
		float e, a;
		unsigned char*** map3d;
		unsigned char*** t3d;
		unsigned short int tx, ty, tz;
		unsigned char** map2d_front;
		unsigned char** map2d_rear;
		unsigned char** map2d;
		bool surrounded(unsigned short int xpos, unsigned short int ypos, unsigned short int zpos);
		bool tsurrounded(unsigned short int xpos, unsigned short int ypos, unsigned short int zpos);
		void draw_smooth();
};

#endif
