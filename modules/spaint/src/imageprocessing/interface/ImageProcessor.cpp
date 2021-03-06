/**
 * spaint: ImageProcessor.cpp
 * Copyright (c) Torr Vision Group, University of Oxford, 2015. All rights reserved.
 */

#include "imageprocessing/interface/ImageProcessor.h"

namespace spaint {

//#################### DESTRUCTOR ####################

ImageProcessor::~ImageProcessor() {}

//#################### PUBLIC STATIC MEMBER FUNCTIONS ####################

Vector2i ImageProcessor::image_size(const AFArray_CPtr& img)
{
  return Vector2i(static_cast<int>(img->dims(1)), static_cast<int>(img->dims(0)));
}

}
