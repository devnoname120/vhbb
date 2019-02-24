#pragma once

#include <global_include.h>
#include "infoProgress.h"

#ifndef PACKAGE_TEMP_FOLDER
	#define PACKAGE_TEMP_FOLDER std::string("ux0:/temp/pkg/")
#endif

class VitaPackage{
public:
	explicit VitaPackage(std::string vpk);
	~VitaPackage();

	int Install(InfoProgress progress);
	int Install(InfoProgress *progress = nullptr);

	// subroutines of Install
	void Extract(InfoProgress *progress = nullptr);
	int InstallExtracted(InfoProgress *progress = nullptr);

private:
	std::string vpk_;
};

class UpdaterPackage : private VitaPackage {
public:
	UpdaterPackage() : VitaPackage("VHBBUpdater") {};

	int InstallUpdater(InfoProgress progress);
	int InstallUpdater(InfoProgress *progress = nullptr);
};

class UpdatePackage : private VitaPackage {
public:
	explicit UpdatePackage(std::string vpk) : VitaPackage(vpk) {};

	void Extract(InfoProgress *progress = nullptr) { VitaPackage::Extract(progress); }
	void MakeHeadBin();
};

class InstalledVitaPackage : private VitaPackage {
public:
	explicit InstalledVitaPackage(std::string title_id) : VitaPackage(""), title_id(std::move(title_id)) {}

	bool IsInstalled();

	int Uninstall(InfoProgress progress);
	int Uninstall(InfoProgress *progress = nullptr);

private:
	std::string title_id;
};

