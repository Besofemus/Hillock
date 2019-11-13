#include <QApplication>
#include <QString>

#include "mapper.hpp"
#include "view/runners_decorator.hpp"
#include "view/mainwindow.h"

#include "hotkey.hpp"

int main(int argc, char* argv[])
{	
	QApplication app {argc, argv};
	QCoreApplication::addLibraryPath("./");

	auto runners {new view::runners_decorator {}};

	view::mainwindow mainwindow {runners};
	
	hotkey::quit quitkey {&mainwindow};
	data::mapper mapper {&mainwindow};
	
	QObject::connect(&mainwindow, &view::mainwindow::new_runner,
	                 [&mapper](const QString& filepath)
	                 {
		                 mapper.append_runner(filepath);
	                 });

	QObject::connect(runners, &view::runners_decorator::remove_runner,
	                 [&mapper](const QString& filepath)
	                 {
		                 mapper.remove_runner(filepath);
	                 });
	
	QObject::connect(&quitkey, &hotkey::quit::registered,
	                 [&mapper](const QKeySequence& sequence)
	                 {
		                 mapper.set_quithotkey(sequence);
	                 });

	QObject::connect(&mainwindow, &view::mainwindow::quit_edited,
	                 [&quitkey](const QKeySequence& quithotkey)
	                 {
		                 quitkey.register_key(quithotkey);
	                 });

	mapper.read(runners, quitkey);
	mainwindow.set_quithotkey(quitkey.sequence());
	mainwindow.show();
	
	return app.exec();
}
