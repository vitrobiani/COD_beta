#pragma once
#include "Soldier.h"


class Squire : public Soldier
{
private:
	int ammo, grenade_count, health_pack;
    int ammo_th, health_pack_th;
	bool prioritize_ammo;
public:
	Squire(Position start_pos, TeamID tid);
    int getHealthPack() const { return health_pack; }
    const char* getType() override { return "Squire"; }
    int getAmmo() override { return ammo; }
    int getGrenades() override { return grenade_count; }
    void moveToTeammate(Position teammate_pos);
    void doesAnybodyNeedHelp();
    int HelpSoldier(Soldier* s);
    void Restock();
    Position findNearestStash(vector<Position> stash);
    bool goToStash(vector<Position> stash);
};

