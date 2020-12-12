#include "spotify.hpp"

lib::spt::core::spotify::spotify(lib::Settings &settings)
	: api(settings)
{
}

nlohmann::json lib::spt::core::spotify::get(const std::string &url)
{
	return api.get(url);
}