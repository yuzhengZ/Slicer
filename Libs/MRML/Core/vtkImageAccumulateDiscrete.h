/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer
  Module:    $RCSfile: vtkImageAccumulateDiscrete.h,v $
  Date:      $Date: 2006/04/12 22:28:50 $
  Version:   $Revision: 1.21 $

=========================================================================auto=*/

#ifndef __vtkImageAccumulateDiscrete_h
#define __vtkImageAccumulateDiscrete_h

// MRML includes
#include "vtkMRML.h"

// VTK includes
#include <vtkImageAlgorithm.h>

/// \brief Generalized histograms up to 4 dimensions.
///
/// This filter divides component space into
/// discrete bins.  It then counts the number of pixels associated
/// with each bin.  The output is this "scatter plot".
/// The input can be any type, but the output is always int.
class VTK_MRML_EXPORT vtkImageAccumulateDiscrete : public vtkImageAlgorithm
{
public:
  static vtkImageAccumulateDiscrete *New();
  vtkTypeMacro(vtkImageAccumulateDiscrete,vtkImageAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

protected:
  vtkImageAccumulateDiscrete();
  ~vtkImageAccumulateDiscrete() {};

  virtual int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  virtual int RequestUpdateExtent (vtkInformation *, vtkInformationVector **, vtkInformationVector *);
  void ExecuteData(vtkDataObject *);

private:
  vtkImageAccumulateDiscrete(const vtkImageAccumulateDiscrete&);
  void operator=(const vtkImageAccumulateDiscrete&);
};

#endif

