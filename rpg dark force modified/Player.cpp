#include "Player.h"

Player::Player()
{
	x = WINDOW_WIDTH / 2;
	y = WINDOW_HEIGHT / 2;
	width = 5;
	height = 5;
	turnDirection = 0;
	walkDirection = 0;
	rotationAngle = 0;
	walkSpeed = 100;
	turnSpeed = 45 * (PI/ 180.0f);
	strafeLeft = true;
	strafeRight = true;
	strafedirection = 0;
	frun = 1;
	lookspeed = 200.0f;
	lookupordown = 0;
	vertlook = 0;
	strafeupspeed = 1.0f;
	fPlayerH = 0.5f;
	lookvert = false;
	movevert = false;
	rotatebefore = 0.0f;
	rotateafter = 0.0f;
	movementbefore = { 0.0f,0.0f };
	movementafter = { 0.0f,0.00f };
	movedifference = { 0.0f, 0.0f };
	strafeafter = { 0.0f,0.0f };
	strafebefore = { 0.0f,0.0f };
	strafedifference = { 0.0f,0.0f };

}

Player::~Player()
{
}

void Player::processInput(olc::PixelGameEngine* PGEptr,bool& pickedup, float deltatime, Map& map)
{
	if (PGEptr->GetKey(olc::SHIFT).bHeld)
	{
		jumping = true;
	}
	if (PGEptr->GetKey(olc::SHIFT).bReleased)
	{
		jumping = false;
	}

	if (PGEptr->GetKey(olc::W).bHeld)
	{
		walkDirection = +1;
	}

	if (PGEptr->GetKey(olc::S).bHeld)
	{
		walkDirection = -1;
	}

	if (PGEptr->GetKey(olc::Q).bHeld)
	{
		strafeLeft = true;
		strafedirection = +1;
	}


	if (PGEptr->GetKey(olc::E).bHeld)
	{
		strafeRight = true;
		strafedirection = -1;
	}


	if (PGEptr->GetKey(olc::D).bHeld)
	{
		turnDirection = +1;
	}

	if (PGEptr->GetKey(olc::A).bHeld)
	{
		turnDirection = -1;
	}

	if (PGEptr->GetKey(olc::SHIFT).bHeld)
	{
		frun = 3;
	}

	if (PGEptr->GetKey(olc::UP).bHeld)
	{
		lookvert = true;
		lookupordown += lookspeed * deltatime;
	}
	if (PGEptr->GetKey(olc::DOWN).bHeld)
	{
		lookvert = true;
		lookupordown -= lookspeed * deltatime;
	}

	if (PGEptr->GetKey(olc::W).bReleased)
	{
		walkDirection = 0;
	}

	if (PGEptr->GetKey(olc::S).bReleased)
	{
		walkDirection = 0;
	}

	if (PGEptr->GetKey(olc::Q).bReleased)
	{

		strafedirection = 0;
	}

	if (PGEptr->GetKey(olc::E).bReleased)
	{

		strafedirection = 0;
	}

	if (PGEptr->GetKey(olc::D).bReleased)
	{
		turnDirection = 0;
	}

	if (PGEptr->GetKey(olc::A).bReleased)
	{
		turnDirection = 0;
	}
	if (PGEptr->GetKey(olc::SHIFT).bReleased)
	{
		frun = 1;
	}
	if (PGEptr->GetKey(olc::UP).bReleased)
	{
		lookvert = false;

	}
	if (PGEptr->GetKey(olc::DOWN).bReleased)
	{
		lookvert = false;

	}

	//NOTE - for multi level rendering there's only clamping to keep fPlayerH > 0.0f, there's no upper limit.

		// cache current height of horizon, so that you can compensate for changes in it via the look up value
	float fCachHorHeight = float(WINDOW_HEIGHT) * fPlayerH + lookupordown;
	//move up or down test

	if (MULTIPLE_LEVELS)
	{
		// if the player height is adapted, keep horizon height stable by compensating with look up value
		if (PGEptr->GetKey(olc::PGUP).bHeld)
		{
			movevert = true;
			fPlayerH += strafeupspeed * frun * deltatime;
			lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
		}
		if (PGEptr->GetKey(olc::PGDN).bHeld)
		{
			movevert = true;
			float fNewHeight = fPlayerH - strafeupspeed * frun * deltatime;
			if (fNewHeight > 0.0f && map.floatheightmap(int(x), int(y)) < fNewHeight)
			{
				fPlayerH = fNewHeight;
				lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
			}
		}
	
	}
	else
	{
		if (PGEptr->GetKey(olc::PGUP).bHeld)
		{
			movevert = true;
			float fNewHeight = fPlayerH + strafeupspeed * frun * deltatime;
			if (fNewHeight < 1.0f)
			{
				fPlayerH = fNewHeight;
				lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
			}
		}
		if (PGEptr->GetKey(olc::PGDN).bHeld)
		{
			movevert = true;
			float fNewHeight = fPlayerH - strafeupspeed * frun * deltatime;
			if (fNewHeight > 0.0f)
			{
				fPlayerH = fNewHeight;
				lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
			}
		}
	}
	
	if (PGEptr->GetKey(olc::PGUP).bReleased)
	{
		movevert = false;
	
	}
	if (PGEptr->GetKey(olc::PGDN).bReleased)
	{
		movevert = false;
	
	}


	//look up or down test
	

	// reset height and lookup factor upon pressing R
	if (PGEptr->GetKey(olc::R).bReleased) { fPlayerH = 0.5f; lookupordown = 0.0f; }
}

void Player::movePlayer(olc::PixelGameEngine* pge,float deltatime, Map& map)
{
	rotatebefore = rotationAngle;
	movementbefore = { x,y };
	auto normalizeAngle = [=](float* angle)
	{
		*angle = remainder(*angle, TWO_PI);
		if (*angle < 0) {
			*angle = TWO_PI + *angle;
		}
	};

	// move forward, backwards, turn right and turn left movment code
	rotationAngle += turnDirection * turnSpeed * deltatime;
	normalizeAngle(&rotationAngle);
	rotateafter = rotationAngle;
	rotationdifference = rotateafter - rotatebefore;
	float moveStep = walkDirection * walkSpeed * deltatime;

	float rotatex = cos(rotationAngle);
	float rotatey = sin(rotationAngle);

	int xoffset = 0; int yoffset = 0;

	if (rotatex < 0)
		xoffset = -40;
	else
		xoffset = 40;

	if (rotatey < 0)
		yoffset = -40;
	else
		yoffset = 40;
	
	float tempplayerx = (x + xoffset) + cos(rotationAngle) * moveStep * frun;
	float tempplayery = (y + yoffset) + sin(rotationAngle) * moveStep * frun;

	float newPlayerX = x + cos(rotationAngle) * moveStep * frun;
	float newPlayerY = y + sin(rotationAngle) * moveStep * frun;

	//wall collision
	if (!map.mapHasWallAt(newPlayerX, newPlayerY)) {
		
		x = newPlayerX;
		y = newPlayerY;
		movementafter = { newPlayerX,newPlayerY };
	}
	//strafe left and strafe right movement code

	movedifference = movementafter - movementbefore;
	float strafeStep = strafedirection * walkSpeed * deltatime;
	strafebefore = { x,y };
	
	if (strafeLeft)
	{
		
		strafePlayerX = x - sin(rotationAngle) * strafeStep;
		strafePlayerY = y + cos(rotationAngle) * strafeStep;
		
	}
	
	if(strafeRight)
	{
		
		strafePlayerX = x + sin(rotationAngle) * strafeStep;
		strafePlayerY = y - cos(rotationAngle) * strafeStep;
		
	}
	

	if (!map.mapHasWallAt(strafePlayerX, strafePlayerY))
	{
		x = strafePlayerX;
		y = strafePlayerY;
		strafeafter = { strafePlayerX,strafePlayerY };
		
	}
	strafedifference = strafeafter - strafebefore;
	

	//look up and look down cod
	//mouse controls
	float fRotFactor, fTiltFactor;
	
	if (bmousecontrol && GetMouseSteering(pge, fRotFactor, fTiltFactor))
	{

		rotationAngle += turnSpeed * fRotFactor * deltatime;
		lookupordown -= lookspeed * fTiltFactor * deltatime;
		pge->DrawString(200, 10, "fRotFactor " + std::to_string(fRotFactor));
		pge->DrawString(200, 20, "fTiltFactor " + std::to_string(fTiltFactor));
		if (fRotFactor < 0.0f)
		{
			turnDirection = -1;
		}
		else if (fRotFactor > 0.0f)
		{
			turnDirection = +1;
		}
		else
		{
			turnDirection  = 0;
		}
		
	}
}

void Player::renderMapPlayer(olc::PixelGameEngine* PGEptr)
{
	olc::Pixel p = olc::CYAN;
	PGEptr->FillRect(
		x * MINIMAP_SCALE_FACTOR,
		y * MINIMAP_SCALE_FACTOR,
		width * MINIMAP_SCALE_FACTOR,
		height * MINIMAP_SCALE_FACTOR,
		p
	);
}

bool Player::GetMouseSteering(olc::PixelGameEngine* pge,float& fHorPerc, float& fVerPerc)
{
	int nMouseX = pge->GetMouseX();
	int nMouseY = pge->GetMouseY();

	
	
		float fRangeX = (nMouseX - (WINDOW_WIDTH / 2)) / float(WINDOW_WIDTH / 2);
		float fRangeY = (nMouseY - (WINDOW_HEIGHT / 2)) / float(WINDOW_HEIGHT / 2);

		// the screen width / height is mapped onto [ -1.0, +1.0 ] range
		// the range [ -0.2f, +0.2f ] is the stable (inactive) zone
		fHorPerc = 0.0f;
		fVerPerc = 0.0f;
		
		// if outside the stable zone, map to [ -1.0f, +1.0f ] again
		if (fRangeX < -0.2f) fHorPerc = (fRangeX + 0.2f) * (1.0f / 0.8f);
		if (fRangeX > 0.2f) fHorPerc = (fRangeX - 0.2f) * (1.0f / 0.8f);
		if (fRangeY < -0.2f) fVerPerc = (fRangeY + 0.2f) * (1.0f / 0.8f);
		if (fRangeY > 0.2f) fVerPerc = (fRangeY - 0.2f) * (1.0f / 0.8f);

		return (fHorPerc != 0.0f || fVerPerc != 0.0f);
	
}

void Player::Jumping(olc::PixelGameEngine *pge,Map& map, float deltatime)
{
	float lift = 0;
	float fCachHorHeight = float(WINDOW_HEIGHT) * fPlayerH + lookupordown;

	float gravity = 9.8f;
	physics.Vertphysicssetup(fPlayerH, 4);
	physics.AddVertForce(gravity);
	lift = physics.VertIntegr(deltatime);

		// if the player height is adapted, keep horizon height stable by compensating with look up value
		if (pge->GetKey(olc::SHIFT).bHeld)
		{
			//movevert = true;
			fPlayerH += strafeupspeed * frun * deltatime;
			lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
		}
		if(!pge->GetKey(olc::SHIFT).bHeld)
		{
			//movevert = true;
			float fNewHeight = fPlayerH - (2.0f * deltatime);
			if (fNewHeight > 0.5f && map.floatheightmap(int(x), int(y)) < fNewHeight)
			{
				fPlayerH = fNewHeight;
				lookupordown = fCachHorHeight - float(WINDOW_HEIGHT * fPlayerH);
			}
		}


}