// Trigonometry and Array Practice.cpp: A program using the TL-Engine

#include <TL-Engine.h>// TL-Engine include file and namespace
#include <math.h>
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	IMesh* starsMesh = myEngine->LoadMesh("stars.x");
	IModel* stars = starsMesh->CreateModel();

	ICamera* myCamera = myEngine->CreateCamera(kFPS);
	myCamera->SetPosition(11, 0, -17);

	IMesh* sphereMesh = myEngine->LoadMesh("sphere.x");

	const int kSphere = 110;
	IModel* spheres[kSphere];

	const float kPi = 3.1415926f;
	const float kEdge = 7 * kPi;


	for (int x = 0; x < kSphere; x++)
	{
		spheres[x] = sphereMesh->CreateModel(x*0.2f, 0, 0);
		spheres[x]->Scale(0.01f);
		spheres[x]->SetSkin("RedBall.jpg");
	}

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		for (int x = 0; x < kSphere; x++)
		{
			if (!(spheres[x]->GetX() >= kEdge))
					{
						spheres[x]->MoveX(0.001f);
						spheres[x]->SetY(sin(spheres[x]->GetX())*2);
					}
			else spheres[x]->SetX(0);

		}
		
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
