/*
 * Copyright (c) 2011-2013 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "applicationui.hpp"

#include <bb/cascades/Application>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/LocaleHandler>

// This is what was added as includes
#include <QStringList>
#include <bb/cascades/SceneCover>
#include <bb/cascades/Container>
// *************

using namespace bb::cascades;

ApplicationUI::ApplicationUI(bb::cascades::Application *app) :
        QObject(app)
{
    // prepare the localization
    m_pTranslator = new QTranslator(this);
    m_pLocaleHandler = new LocaleHandler(this);

    bool res = QObject::connect(m_pLocaleHandler, SIGNAL(systemLanguageChanged()), this, SLOT(onSystemLanguageChanged()));
    // This is only available in Debug builds
    Q_ASSERT(res);
    // Since the variable is not used in the app, this is added to avoid a
    // compiler warning
    Q_UNUSED(res);

    // initial load
    onSystemLanguageChanged();

    // Create scene document from main.qml asset, the parent is set
    // to ensure the document gets destroyed properly at shut down.
    QmlDocument *qml = QmlDocument::create("asset:///main.qml").parent(this);
    qml->setContextProperty("_app", this);

    // Create root object for the UI
    AbstractPane *root = qml->createRootObject<AbstractPane>();

    // Set created root object as the application scene
    app->setScene(root);


    // This is what was added as Active Frame initial setup
    QmlDocument *qmlCover = QmlDocument::create("asset:///cover/AppCover.qml").parent(this);

    if (!qmlCover->hasErrors()) {
        // Create the QML Container from using the QMLDocument.
        Container *coverContainer = qmlCover->createRootObject<Container>();

        // Create a SceneCover and set the application cover
        SceneCover *sceneCover = SceneCover::create().content(coverContainer);
        Application::instance()->setCover(sceneCover);

        m_coverLabel = sceneCover->findChild<Label*>("m_coverLabel");
    }
    // *************

    // Initiate this variable
    iterator = 0;

    // Sets a 2 second timer
    timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(onTimerForActiveFrame()));
    timer->start(2000);

	// Call the Active Frame updater function right away
    onTimerForActiveFrame();
}

void ApplicationUI::onSystemLanguageChanged()
{
    QCoreApplication::instance()->removeTranslator(m_pTranslator);
    // Initiate, load and install the application translation files.
    QString locale_string = QLocale().name();
    QString file_name = QString("ActiveFrameFromSettings_%1").arg(locale_string);
    if (m_pTranslator->load(file_name, "app/native/qm")) {
        QCoreApplication::instance()->installTranslator(m_pTranslator);
    }
}

void ApplicationUI::searchThis(QString searchTerm)
{
	// Retrieve the list of searches and append this new one if it doesn't exist
	QStringList listOfSearchTerms = settings.value("listOfSearchTerms", "").toStringList();
	if (listOfSearchTerms.contains(searchTerm))
		return;
	listOfSearchTerms.append(searchTerm);
	settings.setValue("listOfSearchTerms", listOfSearchTerms);
}

void ApplicationUI::clearSearches()
{
	// Clear the settings
	settings.setValue("listOfSearchTerms", "");
}

void ApplicationUI::onTimerForActiveFrame()
{
	// Called every 2 secs, this function will update the Active Frame text

	// Retrieve the list of searches
	QStringList listOfSearchTerms = settings.value("listOfSearchTerms", "").toStringList();

	// Remove blank result, this happens for empty list : the first item will be a blank one
	for (int i = 0; i < listOfSearchTerms.size(); i++)
		if (listOfSearchTerms[i] == "")
			listOfSearchTerms.removeAt(i);

	// Return if the list is empty
	if (listOfSearchTerms.isEmpty()) {
		m_coverLabel->setText("No search terms yet...");
		return;
	}

	// Goes back to first item if iterator is higher than number of items in the list
	if (iterator >= listOfSearchTerms.size())
		iterator = 0;

	// Sets the Active Frame text
	m_coverLabel->setText(listOfSearchTerms[iterator]);

	// Go to next item for next time this function will be called
	iterator++;
}
