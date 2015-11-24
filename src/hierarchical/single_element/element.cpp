// Given global numbering of vertices, calculate the type and sign flags associated with each element    
// TODO add this function to elemtype  Given global numbering, calculate the sign type.


/* Based on the orientation of the spatial element, this function stores the orientation flags and referenceElementType
 * Global indices of the vertices are given in a 0-based format
 */
#include "element.hpp"

namespace single_h_femus{

void element::SetOrientationFlagsAndType(unsigned ElementType, int* globalIndex){ 
  
  int minIndex, maxIndex;
  unsigned nNodes;
    
  // Some information needed to set face orientation
  const unsigned iFace[6][4] = {{0,1,4,5}, {1,2,5,6}, {3,2,7,6}, {0,3,4,7}, {0,1,3,2}, {4,5,7,6}}; //hex faces
  const unsigned wFace[5][4] = {{0,1,3,4},{1,2,4,5},{2,0,5,3},{0,1,2},{3,4,5}}; //wedge faces
  const unsigned possibleCombinations[8][3] = {{0,1,2},{1,0,3},{2,3,0},{3,2,1},{0,2,1},{1,3,0},{2,0,3},{3,1,2}}; //for tetrahedron
  const int signs[8][3] = {{1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},{1,1,-1},{1,-1,-1},{-1,1,-1},{-1,-1,-1}};
  
    if(ElementType == 0)
      referenceElementType.resize(6);
    else if (ElementType == 2)
      referenceElementType.resize(5);
    else {
      referenceElementType.resize(1);
      referenceElementType[0] = 0; 
    }
     
    switch(ElementType) {
      case 0: {// Hexahedron 
	  nNodes = 8;
	  o.resize(24); // edge+face orientation flags 
	  	  
	  for(int i=0;i<24;i++)
	    o[i]=1;
	  
// 	  for(unsigned i=0;i<nNodes;i++)  
// 	    globalIndex[i] = GlobalNodeIndex(iel,i)-1u;
// 	  
	  /* Edges */
	  if(globalIndex[0] > globalIndex[1])
	    o[0]=-1;
  
	  if(globalIndex[1] > globalIndex[2])
	    o[1]=-1;
  
	  if(globalIndex[3] > globalIndex[2])
	    o[2]=-1;
  
	  if(globalIndex[0] > globalIndex[3])
	    o[3]=-1;
  
	  if(globalIndex[4] > globalIndex[5])
	    o[4]=-1;
  
	  if(globalIndex[5] > globalIndex[6])
	    o[5]=-1;
  
	  if(globalIndex[7] > globalIndex[6])
	    o[6]=-1;
  
	  if(globalIndex[4] > globalIndex[7])
	    o[7]=-1;
  
	  if(globalIndex[0] > globalIndex[4])
	    o[8]=-1;
  
	  if(globalIndex[1] > globalIndex[5])
	    o[9]=-1;
  
	  if(globalIndex[2] > globalIndex[6])
	    o[10]=-1;
  
	  if(globalIndex[3] > globalIndex[7])
	    o[11]=-1;
  
  /* Faces */ 
    
  int minIndex[6][3];
  int min;
  
  // Find A
  for(int i=0; i<6; i++){
    min = globalIndex[iFace[i][0]];
    minIndex[i][0]=0;
    for(int j=1;j<4;j++)
      if(globalIndex[iFace[i][j]]<min){
	minIndex[i][0]=j;
        min = globalIndex[iFace[i][j]];
      }
  }

  int k;
  
  for(int i=0;i<6;i++){
	k=minIndex[i][0];
	minIndex[i][1] = possibleCombinations[k][1];
	minIndex[i][2] = possibleCombinations[k][2];
	
	if(globalIndex[iFace[i][minIndex[i][1]]] > globalIndex[iFace[i][minIndex[i][2]]])
	  k += 4;
	o[12+i*2] = signs[k][0];
	o[12+i*2+1] = signs[k][1];
	referenceElementType[i] = (signs[k][2]==1); // type 0: 03 = -1, type 1: 03 = +1//
      
  }
      break;
    }
  case 1:{
    nNodes = 10;
    int index[nNodes], temp[nNodes];
    int fIndex[4], adjElem[4], fTemp[4];
    //int globalIndex[nNodes];
    
    /* vertex global indices of the tetrahedron */
    for(unsigned i=0;i<nNodes;i++){   
      //globalIndex[i] = GlobalNodeIndex(i)-1u;
      index[i] = i;
    }
    
    for(unsigned i=0;i<4;i++)
      fIndex[i] = i;
  
    minIndex = std::min_element(globalIndex, globalIndex + 4)-globalIndex;
  
    /* align local index 0 with minimum global index */
    if (minIndex==3){
      index[0]=3; //vertices
      index[1]=0;
      index[3]=1;
      index[4]=7; //edges
      index[5]=6;
      index[6]=9;
      index[7]=8;
      index[8]=4;
      index[9]=5;
      fIndex[0]=3;
      fIndex[2]=0;
      fIndex[3]=2;
    }
    
    else
      while(index[0]!=minIndex){ 
	for(int i=0;i<3;i++){
  	index[i] = (index[i]+1)%3; // vertices
	index[i+4] = (index[4+i])%3+4; //edges
	index[i+7] = (index[7+i])%3+7;	
	fIndex[i+1] = (fIndex[i+1])%3+1;//faces
	}
      }
    
    maxIndex = std::max_element(globalIndex,globalIndex + 4)-globalIndex;
    int Index=0;
    while(index[Index]!=maxIndex) Index++;
    maxIndex = Index;
    
    /* align local index 3 with maximum global index by rotating the face opposite to vertex 0 */ 
    if (3 != maxIndex)
    {
      for(unsigned i=0;i<nNodes;i++)
	temp[i] = index[i];
      
      for(unsigned i=0;i<4;i++)
	fTemp[i] = fIndex[i];
    
      index[1] = temp[3];
      index[2] = temp[1];
      index[3] = temp[2];
      index[4] = temp[7];
      index[5] = temp[8];
      index[6] = temp[4];
      index[7] = temp[6];
      index[8] = temp[9];
      index[9] = temp[5];
      fIndex[0] = fTemp[1];
      fIndex[1] = fTemp[3];
      fIndex[3] = fTemp[0];
  }
   
  maxIndex = std::max_element(globalIndex,globalIndex + 4)-globalIndex;
  Index=0;
  while(index[Index]!=maxIndex) Index++;
    maxIndex=Index;
  
  if (3 != maxIndex)
  {
    for(unsigned i=0;i<nNodes;i++)
      temp[i]=index[i];
    
    for(unsigned i=0;i<4;i++)
	fTemp[i]=fIndex[i];
    
      index[1] = temp[3];
      index[2] = temp[1];
      index[3] = temp[2];
      index[4] = temp[7];
      index[5] = temp[8];
      index[6] = temp[4];
      index[7] = temp[6];
      index[8] = temp[9];
      index[9] = temp[5];
      fIndex[0] = fTemp[1];
      fIndex[1] = fTemp[3];
      fIndex[3] = fTemp[0];
  }
  
  if(globalIndex[index[1]]>globalIndex[index[2]])
    referenceElementType[0]=1;
  
  for(unsigned inode=0;inode<nNodes;inode++)
    SetElementNodeGlobalIndex(inode, globalIndex[index[inode]]+1u);
  
  for(unsigned iface=0;iface<4;iface++)
    adjElem[iface]=FacetAdjacentElemIndex(iface);
    
  for(unsigned iface=0;iface<4;iface++)
    SetFacetAdjacentElemIndex(iface,adjElem[fIndex[iface]]);
  break;
  }
  
  case 2:{//wedge
    nNodes = 6;
    o.resize(15);  //9 + 3x2 
    
    for(int i=0;i<15;i++)
	o[i]=1;
    
    //int globalIndex[nNodes];
    //cout<<"global indices"<<endl;
    
//     for(unsigned i=0;i<nNodes;i++) {
//       globalIndex[i] = GlobalNodeIndex(iel,i)-1u;
//     //cout<<globalIndex[i]<<"\t";
//     }
      if(globalIndex[0] > globalIndex[1])
	o[0]=-1;
  
      if(globalIndex[1] > globalIndex[2])
	o[1]=-1;
  
      if(globalIndex[2] > globalIndex[0])
	o[2]=-1;
  
      if(globalIndex[3] > globalIndex[4])
	o[3]=-1;
      
      if(globalIndex[4] > globalIndex[5])
	o[4]=-1;
  
      if(globalIndex[5] > globalIndex[3])
	o[5]=-1;
  
      if(globalIndex[0] > globalIndex[3])
	o[6]=-1;
  
      if(globalIndex[1] > globalIndex[4])
	o[7]=-1;
      
      if(globalIndex[2] > globalIndex[5])
	o[8]=-1;
      
  /* Faces */ 
    
  int minIndex[5][3];
  int min;
  
  // quadrilateral faces
  for(int i=0; i<3; i++){
    min = globalIndex[wFace[i][0]];
    minIndex[i][0]=0;
    for(int j=1;j<4;j++)
      if(globalIndex[wFace[i][j]]<min){
	minIndex[i][0]=j;
        min = globalIndex[wFace[i][j]];
      }
  }
  
  int k;
  
  for(int i=0;i<3;i++){
	k = minIndex[i][0];
	minIndex[i][1] = possibleCombinations[k][1];
	minIndex[i][2] = possibleCombinations[k][2];
	
	if(globalIndex[wFace[i][minIndex[i][1]]] > globalIndex[wFace[i][minIndex[i][2]]])
	  k += 4;
	o[9+i*2]=signs[k][0];
	o[9+i*2+1]=signs[k][1];
	referenceElementType[i]=(signs[k][2]==1); // type 1: 03 = +1, type 0: 03 = -1//  
  }
  //cout<<"ref element type"<<endl;
  // triangular faces
  for(int i=3; i<5; i++){
    min = globalIndex[wFace[i][0]];
    minIndex[i][0] = 0;
    for(int j=1;j<3;j++)
      if(globalIndex[wFace[i][j]] < min){
	minIndex[i][0] = j;
        min = globalIndex[wFace[i][j]];
      }
      
    referenceElementType[i] = minIndex[i][0];
    
    if(globalIndex[wFace[i][(minIndex[i][0]+1)%3]] > globalIndex[wFace[i][(minIndex[i][0]+2)%3]])
      referenceElementType[i] += 3;
    //cout<<referenceElementType[iel][i]<<"\t";
  }
  
      
    break;
  }
  case 3:{ //quad
      nNodes = 4;
      o.resize(nNodes);  
      for(int i=0;i<nNodes;i++)
	o[i] = 1;
      
//      int globalIndex[nNodes];
	  
//       for(unsigned i=0;i<nNodes;i++)  
// 	globalIndex[i] = GlobalNodeIndex(iel,i)-1u;
	  
      if(globalIndex[0] > globalIndex[1])
	o[0]=-1;
  
      if(globalIndex[1] > globalIndex[2])
	o[1]=-1;
  
      if(globalIndex[3] > globalIndex[2])
	o[2]=-1;
  
      if(globalIndex[0] > globalIndex[3])
	o[3]=-1;
      break;
    }
  case 4:{ //triangle
      nNodes = 3;
      o.resize(nNodes); 
      for(int i=0;i<nNodes;i++)
	o[i]=1;
      
      //int globalIndex[nNodes];
      
//       for(unsigned i=0;i<nNodes;i++)  
// 	globalIndex[i] = GlobalNodeIndex(i)-1u;
      
      if(globalIndex[0] > globalIndex[1])
	o[0]=-1;
      if(globalIndex[1] > globalIndex[2])
	o[1]=-1;
      if(globalIndex[2] > globalIndex[0])
	o[2]=-1;
      break;
  }
  case 5: break;
  }
  
}

}
