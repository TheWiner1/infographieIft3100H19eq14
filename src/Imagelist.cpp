#include "Imagelist.h"

Imagelist::Imagelist(vector<ofImage>loadedImages,vector<ofImage>processedImages):x0(0),y0(0), width(0),
  height(400), isSelected(false)
{
  Imagelist::loadedImages = loadedImages;
  Imagelist::processedImages = processedImages;
}
void Imagelist::setTrueFalse(){
  if(isSelected == true)
    isSelected=false;
  else
    isSelected = true;
}
bool Imagelist::getTrueFalse(){
  if(isSelected == true)
    return true;
  else
    return false;
}
