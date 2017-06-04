/*
Author: QIN Shuo
Organization: RC-MIC (CUHK)
Date: 2016/1/10
*/




#include "mainWindow.h"
#include "ui_mainWindow.h"

#include "dicom_retrieve.h"


#include "vtkSmartPointer.h"
#include "vtkRenderWindow.h"


MainWindow::MainWindow(QWidget *parent) :
QMainWindow(parent), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setAcceptDrops(true);// enable drag drop file
	connect(ui->load_btn, SIGNAL(clicked()), this, SLOT(on_Open_Folder()));
	connect(this, SIGNAL(open_folder_signal(std::string)), this, SLOT(on_Load_folder(std::string)));
	connect(this, SIGNAL(open_folder_fail_signal()), this, SLOT(Open_Folder_Warning()));
	connect(ui->pick_radio, SIGNAL(clicked()), this, SLOT(on_EnablePick()));
	connect(ui->pick_radio, SIGNAL(clicked()), this, SLOT(on_DisablePick()));

	m_viewer = vtkSmartPointer<image_viewer>::New();
	m_viewer->SetFrontalWindow(ui->frontal_view->GetRenderWindow());
	m_viewer->SetProfileWindow(ui->profile_view->GetRenderWindow());
	m_viewer->Set3dWindow(ui->td_view->GetRenderWindow());



	// default init for debug
#ifdef DEBUG_MODE_QIN
	std::string test_folder = "F:/Newdata_5_31/June/16-06-01_Y5726156";
	on_Load_folder(test_folder);
	connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(on_addseed()));
#endif

}

MainWindow:: ~MainWindow()
{
	
}

void MainWindow::on_EnablePick()
{
	if (ui->pick_radio->isChecked())
	m_viewer->EnableSeedWidgets();
}

void MainWindow::on_DisablePick()
{
	if (ui->pick_radio->isChecked())
		;
	else
		m_viewer->DisableSeedWidgets();
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
	
	m_viewer->SetProfileImage(reader->GetProfile());
	m_viewer->SetFrontalImage(reader->GetFrontal());
}

void MainWindow::Open_Folder_Warning()
{
	QMessageBox msgBox;
	msgBox.setText("The invalid folder, please select another.");
	msgBox.exec();
}



