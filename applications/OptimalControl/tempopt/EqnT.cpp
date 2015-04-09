#ifndef __eqnt_hpp__
#define __eqnt_hpp__


#include "FemusDefault.hpp"

#include "NumericVector.hpp"
#include "DenseVector.hpp"
#include "SparseMatrix.hpp"
#include "DenseMatrix.hpp"
#include "LinearEquationSolver.hpp"

#include "Files.hpp"
#include "Math.hpp"
#include "MultiLevelMeshTwo.hpp"
#include "MultiLevelProblem.hpp"
#include "SystemTwo.hpp"
#include "FETypeEnum.hpp"
#include "NormTangEnum.hpp"
#include "VBTypeEnum.hpp"
#include "GeomElTypeEnum.hpp"
#include "Domain.hpp"
#include "TimeLoop.hpp"
#include "CurrentGaussPoint.hpp"
#include "CurrentElem.hpp"
#include "OptLoop.hpp"
#include "paral.hpp"

// application
#include "TempQuantities.hpp"


// The question is: WHERE is the ORDER of the VARIABLES established?
// I mean, on one hand there is an ORDER REGARDLESS of the FE family;
//on the other hand there is an ORDER IN TERMS of FE.
// The ORDER is important for defining the BLOCKS in the VECTORS and MATRICES.
//In the initMeshToDof, i order QQ, LL and KK variables, BUT STILL YOU DO NOT KNOW
// to WHICH QUANTITIES those VARIABLES are ASSOCIATED!
// We should make a map that, for each SCALAR VARIABLE of EACH FE TYPE, 
// tells you TO WHICH QUANTITY it is ASSOCIATED.
// The point is then you have SCALAR variables associated to SCALAR QUANTITIES,
// or SCALAR VARIABLES associated to VECTOR QUANTITIES.
// So you should associate SCALAR VARIABLES to COMPONENTS of QUANTITIES.
// The first time when you start associating scalar variables to quantities
// is when you set the BC's


// ================================================================================
// Now, the idea in the construction of the ELEMENT MATRIX and ELEMENT RHS is this.
// You fill the element matrix, and each row (and column) of it will correspond
// to a certain row of the global matrix.
// HOW DO I KNOW THAT THIS CORRESPONDENCE is OK?
// Well, this is given by the el_dof_indices.
// every component of el_dof_indices tells me what is the GLOBAL ROW (and GLOBAL COLUMN)
// So, if the length of your el_dof_indices is 10, then you'll have 10x10 elements to add to the global matrix,
// given by all the possible couples "el_dof_indices[i] X el_dof_indices[j]"
// TODO: Now, how do you know that the phi of one line is the correct phi, so it is the phi of THAT DOF?
// I mean, how is the dependence on the REAL domain (NOT canonical) involved?
// For the phi, the values are the same both on the canonical and on the stretched domain.
// For the derivatives, I think the point is: you must pick the REAL dphidx in the SAME ORDER as you pick the CORRESPONDING DOFS.
// Now, my point is: on a given row, are you sure that the code picks the correct dphidx?

//NOW, PAY ATTENTION: The "iel" written as "iel=0; iel < (nel_e - nel_b);" is used for PICKING the CONNECTIVITY from the ELEMENT CONNECTIVITY MAP!
// But, the iel as DofObject Index must be given in the correct form!
// So, I will distinguish iel into iel_mesh and iel_DofObj:
//In both cases we start from a "Geometrical Entity", the ELEMENT.
//In the mesh case, we only use the ELEMENT to pick its CONNECTIVITY.
//In the DofObj case,we use iel to pick the corresponding Element DOF from the node_dof map! 


/// This function assembles the matrix and the rhs:
void  GenMatRhsT(MultiLevelProblem &ml_prob, unsigned Level, const unsigned &gridn, const bool &assemble_matrix) {

  //if we are just a function not inside a class, we have to retrieve ourselves...
  SystemTwo & my_system = ml_prob.get_system<SystemTwo>("Eqn_T");
// ==========================================  
  Mesh		*mymsh		=  ml_prob._ml_msh->GetLevel(Level);
  elem		*myel		=  mymsh->el;
  const unsigned myproc  = mymsh->processor_id();
	
  
  const double time = 0.; // ml_prob._timeloop._curr_time;

  //======== ELEMENT MAPPING =======
  const uint space_dim =       ml_prob._ml_msh->GetDimension();

  //====== reference values ========================
  const double IRe = 1./ml_prob.GetInputParser().get("Re");
  const double IPr = 1./ml_prob.GetInputParser().get("Pr");
  const double alphaT  = ml_prob.GetInputParser().get("alphaT");
  const double alphaL2 = ml_prob.GetInputParser().get("alphaL2");
  const double alphaH1 = ml_prob.GetInputParser().get("alphaH1");
  
  //==== AUXILIARY ==============
    std::vector<double> dphijdx_g(space_dim);
    std::vector<double> dphiidx_g(space_dim);

        my_system._LinSolver[Level]->_KK->zero();
        my_system._LinSolver[Level]->_RESC->zero();

// ==========================================  
// ==========================================  
 {//BEGIN VOLUME
 
   const uint mesh_vb = VV;
   
   const uint nel_e = ml_prob.GetMeshTwo()._off_el[mesh_vb][ml_prob.GetMeshTwo()._NoLevels*myproc+Level+1];
   const uint nel_b = ml_prob.GetMeshTwo()._off_el[mesh_vb][ml_prob.GetMeshTwo()._NoLevels*myproc+Level];

  for (uint iel=0; iel < (nel_e - nel_b); iel++) {
  
  CurrentElem       currelem(iel,myproc,Level,VV,&my_system,ml_prob.GetMeshTwo(),ml_prob.GetElemType(),mymsh);    
  CurrentGaussPointBase & currgp = CurrentGaussPointBase::build(currelem,ml_prob.GetQrule(currelem.GetDim()));
  

//=========INTERNAL QUANTITIES (unknowns of the equation) =========     
    CurrentQuantity Tempold(currgp);
    Tempold._qtyptr   = ml_prob.GetQtyMap().GetQuantity("Qty_Temperature"); 
    Tempold._SolName = "Qty_Temperature";
    Tempold.VectWithQtyFillBasic();
    Tempold.Allocate();

//====================================
    CurrentQuantity Tlift(currgp);
    Tlift._qtyptr   = ml_prob.GetQtyMap().GetQuantity("Qty_TempLift");
    Tlift._SolName = "Qty_TempLift";
    Tlift.VectWithQtyFillBasic();
    Tlift.Allocate();

//=====================================
    CurrentQuantity TAdj(currgp);
    TAdj._qtyptr   = ml_prob.GetQtyMap().GetQuantity("Qty_TempAdj"); 
    TAdj._SolName = "Qty_TempAdj";
    TAdj.VectWithQtyFillBasic();
    TAdj.Allocate();
   
//=========EXTERNAL QUANTITIES (couplings) =====
    //========= //DOMAIN MAPPING
  CurrentQuantity xyz(currgp);  //no quantity
    xyz._dim      = space_dim;
    xyz._FEord    = MESH_MAPPING_FE;
    xyz._ndof     = currelem.GetElemType(xyz._FEord)->GetNDofs();
    xyz.Allocate();

    //==================Quadratic domain, auxiliary, must be QUADRATIC!!! ==========
  CurrentQuantity xyz_refbox(currgp);  //no quantity
    xyz_refbox._dim      = space_dim;
    xyz_refbox._FEord    = MESH_ORDER;
    xyz_refbox._ndof     = myel->GetElementDofNumber(ZERO_ELEM,BIQUADR_FE);
    xyz_refbox.Allocate();
  
  //==================
    CurrentQuantity velX(currgp);
    velX._qtyptr   = ml_prob.GetQtyMap().GetQuantity("Qty_Velocity0"); 
    velX._SolName = "Qty_Velocity0";
    velX.VectWithQtyFillBasic();
    velX.Allocate();
    
  //==================
    CurrentQuantity velY(currgp);
    velY._qtyptr   = ml_prob.GetQtyMap().GetQuantity("Qty_Velocity1"); 
    velY._SolName = "Qty_Velocity1";
    velY.VectWithQtyFillBasic();
    velY.Allocate();    
    
//===============Tdes=====================
    CurrentQuantity Tdes(currgp);
    Tdes._qtyptr   = ml_prob.GetQtyMap().GetQuantity("Qty_TempDes"); 
    Tdes._SolName = "Qty_TempDes";
    Tdes.VectWithQtyFillBasic();
    Tdes.Allocate();

  
// ==========================================  
// ==========================================  

    currelem.Mat().zero();
    currelem.Rhs().zero(); 

    currelem.SetDofobjConnCoords();
    currelem.SetMidpoint();

    currelem.ConvertElemCoordsToMappingOrd(xyz);
    currelem.TransformElemNodesToRef(ml_prob._ml_msh->GetDomain(),&xyz_refbox._val_dofs[0]);    
    
    
    currelem.SetElDofsBc();

  Tempold.GetElemDofs();
    Tlift.GetElemDofs();
     TAdj.GetElemDofs();
     

  xyz_refbox.SetElemAverage();
  int domain_flag = ElFlagControl(xyz_refbox._el_average,ml_prob._ml_msh);
//====================    
    
//===== FILL the DOFS of the EXTERNAL QUANTITIES:
   if ( velX._eqnptr != NULL )  velX.GetElemDofs();
   else                         velX._qtyptr->FunctionDof(velX,time,&xyz_refbox._val_dofs[0]);
   
   if ( velY._eqnptr != NULL )  velY.GetElemDofs();
   else                         velY._qtyptr->FunctionDof(velY,time,&xyz_refbox._val_dofs[0]);

   if ( Tdes._eqnptr != NULL )  Tdes.GetElemDofs();
   else                         Tdes._qtyptr->FunctionDof(Tdes,time,&xyz_refbox._val_dofs[0]);


   const uint el_ngauss = ml_prob.GetQrule(currelem.GetDim()).GetGaussPointsNumber();
   
   for (uint qp=0; qp< el_ngauss; qp++) {

//======= "COMMON SHAPE PART"==================
for (uint fe = 0; fe < QL; fe++)   { 
  currgp.SetPhiElDofsFEVB_g (fe,qp);
  currgp.SetDPhiDxezetaElDofsFEVB_g (fe,qp); 
}
	  
const double      det = currgp.JacVectVV_g(xyz);
const double dtxJxW_g = det*ml_prob.GetQrule(currelem.GetDim()).GetGaussWeight(qp);
const double     detb = det/el_ngauss;
	  
for (uint fe = 0; fe < QL; fe++)     { 
  currgp.SetDPhiDxyzElDofsFEVB_g   (fe,qp);
  currgp.ExtendDphiDxyzElDofsFEVB_g(fe);
}
//======= end of the "COMMON SHAPE PART"==================

 	Tempold.val_g(); 
          Tlift.val_g(); 
           TAdj.val_g(); 
           velX.val_g(); 
           velY.val_g(); 
           Tdes.val_g();
   
	   // always remember to get the dofs for the variables you use!
           // The point is that you fill the dofs with different functions...
           // you should need a flag to check if the dofs have been correctly filled

      /// d) Local (element) assemblying energy equation
      for (uint i=0; i < Tempold._ndof; i++)     {

        const double phii_g = currgp._phi_ndsQLVB_g[Tempold._FEord][i];

        for (uint idim = 0; idim < space_dim; idim++) dphiidx_g[idim] = currgp._dphidxyz_ndsQLVB_g[Tempold._FEord][i+idim*Tempold._ndof];

//=========== FIRST ROW ===============
        currelem.Rhs()(i) +=      
           currelem.GetBCDofFlag()[i]*dtxJxW_g*( 0. )
	   + (1-currelem.GetBCDofFlag()[i])*detb*(Tempold._val_dofs[i]);
        
        currelem.Mat()(i,i) +=  (1-currelem.GetBCDofFlag()[i])*detb;

//========= SECOND ROW (CONTROL) =====================
	 int ip1 = i + /* 1* */Tempold._ndof;   //suppose that T' T_0 T_adj have the same order
	 currelem.Rhs()(ip1) +=      
           currelem.GetBCDofFlag()[ip1]*dtxJxW_g*( 
                     + alphaT*domain_flag*(Tdes._val_g[0])*phii_g // T_d delta T_0
	  )
	   + (1-currelem.GetBCDofFlag()[ip1])*detb*(Tlift._val_dofs[i]);
        
         currelem.Mat()(ip1,ip1) +=  (1-currelem.GetBCDofFlag()[ip1])*detb;

//======= THIRD ROW (ADJOINT) ===================================
	 int ip2 = i + 2 * Tempold._ndof;   //suppose that T' T_0 T_adj have the same order
           currelem.Rhs()(ip2) +=      
           currelem.GetBCDofFlag()[ip2]*dtxJxW_g*( 
                + alphaT*domain_flag*(Tdes._val_g[0])*phii_g // T_d delta T'
	     )
	   + (1-currelem.GetBCDofFlag()[ip2])*detb*(Tempold._val_dofs[i]);
        
        currelem.Mat()(ip2,ip2) +=  (1-currelem.GetBCDofFlag()[ip2])*detb;

	 // Matrix Assemblying ---------------------------
        for (uint j=0; j<Tempold._ndof; j++) {
          double phij_g = currgp._phi_ndsQLVB_g[Tempold._FEord][j];
	  
        for (uint idim = 0; idim < space_dim; idim++)  dphijdx_g[idim] = currgp._dphidxyz_ndsQLVB_g[Tempold._FEord][j+idim*Tempold._ndof]; 
           
   
          double Lap_g   = Math::dot(&dphijdx_g[0],&dphiidx_g[0],space_dim);
          double Advection = velX._val_g[0]*dphijdx_g[0] + velY._val_g[0]*dphijdx_g[1]; //Math::dot(&vel._val_g[0],&dphijdx_g[0],space_dim);

	    int ip1 = i + Tempold._ndof;
	    int jp1 = j + Tempold._ndof;
	    int ip2 = i + 2*Tempold._ndof;
	    int jp2 = j + 2*Tempold._ndof;

// 	           T     T_0     T_adj
	    
// 	    T      X      X       O
	     
// 	    T_0   
	    
// 	    T_adj
	    
	    
//============ FIRST ROW state  delta T ===============
//======= DIAGONAL =============================
	   currelem.Mat()(i,j) +=        
            currelem.GetBCDofFlag()[i]*dtxJxW_g*( 
            + Advection*phii_g
            + IRe*IPr*Lap_g  
            );

//===============================
    //same operators for T and T_0
	    currelem.Mat()(i,jp1) +=        
            currelem.GetBCDofFlag()[i]*dtxJxW_g*(    
            + Advection*phii_g
            + IRe*IPr*Lap_g
	    );

//====================================
	   currelem.Mat()(i,jp2) +=        
            currelem.GetBCDofFlag()[i]*dtxJxW_g*( 
                0.
            );

	    
//============= SECOND ROW (LIFTING) delta T_0 =============
//===== DIAGONAL ===========================
         currelem.Mat()(ip1,jp1) +=        
            currelem.GetBCDofFlag()[ip1]*
            dtxJxW_g*( 
             + alphaL2*phij_g*phii_g  //L_2 control norm
             + alphaH1*Lap_g          //H_1 control norm
              + alphaT*domain_flag*(phij_g)*phii_g  //T_0 delta T_0  //ADDED///////////////
            ); 
//====================================
	   currelem.Mat()(ip1,j) +=        
            currelem.GetBCDofFlag()[ip1]*
            dtxJxW_g*( 
                + alphaT*domain_flag*(phij_g)*phii_g  //T' delta T_0     //ADDED///////////////
            );
//====================================
	   currelem.Mat()(ip1,jp2) +=        
            currelem.GetBCDofFlag()[ip1]*
             dtxJxW_g*( 
                 -Advection*phii_g
                + IRe*IPr*Lap_g
           );

//============= THIRD ROW (ADJOINT) =============
//======= DIAGONAL ==================
          currelem.Mat()(ip2,jp2) +=        
            currelem.GetBCDofFlag()[ip2]*
              dtxJxW_g*( 
            - Advection*phii_g  //minus sign
            + IRe*IPr*Lap_g
               
            ); 
//====================================
	   currelem.Mat()(ip2,j) +=        
            currelem.GetBCDofFlag()[ip2]*
            dtxJxW_g*( 
               + alphaT*domain_flag*(phij_g)*phii_g  //T' delta T'
            );
//====================================
	   currelem.Mat()(ip2,jp1) +=        
            currelem.GetBCDofFlag()[ip2]*
            dtxJxW_g*( 
               + alphaT*domain_flag*(phij_g)*phii_g  //T_0 delta T'     ///ADDED///////
            );

        }  //end j (col)
      }   //end i (row)
    } // end of the quadrature point qp-loop

       my_system._LinSolver[Level]->_KK->add_matrix(currelem.Mat(),currelem.GetDofIndices());
       my_system._LinSolver[Level]->_RESC->add_vector(currelem.Rhs(),currelem.GetDofIndices());
  } // end of element loop
  // *****************************************************************

 
   }//END VOLUME
  
        my_system._LinSolver[Level]->_KK->close();
        my_system._LinSolver[Level]->_RESC->close();

 
#ifdef DEFAULT_PRINT_INFO
  std::cout << " Matrix and RHS assembled for equation " << my_system.name()
            << " Level "<< Level << " dofs " << my_system._LinSolver[Level]->_KK->n() << std::endl;
#endif

  return;
}




#endif