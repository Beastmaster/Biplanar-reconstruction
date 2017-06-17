/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2016/1/10
*/

#include <QString>
#include <QStringList>

#include <string.h>
#include <functional>

#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"
#include "vtkArrowSource.h"
#include "vtkSphereSource.h"
#include "vtkCylinderSource.h"

#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "dicom_retrieve.h"

MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setAcceptDrops(true);// enable drag drop file
	connect(ui->load_btn, SIGNAL(clicked()), this, SLOT(on_Open_Folder()));
	connect(this, SIGNAL(open_folder_signal(std::string)), this, SLOT(on_Load_folder(std::string)));
	connect(this, SIGNAL(open_folder_fail_signal()), this, SLOT(Open_Folder_Warning()));
	connect(ui->pick_radio, SIGNAL(clicked()), this, SLOT(on_EnablePick()));

	m_seed_callback = vtkSmartPointer<globalEventCallback>::New();

	m_img_viewer = vtkSmartPointer<image_viewer>::New();
	m_img_viewer->SetCallback(m_seed_callback);
	m_img_viewer->SetFrontalWindow(ui->frontal_view->GetRenderWindow());
	m_img_viewer->SetProfileWindow(ui->profile_view->GetRenderWindow());

	m_model_viewer = vtkSmartPointer<model3d_viewer>::New();
	m_model_viewer->SetRenderWindow(ui->td_view->GetRenderWindow());
	m_model_viewer->SetCallback(m_seed_callback);
	m_seed_callback->Set3dWindow(m_model_viewer->GetRenderWindow());

	// default init for debug
#ifdef DEBUG_MODE
	std::string test_folder = "F:/Newdata_5_31/June/16-06-01_Y5726156";
	on_Load_folder(test_folder);
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_addseed()));
	connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(on_addactor()));
	connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(on_move()));
	connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(on_visb()));
	loadConfig();
#endif

}

MainWindow:: ~MainWindow()
{
	
}

void MainWindow::on_EnablePick()
{
	if (ui->pick_radio->isChecked())
		m_img_viewer->EnableSeedWidgets();
	else
		m_img_viewer->DisableSeedWidgets();
}

void MainWindow::AutoInitPosition()
{
	for (size_t i = 0; i < 17; i++)
	{
		double x = 100;
		double y = i * 40;
		double z = 100;

		double front_pos[3] = { x,y,0 };
		double profi_pos[3] = { z,y,0 };
		m_img_viewer->AddFrontalSeed(front_pos);
		m_img_viewer->AddProfileSeed(profi_pos);
	}
	m_img_viewer->InvokeEvent(vtkCommand::InteractionEvent);
}

void MainWindow::dropEvent(QDropEvent* event)
{
	const QMimeData* mimeData = event->mimeData();

	if (mimeData->hasUrls())
	{
		QStringList pathList;
		QList<QUrl> urlList = mimeData->urls();
		for (int i = 0; i < urlList.size() && i < 32; i++)
		{
			pathList.append(urlList.at(i).toLocalFile());
		}
		if (pathList.size()>0)
		{
			emit open_folder_signal(pathList[0].toStdString());
		}
	}
}
void MainWindow::dragEnterEvent(QDragEnterEvent * e)
{
	if (e->mimeData()->hasUrls()) {
		e->acceptProposedAction();
	}
}

void MainWindow::loadConfig()
{
	auto read_stl = [](std::string name) 
	{
		auto reader = vtkSmartPointer<vtkSTLReader>::New();
		reader->SetFileName(name.c_str());
		reader->Update();
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(reader->GetOutput());
		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);

		actor->GetProperty()->SetColor(float(std::rand()%100)/100, float(std::rand()) / RAND_MAX, float(std::rand()) / RAND_MAX);
		return actor;
	};
	auto create_arrow = []()
	{
		auto arrow = vtkSmartPointer<vtkCylinderSource>::New();
		arrow->SetCenter(0, 0, 0);
		arrow->SetRadius(10);
		arrow->SetHeight(30);
		arrow->Update();
		auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
		mapper->SetInputData(arrow->GetOutput());
		auto actor = vtkSmartPointer<vtkActor>::New();
		actor->SetMapper(mapper);
		return actor;
	};

	// read Throcic
	std::string Th_base = "D:/Project/spine_reconstruct/resources/vertebra/T";
	for (size_t i = 1; i <= 12; i++)
	{
#ifdef DEBUG_MODE
		auto actor = create_arrow();
#else
		auto cp = Th_base;
		auto name = cp.append(std::to_string(i)).append( ".stl");
		auto actor = read_stl(name);
#endif
		Vertebra throacic;
		throacic.name = std::string("T").append(std::to_string(i));
		throacic.vertebra = actor;
		m_VertebraHolder[throacic.name] = throacic;
	}
	Th_base = "D:/Project/spine_reconstruct/resources/vertebra/L";
	for (size_t i = 1; i <= 5; i++)
	{
#ifndef DEBUG_MODE
		auto actor = create_arrow();
#else
		auto cp = Th_base;
		auto name = cp.append(std::to_string(i)).append(".stl");
		auto actor = read_stl(name);
#endif // DEBUG_MODE

		Vertebra lumber;
		lumber.name = std::string("L").append(std::to_string(i));
		lumber.vertebra = actor;
		m_VertebraHolder[lumber.name] = lumber;
	}

	for (size_t i = 1; i <= 12; i++)
	{
		auto vname = std::string("T").append(std::to_string(i));
		m_model_viewer->AddActor(m_VertebraHolder[vname].vertebra);
	}
	for (size_t i = 1; i <= 5; i++)
	{
		auto vname = std::string("L").append(std::to_string(i));
		m_model_viewer->AddActor(m_VertebraHolder[vname].vertebra);
	}
}

void MainWindow::on_Open_Folder()
{
	QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"));
	if (dir.isEmpty())
		;
	else
		emit(open_folder_signal(dir.toStdString()));
}
void MainWindow::on_Load_folder(std::string path)
{
	auto reader = vtkSmartPointer<dicom_retrieve>::New();
	if (reader->SetDicomFolder(path) != 0)
	{
		emit open_folder_fail_signal();
		return;
	}
	reader->Update();
	
	m_img_viewer->SetProfileImage(reader->GetProfile());
	m_img_viewer->SetFrontalImage(reader->GetFrontal());
	AutoInitPosition();
}

void MainWindow::Open_Folder_Warning()
{
	QMessageBox msgBox;
	msgBox.setText("The invalid folder, please select another.");
	msgBox.exec();
}



