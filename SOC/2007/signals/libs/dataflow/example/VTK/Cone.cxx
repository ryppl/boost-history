//[ vtk_example_Cone_cxx

/*=========================================================================

  Program:   Visualization Toolkit
  Module:    $RCSfile: Cone.cxx,v $

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

  NOTE:  This is a MODIFIED version of the original Cone.cxx distributed
  with VTK.  Modifications (c) Stjepan Rajko 2007

=========================================================================*/
//
// This example creates a polygonal model of a cone, and then renders it to
// the screen. It will rotate the cone 360 degrees and then exit. The basic
// setup of source -> mapper -> actor -> renderer -> renderwindow is 
// typical of most VTK programs.
//

// First include the required header files for the VTK classes we are using.

#include "vtkAlgorithmOutput.h"
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"

// ... and include the VTK Dataflow support layer.
#include <boost/dataflow/vtk/support.hpp>

int main()
{
    
  // 
  // Next we create an instance of vtkConeSource and set some of its
  // properties. The instance of vtkConeSource "cone" is part of a
  // visualization pipeline (it is a source process object); it produces data
  // (output type is vtkPolyData) which other filters may process.
  //
  vtkConeSource *cone = vtkConeSource::New();
  cone->SetHeight( 3.0 );
  cone->SetRadius( 1.0 );
  cone->SetResolution( 10 );
  
  // 
  // In this example we terminate the pipeline with a mapper process object.
  // (Intermediate filters such as vtkShrinkPolyData could be inserted in
  // between the source and the mapper.)  We create an instance of
  // vtkPolyDataMapper to map the polygonal data into graphics primitives. We
  // connect the output of the cone souece to the input of this mapper.
  //
  vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();

  // 
  // Create an actor to represent the cone. The actor orchestrates rendering
  // of the mapper's graphics primitives. An actor also refers to properties
  // via a vtkProperty instance, and includes an internal transformation
  // matrix. We set this actor's mapper to be coneMapper which we created
  // above.
  //
  vtkActor *coneActor = vtkActor::New();

  //
  // Create the Renderer and assign actors to it. A renderer is like a
  // viewport. It is part or all of a window on the screen and it is
  // responsible for drawing the actors it has.  We also set the background
  // color here.
  //
  vtkRenderer *ren1= vtkRenderer::New();
  ren1->SetBackground( 0.1, 0.2, 0.4 );

  //
  // Finally we create the render window which will show up on the screen.
  // We put our renderer into the render window using AddRenderer. We also
  // set the size to be 300 pixels by 300.
  //
  vtkRenderWindow *renWin = vtkRenderWindow::New();
  renWin->SetSize( 300, 300 );
  
  // Finally, connect everything using the VTK Dataflow support layer:
  *cone >>= *coneMapper >>= *coneActor >>= *ren1 >>= *renWin;
  
  // NOTE: this replaced the following code:
  // coneMapper->SetInput( cone->GetOutput() );
  // coneActor->SetMapper( coneMapper );
  // ren1->AddActor( coneActor );
  // renWin->AddRenderer( ren1 );
  
  //
  // Now we loop over 360 degreeees and render the cone each time.
  //
  int i;
  for (i = 0; i < 360; ++i)
    {
    // render the image
    renWin->Render();
    // rotate the active camera by one degree
    ren1->GetActiveCamera()->Azimuth( 1 );
    }
  
  //
  // Free up any objects we created. All instances in VTK are deleted by
  // using the Delete() method.
  //
  cone->Delete();
  coneMapper->Delete();
  coneActor->Delete();
  ren1->Delete();
  renWin->Delete();

  return 0;
}

//]

void ignore_this()
{
  vtkConeSource *cone = vtkConeSource::New();
  vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();

//[ vtk_connect_unforwarded
    using namespace boost::dataflow;
    
    // connect *cone to *coneMapper
    binary_operation<operations::connect, vtk::tag>
        (*cone->GetOutputPort(), vtk::vtk_algorithm_consumer_adapter(*coneMapper));
    
    // make *cone the only thing connected to *coneMapper
    binary_operation<operations::connect_only, vtk::tag>
        (*cone->GetOutputPort(), vtk::vtk_algorithm_consumer_adapter(*coneMapper));
//]

//[ vtk_connect_forwarded
    using namespace boost::dataflow;
    
    // connect *cone to *coneMapper
    connect(*cone->GetOutputPort(), vtk::vtk_algorithm_consumer_adapter(*coneMapper));
    
    // make *cone the only thing connected to *coneMapper
    connect(*cone->GetOutputPort(), vtk::vtk_algorithm_consumer_adapter(*coneMapper));
//]
}
