#ifndef COMMA_H
#define COMMA_H

#include<GL/gl.h>
#include<math.h>
#include<GL/glut.h>
#include <cstdlib>
#include <iostream>

  
  namespace Digits {

template < class _NT >
class comma {

  typedef _NT NT;

  public:
 
  comma(){}

  void  
  operator()( NT s , NT posx, NT posy)
  {
    NT s1=s,s2=NT(2.0/3.0)*s;

    glBegin(GL_POLYGON);
    glVertex2f(posx+2.7*s+s1*0.0,posy+s2*0.0);
    glVertex2f(posx+2.7*s+s1*0.2,posy+s2*0.0);
    glVertex2f(posx+2.7*s+s1*0.4,posy+s2*0.2);
    glVertex2f(posx+2.7*s+s1*0.0,posy+s2*0.2);
    glEnd();
  }

};

} // comma

#endif //comma
