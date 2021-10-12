#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver( const ImagePPM& image ): image_( image ) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage( const ImagePPM& image ) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}
