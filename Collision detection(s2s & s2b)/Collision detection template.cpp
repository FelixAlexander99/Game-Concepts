// Collision detection template.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

//function declarations
bool sphere2sphere(float s1xPos, float s1zPos, float s1rad, float s2xPos, float s2zPos, float s2rad);
bool sphere2box(float sxPos, float szPos, float srad, float bxPos, float bzPos, float bwidth, float bdepth);

//Game speeds
const float kGameSpeed = 1;
const float movementSpeed = 0.2f;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");

	/**** Set up your scene here ****/
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kFPS);
	myCamera->Move(0, 20, 0);
	myCamera->RotateY(20.0f);
	myCamera->SetMovementSpeed(30);

	//Mesh initialization
	IMesh* sphereMesh;
	IMesh* floorMesh;
	IMesh* boxMesh;

	//Model initialization
	IModel* sphere1;
	IModel* sphere2;
	IModel* floor;
	IModel* box;

	//Mesh Loading
	sphereMesh = myEngine->LoadMesh("Sphere.x");
	floorMesh = myEngine->LoadMesh("Floor.x");
	boxMesh = myEngine->LoadMesh("Box.x");

	//Model creation
	sphere1 = sphereMesh->CreateModel(0, 10, 0);
	sphere2 = sphereMesh->CreateModel(40, 10, 40);
	floor = floorMesh->CreateModel();
	box = boxMesh->CreateModel(90, 0, 90);

	//Model sizes
	float sphereRadius = 10.0f;
	float boxWidth = 10.0f;
	float boxDepth = 10.0f;

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
			sphere1->MoveZ(movementSpeed);
		}
		if (myEngine->KeyHeld(Key_S))
		{
			sphere1->MoveZ(-movementSpeed);
		}
		if (myEngine->KeyHeld(Key_A))
		{
			sphere1->MoveX(-movementSpeed);
		}
		if (myEngine->KeyHeld(Key_D))
		{
			sphere1->MoveX(movementSpeed);
		}
		if (myEngine->KeyHeld(Key_Q))
		{
			sphere1->MoveY(movementSpeed);
		}
		if (myEngine->KeyHeld(Key_E))
		{
			sphere1->MoveY(-movementSpeed);
		}

		//Check for collisions
		bool collisionS2S = sphere2sphere(sphere1->GetX(), sphere1->GetZ(), sphereRadius, sphere2->GetX(), sphere2->GetZ(), sphereRadius);
		bool collisionS2B = sphere2box(sphere1->GetX(), sphere1->GetZ(), sphereRadius, box->GetX(), box->GetZ(), boxWidth, boxDepth);

		//Resolve collisions
		if (collisionS2S)
		{
			sphere1->SetPosition(0, 10, 0);
		}
		if (collisionS2B)
		{
			sphere1->SetPosition(0, 10, 0);
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

//function definitions
bool sphere2sphere(float s1xPos, float s1zPos, float s1rad, float s2xPos, float s2zPos, float s2rad)
{
	float distX = s2xPos - s1xPos;
	float distZ = s2zPos - s1zPos;
	float distance = sqrt(distX * distX + distZ * distZ);

	return (distance < (s1rad + s2rad)); // If all are true then the expression is true
}

bool sphere2box(float sxPos, float szPos, float srad, float bxPos, float bzPos, float bwidth, float bdepth)
{
	float minX = bxPos - bwidth / 2 - srad;
	float maxX = bxPos + bwidth / 2 + srad;
	float minZ = bzPos - bdepth / 2 - srad;
	float maxZ = bzPos + bdepth / 2 + srad;

	return(sxPos > minX && sxPos < maxX && szPos > minZ && szPos < maxZ); // If all are true then the expression is true
}