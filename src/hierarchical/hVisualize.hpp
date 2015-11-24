#ifndef HVISUALIZE_HPP
#define HVISUALIZE_HPP

#include <iostream>
#include <fstream>
#include <algorithm> 
#include <vector>
#include <string>
#include "basis.hpp"
#include "element.hpp"
#include "basish.hpp"

using std::cerr;
using std::endl;
using std::ifstream;
using std::string;
using std::vector;
using std::cout;

namespace hierarchical_femus{

class hVisualize
{
  
private:
  int numberOfVertices;
  int numberOfElements;
  int numberOfDofs;
  unsigned p_l;
  unsigned spatialDim;
  unsigned maxP;
  
  vector < vector <int> > sign; // orientationFlag of each element
  vector < vector <int> > type; // type of the reference element for each element 
  vector < vector <double> > vertex; // coordinates of the vertices of each element
  vector < vector <double> > LNode; // Lagrange nodes in the projected mesh.
  vector <short> elemType;
  vector <unsigned> p; // hierarchical basis polynomial order for each element
  vector <unsigned> elemDof;
  vector < vector <unsigned> > globalDof;
  vector <double> Sol; // hierarchical basis solution
  vector <double> LSol; //projected solution to Lagrange basis
  vector < vector <const element*> > type_elem; 
  vector <vector <unsigned> > Cell; // h-refined cells
  vector < unsigned> CellType; // h-refined cell types
  string SolName;
  
  void hierarchicalToNodalProjection2D();
  void hierarchicalToNodalProjection3D();
  int pTohProjection2D();
  int pTohProjection3D();
  void readHVtk(const char hVtk_file[]);
  void setOrientationFlags();
  void setSignAndType();
  
public:
  ~hVisualize(){ 
};
hVisualize();
hVisualize(const char hVtk_file[]);
void writeVtk();

};
}
#endif
