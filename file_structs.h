#ifndef FILE_STRUCTS_H_
#define FILE_STRUCTS_H_

#include<vector>
#include<fstream>

template < class K>
class Read_file
{

      typedef K                                       Kernel;
      typedef typename Kernel::Point                  Point;
      typedef typename Kernel::Number_type            NT;
      typedef typename Kernel::Constraint             Constraint;
      typedef typename Kernel::Constraint_container   Constraint_container;

public:

      Read_file()
      {}


      void
      operator()(char* filename, std::vector<Point>& vec,
                 Constraint_container& must_link,
                 Constraint_container& cannot_link)
     {
        _read(filename,vec,must_link,cannot_link);
     }

private:

    void
    _read(char* filename, std::vector<Point>& vec,
          Constraint_container& must_link,
          Constraint_container& cannot_link )
    {
      char temp_ch;
      std::ifstream input_f(filename);

      int p_size,m_size,c_size,
          temp_a,temp_b;

      input_f >> p_size;

      for(int i=0; i<p_size; i++)
      {
        input_f >> temp_a;
        input_f >> temp_b;

        vec.push_back(Point(NT(temp_a),NT(temp_b)));
      }   

      if(input_f.eof())
        return;

      if(!(input_f >> m_size))
        return;

      for(int j=0; j<m_size; j++)
      {
        input_f >> temp_a;
        input_f >> temp_b;
         
        if(temp_a<1 || temp_b<1 ||temp_a>vec.size() || temp_b > vec.size())
        {
          std::cout << " Constraint indices are out of bounds. Terminating..." << std::endl;
          exit(-1);
        }

        must_link.insert(std::make_pair(vec[temp_a-1],vec[temp_b-1]));
      }   

      if(!input_f)
        return;


      if(!(input_f >> c_size))
        return;


      std::cout << "c_size = " <<  c_size <<std::endl;

      for(int k=0; k<c_size; k++)
      {
        input_f >> temp_a;
        input_f >> temp_b;

        if(temp_a<1 || temp_b<1 ||temp_a>vec.size() || temp_b > vec.size())
        {
          std::cout << " Constraint indices are out of bounds. Terminating..." << std::endl;
          exit(-1);
        }

        cannot_link.insert(std::make_pair(vec[temp_a-1],vec[temp_b-1]));
      }   


    }

};


#endif /*FILE_STRUCTS_H_*/
