#include "close_window.hpp"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

namespace cvv {
namespace gui {

CloseWindow::CloseWindow(util::Reference<controller::ViewController> controller):
	controller{controller}
{
	TRACEPOINT;
	auto mainWidget = new QWidget{this};
	auto layout = new QVBoxLayout{mainWidget};
	
	auto helpButton = new QPushButton{"Help", mainWidget};
	layout->addWidget(helpButton);
	connect(helpButton, SIGNAL(clicked()), this, SLOT(helpRequested()));
			
	auto resumeButton = new QPushButton{"Resume program execution", mainWidget};
	layout->addWidget(resumeButton);
	connect(resumeButton, SIGNAL(clicked()), this, SLOT(resumeProgramExecution()));
	resumeButton->setStyleSheet("QPushButton {background-color: green; color: white;}");

	auto ffButton = new QPushButton{"Fast Forward", mainWidget};
	layout->addWidget(ffButton);
	connect(ffButton, SIGNAL(clicked()), this, SLOT(fastForwardRequested()));
	auto ffExplanation = new QLabel("Only stops at cvv::finalShow() followed by a call.", mainWidget);
	layout->addWidget(ffExplanation);
	
	auto hideCVVButton = new QPushButton{"Exit program", mainWidget};
	layout->addWidget(hideCVVButton);
	connect(hideCVVButton, SIGNAL(clicked()), this, SLOT(hideCVVRequested()));
	auto hideExplanation = new QLabel{"Hides the application.", mainWidget};
	layout->addWidget(hideExplanation);
	hideCVVButton->setStyleSheet("QPushButton {background-color: red; color: white;}");
			
	auto exitButton = new QPushButton{"Exit(0)", mainWidget};
	layout->addWidget(exitButton);
	connect(exitButton, SIGNAL(clicked()), this, SLOT(exitRequested()));
	exitButton->setStyleSheet("QPushButton {background-color: DarkRed; color: white;}");
			
	auto cancelButton = new QPushButton{"Cancel", mainWidget};
	layout->addWidget(cancelButton);
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

	mainWidget->setLayout(layout);
	setCentralWidget(mainWidget);
	setMinimumSize(layout->sizeHint());
	setWindowTitle("CVVisual | Close window");
	TRACEPOINT;
}

void CloseWindow::helpRequested()
{
	TRACEPOINT;
	controller->openHelpBrowser("closewindow");
	close();
	TRACEPOINT;
}

void CloseWindow::exitRequested()
{
	TRACEPOINT;
	exit(0);
}

void CloseWindow::fastForwardRequested()
{
	TRACEPOINT;
	controller->setMode(controller::Mode::FAST_FORWARD);
	close();
}

void CloseWindow::hideCVVRequested()
{
	TRACEPOINT;
	controller->setMode(controller::Mode::HIDE);
	close();
}

void CloseWindow::resumeProgramExecution()
{
	TRACEPOINT;
	controller->resumeProgramExecution();
	close();
	TRACEPOINT;
}

}
}