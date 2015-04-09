CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(femus)

#############################################################################################
### Source files
#############################################################################################

SET(femus_src 
algebra/AsmPetscLinearEquationSolver.cpp
algebra/DenseMatrixBase.cpp
algebra/DenseMatrix.cpp
algebra/DenseSubmatrix.cpp
algebra/DenseVectorBase.cpp
algebra/Graph.cpp
algebra/LinearEquation.cpp
algebra/LinearEquationSolver.cpp
algebra/NumericVector.cpp
algebra/GmresPetscLinearEquationSolver.cpp
algebra/PetscMatrix.cpp
algebra/PetscPreconditioner.cpp
algebra/PetscVector.cpp
algebra/Preconditioner.cpp
algebra/SparseMatrix.cpp
algebra/VankaPetscLinearEquationSolver.cpp
algebra/FunctionBase.cpp
algebra/ParsedFunction.cpp
equations/DofMap.cpp
equations/BoundaryConditions.cpp
equations/CurrentElem.cpp
equations/CurrentGaussPoint.cpp
equations/CurrentGaussPointBase.cpp
equations/CurrentQuantity.cpp
equations/ExplicitSystem.cpp
equations/ImplicitSystem.cpp
equations/LinearImplicitSystem.cpp
equations/MonolithicFSINonLinearImplicitSystem.cpp
equations/NonLinearImplicitSystem.cpp
equations/MultiLevelProblem.cpp
equations/System.cpp
equations/SystemTwo.cpp
equations/TimeLoop.cpp
equations/TransientSystem.cpp
equations/NewmarkTransientSystem.cpp
fe/ElemType.cpp
fe/Hexaedron.cpp
fe/Line.cpp
fe/Quadrilateral.cpp
fe/Tetrahedral.cpp
fe/Triangle.cpp
fe/Wedge.cpp
meshGencase/Box.cpp
meshGencase/Domain.cpp
meshGencase/ElemSto.cpp
meshGencase/MultiLevelMeshTwo.cpp
meshGencase/GenCase.cpp
meshGencase/FEElemBase.cpp
meshGencase/FEHex1.cpp
meshGencase/FEHex27.cpp
meshGencase/FEHex8.cpp
meshGencase/FEQuad1.cpp
meshGencase/FEQuad4.cpp
meshGencase/FEQuad9.cpp
meshGencase/FETet10.cpp
meshGencase/FETet1.cpp
meshGencase/FETet4.cpp
meshGencase/FETri1.cpp
meshGencase/FETri3.cpp
meshGencase/FETri6.cpp
meshGencase/FEEdge3.cpp
meshGencase/FEEdge2.cpp
meshGencase/FEEdge1.cpp
mesh/Elem.cpp
mesh/Mesh.cpp
mesh/MultiLevelMesh.cpp
mesh/MeshGeneration.cpp
mesh/GambitIO.cpp
mesh/SalomeIO.cpp
mesh/MeshRefinement.cpp
mesh/MeshMetisPartitioning.cpp
mesh/MeshPartitioning.cpp
mesh/MeshASMPartitioning.cpp
physics/Fluid.cpp
physics/Material.cpp
physics/Parameter.cpp
physics/Solid.cpp
quadrature/GaussPoints.cpp
solution/MultiLevelSolution.cpp
solution/Quantity.cpp
solution/Solution.cpp
solution/Writer.cpp
solution/VTKWriter.cpp
solution/GMVWriter.cpp
solution/XDMFWriter.cpp
utils/FemusInit.cpp
utils/Files.cpp
utils/InputParser.cpp
utils/JsonInputParser.cpp
hierarchical/elem.cpp
hierarchical/elem_type.cpp
hierarchical/element.cpp
hierarchical/hex.cpp
hierarchical/hexh.cpp
hierarchical/hFun.cpp
hierarchical/line.cpp
hierarchical/lineh.cpp
hierarchical/linehFun.cpp
hierarchical/linFun.cpp
hierarchical/lsysPDE.cpp
hierarchical/mesh.cpp
hierarchical/multigrid.cpp
hierarchical/quad.cpp
hierarchical/quadh.cpp
hierarchical/tet.cpp
hierarchical/teth.cpp
hierarchical/tetraFun.cpp
hierarchical/tri.cpp
hierarchical/triFun.cpp
hierarchical/trih.cpp
hierarchical/trihFun.cpp
hierarchical/wedge.cpp
hierarchical/wedgeh.cpp
)

IF (NOT LIBRARY_OUTPUT_PATH)
  SET (LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib64 CACHE PATH
       "Single output directory for building all libraries.")
  MARK_AS_ADVANCED(LIBRARY_OUTPUT_PATH)
ENDIF (NOT LIBRARY_OUTPUT_PATH)

ADD_LIBRARY(${PROJECT_NAME} SHARED ${femus_src})

