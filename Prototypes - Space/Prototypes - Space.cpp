// Prototypes - Space.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media

	myEngine->AddMediaFolder("D:\\VANistor\\Downloads\\spaceship");
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/

	ICamera* myCamera = myEngine->CreateCamera(kFPS);

	IMesh* planetMesh = myEngine->LoadMesh("Planet.x");

	IModel* sun = planetMesh->CreateModel(0,0,0);
	IModel* mercury = planetMesh->CreateModel(30, 0, 30);
	IModel* venus = planetMesh->CreateModel(50, 0, 50);
	IModel* earth = planetMesh->CreateModel(70,0,70);
	IModel* moon = planetMesh->CreateModel(2,2,2);
	IModel* mars = planetMesh->CreateModel(90, 0, 90);
	IModel* jupiter = planetMesh->CreateModel(120, 0, 120);
	IModel* saturn = planetMesh->CreateModel(150, 0, 150);
	IModel* uranus = planetMesh->CreateModel(180, 0, 180);
	IModel* neptune = planetMesh->CreateModel(210, 0, 210);
	IModel* pluto = planetMesh->CreateModel(235, 0, 235);

	IMesh* starsMesh = myEngine->LoadMesh("Stars.x");
	IModel* stars = starsMesh->CreateModel();

	IMesh* dummyMesh = myEngine->LoadMesh("dummy.x");
	IModel* dummy1 = dummyMesh->CreateModel(70,0,70);
	IModel* dummy2 = dummyMesh->CreateModel(0, 0, 0);

	stars->SetSkin("StarsHi.jpg");

	myCamera->SetFarClip(1000000);

	myCamera->SetMovementSpeed(50);

	sun->SetSkin("Sun.jpg");
	moon->SetSkin("Moon.jpg");
	mercury->SetSkin("Mercury.jpg");
	venus->SetSkin("Venus.jpg");
	mars->SetSkin("Mars.jpg");
	jupiter->SetSkin("Jupiter.jpg");
	saturn->SetSkin("Saturn.jpg");
	uranus->SetSkin("Uranus.jpg");
	neptune->SetSkin("Neptune.jpg");
	pluto->SetSkin("Pluto.jpg");

	sun->Scale(30);
	earth->Scale(3);
	moon->Scale(1.5);
	mercury->Scale(2.5);
	venus->Scale(3);
	mars->Scale(3.5);
	jupiter->Scale(7);
	saturn->Scale(6);
	uranus->Scale(4);
	neptune->Scale(3);
	pluto->Scale(2);

	dummy1->AttachToParent(dummy2);
	moon->AttachToParent(dummy1);

	mercury->AttachToParent(dummy2);
	venus->AttachToParent(dummy2);
	earth->AttachToParent(dummy2);
	mars->AttachToParent(dummy2);
	jupiter->AttachToParent(dummy2);
	saturn->AttachToParent(dummy2);
	uranus->AttachToParent(dummy2);
	neptune->AttachToParent(dummy2);
	pluto->AttachToParent(dummy2);

	IMesh* spaceshipMesh = myEngine->LoadMesh("FFstarfighter.x");
	IModel* spaceship = spaceshipMesh->CreateModel(0,-3,10);

	spaceship->SetSkin("ff06.jpg");

	myCamera->SetPosition(98, 30, 98);
	spaceship->AttachToParent(myCamera);

	spaceship->Scale(0.5);


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

		earth->RotateLocalY(0.005f);
		mercury->RotateLocalY(0.005f);
		venus->RotateLocalY(0.005f);
		mars->RotateLocalY(0.005f);
		jupiter->RotateLocalY(0.005f);
		saturn->RotateLocalY(0.005f);
		uranus->RotateLocalY(0.005f);
		neptune->RotateLocalY(0.005f);
		pluto->RotateLocalY(0.005f);
		dummy1->RotateLocalY(0.006f);
		dummy2->RotateLocalY(0.0003f);
		sun->RotateLocalY(0.003f);

		if (myEngine->KeyHeld(Key_W))
		{
			spaceship->MoveLocalZ(0.01f);
		}

		if (myEngine->KeyHeld(Key_S))
		{
			spaceship->MoveLocalZ(-0.01);
		}

		if (myEngine->KeyHeld(Key_A))
		{
			spaceship->MoveLocalX(0.01f);
		}

		if (myEngine->KeyHeld(Key_D))
		{
			spaceship->MoveLocalX(-0.01);
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
