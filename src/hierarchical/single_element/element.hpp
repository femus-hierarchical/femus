#ifndef ELEMENT_HPP
#define ELEMENT_HPP
#include "basis.hpp"
#include "basish.hpp"
#include <string>
#include <algorithm>
#include <stdexcept>
#include "main.hpp"

using std::vector;
namespace single_h_femus{
class element{
  
private:
  vector <int> referenceElementType;
  vector <int> o;
  
public:
  void SetOrientationFlagsAndType(unsigned ElementType, int* globalIndex);

  int* o_(){
    return &o[0];
  }
    // TODO see how these functions should be defined
  void SetFacetAdjacentElemIndex(const unsigned &iface, const int &value){
  }
  
  void SetElementNodeGlobalIndex(const unsigned& inode, const unsigned& value){
  }
  
  int FacetAdjacentElemIndex(const unsigned &iface) const{
  }
  int* type_(){
    return &referenceElementType[0];
  }
};
}
#endif