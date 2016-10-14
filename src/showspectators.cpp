#include "showspectators.h"

bool Settings::ShowSpectators::enabled = true;

std::list<std::string> GetObservervators(C_BaseEntity* pEntity)
{
	std::list<std::string> list;

	for (int i = 0; i < engine->GetMaxClients(); ++i)
	{
		C_BaseEntity *entity = entitylist->GetClientEntity(i);
		if (!entity)
			continue;

		if (entity->GetLifeState() == LIFE_ALIVE || entity->GetDormant() || entity->GetHealth() > 0)
			continue;

		if (*entity->GetObserverMode() != ObserverMode_t::OBS_MODE_IN_EYE)
			continue;

		C_BaseEntity *target = entitylist->GetClientEntityFromHandle(entity->GetObserverTarget());
		if (pEntity == target)
		{
			IEngineClient::player_info_t entityInformation;
			engine->GetPlayerInfo(i, &entityInformation);

			list.push_back(entityInformation.name);
		}
	}

	return list;
}

void ShowSpectators::PaintTraverse(VPANEL vgui_panel, bool force_repaint, bool allow_force)
{
	if (!Settings::ShowSpectators::enabled)
		return;

	if (!engine->IsInGame())
		return;

	C_BasePlayer* localplayer = (C_BasePlayer*)entitylist->GetClientEntity(engine->GetLocalPlayer());
	if (localplayer->GetLifeState() != LIFE_ALIVE || localplayer->GetHealth() == 0)
		return;

	std::list<std::string> observators = GetObservervators(localplayer);
	if (observators.size() == 0)
		return;

	int index = 1;
	pstring text;
	text << "Spectators (" << observators.size() << "):";

	Draw::DrawString(text.c_str(), LOC(10, 400), Color(255, 255, 255), normal_font, false);

	for (std::string name : observators)
	{
		Draw::DrawString(name.c_str(), LOC(10, 400 + 20 * index), Color(255, 255, 255), normal_font, false);
		index++;
	}
}