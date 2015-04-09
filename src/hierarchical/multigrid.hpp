#ifndef MULTIGRID_HPP
#define MULTIGRID_HPP

#include "lsysPDE.hpp"

namespace hierarchical_femus{

class elem_type;

class MultiGrid{ 
  
  typedef bool     (*FunctionBoundary)   (const double& x, const double& y, const double& z,const char name[], double& value, const int FaceName);
  typedef double   (*FunctionInit)    	 (const double& x, const double& y, const double& z,const char name[]);
  typedef unsigned (*FunctionlocalP) 	 (const double& x, const double& y, const double& z, unsigned& p);
  
private:
  vector <lsysPDE*> D; // Ku=f at each level
  vector <vector <double> > vt; //grid vertex, vy, vz
  vector <vector <double> > LNode; // Lagrange nodes in the projected mesh.
  vector <vector <double> > Sol; //solution vector (u)
  vector <vector <double> > LSol; //projected solution to Lagrange basis
  vector <vector <unsigned short> > BC; // boundary conditions
  vector <vector <unsigned> > Cell; // h-refined cells
  vector < unsigned> CellType; // h-refined cell types
  vector < map <unsigned,bool> > index;
  vector <int> SolType;
  vector <char*> SolName;
  vector <unsigned> MGIndex;
  
  vector <lsysPDE*> D_old;
  vector <vector <double> > vt_old;
  vector <vector <double> > Sol_old;
  vector <unsigned> elr_old;
  unsigned maxOrder;
  unsigned p;
  unsigned lagrangeP;
  bool time_test;
  unsigned short gridn, gridr;
  vector < vector <const elem_type*> > type_elem; 
  vector <unsigned> DOF; // Number of degrees of freedom at each grid level.
  vector <unsigned> DofstoSolve; //number of dofs to solve at each level
  
  FunctionBoundary _BoundaryFunction;
  FunctionInit     _InitFunction;
  FunctionlocalP   _localPFunction;  
  
public:
  MultiGrid(const char mesh_file[], const unsigned short& gridP, 
	    FunctionBoundary BoundaryPointerOther, FunctionInit InitFunctionOther, FunctionlocalP localPOther);
  
  ~MultiGrid();

  void setDOF(); // sets the number of degrees of freedom at each grid level.
  void solve(const unsigned short &time); 
  void GenerateBC(const char name[]); //const unsigned short& fGrid, const unsigned short& cGrid=0
  void generate_vt(const unsigned short &grid_end, const unsigned short& grid_start=1);
  
  void printVtkHierarchical(const int type=0);
  void Print_vtk_ASCII(const unsigned& time, const int type=0);
  void L2Error() const;
  void GenerateLagrangeSolution2D(int LagrangeOrder);
  void GenerateLagrangeSolution3D(int LagrangeOrder);
  int LagrangeHRefinement2D();
  int LagrangeHRefinement3D();
  
  void AddSolutionVector(const char name[], const unsigned order=0); 
  void ResizeSolutionVector( const char name[]);
  void CheckVectorSize(const unsigned &i);
  void Initialize(const char name[]);
  unsigned GetIndex(const char name[]);
  
  void ClearMGIndex();
  void AddToMGIndex(const char name[]);
  void InitMultigrid();

  int Restrictor(unsigned gridf);
  int Prolongator(unsigned gridf);
  void ProlongatorSol(unsigned gridf);

  int BuildProlongatorMatrix(unsigned gridf);
};

}
#endif
