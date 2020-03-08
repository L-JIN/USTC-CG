#include "mainwindow.h"
#include <QtWidgets>
#include <QImage>
#include <QPainter>
#include "ImageWidget.h"


MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);

	setGeometry(300, 150, 800, 450);

	imagewidget_ = new ImageWidget();
	setCentralWidget(imagewidget_);

	CreateActions();
	CreateMenus();
	CreateToolBars();
	CreateStatusBar();
}

MainWindow::~MainWindow()
{
	delete imagewidget_;
}

void MainWindow::closeEvent(QCloseEvent *e)
{

}

void MainWindow::paintEvent(QPaintEvent* paintevent)
{
	
}

void MainWindow::CreateActions()
{
	action_new_ = new QAction(QIcon(":/MainWindow/Resources/images/new.jpg"), tr("&New"), this);
	action_new_->setShortcut(QKeySequence::New);
	action_new_->setStatusTip(tr("Create a new file"));
	// connect ...

	action_open_ = new QAction(QIcon(":/MainWindow/Resources/images/open.jpg"), tr("&Open..."), this);
	action_open_->setShortcuts(QKeySequence::Open);
	action_open_->setStatusTip(tr("Open an existing file"));
	connect(action_open_, &QAction::triggered, imagewidget_, &ImageWidget::Open);

	action_save_ = new QAction(QIcon(":/MainWindow/Resources/images/save.jpg"), tr("&Save"), this);
	action_save_->setShortcuts(QKeySequence::Save);
	action_save_->setStatusTip(tr("Save the document to disk"));
    connect(action_save_,&QAction::triggered, imagewidget_, &ImageWidget::Save);

	action_saveas_ = new QAction(tr("Save &As..."), this);
	action_saveas_->setShortcuts(QKeySequence::SaveAs);
	action_saveas_->setStatusTip(tr("Save the document under a new name"));
	connect(action_saveas_, &QAction::triggered, imagewidget_, &ImageWidget::SaveAs);

	action_invert_ = new QAction(tr("Inverse"), this);
	action_invert_->setStatusTip(tr("Invert all pixel value in the image"));
	connect(action_invert_, &QAction::triggered, imagewidget_, &ImageWidget::Invert);

	action_mirror_ = new QAction(tr("Mirror"), this);
	action_mirror_->setStatusTip(tr("Mirror image vertically or horizontally"));
	// The slot requires more arguments than the signal provides.
	//connect(action_mirror_, &QAction::triggered, imagewidget_, &ImageWidget::Mirror);
	connect(action_mirror_, &QAction::triggered, [this]() {
		imagewidget_->Mirror(); // use default arguments
		});

	action_gray_ = new QAction(tr("Grayscale"), this);
	action_gray_->setStatusTip(tr("Gray-scale map"));
	connect(action_gray_, &QAction::triggered, imagewidget_, &ImageWidget::TurnGray);

	action_restore_ = new QAction(tr("Restore"), this);
	action_restore_->setStatusTip(tr("Show origin image"));
	connect(action_restore_, &QAction::triggered, imagewidget_, &ImageWidget::Restore);

    action_input_=new QAction(tr("Input"),this);
    action_input_->setStatusTip(tr("Input points"));
    connect(action_input_, &QAction::triggered, imagewidget_, &ImageWidget::Input);

    action_idw_=new QAction(tr("IDW"),this);
    action_idw_->setStatusTip(tr("Warp by IDW"));
    connect(action_idw_, &QAction::triggered, imagewidget_, &ImageWidget::DoIDW);

    action_rbf_=new QAction(tr("RBF"),this);
    action_idw_->setStatusTip(tr("Warp by RBF"));
    connect(action_rbf_, &QAction::triggered, imagewidget_, &ImageWidget::DoRBF);

}

void MainWindow::CreateMenus()
{
	//men_ufile_ = menuBar()->addMenu(tr("&File"));
	//men_ufile_->setStatusTip(tr("File menu"));
	//men_ufile_->addAction(action_new_);
	//men_ufile_->addAction(action_open_);
	//men_ufile_->addAction(action_save_);
	//men_ufile_->addAction(action_saveas_);

	//men_uedit_ = menuBar()->addMenu(tr("&Edit"));
	//men_uedit_->setStatusTip(tr("Edit menu"));
	//men_uedit_->addAction(action_invert_);
	//men_uedit_->addAction(action_mirror_);
	//men_uedit_->addAction(action_gray_);
    //men_uedit_->addAction(action_input_);
	//men_uedit_->addAction(action_restore_);
    //men_uedit_->addAction(action_idw_);
    //men_uedit_->addAction(action_rbf_);
}

void MainWindow::CreateToolBars()
{
	toolbar_file_ = addToolBar(tr("File"));
	toolbar_file_->addAction(action_new_);
	toolbar_file_->addAction(action_open_);
	toolbar_file_->addAction(action_save_);

	// Add separator in toolbar 
	toolbar_file_->addSeparator();
	toolbar_file_->addAction(action_invert_);
	toolbar_file_->addAction(action_mirror_);
	toolbar_file_->addAction(action_gray_);
	toolbar_file_->addAction(action_restore_);
    toolbar_file_->addAction(action_idw_);
    toolbar_file_->addAction(action_rbf_);
    toolbar_file_->addAction(action_input_);

}

void MainWindow::CreateStatusBar()
{
	statusBar()->showMessage(tr("Ready"));
}
