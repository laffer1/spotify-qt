#include "menu/artistlinks.hpp"

Menu::ArtistLinks::ArtistLinks(const lib::spt::artist &artist,
	const lib::http_client &httpClient, QWidget *parent)
	: QMenu(parent),
	artist(artist),
	ddg(httpClient)
{
	setIcon(Icon::get("edit-find"));
	setTitle("Links");

	QMenu::connect(this, &QMenu::aboutToShow,
		this, &Menu::ArtistLinks::onAboutToShow);

	auto *duckDuckGo = addAction("Search on DuckDuckGo");
	QAction::connect(duckDuckGo, &QAction::triggered,
		this, &Menu::ArtistLinks::onDuckDuckGo);

	addSeparator();
	loading = addAction("Searching...");
	loading->setEnabled(false);
}

void Menu::ArtistLinks::onAboutToShow()
{
	if (!loading->isVisible() || !artist.is_valid())
	{
		return;
	}

	ddg.search(artist, [this](const lib::ddg::results &results)
	{
		onLoaded(results);
	});
}

void Menu::ArtistLinks::onDuckDuckGo(bool /*checked*/)
{
	const auto url = lib::fmt::format("https://duckduckgo.com/?t=spotify-qt&q={}", artist.name);
	Url::open(url, LinkType::Web, this);
}

void Menu::ArtistLinks::onLoaded(const lib::ddg::results &results)
{
	loading->setVisible(false);

	for (const auto &item: results.content)
	{
		if (item.data_type == lib::artist_profile::unknown
			|| item.data_type == lib::artist_profile::string)
		{
			continue;
		}

		auto *action = addAction(QString::fromStdString(item.title()));
		QAction::connect(action, &QAction::triggered, [this, item](bool /*checked*/)
		{
			Url::open(item.url(), LinkType::Web, this);
		});
	}
}
