/* draw_bvg.cpp
   Starter code made by B. Bird - 01/02/2016

   Bresenham Algorithm - draw line/triangle/circle and fill circle/triangle/gradient triangle

   @Chenkai Hao - Jan 2016
*/

#include <string>
#include <iostream>
#include "vector2d.h"
#include "colourRGB.h"
#include "bvg.h"
#include "png_canvas.h"

using namespace std;


class BVGRenderer: public BVGRendererBase{
public:
	virtual void create_canvas(Vector2d dimensions, ColourRGB background_colour, int scale_factor){
		cout << "Canvas " << dimensions << background_colour << scale_factor << endl;
		width = dimensions[0];
		height = dimensions[1];
		canvas.initialize_canvas(dimensions[0],dimensions[1]);
		for (int y = 0; y < height; y++)
			for (int x = 0; x < width; x++)
				canvas[x][y] = background_colour;
	}
	virtual void render_line(Vector2d endpoint1, Vector2d endpoint2, ColourRGB colour, int thickness){
		
		ColourRGB pointColour;
		pointColour.setRGB(0,255,0);
		canvas[endpoint2.x][endpoint2.y] = pointColour;
		canvas[endpoint1.x][endpoint1.y] = pointColour;//draw P0

		int x1 = endpoint1.x;
		int y1 = endpoint1.y;
		int x2 = endpoint2.x;
		int y2 = endpoint2.y;

		int dx = x2-x1;
		int dy = y2-y1;
		int lx = abs(dx);//the difference
		int ly = abs(dy);

		int sx = 2*ly-lx;
		int sy = 2*lx-ly;

		int x,y,x_end,y_end,i;

		Vector2d center;



		if(ly<=lx){	//x is longer than y, go along x x++

			if(dx>0){	//x2 is to the right of x1
				x=x1;
				y=y1;
				x_end = x2;
			}else{	//x1 is on the right, so start from x2
				x=x2;
				y=y2;
				x_end = x1;
			}

			canvas[x][y] = colour;
			for(i=0; x<x_end; i++){
				x = x+1;
				if(sx<0){
					sx = sx+ly;
				}else{
					if((dx<0 && dy<0) || (dx>0 && dy>0)){
						y = y+1;
					}else{
						y = y-1;
					}
					sx = sx+(ly-lx);
				}
				canvas[x][y] = colour;

				//line thickness draw parallel line below
				int k = -(thickness/2);

				while(k < (thickness/2)){
					canvas[x][y+k] = colour;
					k+=1;
				}
				

			}

		}else{	//y is longer than x, go along y y++

			if(dy>0){
				x=x1;
				y=y1;
				y_end=y2;
			}else{
				x=x2;
				y=y2;
				y_end=y1;
			}

			canvas[x][y] = colour;
			for(i=0;y<y_end;i++){
				y=y+1;
				if(sy<=0){
					sy=sy+lx;
				}else{
					if((dx<0 && dy<0) || (dx>0 && dy>0)){
						x=x+1;
					}else{
						x=x-1;
					}
					sy=sy+(lx-ly);
				}
				canvas[x][y] = colour;

				//line thickness for dy>dx : draw parallel line to the right

				int h = -(thickness/2);
				while(h < (thickness/2)){
					canvas[x+h][y] = colour;
					h+=1;
				}
			}
		}

		cout << "Line " << endpoint1 << endpoint2 << colour << thickness << endl;
	}

	virtual void render_circle(Vector2d center, int radius, ColourRGB line_colour, int line_thickness){
		//ColourRGB pointColour;
		//pointColour.setRGB(0,255,0);
		//canvas[center.x][center.y] = pointColour;//draw center

		int x0 = center.x;
		int y0 = center.y;

		int k =0;//for thickness from 0 to line_thickness

		while(k<line_thickness){

			int x=radius+k;	//draw k circles with same center NOTE: very ugly!!!
			int y=0;
			int f = 1-x;
			while(y<=x){
				//draw 8 points which belongs to 8 different quadrants
				canvas[x+x0][y+y0] = line_colour;
				canvas[y+x0][x+y0] = line_colour;
				canvas[-x+x0][y+y0] = line_colour;
				canvas[-y+x0][x+y0] = line_colour;
				canvas[-x+x0][-y+y0] = line_colour;
				canvas[-y+x0][-x+y0] = line_colour;
				canvas[x+x0][-y+y0] = line_colour;
				canvas[y+x0][-x+y0] = line_colour;
				y++;

				if(f<=0){
					f = f + 2*y + 1;
				}else{
					x = x - 1;
					f = f+ 2*(y-x)+1;
				}
			
			}

			k+=1;

		}

		cout << "Circle " << center << radius << line_colour << line_thickness << endl;
	}
	virtual void render_filledcircle(Vector2d center, int radius, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
		int x0 = center.x;
		int y0 = center.y;


		ColourRGB pointColour;
		pointColour.setRGB(0,255,0);
		
		
		
		int x=radius;
		int y=0;
		int f = 1-x;

		Vector2d p1,p2,p3,p4,p5,p6,p7,p8;
		

		//render_line(p1,p2,pointColour,line_thickness);

		//cout << "center" << center << endl;

		while(y<=x){
			//draw 4 lines using 8 points which belongs to 8 different quadrants
			
			p1.x = p6.x = x0-x;
			p1.y = p2.y = y0+y;
			p2.x = p5.x = x0+x;
			p5.y = p6.y = y0-y;
			p3.y = p8.y = x0+x;
			p3.x = p4.x = y0+y;
			p4.y = p7.y = x0-x;
			p7.x = p8.x = y0-y;
			render_line(p1,p2,fill_colour,line_thickness);
			render_line(p8,p3,fill_colour,line_thickness);
			render_line(p7,p4,fill_colour,line_thickness);
			render_line(p6,p5,fill_colour,line_thickness);


			y++;

			if(f<=0){
				f = f + 2*y + 1;
			}else{
				x = x - 1;
				f = f+ 2*(y-x)+1;
			}
		}

		render_circle(center, radius, line_colour, line_thickness);//draw the boundary with line_colour

		cout << "Filled Circle " << center << radius << line_colour << line_thickness << fill_colour << endl;
	}

	float cross_product(Vector2d v1, Vector2d v2){
		return (v1.x*v2.y) - (v1.y*v2.x);
	}

	virtual void render_triangle(Vector2d point1, Vector2d point2, Vector2d point3, ColourRGB line_colour, int line_thickness, ColourRGB fill_colour){
		

		//fill with colour
		int max_X = max(point1.x, max(point2.x,point3.x));
		int min_X = min(point1.x, min(point2.x,point3.x));
		int max_Y = max(point1.y, max(point2.y,point3.y));
		int min_Y = min(point1.y, min(point2.y,point3.y));

		//cout << "** minX->maxX minY->maxY" << min_X << max_X << min_Y << max_Y << endl;
		
		Vector2d vs2 = point2 - point1;
		Vector2d vs3 = point3 - point1;
		float denominator = ((vs3.dot(vs3)) * (vs2.dot(vs2))) - ((vs3.dot(vs2)) * (vs2.dot(vs3))) ;

		for(int i=min_X; i<=max_X; i++){
			for(int j=min_Y; j<=max_Y; j++){
				Vector2d temp;
				temp.x = i;
				temp.y = j;

			 	Vector2d vs1 = temp - point1;
				
				float s = ( ((vs3.dot(vs3)) * (vs1.dot(vs2))) - ((vs3.dot(vs2)) * (vs1.dot(vs3))) ) / denominator;
				float t = ( ((vs2.dot(vs2)) * (vs1.dot(vs3))) - ((vs2.dot(vs3)) * (vs1.dot(vs2))) ) / denominator;
				
				if((s>=0) && (t>=0) && (s+t<=1)){
					canvas[i][j] = fill_colour;
				}
			}
		}
		//draw the boundary lines
		render_line(point1, point2, line_colour, line_thickness);
		render_line(point2, point3, line_colour, line_thickness);
		render_line(point3, point1, line_colour, line_thickness);

		
		cout << "Triangle " << point1 << point2 << point3 << line_colour << line_thickness << fill_colour << endl;
	}
	virtual void render_gradient_triangle(Vector2d point1, Vector2d point2, Vector2d point3, ColourRGB line_colour, int line_thickness, ColourRGB colour1, ColourRGB colour2, ColourRGB colour3){
		
		//get the boundary box of the triangle
		int max_X = max(point1.x, max(point2.x,point3.x));
		int min_X = min(point1.x, min(point2.x,point3.x));
		int max_Y = max(point1.y, max(point2.y,point3.y));
		int min_Y = min(point1.y, min(point2.y,point3.y));

		//cout << "** minX->maxX minY->maxY" << min_X << max_X << min_Y << max_Y << endl;
		
		Vector2d vs2 = point2 - point1;//vector from p1 to p2
		Vector2d vs3 = point3 - point1;//vector from p1 to p3
		float denominator = ((vs3.dot(vs3)) * (vs2.dot(vs2))) - ((vs3.dot(vs2)) * (vs2.dot(vs3))) ;
		ColourRGB c;//this is for the gradient colour
		//scan all points that are in the boundary box
		for(int i=min_X; i<=max_X; i++){
			for(int j=min_Y; j<=max_Y; j++){
				Vector2d temp;
				temp.x = i;
				temp.y = j;

			 	Vector2d vs1 = temp - point1;//vector from p1 to Q
				
				float s = ( ((vs3.dot(vs3)) * (vs1.dot(vs2))) - ((vs3.dot(vs2)) * (vs1.dot(vs3))) ) / denominator;
				float t = ( ((vs2.dot(vs2)) * (vs1.dot(vs3))) - ((vs2.dot(vs3)) * (vs1.dot(vs2))) ) / denominator;
				
				//if the point is in the triangle
				if((s>=0) && (t>=0) && (s+t<=1)){
					
					int g = s*colour1[0]+ s*colour2[0] + s*colour3[0];
					int b = t*colour1[0]+ t*colour2[0] + t*colour3[0];
					int r = (1-s-t)*(colour1[0]+ colour2[0] + colour3[0]);
					c.setRGB(r,g,b);
					canvas[i][j] = c;
				}
			}
		}

		//cout << "***." << colour1<<colour2<<colour3<< endl;

		render_line(point1, point2, line_colour, line_thickness);
		render_line(point2, point3, line_colour, line_thickness);
		render_line(point3, point1, line_colour, line_thickness);
		cout << "Triangle " << point1 << point2 << point3 << line_colour << line_thickness << colour1 << colour2 << colour3 << endl;
	}
	
	void save_image(string filename){
		canvas.save_image(filename);
	}
private:
	PNG_Canvas canvas;
	int width,height;
};

int main(int argc, char** argv){
	if (argc < 3){
		cerr << "Usage: " << argv[0] << " <input file> <output file>" << endl;
		return 0;
	}
	string input_filename = argv[1];
	string output_filename = argv[2];
	
	BVGRenderer r;
	BVGReader reader(r);
	if (!reader.parse_file(input_filename)){
		cerr << "Unable to parse file" << endl;
		return 0;
	}

	//call function
	//render_line();

	r.save_image(output_filename);
	
	
}
