// Texture Manipulation.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/

	IMesh* sphereMesh;
	IModel* sphere;

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kFPS);

	sphereMesh = myEngine->LoadMesh("Sphere.x");
	sphere = sphereMesh->CreateModel(10,0,20);

	sphere->SetSkin("EarthPlain.jpg");

	bool cloudy = false;
	int nrFrames = 0;
	int sphereDirection = 1;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		nrFrames++;

		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		if (myEngine->KeyHit(Key_1))
		{
			sphere->SetSkin("EarthHi.jpg");
		}

		 if (cloudy == true)
		{
			 sphere->SetSkin("EarthPlain.jpg");	
		}
		else if (cloudy == false)
		{
				sphere->SetSkin("EarthHi.jpg");
		}
				

		if (sphereDirection == 1)
		{
			sphere->MoveX(0.01);
		}
		else sphere->MoveX(-0.01);

		if (sphere->GetX()>=40)
		{
			cloudy = true;
			sphereDirection = 0;
		}
		else if (sphere->GetX() <= -40)
		{
			cloudy = false;
			sphereDirection = 1;
		}

		sphere->RotateY(0.1);
			 
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
