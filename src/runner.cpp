#include <QString>
#include <QLabel>
#include <QProcess>

#include "runner.hpp"


using model::runner;

runner::runner(QString&& path)
	: m_path {std::move(path)}
{ ;}


void runner::start(QObject* parent)
{
	auto process {new QProcess {parent}};
	
	process->start(m_path);	
}

const QString& runner::name() const
{
	return m_path;
}

void runner::change_path(QString&& path)
{
	m_path = std::move(path);
}

using view::runners;

runners::runners()
	: m_layout {new QVBoxLayout {this}}
{ ;}


void runners::add(model::runner&& runner)
{
	m_layout->addWidget(new QLabel {runner.name()});

	m_runners.push_back(std::move(runner));
}

void runners::run()
{
	for (auto& e : m_runners) {
		e.start(this);
	}
}