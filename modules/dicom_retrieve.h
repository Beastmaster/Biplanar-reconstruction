/*
Author: QIN Shuo
Date:   2017/6/2
Description:
	Retrieve biplanar EOS image from folder
	Frotal view
	Profile view
*/
#ifndef _DICOM_RETRIEVE_H_
#define _DICOM_RETRIEVE_H_


#include "vtkDICOMImageReader.h"
#include "vtkSmartPointer.h"
#include "vtkObjectFactory.h"
#include "vtkImageData.h"
#include "vtkAlgorithm.h"

#include "Windows.h"
#include <vector>

class dicom_retrieve: public vtkAlgorithm
{
public:
	static dicom_retrieve * New();
	vtkTypeMacro(dicom_retrieve, vtkAlgorithm);

	dicom_retrieve();

	int SetDicomFolder(std::string);
	void Update();
	vtkImageData* GetFrontal();
	vtkImageData* GetProfile();
private:
	std::vector<std::string> m_filenames;
	vtkSmartPointer<vtkImageData> m_Frontal;
	vtkSmartPointer<vtkImageData> m_Profile;
};

vtkStandardNewMacro(dicom_retrieve);
dicom_retrieve::dicom_retrieve()
{
}


inline int dicom_retrieve::SetDicomFolder(std::string dir_name)
{
	std::vector<std::string> names;
	std::string search_path = dir_name + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
				names.push_back(fd.cFileName);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}

	for (auto it = names.begin(); it != names.end(); it++)
	{
		if ((*it).find("face.dcm") != std::string::npos)
			m_filenames.push_back(dir_name + "/" + *it);
		else if ((*it).find("profil.dcm") != std::string::npos)
			m_filenames.push_back(dir_name + "/" + *it);
	}
	if (m_filenames.size() != 2)
	{
		return 1;
	}
	return 0;
}

inline void dicom_retrieve::Update()
{
	auto reader1 = vtkDICOMImageReader::New();
	reader1->SetFileName(m_filenames[0].c_str());
	reader1->Update();
	m_Frontal = reader1->GetOutput();

	auto reader2 = vtkDICOMImageReader::New();
	reader2->SetFileName(m_filenames[1].c_str());
	reader2->Update();
	m_Profile = reader2->GetOutput();
}

inline vtkImageData * dicom_retrieve::GetFrontal()
{
	return m_Frontal;
}

inline vtkImageData * dicom_retrieve::GetProfile()
{
	return m_Profile;
}





#endif // !_DICOM_RETRIEVE_H_
