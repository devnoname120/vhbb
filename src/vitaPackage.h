#pragma once

#include <global_include.h>
#include "infoProgress.h"

#define PACKAGE_TEMP_FOLDER std::string("ux0:/temp/pkg/")

class VitaPackage{
public:
  explicit VitaPackage(std::string vpk);
  ~VitaPackage();

  int Install(InfoProgress progress);
  int Install(InfoProgress *progress = nullptr);

private:
  std::string vpk_;
};

