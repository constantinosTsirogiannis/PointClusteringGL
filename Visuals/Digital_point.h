#ifndef DIGITAL_POINT_H
#define DIGITAL_POINT_H

#include<iostream>
#include<GL/gl.h>
#include<GL/glut.h>
#include"GL_Digits/1.h"
#include"GL_Digits/2.h"
#include"GL_Digits/3.h"
#include"GL_Digits/4.h"
#include"GL_Digits/5.h"
#include"GL_Digits/6.h"
#include"GL_Digits/7.h"
#include"GL_Digits/8.h"
#include"GL_Digits/9.h"
#include"GL_Digits/0.h"
#include"GL_Digits/dot.h"
#include"GL_Digits/comma.h"
#include"GL_Digits/minus.h"
#include"GL_Digits/r_par.h"
#include"GL_Digits/l_par.h"

namespace Algorithms {

template < class K>
class Digital_point {

   typedef K                                         Kernel;
   typedef typename Kernel::Point                    Point;
   typedef typename Kernel::Number_type              NT;
  
public:  

   Digital_point()
   {}

   void
   operator()(const Point& p, double s, double x, double y)
   { 
     glLineWidth(2.0);

     Digits::r_par<NT>()(s,x,y);
     std::pair<NT,NT> new_pos = _digitise(p.y(),s,x,y-1.3*s);
     x=new_pos.first;
     y=new_pos.second;
     Digits::comma<NT>()(s,x,y+s);
     new_pos = _digitise(p.x(),s,x,y-0.33*s);
     x=new_pos.first;
     y=new_pos.second;
     Digits::l_par<NT>()(s,x,y-s/2.0);

     //glLineWidth(1.0);
   }

private:

   std::pair<NT,NT>
   _digitise( NT coord, double s, double x, double y) const
   {
     NT num = coord;
     double y_pos=y;

     num*=100.0;

     int n = int(num);
     int i=0;

     while(n!=0)
     {
       ++i;
       _draw_digit(std::abs(n)%10,s,x,y_pos);
       n=n/10;
       y_pos-=1.3*s;
       
       if(i==2)
       {
         Digits::dot<double>()(s,x,y_pos+s);
         y_pos-=0.33*s;
       }
     }

     if(coord<NT(0.0))
     {
       Digits::minus<double>()(s,x,y_pos);
       y_pos-=1.3*s;
     } 

     return std::make_pair(x,y_pos);
   }

   void
   _draw_digit(int d, NT s, NT x, NT y) const 
   {
     switch(d)
     {
       case 0:  
               Digits::d0<double>()(s,x,y);
               return;
       case 1:  
               Digits::d1<double>()(s,x,y);
               return;
       case 2:  
               Digits::d2<double>()(s,x,y);
               return;
       case 3:  
               Digits::d3<double>()(s,x,y);
               return;
       case 4:  
               Digits::d4<double>()(s,x,y);
               return;
       case 5:  
               Digits::d5<double>()(s,x,y);
               return;
       case 6:  
               Digits::d6<double>()(s,x,y);
               return;
       case 7:  
               Digits::d7<double>()(s,x,y);
               return;
       case 8:  
               Digits::d8<double>()(s,x,y);
               return;
       case 9:  
               Digits::d9<double>()(s,x,y);
               return;
     }
   }

};

} // Algorithms

#endif //DIGITAL_POINT_H
