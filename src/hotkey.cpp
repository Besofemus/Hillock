/*
 * SPDX-License-Identifier: GPL-3.0-or-later

 * Copyright (C) 2019 Roman Erdyakov

 * This file is part of Memedar (flashcard system)
 * Memedar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include <windows.h>
#include <QMessageBox>
#include <QApplication>

#include "hotkey.hpp"


bool hotkey::check_button(void* message)
{
	auto msg {static_cast<MSG*>(message)};
	
	if (msg->message == WM_HOTKEY) {
		QApplication::quit();
		return true;
	}

	return false;
}


namespace hotkey::detail {

	struct modkey
	{
		unsigned int mod {0};
		unsigned int key {0};
	};

	unsigned int mod_key(const QKeySequence& sequence);
	unsigned int hot_key(const QKeySequence& sequence);

	modkey parse(const QKeySequence& sequence);
	modkey parse(QKeyEvent* event, QKeySequence& sequence);
	bool register_key(QWidget* window, int keyid, modkey modkey);
	                  

}

bool hotkey::detail::register_key(QWidget* mainwindow, int keyid,
                                  hotkey::detail::modkey modkey)
{
	auto winid {mainwindow->winId()};
	
	::UnregisterHotKey(HWND(winid), keyid);

	return ::RegisterHotKey(HWND(winid), keyid, modkey.mod, modkey.key);
}

hotkey::detail::modkey hotkey::detail::parse(const QKeySequence& sequence)
{
	hotkey::detail::modkey modkey {};
	QStringList list = sequence.toString().split("+");
	
	for (auto& str : list) {
		
		bool iskey = true;
		
		if (str == "Ctrl") {
			modkey.mod += MOD_CONTROL;
			iskey = false;
		}
		if (str == "Alt") {			
			modkey.mod += MOD_ALT;
			iskey = false;
		}
		if (str == "Shift") {
			modkey.mod += MOD_SHIFT;
			iskey = false;
		}
		
		if (iskey) {
			modkey.key = str.at(0).unicode();
		}
	}
	
	return modkey;
}


using hotkey::quit;

quit::quit(QWidget* mainwindow)
	: m_mainwindow {mainwindow}
{ ;}

const QKeySequence& quit::sequence() const
{
	return m_sequence;
}

void quit::register_key(const QKeySequence& sequence)
{
	if (m_sequence == sequence) {
		return;
	}
	
	detail::modkey modkey {detail::parse(sequence)};
	
	if (detail::register_key(m_mainwindow, m_keyid, modkey)) {

		emit registered(sequence);
	}
	else {
		QMessageBox::warning(m_mainwindow, "Warning", "Can't register hotkey error: "
		                     + QString::number(GetLastError()));
	}

	m_sequence = sequence;
}

