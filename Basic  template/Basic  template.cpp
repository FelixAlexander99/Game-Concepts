// Basic  template.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
#include "Basic  template.h"
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kFPS);
	myCamera->Move(0, 20, 0);
	myCamera->RotateY(20.0f);

	//Mesh initialization
	IMesh* sphereMesh;
	IMesh* floorMesh;

	//Model initialization
	IModel* sphere;
	IModel* floor;

	//Model loading
	sphereMesh = myEngine->LoadMesh("Sphere.x");
	sphere = sphereMesh->CreateModel(0, 10, 0);

	floorMesh = myEngine->LoadMesh("Floor.x");
	floor = floorMesh->CreateModel();

	//Game speeds
	const float kGameSpeed = 1;
	const float movementSpeed = 0.1f;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//Exit program
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		//Sphere movement
		if (myEngine->KeyHeld(Key_W))
		{
			sphere->MoveZ(movementSpeed);
		}
		if (myEngine->KeyHeld(Key_S))
		{
			sphere->MoveZ(-movementSpeed);
		}
		if (myEngine->KeyHeld(Key_A))
		{
			sphere->MoveX(-movementSpeed);
		}
		if (myEngine->KeyHeld(Key_D))
		{
			sphere->MoveX(movementSpeed);
		}
		if (myEngine->KeyHeld(Key_Q))
		{
			sphere->MoveY(movementSpeed);
		}
		if (myEngine->KeyHeld(Key_E))
		{
			sphere->MoveY(-movementSpeed);
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
