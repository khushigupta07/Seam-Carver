#include "image_ppm.hpp"

// implement the rest of ImagePPM's functions here

// given functions below, DO NOT MODIFY

ImagePPM::ImagePPM() {
  height_ = 0;
  width_ = 0;
  max_color_value_ = 0;
  pixels_ = nullptr;
}

ImagePPM::ImagePPM(const std::string& path) {
  height_ = 0;
  width_ = 0;
  max_color_value_ = 0;
  pixels_ = nullptr;

  std::ifstream input_file(path);
  input_file >> *this;
  input_file.close();
}

ImagePPM::ImagePPM(const ImagePPM& source) {
  height_ = 0;
  width_ = 0;
  max_color_value_ = 0;
  pixels_ = nullptr;

  *this = source;
}

ImagePPM& ImagePPM::operator=(const ImagePPM& source) {
  if (this == &source) {
    return *this;
  }

  Clear();

  height_ = source.height_;
  width_ = source.width_;
  max_color_value_ = source.max_color_value_;

  pixels_ = new Pixel*[height_];
  for (int row = 0; row < height_; row++) {
    pixels_[row] = new Pixel[width_];

    for (int col = 0; col < width_; col++) {
      pixels_[row][col] = source.pixels_[row][col];
    }
  }

  return *this;
}

ImagePPM::~ImagePPM() { Clear(); }

void ImagePPM::Clear() {
  for (int i = 0; i < height_; i++) {
    delete[] pixels_[i];
  }

  delete[] pixels_;

  height_ = 0;
  width_ = 0;
  pixels_ = nullptr;
}
std::ostream& operator<<(std::ostream& os, const ImagePPM& image) {
  os << "P3" << std::endl;
  os << image.GetWidth() << ' ' << image.GetHeight() << std::endl;
  os << image.GetMaxColorValue() << std::endl;
  for (int row = 0; row < image.GetHeight(); ++row) {
    for (int col = 0; col < image.GetWidth(); ++col) {
      os << image.GetPixel(row, col).GetRed() << std::endl;
      os << image.GetPixel(row, col).GetGreen() << std::endl;
      os << image.GetPixel(row, col).GetBlue() << std::endl;
    }
  }
  return os;
}

std::istream& operator>>(std::istream& is, ImagePPM& image) {
  image.Clear();
  std::string line;

  // ignore magic number line
  getline(is, line);

  // check to see if there's a comment line
  getline(is, line);
  if (line[0] == '#') {
    getline(is, line);
  }

  // parse width and height
  int space = line.find_first_of(' ');
  image.width_ = std::stoi(line.substr(0, space));
  image.height_ = std::stoi(line.substr(space + 1));

  // get max color value
  getline(is, line);
  image.max_color_value_ = std::stoi(line);

  // init and fill in Pixels array
  image.pixels_ = new Pixel*[image.height_];
  for (int i = 0; i < image.height_; i++) {
    image.pixels_[i] = new Pixel[image.width_];
  }

  for (int row = 0; row < image.height_; row++) {
    for (int col = 0; col < image.width_; col++) {
      getline(is, line);
      int red = std::stoi(line);
      getline(is, line);
      int green = std::stoi(line);
      getline(is, line);
      int blue = std::stoi(line);

      Pixel p(red, green, blue);
      image.pixels_[row][col] = p;
    }
  }

  return is;
}

Pixel ImagePPM::GetPixel(int row, int col) const { return pixels_[row][col]; }

int ImagePPM::GetMaxColorValue() const { return max_color_value_; }

void ImagePPM::RemoveHorizontalImageSeam(int* seam_row_values) {
  // declaring carved image (one less row)
  Pixel** carved_img = new Pixel*[height_ - 1];
  for (int row = 0; row < (height_ - 1); ++row) {
    carved_img[row] = new Pixel[width_];
  }
  // next_row will become true when seam row is reached
  bool next_row = false;
  for (int col = 0; col < width_; ++col) {
    next_row = false;
    for (int row = 0; row < (height_ - 1); ++row) {
      if (row == seam_row_values[col]) {
        next_row = true;
      }
      if (!next_row) {
        carved_img[row][col] = pixels_[row][col];
      } else {
        carved_img[row][col] = pixels_[row + 1][col];
      }
    }
  }
  for (int row = 0; row < height_; ++row) {
    delete[] pixels_[row];
  }
  delete[] pixels_;
  pixels_ = carved_img;
  --height_;
}

void ImagePPM::RemoveVerticalImageSeam(int* seam_col_values) {
  // declaring carved image (one less column)
  Pixel** carved_img = new Pixel*[height_];
  for (int row = 0; row < (height_); ++row) {
    carved_img[row] = new Pixel[width_ - 1];
  }
  // next_col will become true when seam col is reached
  bool next_col = false;
  for (int row = 0; row < height_; ++row) {
    next_col = false;
    for (int col = 0; col < (width_ - 1); ++col) {
      if (col == seam_col_values[row]) {
        next_col = true;
      }
      if (!next_col) {
        carved_img[row][col] = pixels_[row][col];
      } else {
        carved_img[row][col] = pixels_[row][col + 1];
      }
    }
  }
  for (int row = 0; row < height_; ++row) {
    delete[] pixels_[row];
  }
  delete[] pixels_;
  pixels_ = carved_img;
  --width_;
}