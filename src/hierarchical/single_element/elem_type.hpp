/* * 
 * Evaluate function (f and df) values for each reference element
 * Create reference element projection matrix
 * */

#ifndef ELEM_TYPE_HPP
#define ELEM_TYPE_HPP
#include "basis.hpp"
#include "basish.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>
#include "main.hpp"
#include "element.hpp"

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::max;

namespace single_h_femus{
  
class elem_type: public element{

public:
  /* Note on gauss order n: if the polynomial to integrate is x^n1 y^n2 z^n3, then following rule apply:
     line, quad, hex: n = max(n1, n2, n3).  tri, tet, wedge: n = n1 + n2 + n3 */ 
  elem_type(const int solid, const int p, const int n, const int orderOfMapping, int* globalIndex);
  ~elem_type();

  void ProjectionMatrix2D(const int p_, vector <vector <double> > vertex, double** P, vector <vector <double> > & node) const;
  void ProjectionMatrix3D(const int p_, vector <vector <double> > vertex, double** P, vector <vector <double> > & node) const;
  void (elem_type::*projectionMatrix)(const int p_, vector <vector <double> > vertex, double** P, vector <vector <double> > & node) const;

  int** getIndices() const{
    return IND;
  }

  unsigned ElementDOF() const{
    return numOfShapeFuns;  
  }

  double* GaussPoint(int gaussIndex)const{
    return X[gaussIndex];
  }
  
  unsigned NumOfGaussPts() const{
    return numOfGaussPts;
  }

  double* GaussWeight() const{
    return gaussWeights;
  }

  double** Phi() const{
    return phi[0];  
  }

  double** DphiDxi() const{
    return dphidxi[0];  
  }

  double** DphiDeta() const{
    return dphideta[0];  
  }

  double** DphiDzeta() const{
    return dphidzeta[0];  
  }
  
  void mapSpatialToRefElem1D(vector<vector<double> > node, const unsigned& gaussIndex, double& weight, double* phi_, double** gradPhi, double* X) const;
  void mapSpatialToRefElem2D(vector<vector<double> > node, const unsigned& gaussIndex, double& weight, double* phi_, double** gradPhi, double* X) const;
  void mapSpatialToRefElem3D(vector<vector<double> > node, const unsigned& gaussIndex, double& weight, double* phi_, double** gradPhi, double* X) const;
  void (elem_type::*spatialToRefPtr)(vector<vector<double> > node, const unsigned& gaussIndex, double& weight, double* phi_, double** gradPhi, double* X) const;
 
  void SetSign(const int* o, const int* referenceElementType);// for given p, calculate sign and type for each set of basis functions
  
private:
  int numOfShapeFuns;
  int numOfGaussPts;
  int solid_;
  double** X;
  int** IND;
  int** I; //lagrange basis indices.
  int numOfNodes;  //modify: store in the class.
  int dim;
  basish* pt_h[6];
  single_h_femus::basis* pt_lag;// TODO change when you change namespace
  double* x;
  unsigned hierarchicP;
  vector <int> sign; // sign for each basis function for each p-element 
  vector <int> type; // type of the reference element for each element 
  
  double* gaussWeights;
  double*** phi;
  double*** dphidxi;
  double*** dphideta;
  double*** dphidzeta;
  double*** Dphi; //hierarchical basis derivatives
  double*** dphi; //lagrange basis derivatives
  double** lphi; //lagrange basis functions
 
  void error(const string& msg) const;

};
}
#endif
