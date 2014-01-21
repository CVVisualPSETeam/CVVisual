#include "../../src/qtutil/registerhelper.hpp"


#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <iostream>
#include "../../src/qtutil/signalslot.hpp"

class LabelRegister:public cvv::qtutil::RegisterHelper<QLabel>
{
public:
	LabelRegister(QWidget* parent = nullptr):
			RegisterHelper<QLabel>{parent},
		lay{new QVBoxLayout{}}, lab{new QLabel{}},
		s{[this](){this->updlabel();}},
		reg{[this](QString s){this->regist(s);}}
	{
		lay->addWidget(comboBox_);
		lay->addWidget(lab);
		setLayout(lay);
		connect(comboBox_,
			 SIGNAL(currentTextChanged(const QString &)),
			 &s, SLOT(slot())
		);
		connect( &signElementRegistered_, SIGNAL(signal(QString)),
			 &reg, SLOT(slot(QString)));
	}

	QVBoxLayout* lay;
	QLabel* lab;

	cvv::qtutil::Slot s;
	cvv::qtutil::SlotQString reg;

	void updlabel()
	{
		std::cout<<"~updatelabel\n";
		std::unique_ptr<QLabel> newl{(*this)()(nullptr)};
		lay->removeWidget(lab);
		lab->setParent(nullptr);
		delete lab;
		lab = newl.release();
		lay->addWidget(lab);

		std::cout<<"\t~current selection\t"<<selection().toStdString()<<"\n"
				<<"\t~txt of func\t"<<lab->text().toStdString()<<"\n";
	}

	void regist(QString s){
		std::cout<<"regevent\t"<<s.toStdString()<<std::endl;
	}

};



void regnewlabelfunc()
{
	unsigned int cnt=LabelRegister::registeredElements().size();
	std::cout<<"#regnewlabelfunc "<<cnt<<std::endl
	<<"\t#has?\t"<<LabelRegister::has(QString::number(cnt))<<"\n";
	LabelRegister::registerElement(QString::number(cnt),
		[=](QWidget*){
			std::cout<<"§label fun\n";
			std::cout<<"\t§cnt in label fun\t"<<cnt<<"\n";
			std::cout<<"\t§&cnt in label fun\t"<<&cnt<<"\n";
			return std::unique_ptr<QLabel>{new QLabel{QString::number(cnt)}};
		}
	);
	std::cout<<"\t#anz now\t"<<LabelRegister::registeredElements().size()<<std::endl;
}



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QWidget w{};

	LabelRegister* r1 = new LabelRegister{};
	LabelRegister* r2 = new LabelRegister{};

	QVBoxLayout* lay = new QVBoxLayout{};
	QPushButton* b = new QPushButton{"add"};
	lay->addWidget(b);
	lay->addWidget(r1);
	lay->addWidget(r2);

	cvv::qtutil::Slot bPushed{&regnewlabelfunc};

	 QObject::connect(b, SIGNAL(clicked()), &bPushed, SLOT(slot()));
	w.setLayout(lay);
	w.show();
	return a.exec();
}


