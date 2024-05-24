// Array and Function Practice.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

const float kPi = 3.1415926f;
const int kSize = 8;
const float kRadius = 100.0f;

float radians;

float x, y, z = 0;

void rotateSpheres(IModel* spheres[kSize], float degrees[kSize])
{

	for (int i = 0; i < kSize; i++)
	{
		degrees[i] += 0.01f;
		if (degrees[i] >= 360.0f)
		{
			degrees[i] = 0;
		}

		radians = (kPi / 180.0f)* degrees[i];

		x = kRadius * cos(radians);    //The angle is given in radians
		y = kRadius * sin(radians);

		spheres[i]->SetPosition(x, y, z);
	}
}

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\ProgramData\\TL-Engine\\Media" );

	/**** Set up your scene here ****/
	IMesh* starsMesh = myEngine->LoadMesh("stars.x");
	IModel* stars = starsMesh->CreateModel(0, 0, 0);

	ICamera* myCamera = myEngine->CreateCamera(kFPS);
	myCamera->SetPosition(0, 0, -250);

	IMesh* sphereMesh = myEngine->LoadMesh("sphere.x");

	IModel* spheres[kSize];

	float degrees[kSize] = { 0,45,90,135,180,225,270,315 };

	for (int i = 0; i < kSize; i++)
	{
		spheres[i] = sphereMesh->CreateModel();
		spheres[i]->SetSkin("RedBall.jpg");
	}

	float xCentre, yCentre, zCentre;

	xCentre = 100;
	yCentre = 100;
	zCentre = 100;

	x = x + xCentre;
	y = y + yCentre;
	z = z + zCentre;

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

		rotateSpheres(spheres, degrees);
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}