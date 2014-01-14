
#include "../../src/qtutil/accordion.hpp"

#include <sstream>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QApplication>


#define TST5 if(!label5){msg.show(); return;};

class Acctester: public QWidget
{
	//Q_OBJECT
	//is required fot test to work
	//atm compiler error if used: CVVisual/manual_test/accordion/main.cpp: The file contains a Q_OBJECT macro, but does not include "main.moc" !
	// if comment is removed it will break build
	// will look into it later
public:
	Acctester(QWidget* parent= nullptr):
		QWidget{parent},
		acc{new cvv::qtutil::Accordion{}},
		label5{nullptr},
		msg{"no last element inserted at 5. (maybe already deleted)"}
	{
		//create
		QPushButton * pfront	=new QPushButton{"pfront"};
		QPushButton * pback	=new QPushButton{"pback"};
		QPushButton * insert5	=new QPushButton{"insert elem at pos 5"};
		QPushButton * remove5	=new QPushButton{"remove last element inserted at 5"};
		QPushButton * clear	=new QPushButton{"clear"};
		QPushButton * hideAll	=new QPushButton{"hideAll"};
		QPushButton * hide5	=new QPushButton{"hide5"};
		QPushButton * showAll	=new QPushButton{"showAll"};
		QPushButton * show5	=new QPushButton{"show5"};
		QPushButton * collapseAll=new QPushButton{"collapseAll"};
		QPushButton * collapse5	=new QPushButton{"collapse5"};
		QPushButton * expandAll	=new QPushButton{"expandAll"};
		QPushButton * expand5	=new QPushButton{"expand5"};

		QVBoxLayout* l=new QVBoxLayout{};

		//build
		l->addWidget(pfront);
		l->addWidget(pback);
		l->addWidget(insert5);
		l->addWidget(remove5);
		l->addWidget(clear);
		l->addWidget(hideAll);
		l->addWidget(hide5);
		l->addWidget(showAll);
		l->addWidget(show5);
		l->addWidget(collapseAll);
		l->addWidget(collapse5);
		l->addWidget(expandAll);
		l->addWidget(expand5);
		l->addWidget(acc);

		//connect
		connect(pfront,		SIGNAL(pressed()),this, SLOT(spfront()));
		connect(pback,		SIGNAL(pressed()),this, SLOT(spback()));
		connect(clear,		SIGNAL(pressed()),this, SLOT(sclear()));
		connect(hideAll,	SIGNAL(pressed()),this, SLOT(shideAll()));
		connect(showAll,	SIGNAL(pressed()),this, SLOT(sshowAll()));
		connect(collapseAll,	SIGNAL(pressed()),this, SLOT(scollapseAll()));
		connect(expandAll,	SIGNAL(pressed()),this, SLOT(sexpandAll()));

		connect(insert5,	SIGNAL(pressed()),this, SLOT(sinsert5()));
		connect(remove5,	SIGNAL(pressed()),this, SLOT(sremove5()));
		connect(hide5,		SIGNAL(pressed()),this, SLOT(shide5()));
		connect(show5,		SIGNAL(pressed()),this, SLOT(sshow5()));
		connect(collapse5,	SIGNAL(pressed()),this, SLOT(scollapse5()));
		connect(expand5,	SIGNAL(pressed()),this, SLOT(sexpand5()));

		setLayout(l);
	}



	cvv::qtutil::Accordion* acc;
	cvv::qtutil::Accordion::Handle label5;


	//errorlabel
	QLabel msg;

	std::string s(std::string at)
	{
		std::stringstream s;
		s<<"inserted at "<<at<<"\t nr: "<<acc->size();
		return s.str();
	}


public slots:
	void sclear(){acc->clear();}

	void shideAll(){acc->hideAll();}
	void sshowAll(){acc->showAll();}
	void scollapseAll(){acc->collapseAll();}
	void sexpandAll(){acc->expandAll();}

	void spfront(){acc->push_front(s("0").c_str(), *(new QLabel{s("0").c_str()}));}
	void spback(){acc->push_back(s("end").c_str(), *(new QLabel{s("end").c_str()}));}
	void sinsert5()
	{
		label5=acc->insert(s("5").c_str(), *(new QLabel{s("5").c_str()}), 5);
	}
	void sremove5()
	{
		TST5
		acc->remove(label5);
		label5 = nullptr;
	}

	void shide5(){TST5 acc->hide(label5); }
	void sshow5(){TST5 acc->show(label5); }
	void sexpand5(){TST5 acc->expand(label5); }
	void scollapse5(){TST5 acc->collapse(label5); }

};

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	Acctester w{};

	w.show();
	return a.exec();
}
