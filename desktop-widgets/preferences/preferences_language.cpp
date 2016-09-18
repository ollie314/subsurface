#include "preferences_language.h"
#include "ui_prefs_language.h"
#include "core/helpers.h"
#include "core/subsurface-qt/SettingsObjectWrapper.h"

#include <QApplication>
#include <QMessageBox>
#include <QSortFilterProxyModel>

#include "qt-models/models.h"

PreferencesLanguage::PreferencesLanguage() : AbstractPreferencesWidget(tr("Language"), QIcon(":/language"), 4)
{
	ui = new Ui::PreferencesLanguage();
	ui->setupUi(this);

	QSortFilterProxyModel *filterModel = new QSortFilterProxyModel();
	filterModel->setSourceModel(LanguageModel::instance());
	filterModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
	ui->languageDropdown->setModel(filterModel);
	filterModel->sort(0);
	connect(ui->languageFilter, &QLineEdit::textChanged,
			filterModel, &QSortFilterProxyModel::setFilterFixedString);
}

PreferencesLanguage::~PreferencesLanguage()
{
	delete ui;
}

void PreferencesLanguage::refreshSettings()
{
	ui->languageSystemDefault->setChecked(prefs.locale.use_system_language);
	ui->timeFormatSystemDefault->setChecked(!prefs.time_format_override);
	ui->dateFormatSystemDefault->setChecked(!prefs.date_format_override);
	ui->timeFormatEntry->setText(prefs.time_format);
	ui->dateFormatEntry->setText(prefs.date_format);
	ui->shortDateFormatEntry->setText(prefs.date_format_short);
	QAbstractItemModel *m = ui->languageDropdown->model();
	QModelIndexList languages = m->match(m->index(0, 0), Qt::UserRole, prefs.locale.language);
	if (languages.count())
		ui->languageDropdown->setCurrentIndex(languages.first().row());
}

#include <QDebug>
void PreferencesLanguage::syncSettings()
{
	bool useSystemLang = prefs.locale.use_system_language;
	QString currentText = ui->languageDropdown->currentText();

	if (useSystemLang != ui->languageSystemDefault->isChecked() ||
		(!useSystemLang && currentText != prefs.locale.language)) {
		QMessageBox::warning(this, tr("Restart required"),
			tr("To correctly load a new language you must restart Subsurface."));
	}

	auto lang = SettingsObjectWrapper::instance()->language_settings;
	lang->setLanguage(currentText);
	lang->setUseSystemLanguage(ui->languageSystemDefault->isChecked());
	lang->setTimeFormatOverride(!ui->timeFormatSystemDefault->isChecked());
	lang->setDateFormatOverride(!ui->dateFormatSystemDefault->isChecked());
	lang->setTimeFormat(ui->timeFormatEntry->text());
	lang->setDateFormat(ui->dateFormatEntry->text());
	lang->setDateFormatShort(ui->shortDateFormatEntry->text());
	uiLanguage(NULL);
}
