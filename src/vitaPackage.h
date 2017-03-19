#pragma once

#include <global_include.h>

#define PACKAGE_TEMP_FOLDER std::string("ux0:/temp/pkg/")

class VitaPackage{
public:
  VitaPackage(const std::string vpk);
  ~VitaPackage();

  int Install();

private:
  std::string vpk_;
};

