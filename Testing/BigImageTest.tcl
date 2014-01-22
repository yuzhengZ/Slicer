

set id [vtkImageData New]

$id SetDimensions 1000 1000 3000
$id SetScalarTypeToUnsignedChar 
$id AllocateScalars

# VTK 6 update
# $id SetScalarType 3
# $id AllocateScalars 3 1

puts "[$id Print]"

puts "memory size is [$id GetActualMemorySize]"

$id Delete
