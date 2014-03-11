/*=auto=========================================================================

Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

See COPYRIGHT.txt
or http://www.slicer.org/copyright/copyright.txt for details.

Program:   3D Slicer
Module:    $RCSfile: vtkMRMLModelDisplayNode.cxx,v $
Date:      $Date: 2006/03/03 22:26:39 $
Version:   $Revision: 1.3 $

=========================================================================auto=*/
// MRML includes
#include "vtkMRMLModelDisplayNode.h"

// VTK includes
#include <vtkAlgorithmOutput.h>
#include <vtkAssignAttribute.h>
#include <vtkCommand.h>
#include <vtkObjectFactory.h>
#include <vtkPassThrough.h>
#include <vtkPolyData.h>
#include <vtkVersion.h>

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLModelDisplayNode);

//-----------------------------------------------------------------------------
vtkMRMLModelDisplayNode::vtkMRMLModelDisplayNode()
{
  this->PassThrough = vtkPassThrough::New();
  this->AssignAttribute = vtkAssignAttribute::New();
  // Be careful, virtualization doesn't work in constructors
  this->UpdatePolyDataPipeline();
}

//-----------------------------------------------------------------------------
vtkMRMLModelDisplayNode::~vtkMRMLModelDisplayNode()
{
  this->PassThrough->Delete();
  this->AssignAttribute->Delete();
}

//---------------------------------------------------------------------------
void vtkMRMLModelDisplayNode::ProcessMRMLEvents(vtkObject *caller,
                                                unsigned long event,
                                                void *callData )
{
  this->Superclass::ProcessMRMLEvents(caller, event, callData);

  if (event == vtkCommand::ModifiedEvent)
    {
    this->UpdatePolyDataPipeline();
    }
}

//---------------------------------------------------------------------------
#if (VTK_MAJOR_VERSION <= 5)
void vtkMRMLModelDisplayNode::SetInputPolyData(vtkPolyData* polyData)
#else
void vtkMRMLModelDisplayNode::SetInputPolyData(vtkAlgorithm* polyDataFilter,
                                               vtkPolyData* polyData)
#endif
{
   if (this->GetInputPolyData() == polyData)
     {
     return;
     }
#if (VTK_MAJOR_VERSION <= 5)
   this->SetInputToPolyDataPipeline(polyData);
#else
   this->SetInputToPolyDataPipeline(polyDataFilter, polyData);
#endif
   this->Modified();
}

//---------------------------------------------------------------------------
#if (VTK_MAJOR_VERSION <= 5)
void vtkMRMLModelDisplayNode::SetInputToPolyDataPipeline(vtkPolyData* polyData)
{
  this->PassThrough->SetInput(polyData);
  this->AssignAttribute->SetInput(polyData);
}
#else
void vtkMRMLModelDisplayNode::SetInputToPolyDataPipeline(vtkAlgorithm* polyDataFilter,
                                                         vtkPolyData* polyData)
{
  if (polyDataFilter != NULL)
    {
    this->PassThrough->SetInputConnection(polyDataFilter->GetOutputPort());
    this->AssignAttribute->SetInputConnection(polyDataFilter->GetOutputPort());
    }
  else
    {
    this->PassThrough->SetInputData(polyData);
    this->AssignAttribute->SetInputData(polyData);
    }
}
#endif

//---------------------------------------------------------------------------
vtkPolyData* vtkMRMLModelDisplayNode::GetInputPolyData()
{
  return vtkPolyData::SafeDownCast(this->AssignAttribute->GetInput());
}

#if (VTK_MAJOR_VERSION > 5)
vtkAlgorithm* vtkMRMLModelDisplayNode::GetInputFilter()
{
  return vtkAlgorithm::SafeDownCast(this->AssignAttribute);
}
#endif

//---------------------------------------------------------------------------
vtkPolyData* vtkMRMLModelDisplayNode::GetOutputPolyData()
{
  if (!this->GetOutputPort())
    {
    return 0;
    }
  if (!this->GetInputPolyData())
    {
    return 0;
    }
  return vtkPolyData::SafeDownCast(
    this->GetOutputPort()->GetProducer()->GetOutputDataObject(
      this->GetOutputPort()->GetIndex()));
}

#if (VTK_MAJOR_VERSION > 5)
vtkAlgorithm* vtkMRMLModelDisplayNode::GetOutputFilter()
{
  if (this->GetActiveScalarName())
    {
    return vtkAlgorithm::SafeDownCast(this->AssignAttribute);
    }
  else
    {
    return vtkAlgorithm::SafeDownCast(this->PassThrough);
    }
}
#endif

//---------------------------------------------------------------------------
vtkAlgorithmOutput* vtkMRMLModelDisplayNode::GetOutputPort()
{
  if (this->GetActiveScalarName())
    {
    return this->AssignAttribute->GetOutputPort();
    }
  else
    {
    return this->PassThrough->GetOutputPort();
    }
}

//---------------------------------------------------------------------------
void vtkMRMLModelDisplayNode::SetActiveScalarName(const char *scalarName)
{
  int wasModifying = this->StartModify();
  this->Superclass::SetActiveScalarName(scalarName);
  this->UpdatePolyDataPipeline();
  this->EndModify(wasModifying);
}

//---------------------------------------------------------------------------
void vtkMRMLModelDisplayNode::SetActiveAttributeLocation(int location)
{
  int wasModifying = this->StartModify();
  this->Superclass::SetActiveAttributeLocation(location);
  this->UpdatePolyDataPipeline();
  this->EndModify(wasModifying);
}

//---------------------------------------------------------------------------
void vtkMRMLModelDisplayNode::UpdatePolyDataPipeline()
{
  this->AssignAttribute->Assign(
    this->GetActiveScalarName(),
    this->GetActiveScalarName() ? vtkDataSetAttributes::SCALARS : -1,
    this->GetActiveAttributeLocation());
  if (this->GetAutoScalarRange() && this->GetOutputPolyData())
    {
#if (VTK_MAJOR_VERSION <= 5)
    this->GetOutputPolyData()->Update();
#else
    this->GetOutputFilter()->Update();
#endif
    this->SetScalarRange(this->GetOutputPolyData()->GetScalarRange());
    }
}
