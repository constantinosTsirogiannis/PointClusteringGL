#ifndef DENDROGRAM_H
#define DENDROGRAM_H

#include<GL/gl.h>
#include<GL/glut.h>
#include<list>
#include<vector>
#include<stack>
#include<iostream>



namespace Algorithms {

template < class K>
class Dendrogram {

   typedef K                                         Kernel;
   typedef Dendrogram<Kernel>                        Self;
   typedef typename Kernel::Point                    Point;
   typedef typename Kernel::Cluster                  Cluster;
   typedef typename Kernel::Digital_point            Digital_point;
   typedef typename Kernel::Hierarchical_algorithm   Hierarchical_algorithm;
   typedef std::pair<Cluster,Cluster>                Cluster_pair;
   typedef std::stack<Cluster_pair>                  Event_stack;
   typedef std::pair<double,double>                  Position;
   typedef std::pair<Cluster,Position>               List_record;
   typedef std::list<List_record>                    List_type;
   typedef std::vector<Point>                        Point_vector;
   typedef std::vector<Position>                     Vertex_vector;
   typedef std::pair<Point,Position>                 Digital_record;
   typedef std::vector<Digital_record>               Digit_container;

   typedef typename List_type::iterator              List_iterator;
  
   List_type                      _list;
   Event_stack                    _stack;
   Hierarchical_algorithm*        _hier;   
   double                         _min_x,_min_y,
                                  _max_x,_max_y;
   int                            _n;
   Vertex_vector                  _vertices;
   Digit_container                _digp;    

public:  

   Dendrogram()
   {}

   Dendrogram(Hierarchical_algorithm& hier)
   { 
     _hier=&hier;
     _n= _hier->size();
     _max_x=double(_n),
     _max_y=double(_n);
     _execute_algo();
     _construct_tree();    
   }

   void
   init()
   {     
            _min_x = _find_min_x(),
            _max_x = _find_max_x(),
            _min_y = _find_min_y(),
            _max_y = _find_max_y();
  

     glOrtho(-5.0+_min_x,5.0+_max_x,
             -5.0+_min_y,5.0+_max_y,-1.0,1.0);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glClearColor(1.0,1.0,1.0,0.0);
   }

   void
   display()
   {
     glClear(GL_COLOR_BUFFER_BIT);

     glColor3f(0.3,0.0,0.7);

     for(int i=0; i<_vertices.size(); i+=2)
     {
       glBegin(GL_LINES);
       glVertex2f(_vertices[i].first,_vertices[i].second);
       glVertex2f(_vertices[i+1].first,_vertices[i+1].second);
       glEnd();
     }

     for(int j=0; j<_digp.size(); j++)
     {
       Digital_point dp;
       Point p=_digp[j].first;
       Position pos=_digp[j].second;

       dp(p,0.2,pos.first-0.25,pos.second-0.25);
     }

   }

   double
   min_x()
   {return _min_x;}

   double
   min_y()
   {return _min_y;}

   double
   max_x()
   {return _max_x;}

   double
   max_y()
   {return _max_y;}


private:

   void
   _construct_tree(void)
   {
     Position p = _drop_down(_max_x/2.0,_max_y);

     Cluster_pair cl_p = _stack.top();
     _stack.pop();

     Cluster all = Cluster(cl_p.first,cl_p.second);
     List_record root_node= std::make_pair(all,p);


     _list.insert(_list.end(),root_node);

     List_iterator beg=_list.begin();

     _expand_dendrogram(cl_p.first,cl_p.second,beg);

     do
     {
       Cluster_pair temp = _stack.top();
       _stack.pop();

       typename List_type::iterator it = _find_in_list(temp);

       _expand_dendrogram(temp.first,temp.second,it);

     }while(!(_stack.empty()));

   }

   void
   _execute_algo()
   {
     while(!(_hier->no_further_agglomeration()))
       _stack.push(_hier->pop_head());
   }
   

   List_iterator
   _find_in_list( Cluster_pair& pr)
   {
     List_iterator it;

     Cluster  together(pr.first,pr.second);

     for( it = _list.begin(); it!= _list.end(); ++it)
       if( it->first.min_point() == together.min_point() )
         return it;
   }


   void
   _expand_dendrogram(const Cluster& cl1, const Cluster& cl2, List_iterator& it)
   {
     int length = cl1.size() + cl2.size();
     double x1,x2;

     Position p=it->second;

     x1 = double(length)/2.0-double(cl1.size()/2.0);
     x2 = double(length)/2.0-double(cl2.size()/2.0);

     //Position cur1 = _move_left(p.first,p.second,cl1.size());
     Position cur1 = _move_left(p.first,p.second,x1);

              cur1 = _drop_down(cur1.first,cur1.second);

     //Position cur2 = _move_right(p.first,p.second,cl2.size());
     Position cur2 = _move_right(p.first,p.second,x2);

              cur2 = _drop_down(cur2.first,cur2.second);

     if(cl1.size()>1)
     {
       List_record lr1 = std::make_pair(cl1,cur1);
       _list.insert(_list.end(),lr1);
     }
     else
      _digp.push_back(std::make_pair((*cl1.begin()),cur1));

     if(cl2.size()>1)
     {
       List_record lr2 = std::make_pair(cl2,cur2);
       _list.insert(_list.end(),lr2);
     }
     else
      _digp.push_back(std::make_pair((*cl2.begin()),cur2));
   
     _list.erase(it);
   }

   Position
   _drop_down(double x, double y, double pos=1.0)
   {
     //Position pn= std::make_pair(x,y-_max_y*10.0*pos/_n);
     Position pn= std::make_pair(x,y-pos);

     _vertices.push_back(std::make_pair(x,y));
     _vertices.push_back(pn);

     return pn;
   }

   Position
   _move_left(double x, double y, double pos=1.0)
   {
     //Position pn= std::make_pair(x-_max_x*10.0*pos/_n,y);
     Position pn= std::make_pair(x-pos,y);

     _vertices.push_back(std::make_pair(x,y));
     _vertices.push_back(pn);

     return pn;
   }

   Position
   _move_right(double x, double y, double pos=1.0)
   { return _move_left(x,y,-pos);}

   double
   _find_min_x()
   {   
     double min=_vertices[0].first;

     for(int i=1 ; i<_vertices.size(); i+=1)
       if(_vertices[i].first<min)
         min=_vertices[i].first;

     return min;
   }

   double
   _find_max_x()
   {   
     double max=_vertices[0].first;

     for(int i=1 ; i<_vertices.size(); i+=1)
       if(_vertices[i].first>max)
         max=_vertices[i].first;

     return max;
   }


   double
   _find_min_y()
   {   
     double min=_vertices[0].second;

     for(int i=1 ; i<_vertices.size(); i+=1)
       if(_vertices[i].second<min)
         min=_vertices[i].second;

     return min;
   }

   double
   _find_max_y()
   {   
     double max=_vertices[0].second;

     for(int i=1 ; i<_vertices.size(); i+=1)
       if(_vertices[i].second>max)
         max=_vertices[i].second;

     return max;
   }

};

} // Algorithms

#endif //DENDROGRAM_H
