// Assignment 2 project.cpp: A program using the TL-Engine

#include <TL-Engine.h>	// TL-Engine include file and namespace

using namespace tle;

//Enums
enum gameStates { Ready, Firing, Contact, Over };

enum mushroomStates { Cold, Hot, Very_Hot, Removed };
enum segmentStates { cold, hot, very_hot, removed};
enum wormDirectionalHeading {Left, Right};

//Constant values
const int kNumMushrooms = 10; // Number of mushrooms on the grid
const int kNumBushes = 6; // Number of bushes on the grid
const int backEdge = 200;
const int numberRows = 20;
const int numberCollumns = 16;
const int kNumBarriers = 22;
const float degreeIncrease = 0.1f;
const float maxDegrees = 45;
const float leftEdge = -75;
const float rightEdge = 75;
const int kNumSegments = 10;

//Constant speeds
const float kGameSpeed = 2.0f; // About 0.2 works fine on the uni machines
const float arrowMovementSpeed = 0.2f;
const float marbleMovementSpeed = 0.2f;
const float arrowSway = 0.1f;
const float cameraRotationSpeed = 0.1f;

//Constant sizes
const float mushroomRadius = 4.1f;
const int marbleRadius = 2;
const float barrierDepth = 18.1f;
const int barrierWidth = 2;
const int segmentRadius = 5;

//Structs
struct WormSegment
{
	segmentStates hitState;
	int xPosition;
	int zPosition;
	wormDirectionalHeading direction;
};

//Initializing an array of structs for the worm segments
WormSegment Segment[kNumSegments];


//Function declarations

bool sphere2sphere(float sphere1xPosition, float sphere1zPosition, float sphere1Radius,
			   	   float sphere2xPosition, float sphere2zPosition, float sphere2Radius);

bool sphere2box(float spherexPosition, float spherezPosition, float sphereRadius,
				float boxxPosition,    float boxzPosition,    float boxWidth, float boxDepth);


void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("C:\\Users\\VANistor\\Desktop\\University files\\CO1301 - Games Concepts\\Assignment 2\\Media");
	//myEngine->AddMediaFolder("F:\\My files\\University files\\CO1301 - Games Concepts\\Assignment 2\\Media");

	/**** Set up your scene here ****/

	//Initialization of the game state

	gameStates currentGameState = Ready;

	//Camera creation

	ICamera* myCamera = myEngine->CreateCamera(kManual, 0, 75, -75);
	myCamera->RotateX(25);

	//Mesh loading
	IMesh* groundMesh = myEngine->LoadMesh("Ground.x");
	IMesh* skyboxMesh = myEngine->LoadMesh("Skybox_SciFi.x");
	IMesh* mushroomMesh = myEngine->LoadMesh("Mushroom.x");
	IMesh* marbleMesh = myEngine->LoadMesh("Marble.x");
	IMesh* arrowMesh = myEngine->LoadMesh("Arrow.x");
	IMesh* bushMesh = myEngine->LoadMesh("Bush.x");
	IMesh* barrierMesh = myEngine->LoadMesh("Barrier.x");
	IMesh* dummyMesh = myEngine->LoadMesh("Dummy.x");
	IMesh* wormSegmentMesh = myEngine->LoadMesh("Segment.x");

	//Model creation
	IModel* ground = groundMesh->CreateModel(0, 0, 0);
	IModel* skybox = skyboxMesh->CreateModel(0, -1000, 0);
	IModel* mushrooms[kNumMushrooms];
	IModel* marble = marbleMesh->CreateModel(0, 2, 0);
	IModel* arrow = arrowMesh->CreateModel(0, 0, -10);
	IModel* bushes[kNumBushes];
	IModel* barriers[kNumBarriers];
	IModel* dummy1 = dummyMesh->CreateModel(0, 2, 0);  // The dummy the marble and the arrow are attached to
	IModel* dummy2 = dummyMesh->CreateModel(0, 0, 100); // The dummy the camera is attached to(created in the middle of the board)
	IModel* wormSegments[kNumSegments];
	IModel* wormMushrooms[kNumSegments];

	//Attaching models to dummy1
	arrow->AttachToParent(dummy1);
	marble->AttachToParent(dummy1);

	//Mushroom positions
	float mushroomXPositions[kNumMushrooms] = { -45,-75,-15,  25,-65, -15, 45, -35, 15, 65 };
	float mushroomZPositions[kNumMushrooms] = { 55, 85, 65, 105, 35, 175, 45, 145, 35, 165 };

	//States of each mushroom
	mushroomStates mushroomHitState[kNumMushrooms] = { Cold, Cold, Cold, Cold, Cold, Cold, Cold, Cold, Cold, Cold };
	mushroomStates wormMushroomHitState[kNumSegments] = { Cold, Cold, Cold, Cold, Cold, Cold, Cold, Cold, Cold, Cold };

	for (int i = 0; i < kNumMushrooms; i++)
	{
		mushrooms[i] = mushroomMesh->CreateModel(mushroomXPositions[i], 0, mushroomZPositions[i]);

	}

	//Bushes positions
	float bushXPositions[kNumBushes] = { -70, -50, -30, -0, 30, 60 };
	float bushScaling[kNumBushes] = { 30, 35, 40, 50, 45, 47 };
	float bushRotation[kNumBushes] = { 20 , 80 ,15, 45, 67, 140 };

	for (int i = 0; i < kNumBushes; i++)
	{
		bushes[i] = bushMesh->CreateModel(bushXPositions[i], 0, 210);
		bushes[i]->Scale(bushScaling[i]);
		bushes[i]->RotateY(bushRotation[i]);
	}

	//Barrier positions
	float barrierXPositions[kNumBarriers] = { -83, -83, -83, -83, -83, -83, -83, -83, -83, -83, -83, 83,  83,  83,  83,  83,  83,  83, 83 ,83, 83, 83 };
	float barrierZPositions[kNumBarriers] = { 191, 173, 155, 137, 119, 101,  83,  65,  47,  29,  11, 191, 173, 155, 137, 119, 101, 83, 65, 47, 29, 11 };

	//Barrier creation
	for (int i = 0; i < kNumBarriers; i++)
	{
		barriers[i] = barrierMesh->CreateModel(barrierXPositions[i], 0, barrierZPositions[i]);
		if (i > 6 && i < 11 || i > 17 && i < 22)  // Barriers start from the back of the grid
		{
			barriers[i]->SetSkin("barrier1.bmp");
		}
		else barriers[i]->SetSkin("barrier1a.bmp");
	}

	//Declaring variables to store the marble's position
	float marbleXPosition;
	float marbleZPosition;

	int mushroomsRemoved = 0; //Number of mushrooms hit 3 times ( removed )

	//Array that stores the x positions of the centres of the squares
	float squareCentreXPositions[numberRows][numberCollumns] =
	{
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75},
	{-75, -65, -55, -45, -35, -25, -15, -5, 5, 15, 25, 35, 45, 55, 65, 75}
	};

	//Array that stores the z positions of the centres of the squares
	float squareCentreZPositions[numberRows][numberCollumns] =
	{
		{195,195,195,195,195,195,195,195,195,195,195,195,195,195,195,195},
		{185,185,185,185,185,185,185,185,185,185,185,185,185,185,185,185},
		{175,175,175,175,175,175,175,175,175,175,175,175,175,175,175,175},
		{165,165,165,165,165,165,165,165,165,165,165,165,165,165,165,165},
		{155,155,155,155,155,155,155,155,155,155,155,155,155,155,155,155},
		{145,145,145,145,145,145,145,145,145,145,145,145,145,145,145,145},
		{135,135,135,135,135,135,135,135,135,135,135,135,135,135,135,135},
		{125,125,125,125,125,125,125,125,125,125,125,125,125,125,125,125},
		{115,115,115,115,115,115,115,115,115,115,115,115,115,115,115,115},
		{105,105,105,105,105,105,105,105,105,105,105,105,105,105,105,105},
		{95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95, 95},
		{85 ,85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85},
		{75, 75, 75, 75, 75, 75, 75, 75 ,75, 75, 75, 75, 75, 75, 75, 75},
		{65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65, 65},
		{55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55, 55},
		{45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45, 45},
		{35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35, 35},
		{25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25},
		{15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15},
		{5,  5,  5,  5,  5,  5 , 5,  5,  5,  5,  5,  5,  5,  5,  5,  5}
	};

	float dummyRotation = 0.0f; // Value that keeps track of the dummy's rotation

	float marbleRotation = 0.0f;

	float segmentXPositions[kNumSegments] = { -75, -65, -55, -45, -35, -25, -15, -5, 5, 15 }; // Starting X positions of the worm segments

	//Creating the worm
	for (int i = 0; i < kNumSegments; i++)
	{
		wormSegments[i] = wormSegmentMesh->CreateModel(segmentXPositions[i], 10, 195);
		wormSegments[i]->SetSkin("wormskin.jpg");
	}

	int segmentsRemoved = 0; // When it reaches 10 the worm has been completely destroyed

	//Segments hit state initialization
	segmentStates segmentHitState[kNumSegments] = { cold, cold, cold, cold, cold, cold, cold, cold, cold, cold };

	float wormMushroomXPositions[kNumSegments] = {};
	float wormMushroomZPositions[kNumSegments] = {};

	//The worm originally moves to the right
	for (int i = 0; i < kNumSegments; i++)
	{
		Segment[i].direction = Right;
	}

	float segmentMovementTimer = 50;

	float timerCounter = 0;




	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//Rotation of the camera
		if (myCamera->GetZ() < 80)
		{
			if (myEngine->KeyHeld(Key_Up))
			{
				myCamera->LookAt(dummy2);
				myCamera->MoveLocalY(cameraRotationSpeed * kGameSpeed);
			}
		}
		if (myCamera->GetY() > 5)
		{
			if (myEngine->KeyHeld(Key_Down))
			{
				myCamera->LookAt(dummy2);
				myCamera->MoveLocalY(-cameraRotationSpeed * kGameSpeed);
			}
		}

		// Reset the camera to it's original position
		if (myEngine->KeyHit(Key_C))
		{
			myCamera->SetPosition(0, 75, -75);
			myCamera->LookAt(dummy2);
		}

		//Exit game
		if (myEngine->KeyHit(Key_Escape))
		{
			myEngine->Stop();
		}

		//Arrow movement in the ready state
		if (currentGameState == Ready)
		{
			if (dummy1->GetX() > leftEdge)
			{
				if (myEngine->KeyHeld(Key_Z))     // Move the arrow to the left (z)
				{
					dummy1->MoveX(-arrowMovementSpeed * kGameSpeed);
				}
			}
			if (dummy1->GetX() < rightEdge)
			{
				if (myEngine->KeyHeld(Key_X))   // Move the arrow to the right (x)
				{
					dummy1->MoveX(arrowMovementSpeed * kGameSpeed);
				}
			}


			//Arrow rotation
			if (dummyRotation < maxDegrees)
			{
				if (myEngine->KeyHeld(Key_Period))
				{
					dummy1->RotateY(degreeIncrease * kGameSpeed);     // Rotate the arrow to the right
					dummyRotation += (degreeIncrease * kGameSpeed);
				}
			}
			if (dummyRotation > -maxDegrees)
			{
				if (myEngine->KeyHeld(Key_Comma))
				{
					dummy1->RotateY(-degreeIncrease * kGameSpeed);     // Rotate the arrow to the left
					dummyRotation -= (degreeIncrease * kGameSpeed);
				}
			}
		}

		//Saving the marble's positions
		marbleXPosition = marble->GetX();
		marbleZPosition = marble->GetZ();
		marbleRotation = dummyRotation;

		//Transitioning to Firing state	
		if (myEngine->KeyHit(Key_Space))
		{
			currentGameState = Firing;
			marble->DetachFromParent();
			marble->SetPosition(marbleXPosition, 0, marbleZPosition);
			marble->RotateY(marbleRotation);
		}

		if (currentGameState == Firing)
		{
			//Checking for collisions(marble to mushrooms)
			for (int i = 0; i < kNumMushrooms; i++)
			{
				// Check for collisions
				if (sphere2sphere(marbleXPosition,		 marbleZPosition,		marbleRadius,
								  mushroomXPositions[i], mushroomZPositions[i], mushroomRadius))
				{
					//Collision resolution
					if (mushroomHitState[i] == Cold)
					{
						mushroomHitState[i] = Hot;
						mushrooms[i]->SetSkin("mushroom_hot.png");
						currentGameState = Contact; // Transition to contact state if collision occurs
					}
					else if (mushroomHitState[i] == Hot)
					{
						mushroomHitState[i] = Very_Hot;
						mushrooms[i]->SetSkin("mushroom_very_hot.png");
						currentGameState = Contact;
					}
					else if (mushroomHitState[i] == Very_Hot)
					{
						mushroomHitState[i] = Removed;
						mushrooms[i]->MoveY(-10);	//Remove the mushroom
						currentGameState = Contact;
						mushroomsRemoved++;
					}
				}
			}

			//Saving the marble's positions
			marbleXPosition = marble->GetX();
			marbleZPosition = marble->GetZ();
			marbleRotation = dummyRotation;

			// Check for collisions(marble to segments)
			for (int i = 0; i < kNumSegments; i++)
			{

				if (sphere2sphere(marbleXPosition, marbleZPosition, marbleRadius,
								  segmentXPositions[i], 195, segmentRadius))
				{
					//Collision resolution
					if (segmentHitState[i] == cold)
					{
						segmentHitState[i] = hot;
						wormSegments[i]->SetSkin("wormskin_hot.jpg");
						currentGameState = Contact; // Transition to contact state if collision occurs
					}
					else if (segmentHitState[i] == hot)
					{
						segmentHitState[i] = very_hot;
						wormSegments[i]->SetSkin("wormskin_very_hot.jpg");
						currentGameState = Contact;
					}
					else if (segmentHitState[i] == very_hot)
					{
						segmentHitState[i] = removed;
						wormSegments[i]->MoveY(-20);	//Remove the worm segment
						currentGameState = Contact;
						segmentsRemoved++;
						wormMushrooms[i] = mushroomMesh->CreateModel(wormSegments[i]->GetX(), 0, wormSegments[i]->GetZ());  // Create a mushroom where the segment was destroyed
						wormMushroomXPositions[i] = wormSegments[i]->GetX();  // Save the worm mushroom positions in 2 arrays
						wormMushroomZPositions[i] = wormSegments[i]->GetZ();
					}
				}
			}

			//Checking for collisions ( marble to barriers )

			for (int i = 0; i < kNumBarriers; i++)
			{
				//Collision resolution
				if (sphere2box(marble->GetX(), marble->GetZ(), marbleRadius, barrierXPositions[i], barrierZPositions[i], barrierWidth, barrierDepth))
				{
					if (marble->GetX() > 0)
					{
						marble->RotateY(-dummyRotation);  //Reverse the marble's direction
					}
					if (marble->GetX() < 0)
					{
						marble->RotateY(dummyRotation);  //Reverse the marble's direction
					}
				}
			}

			//Check for collisions ( marble to worm mushrooms )
			for (int i = 0; i < kNumSegments; i++)
			{
				if (sphere2sphere(marbleXPosition, marbleZPosition, marbleRadius,
								  wormMushroomXPositions[i], wormMushroomZPositions[i], mushroomRadius))
				{
					if (segmentHitState[i] == removed)
					{
						//Collision resolution
						if (wormMushroomHitState[i] == Cold)
						{
							wormMushroomHitState[i] = Hot;
							wormMushrooms[i]->SetSkin("mushroom_hot.png");  
							currentGameState = Contact; // Transition to contact state if collision occurs
						}
						else if (wormMushroomHitState[i] == Hot)
						{
							wormMushroomHitState[i] = Very_Hot;
							wormMushrooms[i]->SetSkin("mushroom_very_hot.png");
							currentGameState = Contact;
						}
						else if (wormMushroomHitState[i] == Very_Hot)
						{
							wormMushroomHitState[i] = Removed;
							wormMushrooms[i]->MoveY(-10);	//Remove the mushroom
							currentGameState = Contact;
							mushroomsRemoved++;
						}
					}
				}
			}

			marble->MoveLocalZ(marbleMovementSpeed * kGameSpeed);

			if (marble->GetZ() > backEdge)
			{
				currentGameState = Contact;
			}
		}

		// If contact is made reset everything to how it was in the ready state
		if (currentGameState == Contact)
		{
			marble->SetPosition(0, 0, 0);
			marble->ResetOrientation();
			dummy1->SetPosition(0, 2, 0);
			dummy1->RotateY(-dummyRotation); // Reset the rotation on the dummy
			dummyRotation = 0;
			marble->AttachToParent(dummy1);
			currentGameState = Ready;

			//Check for game over
			if (mushroomsRemoved >= 5 && segmentsRemoved == 10)
			{
				marble->SetSkin("glass_green.jpg");
				currentGameState = Over;
			}
		}
	}

	//Saving the worm's positions and states
	for(int i=0; i<kNumSegments; i++)  
	{
		Segment[i].xPosition = wormSegments[i]->GetX();
		Segment[i].zPosition = wormSegments[i]->GetZ();
		Segment[i].hitState = segmentHitState[i];
	}

	timerCounter++; // Increase the counter by 1 every frame

	//Moving the worm
	for (int i = 0; i < kNumSegments; i++)
	{
		if (Segment[i].xPosition >= rightEdge)
		{
			wormSegments[i]->MoveZ(10); // Move the segment one square down
			Segment[i].direction = Left;
		}
		if (Segment[i].xPosition >= leftEdge)
		{
			wormSegments[i]->MoveZ(10); // Move the segment one square down
			Segment[i].direction = Right;
		}
		if (timerCounter >= segmentMovementTimer)
		{
			if (Segment[i].direction == Right)
			{
				wormSegments[i]->MoveX(10);  // Move the segment one square to the right
			}
			else wormSegments[i]->MoveX(-10);   // Move the segment one square to the left
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}

//Function definitions

bool sphere2sphere(float sphere1xPosition, float sphere1zPosition, float sphere1Radius,
	float sphere2xPosition, float sphere2zPosition, float sphere2Radius)
{
	float xDistance = sphere2xPosition - sphere1xPosition;
	float zDistance = sphere2zPosition - sphere1zPosition;
	float distance = sqrt(xDistance * xDistance + zDistance * zDistance);

	return (distance < (sphere1Radius + sphere2Radius)); // If all are true then the expression is true
}

bool sphere2box(float spherexPosition, float spherezPosition, float sphereRadius,
	float boxxPosition, float boxzPosition, float boxWidth, float boxDepth)
{
	float minX = boxxPosition - boxWidth / 2 - sphereRadius;
	float maxX = boxxPosition + boxWidth / 2 + sphereRadius;
	float minZ = boxzPosition - boxDepth / 2 - sphereRadius;
	float maxZ = boxzPosition + boxDepth / 2 + sphereRadius;

	return(spherexPosition > minX && spherexPosition < maxX && spherezPosition > minZ && spherezPosition < maxZ);
}
