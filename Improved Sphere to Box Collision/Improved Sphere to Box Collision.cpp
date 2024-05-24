// Improved Sphere to Box Collision.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

//Enums
enum boxSide { leftSide, rightSide, frontSide, backSide, noSide };

//function declarations
boxSide sphere2box(float sxPos, float szPos, float oldsxPos, float oldszPos,
				   float srad, float bxPos, float bzPos, float bwidth, float bdepth);

//Constant variables
const float kGameSpeed = 1;
const float movementSpeed = 0.2f;
const int kNumBoxes = 2;

float boxXs[kNumBoxes] = { 90 , 150 };
float boxZs[kNumBoxes] = { 90 , 150 };

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
	IModel* sphere;
	IModel* floor;
	IModel* boxes[kNumBoxes];

	//Mesh Loading
	sphereMesh = myEngine->LoadMesh("Sphere.x");
	floorMesh = myEngine->LoadMesh("Floor.x");
	boxMesh = myEngine->LoadMesh("Box.x");

	//Model creation
	sphere = sphereMesh->CreateModel(0, 10, 0);
	floor = floorMesh->CreateModel();
	for (int i = 0; i < kNumBoxes; i++)
	{
		boxes[i] = boxMesh->CreateModel(boxXs[i], 0, boxZs[i]);
	}

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

		//Save the old position of the sphere
		float oldX = sphere->GetX();
		float oldZ = sphere->GetZ();

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

		//Check for collisions
		
		for (int i = 0; i < kNumBoxes; i++)
		{
			boxSide collision = sphere2box(sphere->GetX(), sphere->GetZ(), oldX, oldZ, sphereRadius,
										   boxXs[i], boxZs[i], boxWidth, boxDepth);
			//Resolve collisions
			if (collision == frontSide || collision == backSide)
			{
				sphere->SetZ(oldZ);
			}
			else if (collision == leftSide || collision == rightSide)
			{
				sphere->SetX(oldX);
			}
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

//function definitions

boxSide sphere2box(float sxPos, float szPos, float oldsxPos, float oldszPos,
				   float srad, float bxPos, float bzPos, float bwidth, float bdepth)
{
	float minX = bxPos - bwidth / 2 - srad;
	float maxX = bxPos + bwidth / 2 + srad;
	float minZ = bzPos - bdepth / 2 - srad;
	float maxZ = bzPos + bdepth / 2 + srad;

	boxSide result = noSide;

	if (sxPos > minX && sxPos < maxX && szPos > minZ && szPos < maxZ)
	{
		if		(oldsxPos < minX) result = leftSide;
		else if (oldsxPos > maxX) result = rightSide;
		else if (oldszPos < minZ) result = frontSide;
		else					  result = backSide;
	}

	return(result);
}