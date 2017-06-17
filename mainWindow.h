/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2017/6/3

*/


#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include "ui_mainWindow.h"

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <functional>
#include <algorithm>
#include <map>

#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QWheelEvent>
#include <QMessageBox>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QList>

#include "vtkPolyData.h"

#include "image_viewer.h"
#include "model3d_viewer.h"

#ifdef DEBUG_MODE
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyData.h"
#include "vtkSTLReader.h"
#endif // DEBUG_MODE



namespace Ui {
	class MainWindow;
}

typedef struct {
	std::string name;
	double pos[3];
	double rotation[3];
	vtkSmartPointer<vtkPolyData> vertebra_poly;
	vtkSmartPointer<vtkActor> vertebra;
} Vertebra;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void on_Open_Folder();
	void on_Load_folder(std::string path);
	void Open_Folder_Warning();
	void on_EnablePick();
	// auto allocate seed widget position
	void AutoInitPosition();


	void on_addseed() {
		double pos[2] = { 100,100};
		m_img_viewer->AddFrontalSeed(pos );
		std::cout << "movexx" << std::endl;
	};

	void on_addactor() {
		double pos[3] = { 10,10,10 };
		
		auto ballsource = vtkSmartPointer<vtkSphereSource>::New();
		ballsource->SetRadius(10);
		ballsource->Update();
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(ballsource->GetOutput());
		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		m_model_viewer->AddActor(actor);
		m_model_viewer->ResetCamera();
	}

	void on_move() {
		double pos[3] = { 10,10,10 };
		m_model_viewer->GetActor(0)->SetPosition(pos);
		m_model_viewer->ResetCamera();
	}
	void on_visb()
	{
		m_model_viewer->ResetCamera();
	}

signals:
void open_folder_signal(std::string path);
void open_folder_fail_signal();


protected:
	void dropEvent(QDropEvent* event);         //drag and drop folder to read
	void dragEnterEvent(QDragEnterEvent *e);   //drag and drop folder to read

	void loadConfig(); // load xml file

private:
	Ui::MainWindow *ui;
	vtkSmartPointer<image_viewer> m_img_viewer;
	vtkSmartPointer<model3d_viewer> m_model_viewer;
	vtkSmartPointer<globalEventCallback> m_seed_callback;
	std::map<std::string, Vertebra > m_VertebraHolder;
};






#endif
