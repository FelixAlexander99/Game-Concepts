// Attaching Objects Together.cpp: A program using the TL-Engine

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
	
	IMesh*	gridMesh;
	IModel* grid;

	gridMesh = myEngine->LoadMesh("Grid.x");
	grid = gridMesh->CreateModel();

	IMesh* sphereMesh;
	IModel* sphere;

	sphereMesh = myEngine->LoadMesh("Sphere.x");
	sphere = sphereMesh->CreateModel(20,20,20);

	IMesh* cubeMesh;
	IModel* cube;

	cubeMesh = myEngine->LoadMesh("Cube.x");
	cube = cubeMesh->CreateModel(0,0,0);

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kFPS);

	sphere->AttachToParent(cube);

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

		if (myEngine->KeyHeld(Key_A))
		{
			cube->MoveX(0.01);
		}
		if (myEngine->KeyHeld(Key_D))
		{
			cube->MoveX(-0.01);
		}

	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
