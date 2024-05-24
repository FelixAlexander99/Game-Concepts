// Matrices.cpp: A program using the TL-Engine

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

	ICamera* myCamera = myEngine->CreateCamera(kFPS);
	myCamera->SetMovementSpeed(50);

	IMesh* boxMesh = myEngine->LoadMesh("box.x");

	IModel* box = boxMesh->CreateModel(0,-5,30);

	float matrix[4][4]; // Used for storing the matrix of box

	box->GetMatrix(&matrix[0][0]);

	box->SetMatrix(&matrix[0][0]); // The last row of the matrix is its position

	/*
		matrix[3][0] - x coordinate
		matrix[3][1] - y coordinate
		matrix[3][2] - z coordinate
	*/

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		if (myEngine->KeyHit(Key_Escape))
			myEngine->Stop();

		float matrix[4][4];
		box->GetMatrix(&matrix[0][0]);

		// Box movement using matrices
		if (myEngine->KeyHeld(Key_D))
		{
			matrix[3][0] += 0.001f;
			box->SetMatrix(&matrix[0][0]);
		}

		if (myEngine->KeyHeld(Key_A))
		{
			matrix[3][0] -= 0.001f;
			box->SetMatrix(&matrix[0][0]);
		}
		
		if (myEngine->KeyHeld(Key_W))
		{
			matrix[3][1] += 0.001f;
			box->SetMatrix(&matrix[0][0]);
		}
		
		if (myEngine->KeyHeld(Key_S))
		{
			matrix[3][1] -= 0.001f;
			box->SetMatrix(&matrix[0][0]);
		}

		// Scaling along the x axis
		if (myEngine->KeyHeld(Key_E))
		{
			matrix[0][0] *= 1.0001f; // x component of x axis
			matrix[0][1] *= 1.0001f; // y component of x axis
			matrix[0][2] *= 1.0001f; // z component of x axis
			box->SetMatrix(&matrix[0][0]);
		}

		if (myEngine->KeyHeld(Key_Q))
		{
			matrix[0][0] *= 0.9999f;
			matrix[0][1] *= 0.9999f;
			matrix[0][2] *= 0.9999f;
			box->SetMatrix(&matrix[0][0]);
		}

		// Scaling along the y axis
		if (myEngine->KeyHeld(Key_Z))
		{
			matrix[1][0] *= 1.0001f;
			matrix[1][1] *= 1.0001f;
			matrix[1][2] *= 1.0001f;
			box->SetMatrix(&matrix[0][0]);
		}

		if (myEngine->KeyHeld(Key_X))
		{
			matrix[1][0] *= 0.9999f;
			matrix[1][1] *= 0.9999f;
			matrix[1][2] *= 0.9999f;
			box->SetMatrix(&matrix[0][0]);
		}

		if (myEngine->KeyHeld(Key_O))
		{
			box->RotateLocalY(0.01f);
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}
