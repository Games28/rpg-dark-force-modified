#include "Powers.h"
#include "ObjectManager.h"

void Powers::initSprite()
{
	indicatorsprite[0] = new olc::Sprite("npc/newicon.png");
	indicatorsprite[1] = new olc::Sprite("npc/newiconglow.png");
}

void Powers::TKmove(olc::PixelGameEngine* pge, Object& object, Player& player, Map& map)
{
	
	

	float rotatex = cos(player.rotationAngle);
	float rotatey = sin(player.rotationAngle);

	int xoffset = 0; int yoffset = 0;

	if (rotatex < 0)
		xoffset = -40;
	else
		xoffset = 40;

	if (rotatey < 0)
		yoffset = -40;
	else
		yoffset = 40;
	float movex = (object.x + xoffset) + player.movedifference.x;
	float movey = (object.y + yoffset) + player.movedifference.y;

	pge->DrawString(200, 10, "rx: " + std::to_string(rotatex) + "ry: " + std::to_string(rotatey));

	if (!map.mapHasWallAt(movex, movey))
	{
		object.x += player.movedifference.x;
		object.y += player.movedifference.y;
	}
	
}

void Powers::TKstrafe(Object& object, Player& player)
{
	object.x += player.strafedifference.x;
	object.y += player.strafedifference.y;
}

void Powers::TKrotation(olc::PixelGameEngine* pge,Object& object, Player& player, Map& map)
{
	auto Deg2Rad = [=](float angle) {return angle / 180.0f * 3.14159f; };

	float differenceX = object.x - player.x;
	float differenceY = object.y - player.y;

	float distance = sqrtf(differenceX * differenceX + differenceY * differenceY);

	float angle_player_to_object = atan2f(differenceY, differenceX);

	float angle_difference = player.rotationdifference;
	float rotatex = (cosf(angle_player_to_object + angle_difference) - cosf(angle_player_to_object));
	float rotatey = (sinf(angle_player_to_object + angle_difference) - sinf(angle_player_to_object));

	int xoffset = 0; int yoffset = 0;
	if (rotatex < 0)
		xoffset = -40;
	else
		xoffset = 40;

	if (rotatey < 0)
		yoffset = -40;
	else
		yoffset = 40;
	
	float newposX = (object.x + xoffset) + distance * rotatex;
	float newposY = (object.y + yoffset) + distance * rotatey;
	
	if (!map.mapHasWallAt(newposX, newposY))
	{
		object.x += distance * (cosf(angle_player_to_object + angle_difference) - cosf(angle_player_to_object));
		object.y += distance * (sinf(angle_player_to_object + angle_difference) - sinf(angle_player_to_object));
	}
	
}

bool Powers::isinsight(Object& object, Player& player, float fov, float& angle2player)
{
	auto Deg2Rad = [=](float angle) {return angle / 180.0f * 3.14159f; };
	auto ModuloTwoPI = [=](float angle)
		{
			float a = angle;
			while (a < 0) a += 2.0f * 3.14159f;
			while (a >= 2.0f * 3.14159f) a -= 2.0f * 3.14159f;
			return a;
		};

	float tempx = object.x - player.x;
	float tempy = object.y - player.y;

	angle2player = ModuloTwoPI(atan2(tempx, tempy));
	float fAligneda = (2.0f * 3.14159f - player.rotationAngle) - 0.5f * 3.14159f;

	return abs(ModuloTwoPI(fAligneda + 3.14159f) - angle2player) < fov;
}

void Powers::TKpull(Object& object, Player& player,float Height)
{
	float holddistance = 80.0f;

	float differencex = object.x - player.x;
	float differencey = object.y - player.y;

	float distance = sqrtf(differencex * differencex + differencey * differencey);

	object.x = player.x + cos(player.rotationAngle) * holddistance;
	object.y = player.y + sin(player.rotationAngle) * holddistance;

}

void Powers::TKthrow(Object& object, Map& map,Player& player,float& deltatime)
{
	
	
	Vec2 temp;
	object.physics.Horzphysicssetup( 8.0f);
	Vec2 drag = Force::GenerateDragForce(object, 0.2f);
	float newThrowX = 0;
	float newThrowY = 0;
	
	object.physics.push *= 0.95f;
	object.physics.AddHorzForces(object.physics.push);
	object.physics.AddHorzForces(drag);
	
	
		temp = object.physics.HorzIntegrate(object.rotationangle, deltatime);
		newThrowX = object.x + cos(object.offset) * temp.x;
		newThrowY = object.y + sin(object.offset) * temp.y;
	

	

	

	if (!map.mapHasWallAt(newThrowX, newThrowY))
	{
		object.x = newThrowX;
		object.y = newThrowY;
	}
}
