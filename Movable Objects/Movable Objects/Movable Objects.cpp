//Nistor Vlad Alexandru

// Assignment 1 - Game Concepts.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media

	myEngine->AddMediaFolder(".\\media");
	/**** Set up your scene here ****/
	
	const float kGameSpeed = 0.012f;

	IMesh* sphereMesh;
	IModel* ball1;
	IModel* ball2;

	sphereMesh = myEngine->LoadMesh("ball.x");
	ball1 = sphereMesh->CreateModel(-50, 10, 0);
	ball2 = sphereMesh->CreateModel(50, 10, 50);

	IMesh* floorMesh;
	IModel* floor;

	floorMesh = myEngine->LoadMesh("floor.x");
	floor = floorMesh->CreateModel();

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual);

	myCamera->Move(0, 120, -200);
	myCamera->RotateX(20);


	int ball1Direction = 1; //1 = up , 0 = down , 2 = left , 3 = right
	int ball2Direction = 0;


	float sphereHorizontalSpeed = 1.0f;
	const float sphereVerticalSpeed = 0.13f;
	bool gameState = true;
	float currentSpeed;


	ball1->SetSkin("blue.png");
	float ballRotationSpeed = 1.0f;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		//ball 1 movement
		if (ball1Direction == 1)
		{
			ball1->MoveZ(sphereHorizontalSpeed*kGameSpeed);
		}
		else if (ball1Direction == 0)
		{
			ball1->MoveZ(-sphereHorizontalSpeed * kGameSpeed);
		}
		else if (ball1Direction == 2)
		{
			ball1->MoveX(sphereHorizontalSpeed*kGameSpeed);
		}
		else if (ball1Direction == 3)
		{
			ball1->MoveX(-sphereHorizontalSpeed * kGameSpeed);
		}

		//ball 2 movement
		if (ball2Direction == 1)
		{
			ball2->MoveZ(sphereHorizontalSpeed*kGameSpeed);
		}
		else if (ball2Direction == 0)
		{
			ball2->MoveZ(-sphereHorizontalSpeed * kGameSpeed);
		}
		else if (ball2Direction == 2)
		{
			ball2->MoveX(sphereHorizontalSpeed*kGameSpeed);
		}
		else if (ball2Direction == 3)
		{
			ball2->MoveX(-sphereHorizontalSpeed * kGameSpeed);
		}

		//ball 1 rotation
		if (ball1Direction == 1)
		{
			if (ball1->GetY() <= 10.0f) ball1->RotateX(ballRotationSpeed*kGameSpeed);
		}
		if (ball1Direction == 0)
		{
			if (ball1->GetY() > 10.0f)
			{
				ball1->RotateX(0.0f);
				ball1->RotateZ(0.0f);
			}
			else ball1->RotateX(-ballRotationSpeed * kGameSpeed);
		}
		if (ball1Direction == 3)
		{
			if (ball1->GetY() > 10.0f)
			{
				ball1->RotateX(0.0f);
				ball1->RotateZ(0.0f);
			}
			else ball1->RotateZ(ballRotationSpeed*kGameSpeed);
		}
		if (ball1Direction == 2)
		{
			if (ball1->GetY() > 10.0f)
			{
				ball1->RotateX(0.0f);
				ball1->RotateZ(0.0f);
			}
			else ball1->RotateZ(-ballRotationSpeed * kGameSpeed);
		}

		//ball 2 rotation

		if (ball2Direction == 1)
		{
			if (ball2->GetY() > 10.0f)
			{
				ball2->RotateX(0.0f);
				ball2->RotateZ(0.0f);
			}
			else ball2->RotateX(ballRotationSpeed*kGameSpeed);
		}
		if (ball2Direction == 0)
		{
			if (ball2->GetY() > 10.0f)
			{
				ball2->RotateX(0.0f);
				ball2->RotateZ(0.0f);
			}
			else ball2->RotateX(-ballRotationSpeed * kGameSpeed);
		}
		if (ball2Direction == 3)
		{
			if (ball2->GetY() > 10.0f)
			{
				ball2->RotateX(0.0f);
				ball2->RotateZ(0.0f);
			}
			else ball2->RotateZ(ballRotationSpeed*kGameSpeed);
		}
		if (ball2Direction == 2)
		{
			if (ball2->GetY() > 10.0f)
			{
				ball2->RotateX(0.0f);
				ball2->RotateZ(0.0f);
			}
			else ball2->RotateZ(-ballRotationSpeed * kGameSpeed);
		}

		//ball 1 direction changer
		if (ball1->GetZ() >= 100.0f && ball1->GetX() <= -50.0f)
		{
			ball1Direction = 2;

		}
		else if (ball1->GetZ() >= 100.0f && ball1->GetX() >= 50.0f)
		{
			ball1Direction = 0;

		}
		else if (ball1->GetZ() <= 0.0f && ball1->GetX() >= 50.0f)
		{
			ball1Direction = 3;

		}
		else if (ball1->GetZ() <= 0.0f && ball1->GetX() <= -50.0f)
		{
			ball1Direction = 1;

		}

		//ball 2 direction changer
		if (ball2->GetZ() >= 100.0f && ball2->GetX() <= -50.0f)
		{
			ball2Direction = 2;

		}
		else if (ball2->GetZ() >= 100.0f && ball2->GetX() >= 50.0f)
		{
			ball2Direction = 0;

		}
		else if (ball2->GetZ() <= 0.0f && ball2->GetX() >= 50.0f)
		{
			ball2Direction = 3;

		}
		else if (ball2->GetZ() <= 0.0f && ball2->GetX() <= -50.0f)
		{
			ball2Direction = 1;

		}

		//Mouse scroll speed
		float mouseScroll = myEngine->GetMouseWheelMovement();
		if (gameState == 1)
		{
			if (mouseScroll > 0.0f && sphereHorizontalSpeed < 5.0f)
			{
				sphereHorizontalSpeed = sphereHorizontalSpeed + 0.1f;
				ballRotationSpeed = ballRotationSpeed + 0.1f;
			}
			if (mouseScroll < 0.0f && sphereHorizontalSpeed >0.5f)
			{
				sphereHorizontalSpeed = sphereHorizontalSpeed - 0.1f;
				ballRotationSpeed = ballRotationSpeed + 0.1f;
			}

			if (myEngine->KeyHeld(Key_Up) && ball1->GetLocalY() <= 40.0f && ball2->GetLocalY() <= 40.0f)
			{
				ball1->MoveY(sphereVerticalSpeed*kGameSpeed);
				ball2->MoveY(sphereVerticalSpeed*kGameSpeed);
			}
			if (myEngine->KeyHeld(Key_Down) && ball1->GetLocalY() >= 10.0f && ball2->GetLocalY() >= 10.0f)
			{
				ball1->MoveY(-sphereVerticalSpeed * kGameSpeed);
				ball2->MoveY(-sphereVerticalSpeed * kGameSpeed);
			}
		}

		
		//Game pause

		if (gameState == true)
		{
			currentSpeed = sphereHorizontalSpeed;
		}

		if (myEngine->KeyHit(Key_P))
		{
			if (gameState == true)
			{
				sphereHorizontalSpeed = 0.0f;
				ballRotationSpeed = 0.0f;
			}
			else
			{
				sphereHorizontalSpeed = currentSpeed; 
				ballRotationSpeed = currentSpeed;
			}
			gameState = !gameState;
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
