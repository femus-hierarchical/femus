/**
* Test integration and visualization on a single mesh element
*/

//TODO make DIM automatically taken from reading mesh.

#include "main.hpp" // change this to hierarchical.hpp
#include "elem_type.hpp"

using namespace single_h_femus;

int main(int argc, char **argv){
  // Element info
  int gaussOrder = 18;
  unsigned p = 5;
  unsigned type = 0;
  int* s; //sign
  int* t; //type

//   // integration
//   elem_type* e = new elem_type(type, p, gaussOrder, 1);
//   unsigned DOF = e->ElementDOF();
//
//   double A[DOF][DOF]; // define A
//
//   double* phi;
//   phi = new double[DOF];
//
//   double** gradphi;
//   gradphi = new double*[DOF];
//   for(unsigned c=0; c<DOF; c++)
//     gradphi[c] = new double[3];
//
//   double* X = new double[3];
//   double Weight;
//   vector < vector <double> > vertices; //vertices of the element
//   vertex.resize(el->ElementNodeNum(kel,0));
//
//     /* vertices of the element */
//     int vIndex[NumberOfVertices];
//     for(unsigned i=0;i<NumberOfVertices;i++){
// 	vertices[i].resize(3);
// 	vIndex[i] = el->GlobalNodeIndex(i)-1u;
// 	vertices[i][0] = vt[0][vIndex[i]];
// 	vertices[i][1] = vt[1][vIndex[i]];
// 	vertices[i][2] = vt[2][vIndex[i]];
//     }
//
//     s = e.sign(vertices);// call function from orientation.cpp
//     t = e.type(vertices);
//
//   // Construct Stiffness matrix: A
//    for(unsigned ig=0; ig < e->NumOfGaussPts(); ig++){
// 	e->spatialToRefPtr(verices,ig,s,Weight,phi,gradphi,X,t);
//
// 	/* Laplace operator */
// 	for(unsigned i=0; i<DOF; i++)
// 	  for(unsigned j=0; j<DOF; j++)
// 	    A[i][j] += (gradphi[i][0]*gradphi[j][0] + gradphi[i][1]*gradphi[j][1] + gradphi[i][2]*gradphi[j][2])*Weight;
//     }

  // define f
  // Construct Load vector: f

  // boundary condition

  // Solve (direct solve)

  // visualization

return 0;
}

