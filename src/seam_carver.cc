#include "seam_carver.hpp"

// implement the rest of SeamCarver's functions here

// given functions below, DO NOT MODIFY

SeamCarver::SeamCarver(const ImagePPM& image): image_(image) {
  height_ = image.GetHeight();
  width_ = image.GetWidth();
}

void SeamCarver::SetImage(const ImagePPM& image) {
  image_ = image;
  width_ = image.GetWidth();
  height_ = image.GetHeight();
}

const ImagePPM& SeamCarver::GetImage() const {
  const ImagePPM& image_ref = image_;
  return image_ref;
}

int SeamCarver::GetHeight() const { return height_; }

int SeamCarver::GetWidth() const { return width_; }

int SeamCarver::GetEnergy(int row, int col) const {
  Pixel pix_up;
  Pixel pix_down;
  Pixel pix_left;
  Pixel pix_right;
  // setting above pixel
  if (row == 0) {
    pix_up = image_.GetPixel((height_ - 1), col);
  } else {
    pix_up = image_.GetPixel((row - 1), col);
  }
  // setting below pixel
  if (row == (height_ - 1)) {
    pix_down = image_.GetPixel(0, col);
  } else {
    pix_down = image_.GetPixel((row + 1), col);
  }
  // setting left pixel
  if (col == 0) {
    pix_left = image_.GetPixel(row, (width_ - 1));
  } else {
    pix_left = image_.GetPixel(row, (col - 1));
  }
  // setting the right pixel
  if (col == (width_ - 1)) {
    pix_right = image_.GetPixel(row, 0);
  } else {
    pix_right = image_.GetPixel(row, (col + 1));
  }
  // calculating squared differences
  int r_col_en = (pix_up.GetRed() - pix_down.GetRed()) *
                 (pix_up.GetRed() - pix_down.GetRed());
  int g_col_en = (pix_up.GetGreen() - pix_down.GetGreen()) *
                 (pix_up.GetGreen() - pix_down.GetGreen());
  int b_col_en = (pix_up.GetBlue() - pix_down.GetBlue()) *
                 (pix_up.GetBlue() - pix_down.GetBlue());

  int r_row_en = (pix_left.GetRed() - pix_right.GetRed()) *
                 (pix_left.GetRed() - pix_right.GetRed());
  int g_row_en = (pix_left.GetGreen() - pix_right.GetGreen()) *
                 (pix_left.GetGreen() - pix_right.GetGreen());
  int b_row_en = (pix_left.GetBlue() - pix_right.GetBlue()) *
                 (pix_left.GetBlue() - pix_right.GetBlue());

  int col_en = r_col_en + g_col_en + b_col_en;
  int row_en = r_row_en + g_row_en + b_row_en;
  return row_en + col_en;
}

int* SeamCarver::GetHorizontalSeam() const {
  // initializing 2D energy array
  int** img_energies = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    img_energies[row] = new int[width_];
  }
  // filling 2D energy array
  for (int row = 0; row < height_; ++row) {
    for (int col = 0; col < width_; ++col) {
      img_energies[row][col] = GetEnergy(row, col);
    }
  }
  // initializing horizontal seam calculated values array
  int** h_seam_arr = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    h_seam_arr[row] = new int[width_];
  }
  // filling hseam calcs right column
  for (int row = 0; row < height_; ++row) {
    h_seam_arr[row][width_ - 1] = img_energies[row][width_ - 1];
  }
  // filling hseam calcs remaining columns
  int min_val = 0;
  for (int col = width_ - 2; col > -1; --col) {
    for (int row = 0; row < height_; ++row) {
      // determine min row right
      min_val = h_seam_arr[row][col + 1];
      // determine min row right - check top right
      if (row != 0) {
        if (h_seam_arr[row - 1][col + 1] < min_val) {
          min_val = h_seam_arr[row - 1][col + 1];
        }
      }
      // determine min row right - check bottom right
      if (row != (height_ - 1)) {
        if (h_seam_arr[row + 1][col + 1] < min_val) {
          min_val = h_seam_arr[row + 1][col + 1];
        }
      }
      // fill pos with energy + min right
      h_seam_arr[row][col] = img_energies[row][col] + min_val;
    }
  }
  // finding the horizontal seam path using calculated hseam values
  int* row_values = new int[height_];
  // finding path left column
  int path_min_val = 2147483647;
  int current_row = 0;
  for (int row = 0; row < height_; ++row) {
    if (h_seam_arr[row][0] < path_min_val) {
      path_min_val = h_seam_arr[row][0];
      current_row = row;
      row_values[0] = row;
    }
  }
  // finding remaining path
  for (int col = 1; col < width_; ++col) {
    // setting next col as middle
    int min_col_val = h_seam_arr[current_row][col];
    int min_col_num = current_row;
    // checking top right
    if (current_row != 0) {
      if (h_seam_arr[current_row - 1][col] < min_col_val) {
        min_col_val = h_seam_arr[current_row - 1][col];
        min_col_num = current_row - 1;
      }
    }
    // checking bottom right
    if (current_row != (height_ - 1)) {
      if (h_seam_arr[current_row + 1][col] < min_col_val) {
        min_col_val = h_seam_arr[current_row + 1][col];
        min_col_num = current_row + 1;
      }
    }
    // adding col value to array
    row_values[col] = min_col_num;
  }
  // deleting 2d energy array
  for (int row = 0; row < height_; ++row) {
    delete[] img_energies[row];
  }
  delete[] img_energies;
  // deleting 2d value array
  for (int row = 0; row < height_; ++row) {
    delete[] h_seam_arr[row];
  }
  delete[] h_seam_arr;
  int* point_arr = row_values;
  return point_arr;
}

int* SeamCarver::GetVerticalSeam() const {
  // initializing 2D energy array
  int** img_energies = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    img_energies[row] = new int[width_];
  }
  // filling 2D energy array
  for (int row = 0; row < height_; ++row) {
    for (int col = 0; col < width_; ++col) {
      img_energies[row][col] = GetEnergy(row, col);
    }
  }
  // initializing vertical seam calculated values array
  int** v_seam_arr = new int*[height_];
  for (int row = 0; row < height_; ++row) {
    v_seam_arr[row] = new int[width_];
  }
  // filling vseam calcs bottom row
  for (int col = 0; col < width_; ++col) {
    v_seam_arr[height_ - 1][col] = img_energies[height_ - 1][col];
  }
  // filling vseam calcs remaining rows
  int min_val = 0;
  for (int row = height_ - 2; row > -1; --row) {
    for (int col = 0; col < width_; ++col) {
      // determine min col below
      min_val = v_seam_arr[row + 1][col];
      // determine min col below - check bottom left
      if (col != 0) {
        if (v_seam_arr[row + 1][col - 1] < min_val) {
          min_val = v_seam_arr[row + 1][col - 1];
        }
      }
      // determine min col below - check bottom right
      if (col != (width_ - 1)) {
        if (v_seam_arr[row + 1][col + 1] < min_val) {
          min_val = v_seam_arr[row + 1][col + 1];
        }
      }
      // fill pos with energy + min below
      v_seam_arr[row][col] = img_energies[row][col] + min_val;
    }
  }
  // finding the vertical seam path using calculated vseam values
  
  // finding path top row
  int path_min_val = 2147483647;
  int current_col = 0;
  for (int col = 0; col < width_; ++col) {
    if (v_seam_arr[0][col] < path_min_val) {
      path_min_val = v_seam_arr[0][col];
      current_col = col;
      col_values[0] = col;
    }
  }
  // finding remaining path
  for (int row = 1; row < height_; ++row) {
    // setting next row as middle
    int min_row_val = v_seam_arr[row][current_col];
    int min_row_num = current_col;
    // checking bottom left
    if (current_col != 0) {
      if (v_seam_arr[row][current_col - 1] < min_row_val) {
        min_row_val = v_seam_arr[row][current_col - 1];
        min_row_num = current_col - 1;
      }
    }
    // checking bottom right
    if (current_col != (width_ - 1)) {
      if (v_seam_arr[row][current_col + 1] < min_row_val) {
        min_row_val = v_seam_arr[row][current_col + 1];
        min_row_num = current_col + 1;
      }
    }
    // adding col value to array
    col_values[row] = min_row_num;
  }
  // deleting 2d energy array
  for (int row = 0; row < height_; ++row) {
    delete[] img_energies[row];
  }
  delete[] img_energies;
  // deleting 2d value array
  for (int row = 0; row < height_; ++row) {
    delete[] v_seam_arr[row];
  }
  delete[] v_seam_arr;
  // returning col_values
  int* point_arr = col_values;
  return point_arr;
}

void SeamCarver::RemoveHorizontalSeam() {
  int* seam_row_values = GetHorizontalSeam();
  image_.RemoveHorizontalImageSeam(seam_row_values);
  delete[] seam_row_values;
  height_--;
}

void SeamCarver::RemoveVerticalSeam() {
  int* seam_col_values = GetVerticalSeam();
  image_.RemoveVerticalImageSeam(seam_col_values);
  delete[] seam_col_values;
  width_--;
}
