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

#include <QMainWindow>
#include <QApplication>
#include <QFileDialog>
#include <QWheelEvent>
#include <QMessageBox>
#include <QDropEvent>
#include <QMimeData>
#include <QUrl>
#include <QList>

#include "image_viewer.h"



#define DEBUG_MODE_QIN

namespace Ui {
	class MainWindow;
}



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
	void on_DisablePick();

	void on_addseed() {
		double pos[2] = { 100,100};
		m_viewer->AddFrontalSeed(pos );
		std::cout << "movexx" << std::endl;
	};
	
signals:
void open_folder_signal(std::string path);
void open_folder_fail_signal();


protected:
	void dropEvent(QDropEvent* event);
	void dragEnterEvent(QDragEnterEvent *e);

private:
	Ui::MainWindow *ui;
	vtkSmartPointer<image_viewer> m_viewer;
};






#endif
