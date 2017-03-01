#include<iostream>
#include"Clustering_kernel.h"
#include"Tags.h"

typedef Tag_types::Average                      Metric_tag;
typedef Clustering_kernel<double,Metric_tag>    Kernel;
typedef Kernel::Point                           Point;
typedef Kernel::Hierarchical_algorithm          Hierarchical_algorithm;
typedef Kernel::Points_in_disc                  Points_in_disc;
typedef Kernel::Dendrogram                      Dendrogram;


void display(void);
void reshape ( int w ,int h);

Dendrogram dg;

int main(int argc, char* argv[])
{
  std::vector<Point>  vec;  

  Points_in_disc pid(Point(5.3,6.2),7.3,223);

  pid(std::back_inserter(vec),7);

  Hierarchical_algorithm hier(vec);

  dg = Dendrogram(hier);

     glutInit(&argc,argv);

     glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );

     glutInitWindowSize(700,750);
     glutInitWindowPosition(10,10);
     glutCreateWindow("The dendrogram");
     dg.init();
     glutDisplayFunc(display);
     glutReshapeFunc(reshape);

     glutMainLoop();


  return 0;
}

void
display(void)
{
  dg.display();
}

void reshape ( int w ,int h)
{
  glViewport( 0 , 0 , (GLsizei) w , (GLsizei) h );
  glOrtho(-5.0+dg.min_x(),5.0+dg.max_x(),-5.0+dg.min_y(),5.0+dg.max_y(),-1.0,1.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
}

