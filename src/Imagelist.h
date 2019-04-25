#ifndef LISTIMAGES_H
#define LISTIMAGES_H
#include "ofMain.h"
#include "ofImage.h"

class Imagelist
{
public:
  Imagelist(vector<ofImage>loadedImages, vector<ofImage>processedImages);
  vector<ofImage>loadedImages;
  vector<ofImage>processedImages;
  int x0;
  int y0;
  int width;
  int height;
  bool isSelected;

  void setTrueFalse();
  bool getTrueFalse();
};

#endif // LISTIMAGES_H
