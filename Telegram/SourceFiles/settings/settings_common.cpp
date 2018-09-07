/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#include "settings/settings_common.h"

#include "settings/settings_chat.h"
#include "settings/settings_general.h"
#include "settings/settings_information.h"
#include "settings/settings_main.h"
#include "settings/settings_notifications.h"
#include "settings/settings_privacy_security.h"
#include "ui/wrap/padding_wrap.h"
#include "ui/wrap/vertical_layout.h"
#include "ui/widgets/labels.h"
#include "info/profile/info_profile_button.h"
#include "boxes/abstract_box.h"
#include "lang/lang_keys.h"
#include "mainwindow.h"
#include "styles/style_settings.h"

namespace Settings {

object_ptr<Section> CreateSection(
		Type type,
		not_null<QWidget*> parent,
		not_null<Window::Controller*> controller,
		UserData *self) {
	switch (type) {
	case Type::Main:
		return object_ptr<::Settings::Main>(parent, controller, self);
	case Type::Information:
		return object_ptr<::Settings::Information>(parent, self);
	case Type::Notifications:
		return object_ptr<::Settings::Notifications>(parent, self);
	case Type::PrivacySecurity:
		return object_ptr<::Settings::PrivacySecurity>(parent, self);
	case Type::General:
		return object_ptr<::Settings::General>(parent, self);
	case Type::Chat:
		return object_ptr<::Settings::Chat>(parent, self);
	}
	Unexpected("Settings section type in Widget::createInnerWidget.");
}

void AddSkip(not_null<Ui::VerticalLayout*> container) {
	AddSkip(container, st::settingsSectionSkip);
}

void AddSkip(not_null<Ui::VerticalLayout*> container, int skip) {
	container->add(object_ptr<Ui::FixedHeightWidget>(
		container,
		skip));
}

void AddDivider(not_null<Ui::VerticalLayout*> container) {
	container->add(object_ptr<BoxContentDivider>(container));
}

not_null<Button*> AddButton(
		not_null<Ui::VerticalLayout*> container,
		LangKey text,
		const style::InfoProfileButton &st) {
	return container->add(object_ptr<Button>(
		container,
		Lang::Viewer(text),
		st));
}

not_null<Button*> AddButtonWithLabel(
		not_null<Ui::VerticalLayout*> container,
		LangKey text,
		rpl::producer<QString> label,
		const style::InfoProfileButton &st) {
	const auto button = AddButton(container, text, st);
	const auto name = Ui::CreateChild<Ui::FlatLabel>(
		button.get(),
		std::move(label),
		st::settingsButtonRight);
	rpl::combine(
		name->widthValue(),
		button->widthValue()
	) | rpl::start_with_next([=] {
		name->moveToRight(
			st::settingsButtonRightPosition.x(),
			st::settingsButtonRightPosition.y());
	}, name->lifetime());
	name->setAttribute(Qt::WA_TransparentForMouseEvents);
	return button;
}

void FillMenu(Fn<void(Type)> showOther, MenuCallback addAction) {
	addAction(
		lang(lng_settings_edit_info),
		[=] { showOther(Type::Information); });
	addAction(
		lang(lng_settings_logout),
		[=] { App::wnd()->onLogout(); });
}

} // namespace Settings