#ifndef HIERARCHICAL_ALGORITHM_H
#define HIERARCHICAL_ALGORITHM_H

#include<vector>
#include<map>
#include<set>
#include<algorithm>
#include<iostream>
#include<cassert>

namespace Algorithms 
{

  namespace internal_types
  {

    template < class K>
    struct _heap_record_less
    {
      typedef K                                                  Kernel;
      typedef typename Kernel::Cluster                           Cluster;
      typedef typename Kernel::Distance_pair                     Distance_pair;

      typedef int                                                Heap_index;
      typedef std::pair<Cluster, Heap_index>                     Set_record;

      typedef std::map<Cluster, Heap_index>                      Individual_cluster_map;
      typedef std::map< Cluster , Individual_cluster_map >       Cluster_map;

      typedef typename Individual_cluster_map::iterator          ICM_iterator;
      typedef typename Cluster_map::iterator                     CM_iterator;

      typedef std::pair<ICM_iterator,ICM_iterator>               ICM_iterator_pair;
      typedef std::pair<Distance_pair,ICM_iterator_pair>         Heap_record;


      _heap_record_less()
      {}

      bool
      operator()( const Heap_record& a, const Heap_record& b )
      {
         if(a.first.cannot_link()== true && b.first.cannot_link()==false)
           return true;

         if(a.first.cannot_link()== false && b.first.cannot_link()==true)
           return false;

         return a.first.distance()> b.first.distance();
      }
    };

  } //internal_types

template <class K , class Constr_tag = typename K::Tag_false>
class Hierarchical_algorithm 
{   

  typedef K                                             Kernel;
  typedef Constr_tag                                    Constraint_tag;
  typedef Hierarchical_algorithm<Kernel,Constraint_tag> Self;
  typedef typename Kernel::Point                        Point;
  typedef typename Kernel::Cluster                      Cluster;
  typedef typename Kernel::Distance_pair                Distance_pair;
  typedef typename Kernel::Full_distance_pair           Full_distance_pair;
  typedef typename Kernel::Constraint                   Constraint;
  typedef typename Kernel::Constraint_container         Constraint_container;

  typedef int                                           Heap_index;
  typedef std::pair<Cluster, Heap_index>                Set_record;

  typedef std::map<Cluster, Heap_index>                 Individual_cluster_map; 
  typedef std::map< Cluster , Individual_cluster_map >  Cluster_map;
 
  typedef typename Individual_cluster_map::iterator      ICM_iterator;
  typedef typename Cluster_map::iterator                 CM_iterator;

  typedef std::pair<ICM_iterator,ICM_iterator>          ICM_iterator_pair;
  typedef std::pair<Distance_pair,ICM_iterator_pair>    Heap_record;                                


  typedef std::vector<Heap_record>                      Heap_container;

  typedef typename Heap_container::iterator             HC_iterator;

  typedef std::set<Point>                               Point_set;

  // Member_objects

  Heap_container    _heap;
  Cluster_map       _cmap;
  int               _virtual_heap_size;
  HC_iterator       _virtual_heap_end_iterator;
  bool              _no_further_agglomeration;
  bool              _has_constraints;

public:

   
  Hierarchical_algorithm():_virtual_heap_size(0)
  {}


  //Initialization for cases without constraints
  Hierarchical_algorithm( std::vector<Point>& points)
  {
    _no_further_agglomeration=false;
    _has_constraints=false;

    _short_and_check_for_duplicated_points(points);


    std::vector<CM_iterator> vec_cm_it;


    for(int h=0; h< points.size() ; ++h)
    {
      vec_cm_it.push_back(
                           _cmap.insert( _cmap.end(),
                                         std::make_pair( Cluster(points[h]), Individual_cluster_map())));
    }

    for(int i=0; i< points.size() ; ++i)
    {
      Cluster cl(points[i]);

      for(int j=i+1; j< points.size() ; ++j)
      {
        Cluster  new_cl(points[j]);
        Distance_pair dist_p(cl,new_cl);

        ICM_iterator ics_it1=vec_cm_it[i]->second.insert( vec_cm_it[i]->second.end(),std::make_pair(new_cl,-1)),
                     ics_it2=vec_cm_it[j]->second.insert( vec_cm_it[j]->second.end(),std::make_pair(cl,-1));

        _heap.push_back(Heap_record(std::make_pair(dist_p,std::make_pair(ics_it1,ics_it2))));
      }
    }

    std::make_heap( _heap.begin(), _heap.end(),
                    typename internal_types::_heap_record_less<Kernel>());

    for(int k=0; k<_heap.size(); ++k)
    {
      (*_heap[k].second.first).second=k;
      (*_heap[k].second.second).second=k;
    }

    _virtual_heap_size = _heap.size();
    _virtual_heap_end_iterator = _heap.end();

  }



  //Initialization for cases with constraints
  Hierarchical_algorithm( std::vector<Point>& points, 
                          const Constraint_container& must_link,
                          const Constraint_container& cannot_link)
  {
    if(must_link.size()==0 && cannot_link.size()==0)
    {
      (*this)= Hierarchical_algorithm(points);
      return;
    }


    _short_and_check_for_duplicated_points(points);

    _has_constraints=true;
    _no_further_agglomeration=false;

    std::vector<Cluster> initial_clusters;
    std::set<Distance_pair> cannot_link_distances;

    _build_up(points,must_link,cannot_link,
              initial_clusters,cannot_link_distances); 


    std::vector<CM_iterator> vec_cm_it;
     
    for(int h=0; h< initial_clusters.size() ; ++h)
    {
      std::pair<Cluster,Individual_cluster_map> pp = std::make_pair( initial_clusters[h], Individual_cluster_map());

      CM_iterator cccc = (_cmap.insert(pp).first);

      vec_cm_it.push_back(cccc);
    }


    for(int i=0; i< initial_clusters.size() ; ++i)
    {
      typename std::set<Distance_pair>::iterator dist_it,dist_it2;
      Cluster cl(initial_clusters[i]);

      for(int j=i+1; j< initial_clusters.size() ; ++j)
      {
        Cluster  new_cl(initial_clusters[j]);
        Distance_pair dist_p;

        dist_it=cannot_link_distances.find(Distance_pair(cl,new_cl));
        dist_it2=cannot_link_distances.find(Distance_pair(cl,new_cl));

        if(dist_it!=cannot_link_distances.end())
          dist_p=(*dist_it);
        else if(dist_it2!=cannot_link_distances.end())
          dist_p=(*dist_it2);
        else
          dist_p=Full_distance_pair()(cl,new_cl);

        ICM_iterator ics_it1=vec_cm_it[i]->second.insert( std::make_pair(new_cl,-1)).first,
                     ics_it2=vec_cm_it[j]->second.insert( std::make_pair(cl,-1)).first;
        
        _heap.push_back(Heap_record(std::make_pair(dist_p,std::make_pair(ics_it1,ics_it2))));        
      }
    }    

    std::make_heap( _heap.begin(), _heap.end(), 
                    typename internal_types::_heap_record_less<Kernel>());

    for(int k=0; k<_heap.size(); ++k)
    {
      (*_heap[k].second.first).second=k;
      (*_heap[k].second.second).second=k;
    }

    _virtual_heap_size = _heap.size();
    _virtual_heap_end_iterator = _heap.end();

  }

  std::pair<Cluster,Cluster>
  pop_head()
  {
    if(_has_constraints)
      return _pop_head_with_constraints();

    return _pop_head_no_constraints();
  }

  void
  operator()(void)
  {
    while(!no_further_agglomeration())
      pop_head();
  }  

  bool
  no_further_agglomeration(void)
  { return _no_further_agglomeration || size()==1; }

  int
  size()
  { return _virtual_heap_size+1;}

  bool
  empty()
  { return _heap.empty();}

private:

  std::pair<Cluster,Cluster>
  _pop_head_with_constraints()
  {

    assert(_virtual_heap_size>0);

    Heap_record head=_heap[0];

    if(head.first.cannot_link())
    {
      _no_further_agglomeration = true;
      return std::make_pair(Cluster(),Cluster());
    }

    _delete_heap_node(0);

    std::vector<Distance_pair> dist_vec;
    Individual_cluster_map ind_cl_set;

    //std::cout << " The new clusters to be unified are :" << std::endl
    //          <<  head.first.first() << " and \n" << head.first.second() << std::endl; 

    Cluster new_cl(head.first.first(),head.first.second());
    int size1 = head.first.first().size();

    CM_iterator cm_it1 = _cmap.find(head.first.first()),
                cm_it2 = _cmap.find(head.first.second());

    assert( cm_it1->second.size() == cm_it2->second.size() );

    CM_iterator cur_it = _cmap.insert(std::make_pair(new_cl,Individual_cluster_map())).first;

    ICM_iterator icm_it1 = cm_it1->second.begin(); 
    ICM_iterator icm_it2 = cm_it2->second.begin(); 

    while(!(cm_it1->second.empty()))
    {
      icm_it1 = cm_it1->second.begin(); 
      icm_it2 = cm_it2->second.begin(); 

      if(icm_it1->first==icm_it2->first)
      {         

        Cluster related=icm_it1->first;

        Heap_record cl1 = _delete_heap_node(icm_it1->second),
                    cl2 = _delete_heap_node(icm_it2->second);


        Distance_pair new_p(new_cl,related,cl1.first.distance(),cl2.first.distance(),size1,
                            (cl1.first.cannot_link()||cl2.first.cannot_link()));


        ICM_iterator ic_n1 = cur_it->second.insert(std::make_pair(related,-1)).first,
                     ic_n2 =_cmap.find(related)->second.insert(std::make_pair(new_cl,-1)).first;

        Heap_index  new_ind = _insert_heap_node( Heap_record(std::make_pair(new_p,
                                                             std::make_pair(ic_n1,ic_n2))));

        ic_n1->second = new_ind;
        ic_n2->second = new_ind;


      }
      else
        assert(false);
    }

    _cmap.erase(cm_it1);
    _cmap.erase(cm_it2);

    return std::make_pair(head.first.first(),head.first.second());
  }

  std::pair<Cluster,Cluster>
  _pop_head_no_constraints()
  {
    assert(_virtual_heap_size>0);


    Heap_record head=_heap[0];

    
    _delete_heap_node(0);

    std::vector<Distance_pair> dist_vec;
    Individual_cluster_map ind_cl_set;

    //std::cout << " The new clusters to be unified are :" << std::endl
    //          <<  head.first.first() << " and \n" << head.first.second() << std::endl;

    Cluster new_cl(head.first.first(),head.first.second());
    int size1 = head.first.first().size();

    CM_iterator cm_it1 = _cmap.find(head.first.first()),
                cm_it2 = _cmap.find(head.first.second());

    assert( cm_it1->second.size() == cm_it2->second.size() );

    CM_iterator cur_it = _cmap.insert(std::make_pair(new_cl,Individual_cluster_map())).first;

    ICM_iterator icm_it1 = cm_it1->second.begin();
    ICM_iterator icm_it2 = cm_it2->second.begin();


    while(!(cm_it1->second.empty()))
    {
      icm_it1 = cm_it1->second.begin();
      icm_it2 = cm_it2->second.begin();


      if(icm_it1->first==icm_it2->first)
      {

        Cluster related=icm_it1->first;

        Heap_record cl1 = _delete_heap_node(icm_it1->second),
                    cl2 = _delete_heap_node(icm_it2->second);


        Distance_pair new_p(new_cl,related,cl1.first.distance(),cl2.first.distance(),size1);


        ICM_iterator ic_n1 = cur_it->second.insert(std::make_pair(related,-1)).first,
                     ic_n2 =_cmap.find(related)->second.insert(std::make_pair(new_cl,-1)).first;

        Heap_index  new_ind = _insert_heap_node( Heap_record(std::make_pair(new_p,
                                                             std::make_pair(ic_n1,ic_n2))));

        ic_n1->second = new_ind;
        ic_n2->second = new_ind;


      }
      else
        assert(false);
    }


    _cmap.erase(cm_it1);
    _cmap.erase(cm_it2);

    return std::make_pair(head.first.first(),head.first.second());
  }

  void
  _short_and_check_for_duplicated_points( std::vector<Point>& points)
  {
    std::list<Point>  temp_list;
    std::sort(points.begin(),points.end());

    if(points.size()==0)
    {
      std::cout<< " No points read from the input. Terminating ... " << std::endl;
      exit(-1);
    }

    Point tmp_point=points[0];
    temp_list.insert(temp_list.end(),tmp_point);

    for(int li=1; li<points.size(); ++li)
    {
      if(tmp_point!=points[li])
      {  
        temp_list.insert(temp_list.end(),points[li]);      
        tmp_point=points[li];
      }
    }

    points.clear();


    for( typename std::list<Point>::iterator lit=temp_list.begin(); lit!=temp_list.end(); lit++ )
      points.push_back((*lit));
  }

  void
  _build_up( std::vector<Point>& points,
             const Constraint_container& must_link,
             const Constraint_container& cannot_link,
             std::vector<Cluster>& clusters,
             std::set<Distance_pair>& distances
           )
  {
    typedef typename std::list<Point_set>::iterator  clust_it;

    std::list<Point_set>  all_sets;
    typename Constraint_container::iterator  cit;

    for(int i=0; i<points.size(); ++i)
    {
      Point_set ps;
      ps.insert(ps.begin(),points[i]);
      all_sets.insert(all_sets.end(),ps);
    }


    for(cit=must_link.begin(); cit!=must_link.end(); ++cit)
    {

      std::pair<clust_it,clust_it> set_ind = _find_point_sets((*cit),all_sets);

      if(set_ind.first!=set_ind.second)
      {
        if(_able_to_link((*(set_ind.first)),(*(set_ind.second)),cannot_link))
        {
          Point_set new_ps=_merge_point_sets(*(set_ind.first),*(set_ind.second));


          all_sets.erase(set_ind.first);
          all_sets.erase(set_ind.second);

          all_sets.insert(all_sets.end(),new_ps);
        }
        else
        { 
          std::cout << " The input set of constraints does not lead to any solution" << std::endl;
          exit(-1);
        }
      }
    }


    for(cit=cannot_link.begin(); cit!=cannot_link.end(); ++cit)
    {    
      std::pair<clust_it,clust_it> set_ind = _find_point_sets((*cit),all_sets);
      
      Distance_pair temp=Full_distance_pair()((*(set_ind.first)),(*(set_ind.second)),true);
      distances.insert(temp);
    }


    for(clust_it cl_it=all_sets.begin(); cl_it!=all_sets.end(); ++cl_it)
    {
      typename Point_set::iterator ps_it;
      Cluster cl(*(cl_it->begin()));

      for(ps_it=cl_it->begin(); ps_it!=cl_it->end(); ++ps_it)
        if(ps_it!=cl_it->begin())
          cl.insert(*ps_it);

      clusters.push_back(cl);
    }
  }
  
  std::pair < typename std::list<Point_set>::iterator,
              typename std::list<Point_set>::iterator > 
  _find_point_sets( const Constraint& cnt,
                    std::list< Point_set >&  all_sets )
  {
    typename std::list< Point_set >::iterator it,fir,sec;
    int num=0;

    for(it=all_sets.begin(); it!=all_sets.end(); ++it)
    {
      std::set<Point> cur = (*it);
 
      if(cur.find(cnt.first)!=cur.end() ||
         cur.find(cnt.second)!=cur.end()  )
      {
        if(num==0)
          fir=it;
        else
        {
          sec=it;
          return std::make_pair(fir,sec);
        }

        num++;
      }

    }
  }

  bool
  _able_to_link( Point_set& ps1, 
                 Point_set& ps2,
                 const Constraint_container&  cannot_link)
  {
    typename Point_set::iterator it1,it2;

    for(it1=ps1.begin(); it1!=ps1.end(); it1++)
      for(it2=ps2.begin(); it2!=ps2.end(); it2++)
      {
        if( cannot_link.find(std::make_pair(*it1,*it2))!=cannot_link.end() ||
            cannot_link.find(std::make_pair(*it2,*it1))!=cannot_link.end()   )
          return false;
      }

    return true;
  }

  Point_set
  _merge_point_sets( Point_set ps1, Point_set ps2)
  {
    typename Point_set::iterator it1=ps1.begin(),
                                 it2=ps2.begin();
 
    Point_set new_ps;

    while(it1!=ps1.end() && it2!=ps2.end())
    {
      if((*it1)<(*it2))
      {
        Point new_point= *it1;

        new_ps.insert(new_ps.end(),new_point);
        it1++;
      }  
      else
      {
        Point new_point= *it2;

        new_ps.insert(new_ps.end(),new_point);
        it2++;
      }  
    }

    while(it1!=ps1.end())
    {
      Point new_point= *it1;

      new_ps.insert(new_ps.end(),new_point);
      it1++;
    }

    while(it2!=ps2.end())
    {
      Point new_point= *it2;

      new_ps.insert(new_ps.end(),new_point);
      it2++;
    }
      
    return new_ps;
  }

  Heap_index
  _insert_heap_node( const Heap_record& hp_rec)
  {
    _heap[_virtual_heap_size]=hp_rec;

    Heap_index current=_virtual_heap_size;

    _virtual_heap_size++;
    _virtual_heap_end_iterator++;

    while(current > 0)
    {
      Heap_index parent = _find_parent(current);
       
      if(_heap[parent].first.distance()>_heap[current].first.distance())
      {
        Heap_record tmp= _heap[parent];

        // Updating map records
        (*(_heap[parent].second.first)).second = current;
        (*(_heap[parent].second.second)).second = current;

        //Exchange father-child node
        _heap[parent] = _heap[current];
        _heap[current] = tmp;

        current = parent;
  
      }
      else 
      {
        (*(_heap[current].second.first)).second = current;
        (*(_heap[current].second.second)).second = current;

        return current;
      }
    }
   
    return current;

  }


  Heap_record
  _delete_heap_node( const Heap_index& ind)
 {

    Heap_record tmp= _heap[ind];
    _heap[ind] = _heap[_virtual_heap_size-1];
    _virtual_heap_size--;
    _virtual_heap_end_iterator--;

    _restore_heap(ind);

    //Update map
    _cmap.find(tmp.first.first())->second.erase(tmp.second.first);
    _cmap.find(tmp.first.second())->second.erase(tmp.second.second);

    return tmp;
  }

  void
  _restore_heap( const Heap_index& ind)
  {
    Heap_index current = ind;

    while( current < _virtual_heap_size )
    {
      Heap_index child1 = (2*current)+1,
                 child2 = child1+1,
                 chosen_one;

      if( child1 >= _virtual_heap_size && child2 >= _virtual_heap_size )
      {
        (*(_heap[current].second.first)).second = current;
        (*(_heap[current].second.second)).second = current;
        return;
      }

      if(child2 >= _virtual_heap_size)
        chosen_one = child1;
      else if( _heap[child1].first.distance() <= _heap[child2].first.distance() )
        chosen_one = child1;
      else
        chosen_one = child2;
  
      if( _heap[chosen_one].first.distance() < _heap[current].first.distance() )
      {  
        Heap_record tmp= _heap[chosen_one];

        // Update map records    
        (*(_heap[chosen_one].second.first)).second = current;
        (*(_heap[chosen_one].second.second)).second = current;

        //Exchange parent-child node
        _heap[chosen_one] = _heap[current];
        _heap[current] = tmp;

        current = chosen_one;
      }
      else
      {
        (*(_heap[current].second.first)).second = current;
        (*(_heap[current].second.second)).second = current;
        return;
      }
    }
  }

  Heap_index
  _find_parent( const Heap_index& a)
  {
    if(a%2==0)
      return a/2-1;

      return (a-1)/2;
  }

  void
  _print_heap()
  {
       for(int ii=0; ii<_virtual_heap_size; ii++)
       std::cout << " HEAP[" << ii <<"] = " << _heap[ii].first.distance()
                 << " , for clusters : " << (_heap[ii].second.first->first) 
                 << " with heap index = " << (_heap[ii].second.first->second) << std::endl
                 << " and " << (_heap[ii].second.second->first)  
                 << " with heap index = " << (_heap[ii].second.second->second) << std::endl;

  }

  void
  _print_map()
  {
    for(CM_iterator jj=_cmap.begin(); jj!=_cmap.end(); ++jj)
    { 
      std::cout << " Cluster " << jj->first << " has members:" <<std::endl;

      for(ICM_iterator kk=jj->second.begin(); kk!=jj->second.end(); ++kk)
         std::cout << "   Cluster " << kk->first << " on index : " <<  kk->second << std::endl;        
    }

  }

};

} //Algorithms


#endif //HIERARCHICAL_ALGORITHM_H
